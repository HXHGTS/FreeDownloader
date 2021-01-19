#include<stdio.h>
#include<stdlib.h>
#include<io.h>


int AdvanceDownloader(),AutoShutdown(int mode),BroswerMark(),CheckSum(int mode),dir(),downloadengine(),WindowSkin();
int MagnetDownloader(),MediaDownloader(),Netdisk(),NormalDownloader(),proxyswitcher(),threader(),url();
int downloadmode, magnet_mode,ConnectionNum, ProcessNum, config_media, anti_shutdown, Task, IsCheckSum;
int mark, redownload_result, shutdown, filecheck, DownloadList, OpenDir;
int cookie_import, cookie_mode,appid;
int scan_return;//接收返回值，暂时没用
char config_proxy[65], config_url[260], config_dir[35], config_cookie[230], smallcmd[20], Downloader_Use[12];
char reference[216], head[300], head_show[35], pre_proxy[56];
char location[200],split[7],torrent_loca[250],play_list[40], color[4];
char proxy[50];
char rpctoken[40];//定义rpc密钥
char BDUSS[193];
char cmd[300];
FILE* conf,*power_ini,*proxy_ini,*dir_mark, *skin;//定义配置文件
FILE* cookie,*bat;

int TokenGenerate() {
	if (fopen("config\\uuid", "r") == NULL) {
		system("powershell [guid]::NewGuid() | find /v \"Guid\" | find /v \"--\" | find \"-\" > config\\uuid");//随机GUID作为密码，提高RPC功能安全性
	}
	conf = fopen("config\\uuid", "r");
	scan_return=fscanf(conf,"%s", rpctoken);
	fclose(conf);
	return 0;
}

int WindowSkin() {
	if (fopen("config\\skin.ini", "r") == NULL) {
		printf("重置窗体与字体颜色请直接删除config目录下skin.ini文件！\n");
		printf("请按照\"窗口颜色+字体颜色\"的格式设置皮肤，对应关系如下：\n");
		printf("------------------------------------------------------------------\n");
		printf("0=黑色   8=灰色\n");
		printf("1=蓝色   9=淡蓝色\n");
		printf("2=绿色   A=淡绿色\n");
		printf("3=浅绿色 B=淡浅绿色\n");
		printf("4=红色   C=淡红色\n");
		printf("5=紫色   D=淡紫色\n");
		printf("6=黄色   E=淡黄色\n");
		printf("7=白色   F=亮白色\n");
		printf("------------------------------------------------------------------\n");
		printf("请输入以配置界面与字体颜色，如0A：");
		scan_return=scanf("%s", color);
		skin = fopen("config\\skin.ini", "w");
		fprintf(skin,"%s",color);
		fclose(skin);
	}
	else {
		skin = fopen("config\\skin.ini", "r");
		fread(color, 1, 2, skin);
		fclose(skin);
	}
	system("cls");
	return 0;
}

int CreateConfig() {
	printf("正在尝试连接到trackerslist.com服务器. . .\n\n");
	proxyswitcher();
	conf = fopen("config\\bt.conf", "a");
	fprintf(conf, "bt-tracker=");
	fclose(conf);
	sprintf(cmd, "curl %s https://trackerslist.com/best_aria2.txt -# > config\\best_aria2.txt", pre_proxy);
	if (system(cmd) != 0) {
		printf("\n更新失败，建议配合代理或VPN重新打开软件更新列表，正在本地建立BT配置文件. . .\n");
		system("notepad config\\bt.conf");
	}
	else {
		printf("\n更新成功，正在本地建立BT配置文件. . .\n");
		system("type config\\best_aria2.txt >> config\\bt.conf");
		system("del /F /S /Q config\\best_aria2.txt");
	}
	conf = fopen("config\\bt.conf", "a");
	fprintf(conf, "\ncontinue=true\n");
	fprintf(conf, "max-concurrent-downloads=1\n");
	fprintf(conf, "max-connection-per-server=16\n");
	fprintf(conf, "bt-max-peers=999\n");
	fprintf(conf, "min-split-size=1M\n");
	fprintf(conf, "disk-cache=128M\n");
	fprintf(conf, "split=16\n");;
	fprintf(conf, "dir=Downloads/\n");
	fprintf(conf, "max-tries=0\n");
	fprintf(conf, "file-allocation=none\n");
	fprintf(conf, "enable-peer-exchange=true\n");
	fprintf(conf, "content-disposition-default-utf8=true\n");
	fprintf(conf, "disable-ipv6=true\n");
	fprintf(conf, "user-agent=qBittorrent/4.3.2.0\n");
	fprintf(conf, "peer-agent=qBittorrent/4.3.2.0\n");
	fprintf(conf, "peer-id-prefix=-qB4320-\n");
	fclose(conf);
	system("cls");
	return 0;
}

