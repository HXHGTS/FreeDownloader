#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>

int AdvanceDownloader(), AutoShutdown(), ChangeUA(), CheckSum(int mode), Dir(), DLEngine(), WindowSkin();
int MagnetDownloader(), MediaDownloader(), Netdisk(), NormalDownloader(), ProxySetting(), threader(), url(), ListenRPC(), About();
int DHT_Fix();
int downloadmode, magnet_mode, ConnectionNum, ProcessNum, config_media, Task, IsCheckSum;
int mark, shutdown, filecheck, DownloadList, OpenDir;
int cookie_import, cookie_mode, appid;
int scan_return; // 接收返回值,暂时没用
char config_proxy[137], config_url[31], config_dir[65], config_cookie[280], smallcmd[20], Downloader_Use[12];
char config_bt_URL[147];					   // 用于存储BT下载时proxy参数与对应tracker地址
char reference[216], head[300], head_show[35]; // 定义请求头文件
char location[200], split[7], torrent_addr[250], play_list[30], color[4];
char proxy[50];																								 // 定义代理设置
char rpctoken[40];																							 // 定义rpc密钥
char BDUSS[193];																							 // 定义BDUSS登录参数
char cmd[300];																								 // 用于存储执行命令
char tracker_URL_CN[82] = "https://fastly.jsdelivr.net/gh/XIU2/TrackersListCollection@master/all_aria2.txt"; // 国内tracker list由CDN加速获取
char tracker_IP_CN[16] = "151.101.109.229";																	 // 国内tracker ip,测试gitea.com获得
// 可用值: fastly.jsdelivr.net testingcf.jsdelivr.net jsdelivr.b-cdn.net
char tracker_URL_NotCN[84] = "https://raw.githubusercontent.com/XIU2/TrackersListCollection/master/all_aria2.txt"; // 海外tracker list源地址获取
FILE *conf, *power_ini, *proxy_ini, *dir_mark, *skin;															   // 定义配置文件
FILE *cookie, *bat, *dht;

int TokenGenerate()
{
	if ((_access("config\\uuid", 0)) == -1)
	{
		system("powershell [guid]::NewGuid() | find /v \"Guid\" | find /v \"--\" | find \"-\" > config\\uuid"); // 随机GUID作为密码,提高RPC功能安全性
	}
	conf = fopen("config\\uuid", "r");
	scan_return = fscanf(conf, "%s", rpctoken);
	fclose(conf);
	return 0;
}

int WindowSkin()
{
	system("color 0B");
	return 0;
}

int About()
{
	printf("------------------------------------------------------------------\n");
	printf("--------------------------FreeDownloader--------------------------\n");
	printf("------------------------------------------------------------------\n");
	printf("FreeDownloader版本:");
	printf("6.0.0");
	printf("\n\n");
	printf("aria2c版本:");
	system("aria2c -v | findstr version | findstr /v either | findstr /v later");
	printf("\n");
	printf("yt-dlp版本:");
	system("yt-dlp --version");
	printf("\n");
	printf("ffmpeg版本:");
	system("ffmpeg -version | findstr \"Copyright\"");
	printf("------------------------------------------------------------------\n");
	system("pause");
	return 0;
}

int CreateConfig()
{
	system("cls");
	printf("正在优化dht链路. . .\n\n");
	if ((_access("config\\dht.dat__temp", 0)) == 0)
	{
		system("del /F /S /Q config\\dht.dat__temp");
	}
	if ((_access("config\\dht6.dat__temp", 0)) == 0)
	{
		system("del /F /S /Q config\\dht6.dat__temp");
	}
	printf("\n正在尝试连接到trackerslist.com服务器. . .\n\n");
	conf = fopen("config\\bt.conf", "w");
	fprintf(conf, "bt-tracker=");
	fclose(conf);
	ProxySetting();
	sprintf_s(cmd, 300, "curl %s -# > config\\all_aria2.txt", config_bt_URL);
	if (system(cmd) != 0)
	{
		printf("\n更新失败,正在本地建立BT配置文件. . .\n");
		system("notepad config\\bt.conf");
	}
	else
	{
		printf("\n更新成功,正在本地建立BT配置文件. . .\n");
		system("type config\\all_aria2.txt >> config\\bt.conf");
		system("del /F /S /Q config\\all_aria2.txt");
	}
	system("cls");
	ChangeUA();
	conf = fopen("config\\bt.conf", "a");
	fprintf(conf, "\n");
	fprintf(conf, "continue=true\n");
	fprintf(conf, "max-concurrent-downloads=1\n");
	fprintf(conf, "max-connection-per-server=16\n");
	fprintf(conf, "enable-dht=true\n");
	fprintf(conf, "enable-dht6=true\n");
	fprintf(conf, "dht-file-path=config\\dht.dat\n");
	fprintf(conf, "dht-file-path6=config\\dht6.dat\n");
	fprintf(conf, "dht-entry-point=dht.transmissionbt.com:6881\n");
	fprintf(conf, "dht-entry-point6=dht.transmissionbt.com:6881\n");
	fprintf(conf, "bt-max-peers=999\n");
	fprintf(conf, "min-split-size=1M\n");
	fprintf(conf, "disk-cache=128M\n");
	fprintf(conf, "split=16\n");
	fprintf(conf, "dir=Downloads/\n");
	fprintf(conf, "max-tries=0\n");
	fprintf(conf, "enable-peer-exchange=true\n");
	fprintf(conf, "content-disposition-default-utf8=true\n");
	fprintf(conf, "disable-ipv6=false\n"); // 开启IPV6支持
	fprintf(conf, "%s\n", head);
	fclose(conf);
	system("cls");
	return 0;
}

