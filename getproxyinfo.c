/*
 * getproxyinfo.c - 免管理员权限读取当前用户代理设置
 *
 * 实现要点:
 *   1. 首选 WinHttpGetIEProxyConfigForCurrentUser(), 它与 IE / 系统代理界面
 *      读取的是同一份配置, 还能拿到绕过列表和 PAC 地址.
 *   2. 该 API 不可用时回退到 HKCU\...\Internet Settings 注册表读取.
 *   3. HKCU 是当前用户自己的配置单元, 标准用户即可读取, 不需要 UAC.
 *   4. 不使用 system()/reg.exe, 不落临时文件, 固定缓冲区全部有界.
 */

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <string.h>

#include "getproxyinfo.h"

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "advapi32.lib")

#define GPI_KEY "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"

/* ------------------------------------------------------------------ */
/* 字符串工具                                                          */
/* ------------------------------------------------------------------ */

static void gpi_copy(char *dst, size_t cch, const char *src)
{
    if (!dst || cch == 0)
        return;
    if (!src)
    {
        dst[0] = '\0';
        return;
    }
    strncpy(dst, src, cch - 1);
    dst[cch - 1] = '\0';
}

static void gpi_copy_wide(char *dst, size_t cch, const wchar_t *src)
{
    if (!dst || cch == 0)
        return;
    dst[0] = '\0';
    if (!src)
        return;
    WideCharToMultiByte(CP_ACP, 0, src, -1, dst, (int)cch, NULL, NULL);
    dst[cch - 1] = '\0';
}

static void gpi_trim(char *s)
{
    size_t i = 0, n;

    if (!s)
        return;

    while (s[i] == ' ' || s[i] == '\t' || s[i] == ';')
        i++;
    if (i)
        memmove(s, s + i, strlen(s + i) + 1);

    n = strlen(s);
    while (n > 0)
    {
        char c = s[n - 1];
        if (c == ' ' || c == '\t' || c == ';' || c == '\r' || c == '\n')
            s[--n] = '\0';
        else
            break;
    }
}

/*
 * 把 ProxyServer 串解析成单个 host:port.
 *   输入可能是 "127.0.0.1:8888" 这种单值,
 *   也可能是 "http=127.0.0.1:8888;https=127.0.0.1:8888;socks=127.0.0.1:1080".
 *   多协议时按 http > https > socks 的顺序取值.
 */
static void gpi_pick_server(const char *spec, char *out, size_t cch)
{
    char http[GPI_MAX] = "";
    char https[GPI_MAX] = "";
    char socks[GPI_MAX] = "";
    const char *p;
    int has_eq = 0;

    if (!out || cch == 0)
        return;
    out[0] = '\0';
    if (!spec || !spec[0])
        return;

    for (p = spec; *p; p++)
    {
        if (*p == '=')
        {
            has_eq = 1;
            break;
        }
    }

    if (!has_eq)
    {
        gpi_copy(out, cch, spec);
        gpi_trim(out);
        return;
    }

    p = spec;
    while (*p)
    {
        const char *eq = strchr(p, '=');
        const char *end;
        size_t nlen, vlen;
        char *slot = NULL;

        if (!eq)
            break;

        end = strchr(eq + 1, ';');
        if (!end)
            end = eq + strlen(eq + 1);

        nlen = (size_t)(eq - p);
        vlen = (size_t)(end - eq - 1);

        if (nlen == 4 && _strnicmp(p, "http", 4) == 0)
            slot = http;
        else if (nlen == 5 && _strnicmp(p, "https", 5) == 0)
            slot = https;
        else if (nlen == 5 && _strnicmp(p, "socks", 5) == 0)
            slot = socks;

        if (slot)
        {
            if (vlen >= GPI_MAX)
                vlen = GPI_MAX - 1;
            memcpy(slot, eq + 1, vlen);
            slot[vlen] = '\0';
        }

        if (*end == '\0')
            break;
        p = end + 1;
    }

    if (http[0])
        gpi_copy(out, cch, http);
    else if (https[0])
        gpi_copy(out, cch, https);
    else if (socks[0])
        gpi_copy(out, cch, socks);
    else
    {
        const char *eq = strchr(spec, '=');
        gpi_copy(out, cch, eq ? eq + 1 : spec);
    }

    gpi_trim(out);
}

/* ------------------------------------------------------------------ */
/* 注册表回退                                                          */
/* ------------------------------------------------------------------ */

