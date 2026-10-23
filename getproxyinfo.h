/*
 * getproxyinfo.h - 读取当前用户的 Windows 代理设置
 *
 * 设计目标:
 *   - 不申请管理员权限, 不触发 UAC (清单为 asInvoker)
 *   - 只读 HKEY_CURRENT_USER, 不写注册表, 不改任何系统设置
 *   - 优先使用 WinHTTP API, 失败时回退到注册表读取
 *   - 纯 C, 只依赖 Win32, 可直接把 .c/.h 复制进其它工程
 *
 * 典型用法:
 *   GetProxyInfoResult r;
 *   if (GetProxyInfo(&r) == 0 && r.enabled)
 *       printf("proxy=%s\n", r.server);
 */

#ifndef GETPROXYINFO_H
#define GETPROXYINFO_H

#define GPI_VERSION "2.0.0"
#define GPI_MAX 512

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GetProxyInfoResult
{
    int  enabled;          /* 1 = 当前用户配置了代理, 0 = 无代理 */
    char server[GPI_MAX];  /* host:port, 可直接喂给 aria2c / yt-dlp */
    char raw[GPI_MAX];     /* 原始 ProxyServer 串, 例如 http=a:1;https=b:2 */
    char bypass[GPI_MAX];  /* ProxyOverride 绕过列表, 可能为空 */
    char pac_url[GPI_MAX]; /* 自动配置脚本 (PAC) URL, 可能为空 */
    int  auto_detect;      /* 1 = 启用了 "自动检测设置" */
} GetProxyInfoResult;

/*
 * 读取当前用户 (HKCU) 的 IE/WinINET 代理设置.
 * 成功返回 0; 失败返回非 0 (Win32 错误码, 或 -1 参数错误).
 * 调用方无需清理任何内存.
 */
int GetProxyInfo(GetProxyInfoResult *result);

/*
 * 写入兼容 FreeDownloader 的配置文件:
 *   有代理 -> "proxy=host:port"
 *   无代理 -> "proxy=0"
 * 文件内容不带换行, 与旧版 GetProxyInfo.exe 保持一致.
 * ini_path 的上级目录不存在时自动创建.
 * 成功返回 0, 失败返回 -1.
 */
int GetProxyInfoWriteIni(const char *ini_path);

#ifdef __cplusplus
}
#endif

#endif /* GETPROXYINFO_H */