int DHT_Fix()
{
	int dht_download_fallback, dht6_download_fallback;
	dht_download_fallback = 0;
	dht6_download_fallback = 0;
	if (_access("config\\dht.dat", 0) == -1)
	{
		printf("DHT数据缺失,正在下载. . .\n");
		dht_download_fallback = system("aria2c -s16 -x16 -k1M --connect-timeout=10 --allow-overwrite=true --dir=config --out=dht.dat \"https://fastly.jsdelivr.net/gh/HXHGTS/FreeDownloader@refs/heads/master/dht.dat\"");
	}
	if (_access("config\\dht6.dat", 0) == -1)
	{
		printf("DHT6数据缺失,正在下载. . .\n");
		dht6_download_fallback = system("aria2c -s16 -x16 -k1M --connect-timeout=10 --allow-overwrite=true --dir=config --out=dht6.dat \"https://fastly.jsdelivr.net/gh/HXHGTS/FreeDownloader@refs/heads/master/dht6.dat\"");
	}
	if (dht_download_fallback != 0 || dht6_download_fallback != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CreateFolder()
{
	if (_access("Downloads", 0) == -1)
	{
		system("mkdir Downloads");
	}
	if (_access("config", 0) == -1)
	{
		system("mkdir config");
	}
	if (_access("cookies", 0) == -1)
	{
		system("mkdir cookies");
	}
	if (_access("temp", 0) == -1)
	{
		system("mkdir temp");
	}
	if (_access("config\\power.ini", 0) == 0)
	{
		system("del /F /S /Q config\\power.ini");
	}
	return 0;
}

int preload()
{
	filecheck = 0;
	shutdown = 0;
	system("title FreeDownloader");
	CreateFolder();
	TokenGenerate();
	WindowSkin();
	if (_access("GetProxyInfo.exe", 0) == -1)
	{
		printf("------------------------------------------------------------------\n");
		printf("无法检测计算机代理设置,以无代理配置运行!\n");
		printf("------------------------------------------------------------------\n");
		system("TIMEOUT /T 2 > nul");
		conf = fopen("config\\proxy.ini", "w");
		fprintf(conf, "proxy=0");
		fclose(conf);
	}
	else
	{
		printf("------------------------------------------------------------------\n");
		printf("正在检测计算机代理设置. . .\n");
		printf("------------------------------------------------------------------\n");
		system("GetProxyInfo");
	}
	return 0;
}
int main()
{
	preload();
MainMenu:
	WindowSkin();
	system("cls");
	printf("------------------------------------------------------------------\n");
	printf("--------------------------FreeDownloader--------------------------\n");
	printf("------------------------------------------------------------------\n");
	printf("请选择下载功能:\n1.普通下载模式\n2.百度网盘模式\n3.视频下载模式\n4.高级下载模式\n5.磁力下载模式\n");
	printf("6.继续上次下载\n7.文件完整性测试\n8.启动RPC被动监听\n9.打开下载文件夹\n10.关于\n0.退出\n");
	printf("------------------------------------------------------------------\n");
	printf("请输入:");
	scan_return = scanf("%d", &downloadmode);
	system("cls");
	if (downloadmode == 1)
	{
		NormalDownloader();
	}
	else if (downloadmode == 2)
	{
		Netdisk();
	}
	else if (downloadmode == 3)
	{
		if (MediaDownloader() == 2)
		{
			goto MainMenu;
		}
	}
	else if (downloadmode == 4)
	{
		AdvanceDownloader();
	}
	else if (downloadmode == 5)
	{
		if (MagnetDownloader() == 2)
		{
			goto MainMenu;
		}
	}
	else if (downloadmode == 6)
	{
		if (fopen("temp\\Download.bat", "r") == NULL)
		{
			goto MainMenu;
		}
	}
	else if (downloadmode == 7)
	{
		IsCheckSum = 1;
		CheckSum(IsCheckSum);
		system("cls");
		goto MainMenu;
	}
	else if (downloadmode == 8)
	{
		ListenRPC();
		system("cls");
		goto MainMenu;
	}
	else if (downloadmode == 9)
	{
		system("explorer Downloads");
		system("cls");
		goto MainMenu;
	}
	else if (downloadmode == 10)
	{
		About();
		goto MainMenu;
	}
	else
	{
		exit(0);
	}
	DLEngine();
	system("rd /s /q %%USERPROFILE%%\\.cache\\yt-dlp");
	system("cls");
	printf("------------------------------------------------------------------\n");
	printf("-----------------------------下载完成!----------------------------\n");
	printf("------------------------------------------------------------------\n");
	AutoShutdown();
	system("explorer Downloads");
	goto MainMenu;
	return 0;
} // 下载工具主程序

int NormalDownloader()
{
	url();
	Dir();
	threader();
	ProxySetting();
	ChangeUA();
	return 0;
}

int ListenRPC()
{
	TokenGenerate();
	printf("正在本地建立RPC配置文件. . .\n");
	conf = fopen("config\\rpc.conf", "w");
	fprintf(conf, "continue=true\n");
	fprintf(conf, "max-concurrent-downloads=1\n");
	fprintf(conf, "max-connection-per-server=16\n");
	fprintf(conf, "min-split-size=1M\n");
	fprintf(conf, "disk-cache=128M\n");
	fprintf(conf, "split=16\n");
	fprintf(conf, "dir=Downloads\n");
	fprintf(conf, "file-allocation=none\n");
	fprintf(conf, "enable-rpc=true\n");
	fprintf(conf, "rpc-allow-origin-all=true\n");
	fprintf(conf, "content-disposition-default-utf8=true\n");
	fprintf(conf, "disable-ipv6=false\n");
	fprintf(conf, "user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Safari/537.36\n");
	fprintf(conf, "rpc-listen-all=true\n");
	fprintf(conf, "rpc-listen-port=6800\n");
	fprintf(conf, "rpc-secret=%s\n", rpctoken);
	fclose(conf);
	ProxySetting();
	conf = fopen("temp\\rpc.bat", "w");
	fprintf(conf, "@echo off\n");
	fprintf(conf, "start /min aria2c --conf-path=config\\rpc.conf\n");
	fclose(conf);
	system("cls");
	printf("------------------------------------------------------------------\n");
	printf("----------------------------RPC监听模式---------------------------\n");
	printf("------------------------------------------------------------------\n");
	printf("RPC监听:允许远程接入\nRPC监听端口:6800\nRPC密码:%s\n", rpctoken);
	printf("------------------------------------------------------------------\n\n");
	printf("RPC监听已启动!\n\n");
	system("temp\\rpc.bat");
	printf("退出");
	system("pause");
	system("taskkill /f /im aria2c*");
	return 0;
}

int MagnetDownloader()
{
	CreateConfig();
	printf("----------------------------------------------------------------------------------\n");
	printf("注意:本模式下代理在下载阶段不起作用,若需要使用代理请配合Tap/Tun工具强制接管流量！\n");
	printf("----------------------------------------------------------------------------------\n");
	printf("请选择下载模式:\n\n1.*.torrent文件\n\n2.Magnet://链接\n\n0.返回\n\n请输入:");
	scan_return = scanf("%d", &magnet_mode);
	if (magnet_mode == 2)
	{
		if (_access("config\\dht.dat", 0) == -1 || _access("config\\dht6.dat", 0) == -1)
		{
			system("cls");
			if (DHT_Fix() == 1)
			{
				printf("DHT数据下载失败,请在使用磁力链下载前前往以下网址下载种子文件后导入下载!\n");
				printf("------------------------------------------------------------------------------------------------\n");
				printf("https://cdimage.debian.org/debian-cd/12.2.0/amd64/bt-cd/debian-12.2.0-amd64-netinst.iso.torrent\n");
				printf("------------------------------------------------------------------------------------------------\n");
				printf("务必导入并下载此种子!否则磁力链下载无速度!\n");
				system("pause > nul");
				exit(0);
			}
		}
		Dir();
	}
	else if (magnet_mode == 0)
	{
		return 2;
	}
	threader();
	system("cls");
	url();
	return 0;
}

int url()
{
	FILE *url;
	if (downloadmode == 1)
	{
		if (fopen("temp\\normal.download", "r") == NULL)
		{
			url = fopen("temp\\normal.download", "w");
			fclose(url);
		}
		system("cls");
		printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
		system("notepad temp\\normal.download");
		sprintf_s(config_url, 30, "%s", "-i temp\\normal.download");
	}
	else if (downloadmode == 2)
	{
		if (fopen("temp\\netdisk.download", "r") == NULL)
		{
			url = fopen("temp\\netdisk.download", "w");
			fclose(url);
		}
		system("cls");
		printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
		system("notepad temp\\netdisk.download");
		sprintf_s(config_url, 30, "%s", "-i temp\\netdisk.download");
	}
	else if (downloadmode == 3)
	{
		if (config_media == 1)
		{
			if (fopen("temp\\ytb.download", "r") == NULL)
			{
				url = fopen("temp\\ytb.download", "w");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad temp\\ytb.download");
			sprintf_s(config_url, 30, "%s", "-a temp\\ytb.download");
		}
		else if (config_media == 2)
		{
			if (fopen("temp\\Bilibili.download", "r") == NULL)
			{
				url = fopen("temp\\Bilibili.download", "w");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad temp\\Bilibili.download");
			sprintf_s(config_url, 30, "%s", "-F temp\\Bilibili.download");
		}
		else if (config_media == 3)
		{
			if (fopen("temp\\QQVideo.download", "r") == NULL)
			{
				url = fopen("temp\\QQVideo.download", "w");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad temp\\QQVideo.download");
			sprintf_s(config_url, 30, "%s", "-F temp\\QQVideo.download");
		}
		else if (config_media == 4)
		{
			if (fopen("temp\\iqiyi.download", "r") == NULL)
			{
				url = fopen("temp\\iqiyi.download", "w");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad temp\\iqiyi.download");
			sprintf_s(config_url, 30, "%s", "-F temp\\iqiyi.download");
		}
		else if (config_media == 5)
		{
			if (fopen("temp\\Youku.download", "r") == NULL)
			{
				url = fopen("temp\\Youku.download", "w");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad temp\\Youku.download");
			sprintf_s(config_url, 30, "%s", "-F temp\\Youku.download");
		}
		else
		{
			if (fopen("temp\\Other.download", "r") == NULL)
			{
				url = fopen("temp\\Other.download", "w");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad temp\\Other.download");
			if (config_media == 6)
			{
				sprintf_s(config_url, 30, "%s", "-a temp\\Other.download");
			}
			else
			{
				sprintf_s(config_url, 30, "%s", "-F temp\\Other.download");
			}
		}
	}
	else if (downloadmode == 4)
	{
		if (fopen("temp\\advance.download", "r") == NULL)
		{
			url = fopen("temp\\advance.download", "w");
			fclose(url);
		}
		system("cls");
		printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
		system("notepad temp\\advance.download");
		sprintf_s(config_url, 30, "%s", "-i temp\\advance.download");
	}
	else if (downloadmode == 5)
	{
		if (magnet_mode == 2)
		{
			if (fopen("temp\\magnet.download", "r") == NULL)
			{
				url = fopen("temp\\magnet.download", "w");
				fprintf(url, "magnet:?xt=urn:btih:");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad temp\\magnet.download");
			sprintf_s(config_url, 30, "%s", "-i temp\\magnet.download");
		}
		else
		{
			system("cls");
			printf("若种子文件名过长建议重命名成简单字母或数字再拖入窗口,否则可能报错！\n\n");
			printf("请将种子文件以拖拽至本窗口中:");
			scan_return = scanf("%s", torrent_addr);
		}
	}
	return 0;
} // 网址输入插件

int threader()
{
	if (downloadmode == 1 || downloadmode == 4)
	{
		Task = 1; // 同时下载任务数
		ConnectionNum = 16;
		ProcessNum = 16;
		sprintf_s(Downloader_Use, 12, "%s", "aria2c");
		sprintf_s(split, 3, "1M");
	}
	else if (downloadmode == 2)
	{
		ConnectionNum = 16;
		ProcessNum = 16;
		sprintf_s(split, 3, "1M");
		sprintf_s(Downloader_Use, 12, "%s", "aria2c");
		Task = 1; // 同时下载任务数
	}
	else if (downloadmode == 3)
	{
		Task = 1; // 同时下载任务数
		if (config_media == 1 || config_media == 6)
		{
			sprintf_s(Downloader_Use, 12, "%s", "yt-dlp");
		}
		else
		{
			sprintf_s(Downloader_Use, 12, "%s", "lux");
		}
		ConnectionNum = 1; // 4k视频状态下aria2调用出现bug
	}
	else if (downloadmode == 5)
	{
		Task = 1; // 同时下载任务数
		sprintf_s(Downloader_Use, 12, "%s", "aria2c");
		ConnectionNum = 16;
		if (magnet_mode == 2)
			sprintf_s(split, 3, "1M");
	}
	return 0;
} // 线程数修改与引擎选择

int Dir()
{
	if (downloadmode == 3)
	{
		if (config_media == 1 || config_media == 6)
		{
			sprintf_s(config_dir, 65, "%s", "-o Downloads\\%%(uploader)s-%%(title)s-%%(resolution)s.%%(ext)s");
		}
		else
		{
			sprintf_s(config_dir, 16, "%s", "-o Downloads");
		}
	}
	else
	{
		if (downloadmode == 5 && magnet_mode == 1)
		{
		}
		else
			sprintf_s(config_dir, 21, "%s", "--dir=\"Downloads\"");
	}
	return 0;
}

int ProxySetting()
{
	char ProxyChoose[3];
	int ReturnResult = 0;
	if (system("type config\\proxy.ini | find \"proxy=0\"") == 0)
	{
		if (downloadmode == 5)
		{
			sprintf_s(config_bt_URL, 146, "--resolve fastly.jsdelivr.net:443:%s %s", tracker_IP_CN, tracker_URL_CN);
			sprintf_s(config_proxy, 1, "");
		}
		else
		{
			sprintf_s(config_proxy, 1, "");
		}
	}
	else
	{
		printf("\n");
		printf("是否使用代理设置(是=Y/否=N)?");
		ReturnResult = scanf("%s", ProxyChoose);
		if (ProxyChoose[0] == 'Y' || ProxyChoose[0] == 'y') // 使用代理
		{
			proxy_ini = fopen("config\\proxy.ini", "r");
			scan_return = fscanf(proxy_ini, "proxy=%s", proxy);
			fclose(proxy_ini);
			if (downloadmode == 1 || downloadmode == 2 || downloadmode == 4)
			{
				sprintf_s(config_proxy, 65, "--all-proxy=%s", proxy);
			}
			else if (downloadmode == 3)
			{
				sprintf_s(config_proxy, 137, "set http_proxy=%s\nset https_proxy=%s", proxy, proxy);
			}
			else if (downloadmode == 5)
			{
				sprintf(config_bt_URL, "-x %s %s", proxy, tracker_URL_NotCN);
				sprintf(config_proxy, "-x %s", proxy);
			}
			else if (downloadmode == 8)
			{
				sprintf_s(config_proxy, 1, "");
			}
		}
		else // 不使用代理
		{
			if (downloadmode == 5)
			{
				sprintf_s(config_bt_URL, 146, "--resolve fastly.jsdelivr.net:443:%s %s", tracker_IP_CN, tracker_URL_CN);
				sprintf_s(config_proxy, 1, "");
			}
			else
			{
				sprintf_s(config_proxy, 1, "");
			}
		}
	}
	system("cls");
	return 0; // /config/proxy.ini中proxy=0或此文件不存在为无代理状态,否则使用代理(仅支持http/https代理)
}

int ChangeUA()
{
	if (downloadmode == 1)
	{
		sprintf(head, "-U \"%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Safari/537.36"); // Chrome浏览器
		sprintf(head_show, "Chrome");
	}
	else if (downloadmode == 2)
	{
		sprintf(head, "-U \"%s\"", "pan.baidu.com"); // 百度网盘客户端
		sprintf(head_show, "Netdisk");
	}
	else if (downloadmode == 3)
	{
		if (config_media == 1 || config_media == 6)
		{
			sprintf(head_show, "Default"); // yt-dlp,默认UserAgent
		}
		else if (config_media == 3)
		{
			sprintf(head, "--user-agent=%s", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Safari/537.36"); // Chrome浏览器
			sprintf(head_show, "Chrome");
		}
		else
		{
			sprintf(head, "--user-agent=%s", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Safari/537.36"); // Chrome浏览器
			sprintf(head_show, "Chrome");
		}
	}
	else if (downloadmode == 4)
	{
		printf("\n请选择浏览器标识:\n\n1.IE\n\n2.Chrome\n\n3.Chrome(Mobile)\n\n4.Chrome(Linux)\n\n请输入:");
		scan_return = scanf("%d", &mark);
		if (mark == 1)
		{
			sprintf(head, "-U \"%s\"", "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 10.0; WOW64; Trident/7.0; .NET4.0C; .NET4.0E)"); // IE浏览器
			sprintf(head_show, "IE");
		}
		else if (mark == 2)
		{
			sprintf(head, "-U \"%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Safari/537.36"); // Chrome浏览器
			sprintf(head_show, "Chrome");
		}
		else if (mark == 3)
		{
			sprintf(head, "-U \"%s\"", "Mozilla/5.0 (Linux; Android 10; K) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Mobile Safari/537.36"); // Chrome浏览器
			sprintf(head_show, "Chrome(Mobile)");
		}
		else if (mark == 4)
		{
			sprintf(head, "-U \"%s\"", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Safari/537.36"); // Chrome浏览器(Linux)
			sprintf(head_show, "Chrome(Linux)");
		}
		else
		{
			mark = 2;
			sprintf(head, "-U \"%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Safari/537.36"); // Chrome浏览器
			sprintf(head_show, "Chrome");
		}
	}
	else if (downloadmode == 5)
	{
		printf("\n请选择BT下载工具标识:\n\n1.qBittorrent\n\n2.Transmission\n\n请输入:");
		scan_return = scanf("%d", &mark);
		if (mark == 1)
		{
			sprintf(head, "user-agent=%s\npeer-agent=%s\npeer-id-prefix=%s", "qBittorrent/5.0.4", "qBittorrent/5.0.4", "-qB5040-"); // qBittorrent
			sprintf(head_show, "qBittorrent/5.0.4");
		}
		else if (mark == 2)
		{
			sprintf(head, "user-agent=%s\npeer-agent=%s\npeer-id-prefix=%s", "Transmission/4.1.0-beta.1", "Transmission/4.1.0-beta.1", "-TR410B-"); // Transmission
			sprintf(head_show, "Transmission/4.1.0-beta.1");
		}
		else
		{
			mark = 2;
			sprintf(head, "user-agent=%s\npeer-agent=%s\npeer-id-prefix=%s", "Transmission/4.1.0-beta.1", "Transmission/4.1.0-beta.1", "-TR410B-"); // Transmission
			sprintf(head_show, "Transmission/4.1.0-beta.1");
		}
	}
	return 0;
}

int AdvanceDownloader()
{
	int cookie_import;
	char reference_input[200];
	FILE *cookie;
	ChangeUA();
	printf("\n请输入下载引用页地址:\n");
	scan_return = scanf("%s", reference_input);
	sprintf(reference, "--referer=\"%s\"", reference_input);
	if (fopen("cookies\\Cookies.txt", "r") == NULL)
	{
	p_4:
		cookie = fopen("cookies\\Cookies.txt", "w");
		fprintf(cookie, "# Input Cookie below#\n");
		fclose(cookie);
		printf("\n请在弹出窗口中导入Cookies信息,");
		system("notepad cookies\\Cookies.txt");
		system("pause");
	}
	else
	{
		printf("\n检测到存在Cookies信息,是否继续使用上次的信息登录（是=1 否=0）:");
		scan_return = scanf("%d", &cookie_import);
		if (cookie_import != 1)
		{
			goto p_4;
		}
	}
	sprintf(config_cookie, "--load-cookies=\"cookies\\Cookies.txt\"");
	url();
	Dir();
	ProxySetting();
	threader();
	return 0;
}

int Netdisk()
{
	ChangeUA();
	// sprintf(reference, "%s", "--referer=\"https://pan.baidu.com/disk/home?from=newversion&stayAtHome=true#/all?path=%2F&vmode=list\"");
	printf("\n是否使用插件导入Cookie(1=插件手动导入 0=浏览器手动导入):");
	scan_return = scanf("%d", &cookie_mode);
	if (cookie_mode != 0)
	{
		if (fopen("cookies\\Netdisk_Cookies.txt", "r") == NULL)
		{
		p_4:
			cookie = fopen("temp\\Netdisk_Cookies_tmp.txt", "w");
			fprintf(cookie, "# Input Cookie below#\n");
			fclose(cookie);
			printf("\n请在弹出窗口中导入百度网盘Cookies信息 . . .\n");
			system("notepad temp\\Netdisk_Cookies_tmp.txt");
			system("type temp\\Netdisk_Cookies_tmp.txt | find \"BDUSS\" | find /v \"BDUSS_BFESS\" > cookies\\Netdisk_Cookies.txt");
			system("del temp\\Netdisk_Cookies_tmp.txt");
		}
		else
		{
			printf("\n检测到存在Cookies信息,是否继续使用上次的信息登录（是=1 否=0）:");
			scan_return = scanf("%d", &cookie_import);
			if (cookie_import != 1)
			{
				goto p_4;
			}
		}
		sprintf(config_cookie, "--load-cookies=\"cookies\\Netdisk_Cookies.txt\"");
	}
	else
	{
		if (fopen("cookies\\BDUSS.txt", "r") == NULL)
		{
		p_5:
			printf("\n请输入BDUSS值:\n");
			scan_return = scanf("%s", BDUSS);

			cookie = fopen("cookies\\BDUSS.txt", "w");
			fprintf(cookie, "%s", BDUSS);
			fclose(cookie);
		}
		else
		{
			printf("\n检测到存在Cookies信息,是否继续使用上次的信息登录（是=1 否=0）:");
			scan_return = scanf("%d", &cookie_import);
			if (cookie_import != 1)
			{
				goto p_5;
			}
		}
		cookie = fopen("cookies\\BDUSS.txt", "r");
		scan_return = fscanf(cookie, "%s", BDUSS);
		fclose(cookie);

		sprintf(config_cookie, "--header=\"Cookie: BDUSS=%s\"", BDUSS);
	}
	url();
	Dir();
	threader();
	return 0;
}

int CheckSum(int mode)
{
	if (mode == 1)
	{
		printf("\n如果需要检测文件完整性,可以将文件拖拽至本窗口内！建议先将文件名称最简化并移至磁盘根目录下再检测！\n");
		printf("\n文件名称中不允许空格！若md5不一致,则文件损坏,需重新下载！\n\n文件路径:");
		scan_return = scanf("%s", location);
		sprintf(cmd, "CertUtil -hashfile \"%s\" MD5", location);
		printf("\n\n正在计算MD5值. . .\n");
		printf("---------------------------------------------\n");
		system(cmd);
		printf("\n\n");
		sprintf(cmd, "CertUtil -hashfile \"%s\" SHA1", location);
		printf("正在计算SHA1值. . .\n");
		printf("---------------------------------------------\n");
		system(cmd);
		printf("\n\n");
		sprintf(cmd, "CertUtil -hashfile \"%s\" SHA256", location);
		printf("正在计算SHA256值. . .\n");
		printf("---------------------------------------------\n");
		system(cmd);
		printf("\n\n");
		sprintf(cmd, "CertUtil -hashfile \"%s\" SHA512", location);
		printf("正在计算SHA512值. . .\n");
		printf("---------------------------------------------\n");
		system(cmd);
		printf("\n\n");
		system("pause");
	}
	return 0;
}

int AutoShutdown()
{
	if (fopen("config\\power.ini", "r") != NULL)
	{
		shutdown = 1; // 设置后只生效一次,自动还原状态,避免每次都自动关机
	}
	else
	{
		shutdown = 0;
	}
	if (shutdown == 1)
	{
		printf("正在执行关机操作. . .\n");
		system("shutdown -s -t 30");
		exit(0);
	}
	return 0;
}

int MediaDownloader()
{
	FILE *Bilibili_Cookies, *ytb_Cookies, *QQVideo_Cookies, *iqiyi_Cookies, *Youku_Cookies, *Other_Cookies;
	char chapter[10];
	ProxySetting();
	printf("下载来源:\n\n1.油管\n\n2.哔哩哔哩\n\n3.腾讯视频\n\n4.爱奇艺\n\n5.优酷\n\n6.其它(调用yt-dlp)\n\n7.其它(调用lux)\n\n0.返回\n\n请输入:");
	scan_return = scanf("%d", &config_media);
	system("cls");
	if (config_media == 1 || config_media == 6)
	{ // 调用yt-dlp
		printf("\n下载整个列表内所有音视频?\n\n1.是\n\n2.只下载当前视频\n\n0.选择集数\n\n请输入:");
		scan_return = scanf("%d", &DownloadList);
		if (DownloadList == 1)
		{
			sprintf(play_list, "--yes-playlist");
		}
		else if (DownloadList == 2)
		{
			sprintf(play_list, "--no-playlist");
		}
		else
		{
			system("cls");
			printf("请按照格式输入下载范围,如1-5,6,7,8-15:");
			scan_return = scanf("%s", chapter);
			sprintf(play_list, "--playlist-items %s", chapter);
		}
		system("cls");
	}
	else if (config_media != 0)
	{ // 调用yt-dlp
		printf("\n下载整个列表内所有音视频?\n\n1.是\n\n2.只下载当前视频\n\n请输入:");
		scan_return = scanf("%d", &DownloadList);
		if (DownloadList == 1)
		{
			sprintf(play_list, "-p");
		}
		else if (DownloadList == 2)
		{
			sprintf(play_list, " ");
		}
	}
	else
	{
		return 2;
	}
	if (config_media == 1)
	{
		if (fopen("cookies\\ytb_Cookies.txt", "r") == NULL)
		{
			ytb_Cookies = fopen("cookies\\ytb_Cookies.txt", "w");
			fprintf(ytb_Cookies, "# Input Cookie below#\n");
			fclose(ytb_Cookies);
		}
		system("notepad cookies\\ytb_Cookies.txt");
	}
	else if (config_media == 2)
	{
		if (fopen("cookies\\Bilibili_Cookies.txt", "r") == NULL)
		{
			Bilibili_Cookies = fopen("cookies\\Bilibili_Cookies.txt", "w");
			fprintf(Bilibili_Cookies, "# Input Cookie below#\n");
			fclose(Bilibili_Cookies);
		}
		system("notepad cookies\\Bilibili_Cookies.txt");
	}
	else if (config_media == 3)
	{
		if (fopen("cookies\\QQVideo_Cookies.txt", "r") == NULL)
		{
			QQVideo_Cookies = fopen("cookies\\QQVideo_Cookies.txt", "w");
			fprintf(QQVideo_Cookies, "# Input Cookie below#\n");
			fclose(QQVideo_Cookies);
		}
		system("notepad cookies\\QQVideo_Cookies.txt");
	}
	else if (config_media == 4)
	{
		if (fopen("cookies\\iqiyi_Cookies.txt", "r") == NULL)
		{
			iqiyi_Cookies = fopen("cookies\\iqiyi_Cookies.txt", "w");
			fprintf(iqiyi_Cookies, "# Input Cookie below#\n");
			fclose(iqiyi_Cookies);
		}
		system("notepad cookies\\iqiyi_Cookies.txt");
	}
	else if (config_media == 5)
	{
		if (fopen("cookies\\Youku_Cookies.txt", "r") == NULL)
		{
			Youku_Cookies = fopen("cookies\\Youku_Cookies.txt", "w");
			fprintf(Youku_Cookies, "# Input Cookie below#\n");
			fclose(Youku_Cookies);
		}
		system("notepad cookies\\Youku_Cookies.txt");
	}
	else
	{
		if (fopen("cookies\\Other_Cookies.txt", "r") == NULL)
		{
			Other_Cookies = fopen("cookies\\Other_Cookies.txt", "w");
			fprintf(Other_Cookies, "# Input Cookie below#\n");
			fclose(Other_Cookies);
		}
		system("notepad cookies\\Other_Cookies.txt");
	}
	system("cls");
	printf("请在弹出窗口中导入Cookies,同一账号可以反复使用\n\n");
	url();
	threader();
	Dir();
	ChangeUA();
	return 0;
}

int DLEngine()
{
	FILE *Download;
	char *CoreDir = _getcwd(NULL, 0);
	if (downloadmode >= 1 && downloadmode <= 5)
	{
		Download = fopen("temp\\Download.bat", "w");
		fprintf(Download, "@echo off\n");
		fprintf(Download, "set PATH=%s\n", CoreDir);
		if (downloadmode == 1)
		{
			fprintf(Download, "%s -c -x%d -s%d -k%s --follow-torrent=false --content-disposition-default-utf8=true -j %d %s %s %s %s\n", Downloader_Use, ConnectionNum, ProcessNum, split, Task, config_dir, config_proxy, head, config_url);
			fclose(Download);
		}
		else if (downloadmode == 2)
		{
			// fprintf(Download, "%s -c -x%d -s%d --follow-torrent=false -k%s -j %d %s %s %s %s --content-disposition-default-utf8=true %s\n", Downloader_Use, ConnectionNum, ProcessNum, split, Task, config_dir, reference, head, config_cookie, config_url);
			fprintf(Download, "%s -c -x%d -s%d --follow-torrent=false -k%s -j %d %s %s %s --content-disposition-default-utf8=true %s\n", Downloader_Use, ConnectionNum, ProcessNum, split, Task, config_dir, head, config_cookie, config_url);
			fclose(Download);
		}
		else if (downloadmode == 3)
		{
			fprintf(Download, "%s\n", config_proxy);
			if (config_media == 1)
			{
				fprintf(Download, "%s --cookies cookies\\ytb_Cookies.txt -f \"bv[ext=mp4]+ba[ext=m4a]/b[ext=mp4]\" --merge-output-format mp4 --embed-thumbnail --embed-metadata --write-sub --write-auto-subs --sub-langs \"en,zh-Hans,zh-Hant\" --convert-subs srt %s %s %s --downloader aria2c --downloader-args \"aria2c:-s4 -x4 -k1M\"\n", Downloader_Use, play_list, config_dir, config_url);
			}
			else if (config_media == 2)
			{
				fprintf(Download, "%s -c cookies\\Bilibili_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			}
			else if (config_media == 3)
			{
				fprintf(Download, "%s -c cookies\\QQVideo_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			}
			else if (config_media == 4)
			{
				fprintf(Download, "%s -c cookies\\iqiyi_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			}
			else if (config_media == 5)
			{
				fprintf(Download, "%s -c cookies\\Youku_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			}
			else if (config_media == 6)
			{
				fprintf(Download, "%s --cookies cookies\\Other_Cookies.txt -f \"bv[ext=mp4]+ba[ext=m4a]/b[ext=mp4]\" --merge-output-format mp4 --embed-thumbnail --embed-metadata --write-sub --write-auto-subs --sub-langs \"en,zh-Hans,zh-Hant\" --convert-subs srt %s %s %s --downloader aria2c --downloader-args \"aria2c:-s4 -x4 -k1M\"\n", Downloader_Use, play_list, config_dir, config_url);
			}
			else
			{
				fprintf(Download, "%s -c cookies\\Other_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			}
		}
		else if (downloadmode == 4)
		{
			fprintf(Download, "%s -c -x%d -s%d -k%s -j %d %s %s %s %s %s --max-tries=0 --content-disposition-default-utf8=true %s\n", Downloader_Use, ConnectionNum, ConnectionNum, split, Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
		}
		else if (downloadmode == 5)
		{
			if (magnet_mode == 2)
			{
				fprintf(Download, "%s --conf-path=config\\bt.conf %s\n", Downloader_Use, config_url); // Magnet磁力链
			}
			else
			{
				fprintf(Download, "%s --conf-path=config\\bt.conf %s\n", Downloader_Use, torrent_addr); // 种子文件
			}
		}
		fprintf(Download, "exit 0\n");
		fclose(Download);
	}
	system("cls");
	printf("------------------------------------------------------------------\n");
	if (downloadmode >= 1 && downloadmode < 5)
	{
		printf("代理地址:%s(为空代表没有设置代理)\n", config_proxy);
		printf("下载线程数:%d\n", ConnectionNum);
		printf("下载引擎:%s\n", Downloader_Use);
		printf("浏览器标识:%s\n", head_show);
	}
	else if (downloadmode = 5)
	{
		printf("代理地址:(为空代表没有设置代理)\n");
		printf("下载线程数:%d\n", ConnectionNum);
		printf("下载引擎:%s\n", Downloader_Use);
		printf("浏览器标识:%s\n", head_show);
	}
	else
	{
		printf("下载配置:依照上一次的设置\n");
	}
	printf("------------------------------------------------------------------\n");
	printf("下载过程中出现的红色ERROR报错可忽略,对下载没有影响！\n");
	printf("下载正在执行,希望中断下载建议按Ctrl+C以正常退出. . .\n");
	printf("------------------------------------------------------------------\n");
	system("temp\\Download.bat");
	return 0;
}
