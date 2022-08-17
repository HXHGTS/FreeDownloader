#include<stdio.h>
#include<stdlib.h>
#include<io.h>


int AdvanceDownloader(),AutoShutdown(),ChangeUA(),CheckSum(int mode),Dir(),DLEngine(),WindowSkin();
int MagnetDownloader(),MediaDownloader(),Netdisk(),NormalDownloader(),ProxySetting(),threader(),url();
int downloadmode, magnet_mode,ConnectionNum, ProcessNum, config_media, Task, IsCheckSum;
int mark, shutdown, filecheck, DownloadList, OpenDir;
int cookie_import, cookie_mode,appid;
int scan_return;//接收返回值,暂时没用
char config_proxy[65], config_url[30], config_dir[65], config_cookie[280], smallcmd[20], Downloader_Use[12];
char config_bt_URL[142];//用于存储BT下载时proxy参数与对应tracker地址
char reference[216], head[300], head_show[35];//定义请求头文件
char location[200],split[7],torrent_addr[250],play_list[30], color[4];
char proxy[50];//定义代理设置
char rpctoken[40];//定义rpc密钥
char BDUSS[193],pcsett[45];//定义BDUSS与pcsett登录参数
char cmd[300];//用于存储执行命令
char tracker_URL_CN[79] = "https://cdn.staticaly.com/gh/XIU2/TrackersListCollection/master/best_aria2.txt";//国内tracker list加速
char tracker_URL_NotCN[84] = "https://raw.githubusercontent.com/XIU2/TrackersListCollection/master/best_aria2.txt";//海外tracker list加速
FILE* conf,*power_ini,*proxy_ini,*dir_mark, *skin;//定义配置文件
FILE* cookie,*bat,*dht;

int TokenGenerate() {
	if ((_access("config\\uuid", 0))==-1) {
		system("powershell [guid]::NewGuid() | find /v \"Guid\" | find /v \"--\" | find \"-\" > config\\uuid");//随机GUID作为密码,提高RPC功能安全性
	}
	conf = fopen("config\\uuid", "r");
	scan_return=fscanf(conf,"%s", rpctoken);
	fclose(conf);
	return 0;
}

int WindowSkin() {
	system("color 0B");
	return 0;
}

int CreateConfig() {
	system("cls");
	printf("正在优化dht链路. . .\n\n");
	if ((_access("config\\dht.dat__temp", 0))==0) {
		system("del /F /S /Q config\\dht.dat__temp");
	}
	if ((_access("config\\dht6.dat__temp", 0)) == 0) {
		system("del /F /S /Q config\\dht6.dat__temp");
	}
	printf("\n正在尝试连接到trackerslist.com服务器. . .\n\n");
	conf = fopen("config\\bt.conf", "w");
	fprintf(conf, "bt-tracker=");
	fclose(conf);
	ProxySetting();
	sprintf(cmd, "curl %s -# > config\\best_aria2.txt",config_bt_URL);
	if (system(cmd) != 0) {
		printf("\n更新失败,正在本地建立BT配置文件. . .\n");
		system("notepad config\\bt.conf");
	}
	else {
		printf("\n更新成功,正在本地建立BT配置文件. . .\n");
		system("type config\\best_aria2.txt >> config\\bt.conf");
		system("del /F /S /Q config\\best_aria2.txt");
	}
	system("cls");
	ChangeUA();
	conf = fopen("config\\bt.conf", "a");
	fprintf(conf, "\ncontinue=true\n");
	fprintf(conf, "max-concurrent-downloads=1\n");
	fprintf(conf, "max-connection-per-server=16\n");
	fprintf(conf, "dht-entry-point=router.bittorrent.com:6881\n");
	fprintf(conf, "dht-entry-point6=router.bittorrent.com:6881\n");
	fprintf(conf, "dht-file-path=config\\dht.dat\n");
	fprintf(conf, "dht-file-path6=config\\dht6.dat\n");
	fprintf(conf, "enable-dht6=true\n");
	fprintf(conf, "bt-max-peers=999\n");
	fprintf(conf, "min-split-size=1M\n");
	fprintf(conf, "disk-cache=128M\n");
	fprintf(conf, "split=16\n");;
	fprintf(conf, "dir=Downloads/\n");
	fprintf(conf, "max-tries=0\n");
	fprintf(conf, "enable-peer-exchange=true\n");
	fprintf(conf, "content-disposition-default-utf8=true\n");
	fprintf(conf, "disable-ipv6=false\n");//开启IPV6支持
	fprintf(conf, "%s\n",head);
	fclose(conf);
	system("cls");
	return 0;
}