static int gpi_read_registry(GetProxyInfoResult *r)
{
    DWORD enable = 0;
    DWORD size, type;
    char buf[GPI_MAX * 2];

    size = sizeof(enable);
    if (RegGetValueA(HKEY_CURRENT_USER, GPI_KEY, "ProxyEnable",
                     RRF_RT_REG_DWORD, &type, &enable, &size) != ERROR_SUCCESS)
        return -1;

    size = sizeof(buf);
    if (RegGetValueA(HKEY_CURRENT_USER, GPI_KEY, "ProxyServer",
                     RRF_RT_REG_SZ, &type, buf, &size) == ERROR_SUCCESS)
    {
        gpi_copy(r->raw, sizeof(r->raw), buf);
        if (enable)
            gpi_pick_server(r->raw, r->server, sizeof(r->server));
    }

    size = sizeof(buf);
    if (RegGetValueA(HKEY_CURRENT_USER, GPI_KEY, "ProxyOverride",
                     RRF_RT_REG_SZ, &type, buf, &size) == ERROR_SUCCESS)
        gpi_copy(r->bypass, sizeof(r->bypass), buf);

    size = sizeof(buf);
    if (RegGetValueA(HKEY_CURRENT_USER, GPI_KEY, "AutoConfigURL",
                     RRF_RT_REG_SZ, &type, buf, &size) == ERROR_SUCCESS)
        gpi_copy(r->pac_url, sizeof(r->pac_url), buf);

    r->enabled = (enable && r->server[0]) ? 1 : 0;
    return 0;
}

/* ------------------------------------------------------------------ */
/* 公开接口                                                            */
/* ------------------------------------------------------------------ */

int GetProxyInfo(GetProxyInfoResult *result)
{
    WINHTTP_CURRENT_USER_IE_PROXY_CONFIG cfg;
    DWORD err;

    if (!result)
        return -1;

    memset(result, 0, sizeof(*result));
    ZeroMemory(&cfg, sizeof(cfg));

    if (WinHttpGetIEProxyConfigForCurrentUser(&cfg))
    {
        result->auto_detect = cfg.fAutoDetect ? 1 : 0;

        if (cfg.lpszProxy)
        {
            gpi_copy_wide(result->raw, sizeof(result->raw), cfg.lpszProxy);
            gpi_pick_server(result->raw, result->server, sizeof(result->server));
        }
        if (cfg.lpszProxyBypass)
            gpi_copy_wide(result->bypass, sizeof(result->bypass), cfg.lpszProxyBypass);
        if (cfg.lpszAutoConfigUrl)
            gpi_copy_wide(result->pac_url, sizeof(result->pac_url), cfg.lpszAutoConfigUrl);

        result->enabled = result->server[0] ? 1 : 0;

        if (cfg.lpszProxy)
            GlobalFree(cfg.lpszProxy);
        if (cfg.lpszProxyBypass)
            GlobalFree(cfg.lpszProxyBypass);
        if (cfg.lpszAutoConfigUrl)
            GlobalFree(cfg.lpszAutoConfigUrl);

        return 0;
    }

    err = GetLastError();

    if (gpi_read_registry(result) == 0)
        return 0;

    return (int)(err ? err : ERROR_GEN_FAILURE);
}

static void gpi_make_dirs(const char *path)
{
    char tmp[MAX_PATH];
    char *p, *start;

    if (!path || !path[0])
        return;

    strncpy(tmp, path, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    start = (tmp[0] && tmp[1] == ':') ? tmp + 3 : tmp + 1;
    if (start >= tmp + strlen(tmp))
        return;

    for (p = start; *p; p++)
    {
        if (*p == '\\' || *p == '/')
        {
            char saved = *p;
            *p = '\0';
            CreateDirectoryA(tmp, NULL);
            *p = saved;
        }
    }
}

int GetProxyInfoWriteIni(const char *ini_path)
{
    GetProxyInfoResult r;
    FILE *fp;

    if (!ini_path || !ini_path[0])
        return -1;

    if (GetProxyInfo(&r) != 0)
        return -1;

    gpi_make_dirs(ini_path);

    fp = fopen(ini_path, "w");
    if (!fp)
        return -1;

    fprintf(fp, "proxy=%s", r.enabled ? r.server : "0");
    fclose(fp);
    return 0;
}
