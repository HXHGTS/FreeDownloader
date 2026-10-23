/* Regression harness for CleanupDHTTempFiles() in FreeDownloader.c.
 *
 * 背景: aria2 保存 DHT 路由表时先写 "<目标文件>__temp", 再用 MoveFileEx 覆盖
 * 目标文件; Windows 上这次重命名会因为文件被占用 (残留进程 / 杀软扫描) 而失败,
 * 于是 config\dht.dat__temp / dht6.dat__temp 就留在目录里. 这个用例验证清理逻辑
 * 能删掉各种残留形态, 同时不误伤正常配置、也不能把被独占的文件谎报成已删.
 *
 * 为什么直接 include 源文件: CleanupDHTTempFiles 是 static, 只能同编译单元调用;
 * 这里用 #define main 把程序的 main 改名后整体包含 (与 bilibili-url-normalize.c 同法).
 *
 * 构建 (已执行 vcvars64.bat, 或 Developer Command Prompt):
 *   cl /nologo /W0 /utf-8 /MT /D_CRT_SECURE_NO_WARNINGS /Fo:<obj目录>\ /Fe:regress-dht.exe tests\dht-temp-cleanup.c getproxyinfo.c
 *
 * 运行 (工作目录里需要有可写的 config 子目录, 脚本自己会建):
 *   regress-dht.exe
 *
 * 退出码: 0=全部符合预期  1=有不符合预期的项
 *
 * 用例:
 *   1) config\dht.dat__temp         普通残留              -> 被删
 *   2) config\dht6.dat__temp        只读+隐藏属性         -> 清属性后被删
 *   3) config\dht.dat__temp_file    aria2 的其它临时后缀 -> 被通配扫描删掉
 *   4) config\dht.dat__temp_locked  被独占打开            -> 删不掉, failed 计数 +1
 *   5) config\keep.dat              普通配置              -> 必须原样保留
 */
#define main fd_real_main
#include "../FreeDownloader.c"
#undef main

static int failures = 0;

static void check(const char *what, int ok)
{
	printf("%-46s %s\n", what, ok ? "PASS" : "FAIL");
	if (!ok)
	{
		failures++;
	}
}

static int exists(const char *path)
{
	return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}

static void make_file(const char *path, DWORD attr)
{
	FILE *f;

	f = fopen(path, "wb");
	if (f != NULL)
	{
		fputs("x", f);
		fclose(f);
	}
	if (attr != FILE_ATTRIBUTE_NORMAL)
	{
		SetFileAttributesA(path, attr);
	}
}

int main(void)
{
	HANDLE busy;
	int cleaned, failed, cleaned_again, failed_again;

	system("if not exist config mkdir config"); /* 标签里不能出现 & 之类, 保持简单 */
	remove("config\\keep.dat");
	remove("config\\dht.dat__temp");
	remove("config\\dht6.dat__temp");
	remove("config\\dht.dat__temp_file");
	remove("config\\dht.dat__temp_locked");

	make_file("config\\keep.dat", FILE_ATTRIBUTE_NORMAL);
	make_file("config\\dht.dat__temp", FILE_ATTRIBUTE_NORMAL);
	make_file("config\\dht6.dat__temp", FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);
	make_file("config\\dht.dat__temp_file", FILE_ATTRIBUTE_NORMAL);
	make_file("config\\dht.dat__temp_locked", FILE_ATTRIBUTE_NORMAL);

	/* 只给读共享, 不给 FILE_SHARE_DELETE -> DeleteFileA 会拿共享冲突 */
	busy = CreateFileA("config\\dht.dat__temp_locked", GENERIC_READ, FILE_SHARE_READ,
					   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	cleaned = CleanupDHTTempFiles(&failed);
	printf("cleaned=%d failed=%d\n", cleaned, failed);

	check("普通残留 dht.dat__temp 已删", !exists("config\\dht.dat__temp"));
	check("只读+隐藏的 dht6.dat__temp 已删", !exists("config\\dht6.dat__temp"));
	check("其它后缀 dht.dat__temp_file 已删", !exists("config\\dht.dat__temp_file"));
	check("被占用的 dht.dat__temp_locked 仍在", exists("config\\dht.dat__temp_locked"));
	check("正常文件 keep.dat 未被误删", exists("config\\keep.dat"));
	check("cleaned 计数=3", cleaned == 3);
	check("failed 计数=1", failed == 1);

	if (busy != INVALID_HANDLE_VALUE)
	{
		CloseHandle(busy);
	}

	/* 释放占用后再清一次: 该删的删, 不该报失败的别报 */
	cleaned_again = CleanupDHTTempFiles(&failed_again);
	printf("after release: cleaned=%d failed=%d\n", cleaned_again, failed_again);
	check("释放占用后 locked 文件被删", !exists("config\\dht.dat__temp_locked"));
	check("第二次 cleaned=1", cleaned_again == 1);
	check("第二次 failed=0", failed_again == 0);

	remove("config\\keep.dat");

	printf("\n%s (%d failure(s))\n", failures == 0 ? "ALL PASS" : "SOME FAILED", failures);
	return failures == 0 ? 0 : 1;
}