int CreateFolder() {
	if (_access("Downloads", 0)==-1) {
		system("mkdir Downloads");
	}
	if (_access("config", 0) == -1) {
		system("mkdir config");
	}
	if (_access("cookies", 0) == -1) {
		system("mkdir cookies");
	}
	if (_access("temp", 0) == -1) {
		system("mkdir temp");
	}
	if (_access("config\\power.ini", 0) == 0) {
		system("del /F /S /Q config\\power.ini");
	}
	return 0;
}

int preload() {
	filecheck = 0;
	shutdown = 0;
	system("title FreeDownloader");
	CreateFolder();
	TokenGenerate();
	WindowSkin();
	printf("需要系统UAC权限读取代理服务器数据,若需要使用代理服务器请在打开本软件前打开代理. . .\n\n");
	system("Timeout /T 2");
	if (system("start /min GetProxyInfo.exe") != 0) {
		printf("UAC授权失败,无法检测计算机代理设置！\n\n");
		conf = fopen("config\\proxy.ini", "w");
		fprintf(conf, "proxy=0");
		fclose(conf);
	}
	return 0;
}
int main() {
	preload();
MainMenu:WindowSkin();
	system("cls");
	printf("------------------------------------------------------------------\n");
	printf("--------------------------FreeDownloader--------------------------\n");
	printf("------------------------------------------------------------------\n");
	printf("请选择下载功能:\n1.普通下载模式\n2.百度网盘模式\n3.视频下载模式\n4.高级下载模式\n5.磁力下载模式\n6.继续上次下载\n7.文件完整性测试\n8.启动RPC被动监听\n9.打开下载文件夹\n0.退出\n");
	printf("------------------------------------------------------------------\n");
	printf("请输入:");
	scan_return=scanf("%d", &downloadmode);
	system("cls");
	if (downloadmode == 1) {
		NormalDownloader();
	}
	else if (downloadmode == 2) {
		Netdisk();
	}
	else if (downloadmode == 3) {
		if (MediaDownloader() == 2) {
			goto MainMenu;
		}
	}
	else if (downloadmode == 4) {
		AdvanceDownloader();
	}
	else if (downloadmode == 5) {
		if (MagnetDownloader() == 2) {
			goto MainMenu;
		}
	}
	else if (downloadmode == 6) {
		if (fopen("temp\\Download.bat","r") == NULL) {
			goto MainMenu;
		}
	}
	else if (downloadmode == 7) {
		IsCheckSum = 1;
		CheckSum(IsCheckSum);
		system("cls");
		goto MainMenu;
	}
	else if (downloadmode == 8) {
		ListenRPC();
		system("cls");
		goto MainMenu;
	}
	else if (downloadmode == 9) {
		system("explorer.exe Downloads");
		system("cls");
		goto MainMenu;
	}
	else {
		exit(0);
	}
Downloading:DLEngine();
	system("cls");
	printf("------------------------------------------------------------------\n");
	printf("-----------------------------下载完成!----------------------------\n");
	printf("------------------------------------------------------------------\n");
	AutoShutdown();
	system("explorer Downloads");
	goto MainMenu;
	return 0;
}//下载工具主程序

int NormalDownloader() {
	url();
	Dir();
	threader();
	ProxySetting();
	ChangeUA();
	return 0;
}