int CreateFolder() {
	if (_access("Downloads", 0)) {
		system("mkdir Downloads");
	}
	if (_access("config", 0)) {
		system("mkdir config");
	}
	if (_access("cookies", 0)) {
		system("mkdir cookies");
	}
	if (_access("temp", 0)) {
		system("mkdir temp");
	}
	return 0;
}

int preload() {
	redownload_result = 0;
	filecheck = 0;
	anti_shutdown = shutdown = 0;
	system("title FreeDownloader");
	CreateFolder();
	TokenGenerate();
	WindowSkin();
	sprintf(smallcmd, "color %s", color);
	system(smallcmd);
	printf("需要系统UAC权限读取代理服务器数据，若需要使用代理服务器请在打开本软件前打开代理. . .\n\n");
	system("Timeout /T 2");
	if (system("start /min GetProxyInfo.exe") != 0) {
		printf("UAC授权失败，无法检测计算机代理设置！\n\n");
		conf = fopen("config\\proxy.ini", "w");
		fprintf(conf, "proxy=0");
		fclose(conf);
	}
	return 0;
}
int main() {
	preload();
MainMenu:system(smallcmd);
	system("cls");
	printf("------------------------------------------------------------------\n");
	printf("--------------------------FreeDownloader--------------------------\n");
	printf("------------------------------------------------------------------\n");
	printf("请选择下载功能：\n1.普通下载模式\n2.百度网盘模式\n3.视频下载模式\n4.高级下载模式\n5.磁力下载模式\n6.文件完整性测试\n7.Github上的软件帮助\n8.启动RPC被动监听\n9.打开下载文件夹\n0.退出\n");
	printf("------------------------------------------------------------------\n");
	printf("请输入：");
	scan_return=scanf("%d", &downloadmode);
	if (system("type config\\power.ini | find \"power=1\"") == 0) {
		shutdown = 1;
		system("echo \"power=0\" config\\power.ini"); //设置后只生效一次，自动还原状态，避免每次都自动关机
	}
	else {
		shutdown = 0;
	}
	system("cls");
	if (downloadmode == 1) {
		NormalDownloader();
	}
	else if (downloadmode == 2) {
		Netdisk();
	}
	else if (downloadmode == 3) {
		MediaDownloader();
	}
	else if (downloadmode == 4) {
		AdvanceDownloader();
	}
	else if (downloadmode == 5) {
		MagnetDownloader();
	}
	else if (downloadmode == 6) {
		IsCheckSum = 1;
		CheckSum(IsCheckSum);
		system("cls");
		goto MainMenu;
	}
	else if (downloadmode == 7) {
		printf("正在打开帮助界面. . .\n");
		system("explorer.exe \"https://hxhgts.ml/FreeDownloader/\"");
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
Downloading:redownload_result = downloadengine();
	system("cls");
	if (redownload_result == 0) {
		printf("------------------------------------------------------------------\n");
		printf("-----------------------------下载成功!----------------------------\n");
		printf("------------------------------------------------------------------\n");
		AutoShutdown(shutdown);
		printf("\n是否打开下载文件夹：\n\n1.是\n\n0.否\n\n请输入：");
		scan_return=scanf("%d", &OpenDir);
		if (OpenDir != 0) {
			system("explorer Downloads");
		}
		system("cls");
		goto MainMenu;
	}
	else {
		printf("------------------------------------------------------------------\n");
		printf("-----------------------------下载失败!----------------------------\n");
		printf("------------------------------------------------------------------\n");
		system("cls");
		goto Downloading;
	}
	return 0;
}//下载工具主程序

int NormalDownloader() {
	url();
	dir();
	threader();
	proxyswitcher();
	BroswerMark();
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
	fprintf(conf, "disable-ipv6=true\n");
	fprintf(conf, "user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36 Edg/87.0.664.75\n");
	fprintf(conf, "rpc-listen-all=true\n");
	fprintf(conf, "rpc-listen-port=6800\n");
	fprintf(conf, "rpc-secret=%s\n",rpctoken);
	fclose(conf);
	proxyswitcher();
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
	printf("请选择下载模式：\n\n1.*.torrent文件\n\n2.Magnet://链接\n\n请输入：");
	scan_return=scanf("%d", &magnet_mode);
	if (magnet_mode == 2) {
		dir();
	}
	threader();
	system("cls");
	BroswerMark();
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
		printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
		system("notepad.exe temp\\normal.download");
		sprintf(config_url, "%s", "-i temp\\normal.download");
	}
	else if (downloadmode == 2) {
		if (fopen("temp\\netdisk.download", "r") == NULL) {
					url = fopen("temp\\netdisk.download", "w");
					fclose(url);
		}
		printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
		system("notepad.exe temp\\netdisk.download");
		sprintf(config_url, "%s", "-i temp\\netdisk.download");
		system("del temp\\url.bat");
	}
	else if (downloadmode == 3) {
		if(config_media == 1){
			if (fopen("temp\\ytb.download", "r") == NULL) {
				url = fopen("temp\\ytb.download", "w");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\ytb.download");
			sprintf(config_url, "%s", "-a temp\\ytb.download");
		}
		else if (config_media == 2) {
			if (fopen("temp\\Bilibili.download", "r") == NULL) {
				url = fopen("temp\\Bilibili.download", "w");
				fprintf(url, "---Input URL(Don't delete this line)---\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\Bilibili.download");
			sprintf(config_url, "%s", "-F temp\\Bilibili.download");
		}
		else if (config_media == 3) {
			if (fopen("temp\\QQVideo.download", "r") == NULL) {
				url = fopen("temp\\QQVideo.download", "w");
				fprintf(url, "---Input URL(Don't delete this line)---\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\QQVideo.download.1");
			sprintf(config_url, "%s", "-F temp\\QQVideo.download");
		}
		else if (config_media == 4) {
			if (fopen("temp\\iqiyi.download", "r") == NULL) {
				url = fopen("temp\\iqiyi.download", "w");
				fprintf(url, "---Input URL(Don't delete this line)---\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\iqiyi.download");
			sprintf(config_url, "%s", "-F temp\\iqiyi.download");
		}
		else {
			if (fopen("temp\\Youku.download", "r") == NULL) {
				url = fopen("temp\\Youku.download", "w");
				fprintf(url, "---Input URL(Don't delete this line)---\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\Youku.download");
			sprintf(config_url, "%s", "-F temp\\Youku.download");
		}
		}
	else if (downloadmode == 4) {
		if (fopen("temp\\advance.download", "r") == NULL) {
			url = fopen("temp\\advance.download", "w");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
		system("notepad.exe temp\\advance.download");
		sprintf(config_url, "%s", "-i temp\\advance.download");
	}
	else if (downloadmode == 5) {
		if (magnet_mode == 2) {
			if (fopen("temp\\magnet.download", "r") == NULL) {
				url = fopen("temp\\magnet.download", "w");
				fclose(url);
			}
			printf("请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\magnet.download");
			sprintf(config_url, "%s", "-i temp\\magnet.download");
		}
		else {
			printf("若种子文件名过长建议重命名成简单字母或数字再拖入窗口，否则可能报错！\n\n");
			printf("请将种子文件以拖拽至本窗口中：");
			scan_return=scanf("%s", torrent_loca);
			sprintf(config_url, "\"%s\"",torrent_loca);
		}
	}
	return 0;
}//网址输入插件

int threader() {
	if (downloadmode == 1 || downloadmode == 4) {
		Task = 1;//同时下载任务数
		ConnectionNum = 16;
		ProcessNum = 128;
		sprintf(Downloader_Use, "%s", "aria2c");
		sprintf(split, "2M");
	}
	else if (downloadmode == 2) {
		ConnectionNum = 2;
		ProcessNum = 4;
		sprintf(split, "1M");
		sprintf(Downloader_Use, "%s", "aria2c");
		Task = 1;//同时下载任务数
	}
	else if (downloadmode == 3) {
		Task = 1;//同时下载任务数
		if (config_media == 1) {
			sprintf(Downloader_Use, "%s", "youtube-dl");
		}
		else {
			sprintf(Downloader_Use, "%s", "annie");
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

int dir() {
	if (downloadmode == 3) {
		if (config_media == 1) {
			sprintf(config_dir, "%s", "-o Downloads\\%%(title)s.%%(ext)s");
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

int proxyswitcher() {
	if (system("type config\\proxy.ini | find \"proxy=0\"") == 0) {
		sprintf(config_proxy, "%s", "");
		sprintf(pre_proxy, "");
	}
	else {
			proxy_ini = fopen("config\\proxy.ini", "r");
			scan_return=fscanf(proxy_ini, "proxy=%s", proxy);
			fclose(proxy_ini);
			sprintf(pre_proxy, "-x %s", proxy);//curl proxy
			if (downloadmode == 1 || downloadmode == 2 || downloadmode == 4) {
				sprintf(config_proxy, "--all-proxy=%s", proxy);
			}
			else if(downloadmode==3){
				sprintf(config_proxy, "set http_proxy=%s & set https_proxy=%s", proxy,proxy);
			}
			else if (downloadmode == 5) {
				sprintf(config_proxy, "%s", proxy);
				conf = fopen("config\\bt.conf", "w");
				fprintf(conf, "all-proxy=%s\n", proxy);
				fclose(conf);//magnet proxy
			}
			else if (downloadmode == 8) {
				conf = fopen("config\\rpc.conf", "a");
				fprintf(conf, "all-proxy=%s\n", proxy);
				fclose(conf);//RPC proxy
			}
			
	}
	system("cls");
	return 0;// /config/proxy.ini中proxy=0或此文件不存在为无代理状态，否则使用代理(仅支持http/https代理)
}

int BroswerMark() {
	char UserAgent_DIY[275];
	if (downloadmode == 1) {
		sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36 Edg/87.0.664.75");//Chrome浏览器
		sprintf(head_show, "Chrome");
	}
	else if (downloadmode == 2) {
		sprintf(head, "--header=\"User-Agent:%s\"", "netdisk;7.0.10.2;PC;PC-Windows;10.0.14393;WindowsBaiduYunGuanJia");//百度网盘客户端
		sprintf(head_show, "Netdisk");
	}
	else if (downloadmode == 3) {
		sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36 Edg/87.0.664.75");//Chrome浏览器
		sprintf(head_show, "Chrome");
	}
	else if (downloadmode == 4) {
		printf("\n请选择浏览器标识：\n\n1.IE\n\n2.Chrome\n\n3.Chrome(Mobile)\n\n请输入：");
		scan_return=scanf("%d", &mark);
		if (mark == 1) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 10.0; WOW64; Trident/7.0; .NET4.0C; .NET4.0E; .NET CLR 2.0.50727; .NET CLR 3.0.30729; .NET CLR 3.5.30729)");//IE浏览器
			sprintf(head_show, "IE");
		}
		else if (mark == 2) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36 Edg/87.0.664.75");//Chrome浏览器
			sprintf(head_show, "Chrome");
		}
		else if (mark == 3) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Linux; Android 9; DUK-AL20) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.116 Mobile Safari/537.36 EdgA/45.12.4.5121");//Chrome浏览器
			sprintf(head_show, "Chrome(Mobile)");
		}
		else {
			mark = 2;
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36 Edg/87.0.664.75");//Chrome浏览器
			sprintf(head_show, "Chrome");
		}
	}
	else if (downloadmode == 5) {
		sprintf(head_show, "qBittorrent/4.3.2.0");
	}
	return 0;
}

int AdvanceDownloader() {
	int cookie_import;
	char reference_input[200];
	FILE* cookie;
	BroswerMark();
	printf("\n请输入下载引用页地址：\n");
	scan_return=scanf("%s", reference_input);
	sprintf(reference, "--referer=\"%s\"", reference_input);
	if (fopen("cookies\\Cookies.txt", "r") == NULL) {
	p_4:cookie = fopen("cookies\\Cookies.txt", "w");
		fprintf(cookie, "# Input Cookie below#\n");
		fclose(cookie);
		printf("\n请在弹出窗口中导入Cookies信息，");
		system("notepad cookies\\Cookies.txt");
		system("pause");
	}
	else {
		printf("\n检测到存在Cookies信息，是否继续使用上次的信息登录（是=1 否=0）：");
		scan_return=scanf("%d", &cookie_import);
		if (cookie_import != 1) {
			goto p_4;
		}
	}
	sprintf(config_cookie, "--load-cookies=\"cookies\\Cookies.txt\"");
	url();
	dir();
	proxyswitcher();
	threader();
	return 0;
}

int Netdisk() {
	BroswerMark();
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
				system("type temp\\Netdisk_Cookies_tmp.txt | find \"BDUSS\" > cookies\\Netdisk_Cookies.txt");
				system("del temp\\Netdisk_Cookies_tmp.txt");
			}
			else {
				printf("\n检测到存在Cookies信息，是否继续使用上次的信息登录（是=1 否=0）：");
				scan_return=scanf("%d", &cookie_import);
				if (cookie_import != 1) {
					goto p_4;
				}
			}
			sprintf(config_cookie, "--load-cookies=\"cookies\\Netdisk_Cookies.txt\"");
		}
		else {
			if (fopen("cookies\\BDUSS.txt", "r") == NULL) {
				p_5:printf("\n请输入BDUSS值:\n");
				scan_return = scanf("%s", BDUSS);
				cookie = fopen("cookies\\BDUSS.txt", "w");
				fprintf(cookie, "%s", BDUSS);
				fclose(cookie);
			}
			else {
				printf("\n检测到存在Cookies信息，是否继续使用上次的信息登录（是=1 否=0）：");
				scan_return = scanf("%d", &cookie_import);
				if (cookie_import != 1) {
					goto p_5;
				}
			}
			cookie = fopen("cookies\\BDUSS.txt", "r");
			scan_return = fscanf(cookie, "%s", BDUSS);
			fclose(cookie);
			sprintf(config_cookie, "--header=\"Cookie: BDUSS=%s\"", BDUSS);
		}
	url();
	dir();
	proxyswitcher();
	threader();
	return 0;
}

int CheckSum(int mode) {
	if (mode == 1) {
		printf("\n如果需要检测文件完整性，可以将文件拖拽至本窗口内！建议先将文件名称最简化并移至磁盘根目录下再检测！\n");
		printf("\n文件名称中不允许空格！若md5不一致，则文件损坏，需重新下载！\n\n文件路径：");
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

int AutoShutdown(int mode) {
	if (mode == 1) {
		system("shutdown -s -t 60");
		printf("是否阻止系统关机(是=1)：");
		scan_return=scanf("%d", &anti_shutdown);
		if (anti_shutdown == 1) {
			system("shutdown -a");
		}
	}
	return 0;
}

int MediaDownloader() {
	FILE* Bilibili_Cookies,*ytb_Cookies,* QQVideo_Cookies,*iqiyi_Cookies,*Youku_Cookies;
	char chapter[14];
	printf("下载音视频来源：\n\n1.油管\n\n2.哔哩哔哩\n\n3.腾讯视频\n\n4.爱奇艺\n\n5.优酷\n\n请输入：");
	scan_return=scanf("%d", &config_media);
	proxyswitcher();
	if (config_media != 1) {
		printf("\n下载整个列表内所有音视频？\n\n1.是\n\n2.只下载当前视频\n\n请输入：");
		scan_return = scanf("%d", &DownloadList);
		if (DownloadList == 1) {
			sprintf(play_list, "-p");
		}
		else if (DownloadList == 2) {
			sprintf(play_list, "");
		}
	}
	else {
		printf("\n下载整个列表内所有音视频？\n\n1.是\n\n2.只下载当前视频\n\n0.选择集数\n\n请输入：");
		scan_return = scanf("%d", &DownloadList);
		if (DownloadList == 1) {
			sprintf(play_list, "--yes-playlist");
		}
		else if (DownloadList == 2) {
			sprintf(play_list, "--no-playlist");
		}
		else {
			printf("\n请按照格式输入下载范围，如1-5,6,7,8-15：");
			scan_return = scanf("%s", chapter);
			sprintf(play_list, "--playlist-items %s", chapter);
		}
	}
	if (config_media == 1) {
			if (fopen("cookies\\ytb_Cookies.txt", "r") == NULL) {
				ytb_Cookies = fopen("cookies\\ytb_Cookies.txt", "w");
				fprintf(ytb_Cookies, "# Input Cookie below#\n");
				fclose(ytb_Cookies);
			}
			system("notepad cookies\\ytb_Cookies.txt");
			printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
	}
	else if (config_media == 2) {
			if (fopen("cookies\\Bilibili_Cookies.txt", "r") == NULL) {
				Bilibili_Cookies = fopen("cookies\\Bilibili_Cookies.txt", "w");
				fprintf(Bilibili_Cookies, "# Input Cookie below#\n");
				fclose(Bilibili_Cookies);
			}
			system("notepad cookies\\Bilibili_Cookies.txt");
			printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
	}
	else if (config_media == 3) {
			if (fopen("cookies\\QQVideo_Cookies.txt", "r") == NULL) {
				QQVideo_Cookies = fopen("cookies\\QQVideo_Cookies.txt", "w");
				fprintf(QQVideo_Cookies, "# Input Cookie below#\n");
				fclose(QQVideo_Cookies);
			}
			system("notepad cookies\\QQVideo_Cookies.txt");
			printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
	}
	else if (config_media == 4) {
			if (fopen("cookies\\iqiyi_Cookies.txt", "r") == NULL) {
				iqiyi_Cookies = fopen("cookies\\iqiyi_Cookies.txt", "w");
				fprintf(iqiyi_Cookies, "# Input Cookie below#\n");
				fclose(iqiyi_Cookies);
			}
			system("notepad cookies\\iqiyi_Cookies.txt");
			printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
	}
	else {
			if (fopen("cookies\\Youku_Cookies.txt", "r") == NULL) {
				Youku_Cookies = fopen("cookies\\Youku_Cookies.txt", "w");
				fprintf(Youku_Cookies, "# Input Cookie below#\n");
				fclose(Youku_Cookies);
			}
			system("notepad cookies\\Youku_Cookies.txt");
			printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
	}
	url();
	threader();
	dir();
	proxyswitcher();
	BroswerMark();
	return 0;
}

int downloadengine() {
	FILE*Download,* Bilibili_Download,*ytb_Download,*QQVideo_Download,*iqiyi_Download,*Youku_Download;
	int download_result;
	if (downloadmode == 1) {
		Download= fopen("temp\\Download.bat", "w");
		fprintf(Download, "@echo off\n");
		fprintf(Download, "%s -c -x%d -s%d -k%s --file-allocation=none -j %d %s %s %s %s\n", Downloader_Use, ConnectionNum,ProcessNum, split, Task, config_dir, config_proxy, head, config_url);
		fclose(Download);
	}
	else if (downloadmode == 2) {
		Download = fopen("temp\\Download.bat", "w");
		fprintf(Download, "@echo off\n");
		fprintf(Download, "%s -c -x%d -s%d --file-allocation=none -k%s -j %d %s %s %s %s %s --content-disposition-default-utf8=true %s\n", Downloader_Use, ConnectionNum, ProcessNum, split, Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
		fclose(Download);
	}
	else if (downloadmode == 3) {
		if (config_media == 1) {
			ytb_Download = fopen("temp\\ytb_Download.bat", "w");
			fprintf(ytb_Download, "@echo off\n");
			fprintf(ytb_Download, "%s\n", config_proxy);
			fprintf(ytb_Download, "%s --cookies cookies\\ytb_Cookies.txt --write-sub --all-subs %s %s %s --external-downloader aria2c --external-downloader-args \"-x16 -k2M --file-allocation=none\"\n", Downloader_Use, play_list, config_dir, config_url);
			fclose(ytb_Download);
		}
		else if (config_media == 2) {
			Bilibili_Download = fopen("temp\\Bilibili_Download.bat", "w");
			fprintf(Bilibili_Download, "@echo off\n");
			fprintf(Bilibili_Download, "%s\n", config_proxy);
			fprintf(Bilibili_Download, "%s -c cookies\\Bilibili_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			fclose(Bilibili_Download);
		}
		else if (config_media == 3) {
			QQVideo_Download = fopen("temp\\QQVideo_Download.bat", "w");
			fprintf(QQVideo_Download, "@echo off\n");
			fprintf(QQVideo_Download, "%s -c cookies\\QQVideo_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			fclose(QQVideo_Download);
		}
		else if (config_media == 4) {
			iqiyi_Download = fopen("temp\\iqiyi_Download.bat", "w");
			fprintf(iqiyi_Download, "@echo off\n");
			fprintf(iqiyi_Download, "%s -c cookies\\iqiyi_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			fclose(iqiyi_Download);
		}
		else {
			Youku_Download = fopen("temp\\Youku_Download.bat", "w");
			fprintf(Youku_Download, "@echo off\n");
			fprintf(Youku_Download, "%s -c cookies\\Youku_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir, config_url);
			fclose(Youku_Download);
		}
	}
	else if (downloadmode == 4) {
		Download = fopen("temp\\Download.bat", "w");
		fprintf(Download, "@echo off\n");
		fprintf(Download, "%s -c -x%d -s%d -k%s -j %d %s %s %s %s %s --max-tries=0 --file-allocation=none --content-disposition-default-utf8=true %s\n", Downloader_Use, ConnectionNum, ConnectionNum, split, Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
		fclose(Download);
	}
	else if (downloadmode == 5) {
		if (magnet_mode == 2) {
			Download = fopen("temp\\Download.bat", "w");
			fprintf(Download, "@echo off\n");
			fprintf(Download, "%s --conf-path=config\\bt.conf %s\n", Downloader_Use, config_url);
			fclose(Download);
		}
		else {
			Download = fopen("temp\\Download.bat", "w");
			fprintf(Download, "@echo off\n");
			fprintf(Download, "%s --conf-path=config\\bt.conf %s\n", Downloader_Use, config_url);
			fclose(Download);
		}
	}
	system("cls");
	printf("------------------------------------------------------------------\n");
	printf("代理地址：%s(为空代表没有设置代理)\n", config_proxy);
	printf("下载线程数：%d\n", ConnectionNum);
	printf("下载引擎：%s\n", Downloader_Use);
	printf("浏览器标识：%s\n", head_show);
	printf("------------------------------------------------------------------\n");
	printf("下载过程中出现的红色ERROR报错可忽略，对下载没有影响！\n");
	printf("下载正在执行，希望中断下载建议按Ctrl+C以正常退出. . .\n");
	printf("------------------------------------------------------------------\n");
	if (downloadmode == 3) {
		if (config_media == 1) {
			download_result = system("temp\\ytb_Download.bat");
		}
		else if (config_media == 2) {
			download_result = system("temp\\Bilibili_Download.bat");
		}
		else if (config_media == 3) {
			download_result = system("temp\\QQVideo_Download.bat");
		}
		else if (config_media == 4) {
			download_result = system("temp\\iqiyi_Download.bat");
		}
		else {
			download_result = system("temp\\Youku_Download.bat");
		}
	}
	else {
		download_result = system("temp\\Download.bat");
	}
	if(download_result == 0) {
	//	system("del /f /s /q temp\\*.bat");
	//	system("cls");
	//	return 0;
	}
	else {
		return 1;
	}
}
