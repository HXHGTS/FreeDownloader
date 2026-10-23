/* Regression harness for NormalizeBilibiliUrls() in FreeDownloader.c.
 *
 * 为什么直接 include 源文件而不是链接: NormalizeBilibiliUrls 是 static,
 * 只能在同一编译单元里调用; 这里用 #define main 把程序的 main 改名后整体包含。
 *
 * 构建 (已执行过 vcvars64.bat, 或 Developer Command Prompt):
 *   cl /nologo /W0 /MT /D_CRT_SECURE_NO_WARNINGS /Fo:<obj目录>\ /Fe:regress-bili.exe tests\bilibili-url-normalize.c
 *
 * 运行:
 *   regress-bili.exe list.txt
 *
 * 退出码: 0=正常执行(看 changed 输出)  2=缺少参数  3=打不开修正后的副本
 *
 * 预期行为 (changed=1):
 *   https://www.bilibili.com/zh-Hans/video/BV1xx  ->  https://www.bilibili.com/video/BV1xx
 *   https://bilibili.com/zh-CN/video/BV1yy         ->  https://bilibili.com/video/BV1yy
 *   非 B 站链接 / 已无语言前缀的链接               ->  原样保留
 *   原列表文件本身不被改写, 修正内容写入同目录 urlfix.tmp
 */
#define main fd_real_main
#include "../FreeDownloader.c"
#undef main

int main(int argc, char **argv)
{
	char out[64];
	int r;
	FILE *f;
	char buf[8192];
	size_t n;

	if (argc < 2)
	{
		printf("usage: regress-bili <listfile>\n");
		return 2;
	}
	memset(out, 0, sizeof(out));
	r = NormalizeBilibiliUrls(argv[1], out, sizeof(out));
	printf("changed=%d\n", r);
	if (r)
	{
		printf("fixed_file=%s\n", out);
		f = fopen(out, "rb");
		if (f == NULL)
		{
			printf("ERROR: cannot open fixed file\n");
			return 3;
		}
		printf("--- fixed content ---\n");
		while ((n = fread(buf, 1, sizeof(buf) - 1, f)) > 0)
		{
			buf[n] = 0;
			printf("%s", buf);
		}
		printf("\n--- end ---\n");
		fclose(f);
	}
	return 0;
}