int ListenRPC() {
	TokenGenerate();
	printf("正在本地建立RPC配置文件. . .\n");
	conf = fopen("config\\rpc.conf", "w");
	fprintf(conf, "continue=true\n");
	fprintf(conf, "max-concurrent-downloads=1\n");
	fprintf(conf, "max-connection-per-server=16\n");
	fprintf(conf, "min-split-size=1M\n");
	fprintf(conf, "disk-cache=128M\n");
	fprintf(conf, "split=16\n");;
	fprintf(conf, "dir=Downloads\n");
	fprintf(conf, "file-allocation=none\n");
	fprintf(conf, "enable-rpc=true\n");
	fprintf(conf, "rpc-allow-origin-all=true\n");
	fprintf(conf, "content-disposition-default-utf8=true\n");
	fprintf(conf, "disable-ipv6=false\n");
	fprintf(conf, "user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.0.0 Safari/537.36\n");
	fprintf(conf, "rpc-listen-all=true\n");
	fprintf(conf, "rpc-listen-port=6800\n");
	fprintf(conf, "rpc-secret=%s\n",rpctoken);
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
	printf("RPC监听:允许远程接入\nRPC监听端口:6800\nRPC密码:%s\n",rpctoken);
	printf("------------------------------------------------------------------\n\n");
	printf("RPC监听已启动!\n\n");
	system("temp\\rpc.bat");
	printf("退出");
	system("pause");
	system("taskkill /f /im aria2c.exe");
	return 0;
}

int MagnetDownloader() {
	CreateConfig();
	printf("----------------------------------------------------------------------------------\n");
	printf("注意:本模式下代理在下载阶段不起作用,若需要使用代理请配合Tap/Tun工具强制接管流量！\n");
	printf("----------------------------------------------------------------------------------\n");
	printf("请选择下载模式:\n\n1.*.torrent文件\n\n2.Magnet://链接\n\n0.返回\n\n请输入:");
	scan_return=scanf("%d", &magnet_mode);
	if (magnet_mode == 2) {
		Dir();
	}
	else if (magnet_mode == 0) {
		return 2;
	}
	threader();
	system("cls");
	url();
	return 0;
}

