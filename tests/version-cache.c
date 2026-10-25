/* Regression harness for VersionCacheIsCurrent() in FreeDownloader.c.
 *
 * 背景: .version 是"关于"页的版本信息缓存. 旧逻辑只在缓存缺失时才重建,
 * 换包升级后缓存里还是升级前的版本号, "关于"页就一直显示旧版本, 看起来像
 * 升级没生效. 现在改成比对缓存里的版本串与编译期 FD_VERSION, 不一致就重建.
 *
 * 为什么直接 include 源文件: VersionCacheIsCurrent 是 static, 只能同编译单元调用;
 * 这里用 #define main 把程序的 main 改名后整体包含 (与 bilibili-url-normalize.c 同法).
 *
 * 构建 (已执行 vcvars64.bat, 或 Developer Command Prompt):
 *   cl /nologo /W0 /utf-8 /MT /D_CRT_SECURE_NO_WARNINGS /Fo:<obj目录>\ /Fe:regress-ver.exe tests\version-cache.c getproxyinfo.c
 *
 * 运行 (在可写目录里跑, 用例会在当前目录创建/删除 .version):
 *   regress-ver.exe
 *
 * 退出码: 0=全部符合预期  1=有不符合预期的项
 */
#define main fd_real_main
#include "../FreeDownloader.c"
#undef main

static int failures = 0;

static void check(const char *what, int ok)
{
	printf("%-52s %s\n", what, ok ? "PASS" : "FAIL");
	if (!ok)
	{
		failures++;
	}
}

/* 按 GenerateVersionFile() 的格式写缓存, ver 为版本串 */
static void write_cache(const char *ver)
{
	FILE *f;

	f = fopen(".version", "w");
	if (f == NULL)
	{
		printf("ERROR: cannot write .version\n");
		return;
	}
	fprintf(f, "------------------------------------------------------------------\n");
	fprintf(f, "--------------------------FreeDownloader--------------------------\n");
	fprintf(f, "------------------------------------------------------------------\n");
	fprintf(f, "FreeDownloader版本:");
	fprintf(f, ver);
	fprintf(f, "\n");
	fprintf(f, "------------------------------------------------------------------\n");
	fclose(f);
}

static void write_raw(const char *text)
{
	FILE *f;

	f = fopen(".version", "w");
	if (f == NULL)
	{
		printf("ERROR: cannot write .version\n");
		return;
	}
	fputs(text, f);
	fclose(f);
}

int main(void)
{
	char mismatch[64];

	printf("FD_VERSION = %s\n\n", FD_VERSION);

	/* 1. 缓存版本与当前程序一致 -> 有效, 不该重建 */
	write_cache(FD_VERSION);
	check("版本一致 -> 缓存有效", VersionCacheIsCurrent() == 1);

	/* 2. 缓存是升级前的旧版本 -> 必须重建 */
	sprintf_s(mismatch, sizeof(mismatch), "%sx", FD_VERSION);
	write_cache(mismatch);
	check("版本串不同(长度也不同) -> 需要重建", VersionCacheIsCurrent() == 0);
	write_cache("0.0.0-old");
	check("任意旧版本号 -> 需要重建", VersionCacheIsCurrent() == (strcmp("0.0.0-old", FD_VERSION) == 0));

	/* 3. 前缀相同但更长 (6.2.0 vs 6.2.01) -> 不能误判为有效 */
	write_cache(mismatch);
	check("前缀相同但更长 -> 需要重建", VersionCacheIsCurrent() == 0);

	/* 4. 缺标记的旧格式/损坏文件 -> 需要重建 */
	write_raw("some other cache\n");
	check("无版本标记 -> 需要重建", VersionCacheIsCurrent() == 0);

	/* 5. 缓存不存在 -> 需要重建 */
	remove(".version");
	check("缓存缺失 -> 需要重建", VersionCacheIsCurrent() == 0);
	check("缓存缺失时 fopen 失败也返回 0", VersionCacheIsCurrent() == 0);

	/* 6. 行尾是 CRLF 也要认出来 */
	write_raw("FreeDownloader版本:");
	{
		FILE *f = fopen(".version", "ab");
		if (f != NULL)
		{
			fprintf(f, "%s\r\n", FD_VERSION);
			fclose(f);
		}
	}
	check("CRLF 行尾 -> 缓存有效", VersionCacheIsCurrent() == 1);

	remove(".version");
	printf("\n%s (%d failure(s))\n", failures == 0 ? "ALL PASS" : "SOME FAILED", failures);
	return failures == 0 ? 0 : 1;
}