int url() {
	FILE* url;
	if (downloadmode == 1) {
		if (fopen("temp\\normal.download", "r")==NULL) {
			url = fopen("temp\\normal.download", "w");
			fclose(url);
		}
		system("cls");
		printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
		system("notepad.exe temp\\normal.download");
		sprintf(config_url, "%s", "-i temp\\normal.download");
	}
	else if (downloadmode == 2) {
		if (fopen("temp\\netdisk.download", "r") == NULL) {
					url = fopen("temp\\netdisk.download", "w");
					fclose(url);
		}
		system("cls");
		printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
		system("notepad.exe temp\\netdisk.download");
		sprintf(config_url, "%s", "-i temp\\netdisk.download");
	}
	else if (downloadmode == 3) {
		if(config_media == 1){
			if (fopen("temp\\ytb.download", "r") == NULL) {
				url = fopen("temp\\ytb.download", "w");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\ytb.download");
			sprintf(config_url, "%s", "-a temp\\ytb.download");
		}
		else if (config_media == 2) {
			if (fopen("temp\\Bilibili.download", "r") == NULL) {
				url = fopen("temp\\Bilibili.download", "w");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\Bilibili.download");
			sprintf(config_url, "%s", "-F temp\\Bilibili.download");
		}
		else if (config_media == 3) {
			if (fopen("temp\\QQVideo.download", "r") == NULL) {
				url = fopen("temp\\QQVideo.download", "w");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\QQVideo.download");
			sprintf(config_url, "%s", "-F temp\\QQVideo.download");
		}
		else if (config_media == 4) {
			if (fopen("temp\\iqiyi.download", "r") == NULL) {
				url = fopen("temp\\iqiyi.download", "w");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\iqiyi.download");
			sprintf(config_url, "%s", "-F temp\\iqiyi.download");
		}
		else {
			if (fopen("temp\\Youku.download", "r") == NULL) {
				url = fopen("temp\\Youku.download", "w");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\Youku.download");
			sprintf(config_url, "%s", "-F temp\\Youku.download");
		}
		}
	else if (downloadmode == 4) {
		if (fopen("temp\\advance.download", "r") == NULL) {
			url = fopen("temp\\advance.download", "w");
			fclose(url);
		}
		system("cls");
		printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
		system("notepad.exe temp\\advance.download");
		sprintf(config_url, "%s", "-i temp\\advance.download");
	}
	else if (downloadmode == 5) {
		if (magnet_mode == 2) {
			if (fopen("temp\\magnet.download", "r") == NULL) {
				url = fopen("temp\\magnet.download", "w");
				fprintf(url, "magnet:?xt=urn:btih:");
				fclose(url);
			}
			system("cls");
			printf("请在弹出页输入下载地址,多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\magnet.download");
			sprintf(config_url, "%s", "-i temp\\magnet.download");
		}
		else {
			system("cls");
			printf("若种子文件名过长建议重命名成简单字母或数字再拖入窗口,否则可能报错！\n\n");
			printf("请将种子文件以拖拽至本窗口中:");
			scan_return=scanf("%s", torrent_addr);
			sprintf(config_url, "\"%s\"",torrent_addr);
		}
	}
	return 0;
}//网址输入插件

int threader() {
	if (downloadmode == 1 || downloadmode == 4) {
		Task = 1;//同时下载任务数
		ConnectionNum = 16;
		ProcessNum = 4;
		sprintf(Downloader_Use, "%s", "aria2c");
		sprintf(split, "1M");
	}
	else if (downloadmode == 2) {
		ConnectionNum = 16;
		ProcessNum = 16;
		sprintf(split, "1M");
		sprintf(Downloader_Use, "%s", "aria2c");
		Task = 1;//同时下载任务数
	}
	else if (downloadmode == 3) {
		Task = 1;//同时下载任务数
		if (config_media == 1) {
			sprintf(Downloader_Use, "%s", "yt-dlp");
		}
		else {
			sprintf(Downloader_Use, "%s", "lux");
		}
		ConnectionNum = 1;//4k视频状态下aria2调用出现bug
	}
	else if (downloadmode == 5) {
		Task = 1;//同时下载任务数
		sprintf(Downloader_Use, "%s", "aria2c");
		ConnectionNum = 16;
		if (magnet_mode == 2)sprintf(split, "1M");
	}
	return 0;
}//线程数修改与引擎选择        

int Dir() {
	if (downloadmode == 3) {
		if (config_media == 1) {
			sprintf(config_dir, "%s", "-o Downloads\\%%(uploader)s-%%(title)s-%%(resolution)s.%%(ext)s");
		}
		else {
			sprintf(config_dir, "%s", "-o Downloads");
		}
	}
	else {
		if (downloadmode == 5 && magnet_mode == 1) {

		}
		else sprintf(config_dir, "%s", "--dir=\"Downloads\"");
	}
	return 0;
}

int ProxySetting() {
	if (system("type config\\proxy.ini | find \"proxy=0\"") == 0) {
		if (downloadmode == 5) {
			sprintf(config_bt_URL, "%s", tracker_URL_CN);
			sprintf(config_proxy, "");
		}
		else {
			sprintf(config_proxy, "");
		}
	}
	else {
			proxy_ini = fopen("config\\proxy.ini", "r");
			scan_return=fscanf(proxy_ini, "proxy=%s", proxy);
			fclose(proxy_ini);
			if (downloadmode == 1 || downloadmode == 2 || downloadmode == 4) {
				sprintf(config_proxy, "--all-proxy=%s", proxy);
			}
			else if(downloadmode==3){
				sprintf(config_proxy, "set http_proxy=%s & set https_proxy=%s", proxy,proxy);
			}
			else if (downloadmode == 5) {
				sprintf(config_bt_URL, "-x %s %s",proxy,tracker_URL_NotCN);
				sprintf(config_proxy, "-x %s", proxy);
			}
			else if (downloadmode == 8) {
				sprintf(config_proxy, "");
			}
	}
	system("cls");
	return 0;// /config/proxy.ini中proxy=0或此文件不存在为无代理状态,否则使用代理(仅支持http/https代理)
}

int ChangeUA() {
	if (downloadmode == 1) {
		sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.0.0 Safari/537.36");//Chrome浏览器
		sprintf(head_show, "Chrome");
	}
	else if (downloadmode == 2) {
		sprintf(head, "--header=\"User-Agent:%s\"", "LogStatistic");//百度网盘客户端
		sprintf(head_show, "Netdisk");
	}
	else if (downloadmode == 3) {
		if (config_media != 2) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.0.0 Safari/537.36");//Chrome浏览器
			sprintf(head_show, "Chrome");
		}
		else {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) bilibili_pc/1.1.0 Chrome/98.0.4758.141 Electron/17.4.0 Safari/537.36");//Bilibili PC Client
			sprintf(head_show, "Bilibili PC Client");
		}
	}
	else if (downloadmode == 4) {
		printf("\n请选择浏览器标识:\n\n1.IE\n\n2.Chrome\n\n3.Chrome(Mobile)\n\n请输入:");
		scan_return=scanf("%d", &mark);
		if (mark == 1) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko");//IE浏览器
			sprintf(head_show, "IE");
		}
		else if (mark == 2) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.0.0 Safari/537.36");//Chrome浏览器
			sprintf(head_show, "Chrome");
		}
		else if (mark == 3) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Linux; Android 12; LE2110) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.0.0 Mobile Safari/537.36");//Chrome浏览器
			sprintf(head_show, "Chrome(Mobile)");
		}
		else {
			mark = 2;
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.0.0 Safari/537.36");//Chrome浏览器
			sprintf(head_show, "Chrome");
		}
	}
	else if (downloadmode == 5) {
		printf("\n请选择BT下载工具标识:\n\n1.qBittorrent\n\n2.Transmission\n\n3.uTorrent\n\n4.BitComet\n\n请输入:");
		scan_return = scanf("%d", &mark);
		if (mark == 1) {
			sprintf(head, "user-agent=%s\npeer-agent=%s\npeer-id-prefix=%s", "qBittorrent/4.4.0.0", "qBittorrent/4.4.0.0", "-qB4400-");//qBittorrent
			sprintf(head_show, "qBittorrent/4.4.0.0");
		}
		else if (mark == 2) {
			sprintf(head, "user-agent=%s\npeer-agent=%s\npeer-id-prefix=%s", "Transmission/3.00", "Transmission/3.00", "-TR3000-");//Transmission
			sprintf(head_show, "Transmission/3.00");
		}
		else if (mark == 3) {
			sprintf(head, "user-agent=%s\npeer-agent=%s\npeer-id-prefix=%s", "uTorrent/3550(46096)", "uTorrent/3550(46096)", "-UT3550-");//uTorrent
			sprintf(head_show, "uTorrent/3550(46096)");
		}
		else if (mark == 4) {
			sprintf(head, "user-agent=%s\npeer-agent=%s\npeer-id-prefix=%s", "BitComet/1.76.4.8", "BitComet/1.85.1.18", "-BC0185-");//BitComet
			sprintf(head_show, "BitComet/1.85.1.18");
		}
		else {
			mark = 1;
			sprintf(head, "user-agent=%s\npeer-agent=%s\npeer-id-prefix=%s", "qBittorrent/4.4.0.0", "qBittorrent/4.4.0.0", "-qB4400-");//qBittorrent
			sprintf(head_show, "qBittorrent/4.4.0.0");
		}
	}
	return 0;
}

int AdvanceDownloader() {
	int cookie_import;
	char reference_input[200];
	FILE* cookie;
	ChangeUA();
	printf("\n请输入下载引用页地址:\n");
	scan_return=scanf("%s", reference_input);
	sprintf(reference, "--referer=\"%s\"", reference_input);
	if (fopen("cookies\\Cookies.txt", "r") == NULL) {
	p_4:cookie = fopen("cookies\\Cookies.txt", "w");
		fprintf(cookie, "# Input Cookie below#\n");
		fclose(cookie);
		printf("\n请在弹出窗口中导入Cookies信息,");
		system("notepad cookies\\Cookies.txt");
		system("pause");
	}
	else {
		printf("\n检测到存在Cookies信息,是否继续使用上次的信息登录（是=1 否=0）:");
		scan_return=scanf("%d", &cookie_import);
		if (cookie_import != 1) {
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

int Netdisk() {
	ChangeUA();
	sprintf(reference, "%s", "--referer=\"https://pan.baidu.com/disk/home#/all?path=%2F&vmode=list\"");
		printf("\n是否使用插件导入Cookie(1=插件手动导入 0=浏览器手动导入):");
		scan_return=scanf("%d", &cookie_mode);
		if (cookie_mode != 0) {
			if (fopen("cookies\\Netdisk_Cookies.txt", "r") == NULL) {
			p_4:cookie = fopen("temp\\Netdisk_Cookies_tmp.txt", "w");
				fprintf(cookie, "# Input Cookie below#\n");
				fclose(cookie);
				printf("\n请在弹出窗口中导入百度网盘Cookies信息 . . .\n");
				system("notepad temp\\Netdisk_Cookies_tmp.txt");
				system("type temp\\Netdisk_Cookies_tmp.txt | find \"BDUSS\" | find /v \"BFESS\" > cookies\\Netdisk_Cookies.txt");
				system("type temp\\Netdisk_Cookies_tmp.txt | find \"pcsett\" >> cookies\\Netdisk_Cookies.txt");
				system("del temp\\Netdisk_Cookies_tmp.txt");
			}
			else {
				printf("\n检测到存在Cookies信息,是否继续使用上次的信息登录（是=1 否=0）:");
				scan_return=scanf("%d", &cookie_import);
				if (cookie_import != 1) {
					goto p_4;
				}
			}
			sprintf(config_cookie, "--load-cookies=\"cookies\\Netdisk_Cookies.txt\"");
		}
		else {
			if (fopen("cookies\\BDUSS.txt", "r") == NULL || fopen("cookies\\pcsett.txt", "r") == NULL) {
				p_5:printf("\n请输入BDUSS值:\n");
				scan_return = scanf("%s", BDUSS);
				printf("\n请输入pcsett值:\n");
				scan_return = scanf("%s", pcsett);
				cookie = fopen("cookies\\BDUSS.txt", "w");
				fprintf(cookie, "%s", BDUSS);
				fclose(cookie);
				cookie = fopen("cookies\\pcsett.txt", "w");
				fprintf(cookie, "%s", pcsett);
				fclose(cookie);
			}
			else {
				printf("\n检测到存在Cookies信息,是否继续使用上次的信息登录（是=1 否=0）:");
				scan_return = scanf("%d", &cookie_import);
				if (cookie_import != 1) {
					goto p_5;
				}
			}
			cookie = fopen("cookies\\BDUSS.txt", "r");
			scan_return = fscanf(cookie, "%s", BDUSS);
			fclose(cookie);
			cookie = fopen("cookies\\pcsett.txt", "r");
			scan_return = fscanf(cookie, "%s", pcsett);
			fclose(cookie);
			sprintf(config_cookie, "--header=\"Cookie: BDUSS=%s;pcsett=%s\"", BDUSS,pcsett);
		}
	url();
	Dir();
	threader();
	return 0;
}

int CheckSum(int mode) {
	if (mode == 1) {
		printf("\n如果需要检测文件完整性,可以将文件拖拽至本窗口内！建议先将文件名称最简化并移至磁盘根目录下再检测！\n");
		printf("\n文件名称中不允许空格！若md5不一致,则文件损坏,需重新下载！\n\n文件路径:");
		scan_return=scanf("%s", location);
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

int AutoShutdown() {
	if (fopen("config\\power.ini", "r") != NULL) {
		shutdown = 1;//设置后只生效一次,自动还原状态,避免每次都自动关机
	}
	else {
		shutdown = 0;
	}
	if (shutdown == 1) {
	printf("正在执行关机操作. . .\n");
	system("shutdown -s -t 30");
	exit(0);
	}
	return 0;
}

int MediaDownloader() {
	FILE* Bilibili_Cookies,*ytb_Cookies,* QQVideo_Cookies,*iqiyi_Cookies,*Youku_Cookies;
	char chapter[10];
	printf("下载音视频来源:\n\n1.油管\n\n2.哔哩哔哩\n\n3.腾讯视频\n\n4.爱奇艺\n\n5.优酷\n\n0.返回\n\n请输入:");
	scan_return=scanf("%d", &config_media);
	ProxySetting();
	if (config_media != 1 && config_media != 0) {//调用lux
		printf("\n下载整个列表内所有音视频?\n\n1.是\n\n2.只下载当前视频\n\n请输入:");
		scan_return = scanf("%d", &DownloadList);
		if (DownloadList == 1) {
			sprintf(play_list, "-p");
		}
		else if (DownloadList == 2) {
			sprintf(play_list, " ");
		}
	}
	else if(config_media!=0){//调用yt-dlp
		printf("\n下载整个列表内所有音视频?\n\n1.是\n\n2.只下载当前视频\n\n0.选择集数\n\n请输入:");
		scan_return = scanf("%d", &DownloadList);
		if (DownloadList == 1) {
			sprintf(play_list, "--yes-playlist");
		}
		else if (DownloadList == 2) {
			sprintf(play_list, "--no-playlist");
		}
		else {
			system("cls");
			printf("请按照格式输入下载范围,如1-5,6,7,8-15:");
			scan_return = scanf("%s", chapter);
			sprintf(play_list, "--playlist-items %s", chapter);
		}
		system("cls");
	}
	else {
		return 2;
	}
	if (config_media == 1) {
			if (fopen("cookies\\ytb_Cookies.txt", "r") == NULL) {
				ytb_Cookies = fopen("cookies\\ytb_Cookies.txt", "w");
				fprintf(ytb_Cookies, "# Input Cookie below#\n");
				fclose(ytb_Cookies);
			}
			system("notepad cookies\\ytb_Cookies.txt");
	}
	else if (config_media == 2) {
			if (fopen("cookies\\Bilibili_Cookies.txt", "r") == NULL) {
				Bilibili_Cookies = fopen("cookies\\Bilibili_Cookies.txt", "w");
				fprintf(Bilibili_Cookies, "# Input Cookie below#\n");
				fclose(Bilibili_Cookies);
			}
			system("notepad cookies\\Bilibili_Cookies.txt");
	}
	else if (config_media == 3) {
			if (fopen("cookies\\QQVideo_Cookies.txt", "r") == NULL) {
				QQVideo_Cookies = fopen("cookies\\QQVideo_Cookies.txt", "w");
				fprintf(QQVideo_Cookies, "# Input Cookie below#\n");
				fclose(QQVideo_Cookies);
			}
			system("notepad cookies\\QQVideo_Cookies.txt");
	}
	else if (config_media == 4) {
			if (fopen("cookies\\iqiyi_Cookies.txt", "r") == NULL) {
				iqiyi_Cookies = fopen("cookies\\iqiyi_Cookies.txt", "w");
				fprintf(iqiyi_Cookies, "# Input Cookie below#\n");
				fclose(iqiyi_Cookies);
			}
			system("notepad cookies\\iqiyi_Cookies.txt");
	}
	else if (config_media == 5) {
			if (fopen("cookies\\Youku_Cookies.txt", "r") == NULL) {
				Youku_Cookies = fopen("cookies\\Youku_Cookies.txt", "w");
				fprintf(Youku_Cookies, "# Input Cookie below#\n");
				fclose(Youku_Cookies);
			}
			system("notepad temp\\Youku_Cookies.txt");
	}
	system("cls");
	printf("请在弹出窗口中导入Cookies,同一账号可以反复使用\n\n");
	url();
	threader();
	Dir();
	ProxySetting();
	ChangeUA();
	return 0;
}

int DLEngine() {
	FILE* Download;
	if (downloadmode >= 1 && downloadmode <= 5) {
		Download = fopen("temp\\Download.bat", "w");
		fprintf(Download, "@echo off\n");
		if (downloadmode == 1) {
			fprintf(Download, "%s -c -x%d -s%d -k%s --follow-torrent=false --content-disposition-default-utf8=true -j %d %s %s %s %s\n", Downloader_Use, ConnectionNum, ProcessNum, split, Task, config_dir, config_proxy, head, config_url);
			fclose(Download);
		}
		else if (downloadmode == 2) {
			fprintf(Download, "%s -c -x%d -s%d --follow-torrent=false -k%s -j %d %s %s %s %s --content-disposition-default-utf8=true %s\n", Downloader_Use, ConnectionNum, ProcessNum, split, Task, config_dir, reference, head, config_cookie, config_url);
			fclose(Download);
		}
		else if (downloadmode == 3) {
			fprintf(Download, "%s\n", config_proxy);
			if (config_media == 1) {
				fprintf(Download, "%s --cookies cookies\\ytb_Cookies.txt --merge-output-format mkv --write-sub --all-subs %s %s %s --downloader aria2c --downloader-args \"aria2c:-x2 -k2M\"\n", Downloader_Use, play_list, config_dir, config_url);
			}
			else if (config_media == 2) {
				fprintf(Download, "%s -c cookies\\Bilibili_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			}
			else if (config_media == 3) {
				fprintf(Download, "%s -c cookies\\QQVideo_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			}
			else if (config_media == 4) {
				fprintf(Download, "%s -c cookies\\iqiyi_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			}
			else {
				fprintf(Download, "%s -c cookies\\Youku_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			}
		}
		else if (downloadmode == 4) {
			fprintf(Download, "%s -c -x%d -s%d -k%s -j %d %s %s %s %s %s --max-tries=0 --content-disposition-default-utf8=true %s\n", Downloader_Use, ConnectionNum, ConnectionNum, split, Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
		}
		else if (downloadmode == 5) {
			if (magnet_mode == 2) {
				fprintf(Download, "%s --conf-path=config\\bt.conf %s\n", Downloader_Use, config_url);
			}
			else {
				fprintf(Download, "%s --conf-path=config\\bt.conf %s\n", Downloader_Use, config_url);
			}
		}
		fprintf(Download, "exit 0\n");
		fclose(Download);
	}
	system("cls");
	printf("------------------------------------------------------------------\n");
	if (downloadmode >= 1 && downloadmode <= 5) {
		printf("代理地址:%s(为空代表没有设置代理)\n", config_proxy);
		printf("下载线程数:%d\n", ConnectionNum);
		printf("下载引擎:%s\n", Downloader_Use);
		printf("浏览器标识:%s\n", head_show);
	}
	else {
		printf("下载配置:依照上一次的设置\n");
	}
	printf("------------------------------------------------------------------\n");
	printf("下载过程中出现的红色ERROR报错可忽略,对下载没有影响！\n");
	printf("下载正在执行,希望中断下载建议按Ctrl+C以正常退出. . .\n");
	printf("------------------------------------------------------------------\n");
	system("temp\\Download.bat");
	return 0;
}
