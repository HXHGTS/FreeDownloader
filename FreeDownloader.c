#include<stdio.h>
#include<stdlib.h>

int AdvanceDownloader(),AutoShutdown(int mode),BroswerMark(),CheckSum(int mode),dir(),downloadengine(),WindowSkin();
int MagnetDownloader(),MediaDownloader(),Netdisk(),NormalDownloader(),proxyswitcher(),threader(),url();
int downloadmode, magnet_mode,config_thread, config_media, anti_shutdown, Task, IsCheckSum;
int mark, redownload_result, shutdown, filecheck, DownloadList, OpenDir;
int cookie_import, cookie_mode,appid;
int scan_return;//接收返回值，暂时没用
char config_proxy[65], config_url[260], config_dir[35], config_cookie[230], smallcmd[20], Downloader_Use[12];
char reference[216], head[300], head_show[35], pre_proxy[56];
char location[200],split[7],torrent_loca[250],play_list[30], color[4];
char rpctoken[40];//定义rpc密钥
char BDUSS[193];
char cmd[100];
FILE* conf,*power_ini,*proxy_ini,*dir_mark, *skin;//定义配置文件
FILE* cookie,*bat;

int TokenGenerate() {
	system("powershell [guid]::NewGuid() | find /v \"Guid\" | find /v \"--\" | find \"-\" > config\\uuid.txt");
	conf = fopen("config\\uuid.txt", "r");
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
	conf = fopen("config\\bt.conf", "w");
	fprintf(conf, "bt-tracker=");
	fclose(conf);
	sprintf(cmd, "curl %s https://trackerslist.com/all_aria2.txt -# > config\\all_aria2.txt", pre_proxy);
	if (system(cmd) != 0) {
		printf("\n更新失败，建议配合代理或VPN重新打开软件更新列表，正在本地建立BT配置文件. . .\n");
		if (fopen("config\\all_aria2.txt", "r") != NULL) {
			printf("\n检测到已有的trackerlist记录，正在读取配置文件. . .\n");
			system("type config\\all_aria2.txt>>config\\bt.conf");
		}
		else {
			printf("\n请在弹出窗口中修改BT配置文件. . .\n");
			system("notepad config\\bt.conf");
		}
	}
	else {
		printf("\n更新成功，正在本地建立BT配置文件. . .\n");
		system("type config\\all_aria2.txt >> config\\bt.conf");
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
	fprintf(conf, "seed-ratio=0.0\n");
	fprintf(conf, "content-disposition-default-utf8=true\n");
	fprintf(conf, "user-agent=qBittorrent/4.2.5\n");
	fprintf(conf, "peer-agent=qBittorrent/4.2.5\n");
	fprintf(conf, "peer-id-prefix=-qB4250-\n");
	fclose(conf);
	system("cls");
	return 0;
}

int CreateFolder() {
	if (fopen("Downloads\\dir.md", "r")==NULL) {
		system("mkdir Downloads");
		dir_mark = fopen("Downloads\\dir.md", "w");
		fprintf(dir_mark, "##本文件夹存储下载数据\n");
		fclose(dir_mark);
	}
	if (fopen("config\\dir.md", "r") == NULL) {
		system("mkdir config");
		dir_mark = fopen("config\\dir.md", "w");
		fprintf(dir_mark, "##需要配置代理请修改proxy.ini文件，软件支持http/https代理\n");
		fprintf(dir_mark, "##标准格式为proxy=http://127.0.0.1:1080，请根据实际情况设置代理！\n");
		fclose(dir_mark);
	}
	if (fopen("cookies\\dir.md", "r") == NULL) {
		system("mkdir cookies");
		dir_mark = fopen("cookies\\dir.md", "w");
		fprintf(dir_mark, "##本文件夹主要存放账号登录的Cookie信息\n");
		fclose(dir_mark);
	}
	if (fopen("temp\\dir.md", "r") == NULL) {
		system("mkdir temp");
		dir_mark = fopen("temp\\dir.md", "w");
		fprintf(dir_mark, "##本文件夹为缓存信息文件夹\n");
		fclose(dir_mark);
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
	printf("已知对Clash(R)代理支持不足，若使用Clash(R)，请切换到Tap模式路由全局流量！\n\n");
	system("Timeout /T 3");
	if (system("GetProxyInfo.exe") != 0) {
		printf("UAC授权失败，请自行导入计算机代理设置！\n\n");
		system("echo proxy=0 > config\\proxy.ini");
		system("notepad config\\proxy.ini");
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
	printf("请选择下载功能：\n1.普通下载模式\n2.百度网盘模式\n3.视频下载模式\n4.高级下载模式\n5.磁力下载模式\n6.文件完整性测试\n7.Github上的软件帮助\n8.删除保存的账号登录信息与下载历史\n9.打开下载文件夹\n0.退出\n");
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
		system("echo 185.199.110.153 hxhgts.github.io > C:\\Windows\\System32\\drivers\\etc\\hosts");
		system("explorer.exe \"https://hxhgts.github.io/FreeDownloader/\"");
		system("echo 127.0.0.1 localhost > C:\\Windows\\System32\\drivers\\etc\\hosts");
		system("cls");
		goto MainMenu;
	}
	else if (downloadmode == 8) {
		printf("警告：此操作将删除软件保存的所有登录信息，确定请按任意键继续，取消请直接关闭软件窗口！！！\n");
		system("pause");
		printf("正在删除历史记录. . .\n");
		system("del /f /s /q temp\\*.bat");
		system("del /f /s /q temp\\*.download");
		printf("正在删除登录信息. . .\n");
		system("del /f /s /q cookies\\*.txt");
		printf("删除操作执行完成！\n");
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
		system("cls");
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

int MagnetDownloader() {
	proxyswitcher();
	CreateConfig();
	printf("请选择下载模式：\n\n1.种子文件导入\n\n2.输入磁力链\n\n请输入：");
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
			system("echo. >> temp\\ytb.download");
			printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\ytb.download");
			sprintf(config_url, "%s", "-a temp\\ytb.download");
		}
		else if (config_media == 2) {
			if (fopen("temp\\Bilibili.download.1", "r") == NULL) {
				url = fopen("temp\\Bilibili.download.1", "w");
				fclose(url);
			}
			system("echo ----------------------URL--------------------- > temp\\Bilibili.download");
			printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\Bilibili.download.1");
			system("type temp\\Bilibili.download.1 >> temp\\Bilibili.download");
			sprintf(config_url, "%s", "-F temp\\Bilibili.download");
		}
		else if (config_media == 3) {
			if (fopen("temp\\QQVideo.download.1", "r") == NULL) {
				url = fopen("temp\\QQVideo.download.1", "w");
				fclose(url);
			}
			system("echo ----------------------URL--------------------- > temp\\QQVideo.download");
			printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\QQVideo.download.1");
			system("type temp\\QQVideo.download.1 >> temp\\QQVideo.download");
			sprintf(config_url, "%s", "-F temp\\QQVideo.download");
		}
		else if (config_media == 4) {
			if (fopen("temp\\iqiyi.download", "r") == NULL) {
				url = fopen("temp\\iqiyi.download", "w");
				fclose(url);
			}
			system("echo ----------------------URL--------------------- > temp\\iqiyi.download");
			printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\iqiyi.download.1");
			system("type temp\\iqiyi.download.1 >> temp\\iqiyi.download");
			sprintf(config_url, "%s", "-F temp\\iqiyi.download");
		}
		else {
			if (fopen("temp\\Youku.download", "r") == NULL) {
				url = fopen("temp\\Youku.download", "w");
				fclose(url);
			}
			system("echo ----------------------URL--------------------- > temp\\Youku.download");
			printf("\n请在弹出页输入下载地址，多个地址可以用回车隔开. . .\n\n");
			system("notepad.exe temp\\Youku.download.1");
			system("type temp\\Youku.download.1 >> temp\\Youku.download");
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
		config_thread = 16;
		sprintf(Downloader_Use, "%s", "aria2c");
		sprintf(split, "2M");
	}
	else if (downloadmode == 2) {
		config_thread = 4;
		sprintf(split, "1M");
		sprintf(Downloader_Use, "%s", "aria2c");
		Task = 1;//同时下载任务数
	}
	else if (downloadmode == 3) {
		Task = 1;//同时下载任务数
		if (config_media != 1) {
			sprintf(Downloader_Use, "%s", "annie");
			config_thread = 1;//4k视频状态下aria2调用出现bug
		}
		else {
			sprintf(Downloader_Use, "%s", "youtube-dl");
			config_thread = 16;//aria2调用bug已修复
		}
	}
	else if (downloadmode == 5) {
		Task = 1;//同时下载任务数
		sprintf(Downloader_Use, "%s", "aria2c");
		config_thread = 16;
		if (magnet_mode == 2)sprintf(split, "1M");
	}
	return 0;
}//线程数修改与引擎选择        

int dir() {
	if (downloadmode == 3) {
		if (config_media != 1) {
			sprintf(config_dir, "%s", "-o Downloads");
		}
		else {
			sprintf(config_dir, "%s", "-o \"/Downloads/\%%(title)s.\%%(ext)s\"");
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
	char proxy[50];
	if (system("type config\\proxy.ini | find \"proxy=0\"") == 0) {
		sprintf(config_proxy, "%s", "");
	}
	else {
			proxy_ini = fopen("config\\proxy.ini", "r");
			scan_return=fscanf(proxy_ini, "proxy=%s", proxy);
			fclose(proxy_ini);
			if (downloadmode == 1 || downloadmode == 2 || downloadmode == 4) {
				sprintf(config_proxy, "--all-proxy=%s", proxy);
			}
			else if(downloadmode==3){
				if (config_media == 1) {
					sprintf(config_proxy, "--proxy http://%s", proxy);
				}
				else {
					sprintf(config_proxy, "set HTTP_PROXY=\"http://%s/\" &", proxy);
				}
			}
			else if (downloadmode == 5) {
				sprintf(pre_proxy, "-x %s", proxy);//curl proxy
				sprintf(config_proxy, "--all-proxy=%s", proxy);//magnet proxy
			}
	}
	return 0;// /config/proxy.ini中proxy=0或此文件不存在为无代理状态，否则使用代理(仅支持http/https代理)
}

int BroswerMark() {
	char UserAgent_DIY[275];
	if (downloadmode == 1) {
		sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.75 Safari/537.36");//Chrome浏览器
		sprintf(head_show, "Chrome浏览器");
	}
	else if (downloadmode == 2) {
		sprintf(head, "--header=\"User-Agent:%s\"", "LogStatistic");//百度网盘
		sprintf(head_show, "百度网盘");
	}
	else if (downloadmode == 3) {
		if (config_media == 1) {
			sprintf(head, "--user-agent \"%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.75 Safari/537.36");//Chrome浏览器
		}
		else {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.75 Safari/537.36");//Chrome浏览器
		}
		sprintf(head_show, "Chrome浏览器");
	}
	else if (downloadmode == 4) {
		printf("\n请选择浏览器标识：\n\n1.IE浏览器\n\n2.Chrome浏览器\n\n请输入：");
		scan_return=scanf("%d", &mark);
		if (mark == 1) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 10.0; WOW64; Trident/7.0; .NET4.0C; .NET4.0E; .NET CLR 2.0.50727; .NET CLR 3.0.30729; .NET CLR 3.5.30729)");//IE浏览器
			sprintf(head_show, "IE浏览器");
		}
		else if (mark == 2) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.75 Safari/537.36");//Chrome浏览器
			sprintf(head_show, "Chrome浏览器");
		}
		else {
			mark = 2;
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.75 Safari/537.36");//Chrome浏览器
			sprintf(head_show, "Chrome浏览器");
		}
	}
	else if (downloadmode == 5) {
		sprintf(head_show, "qBittorrent/4.2.5");
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
	sprintf(reference, "%s", "--referer=\"https://pan.baidu.com/wap/home#/\"");
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
			printf("\n请输入BDUSS值:\n");
			scan_return=scanf("%s", BDUSS);
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
	if (config_media == 1) {
		printf("\n下载整个列表内所有音视频？\n\n1.是\n\n0.否\n\n请输入：");
		scan_return=scanf("%d", &DownloadList);
		if (DownloadList == 0) {
			sprintf(play_list, "--no-playlist");
		}
		else {
			sprintf(play_list, "--yes-playlist");
		}
	}
	else {
			printf("\n下载整个列表内所有音视频？\n\n1.是\n\n2.只下载当前视频\n\n0.选择集数\n\n请输入：");
			scan_return=scanf("%d", &DownloadList);
			if (DownloadList == 1) {
				sprintf(play_list, "-p");
			}
			else if (DownloadList == 2) {
				sprintf(play_list, "");
			}
			else {
				printf("\n请按照格式输入下载范围，如1-5,6,7,8-15：");
				scan_return=scanf("%s", chapter);
				sprintf(play_list, "-p -items %s", chapter);
			}
	}
	if (config_media == 1) {
		if (fopen("cookies\\ytb_Cookies.txt", "r") == NULL) {
			ytb_Cookies = fopen("cookies\\ytb_Cookies.txt", "w");
			fprintf(ytb_Cookies, "# Input Cookie below (must delete this line!!!)#\n");
			fclose(ytb_Cookies);
		}
		system("notepad cookies\\ytb_Cookies.txt");
		printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
		ytb_Cookies = fopen("cookies\\ytb_Cookies.txt", "a");
		fprintf(ytb_Cookies, "\n");
		fclose(ytb_Cookies);
	}
	else if (config_media == 2) {
		if (fopen("cookies\\Bilibili_Cookies.txt", "r") == NULL) {
			Bilibili_Cookies = fopen("cookies\\Bilibili_Cookies.txt", "w");
			fprintf(Bilibili_Cookies, "# Input Cookie below#\n");
			fclose(Bilibili_Cookies);
		}
		system("notepad cookies\\Bilibili_Cookies.txt");
		printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
		Bilibili_Cookies = fopen("cookies\\Bilibili_Cookies.txt", "a");
		fprintf(Bilibili_Cookies, "\n");
		fclose(Bilibili_Cookies);
	}
	else if (config_media == 3) {
		if (fopen("cookies\\QQVideo_Cookies.txt", "r") == NULL) {
			QQVideo_Cookies = fopen("cookies\\QQVideo_Cookies.txt", "w");
			fprintf(QQVideo_Cookies, "# Input Cookie below#\n");
			fclose(QQVideo_Cookies);
		}
		system("notepad cookies\\QQVideo_Cookies.txt");
		printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
		QQVideo_Cookies = fopen("cookies\\QQVideo_Cookies.txt", "a");
		fprintf(QQVideo_Cookies, "\n");
		fclose(QQVideo_Cookies);
	}
	else if (config_media == 4) {
		if (fopen("cookies\\iqiyi_Cookies.txt", "r") == NULL) {
			iqiyi_Cookies = fopen("cookies\\iqiyi_Cookies.txt", "w");
			fprintf(iqiyi_Cookies, "# Input Cookie below#\n");
			fclose(iqiyi_Cookies);
		}
		system("notepad cookies\\iqiyi_Cookies.txt");
		printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
		iqiyi_Cookies = fopen("cookies\\iqiyi_Cookies.txt", "a");
		fprintf(iqiyi_Cookies, "\n");
		fclose(iqiyi_Cookies);
	}
	else {
		if (fopen("cookies\\Youku_Cookies.txt", "r") == NULL) {
			Youku_Cookies = fopen("cookies\\Youku_Cookies.txt", "w");
			fprintf(Youku_Cookies, "# Input Cookie below#\n");
			fclose(Youku_Cookies);
		}
		system("notepad cookies\\Youku_Cookies.txt");
		printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
		Youku_Cookies = fopen("cookies\\Youku_Cookies.txt", "a");
		fprintf(Youku_Cookies, "\n");
		fclose(Youku_Cookies);
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
		fprintf(Download, "%s -c -x%d -s%d -k%s --file-allocation=none -j %d %s %s %s %s\n", Downloader_Use, config_thread,config_thread, split, Task, config_dir, config_proxy, head, config_url);
		fclose(Download);
	}
	else if (downloadmode == 2) {
		Download = fopen("temp\\Download.bat", "w");
		fprintf(Download, "@echo off\n");
		fprintf(Download, "%s -c -x%d -s%d --log-level=error --file-allocation=none -k%s -j %d %s %s %s %s %s --content-disposition-default-utf8=true %s\n", Downloader_Use, config_thread, config_thread, split, Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
		fclose(Download);
	}
	else if (downloadmode == 3) {
		if (config_media == 1) {
			ytb_Download = fopen("temp\\ytb_Download.bat", "w");
			fprintf(ytb_Download, "@echo off\n");
			fprintf(ytb_Download, "%s -f bestvideo+bestaudio --write-sub --all-subs %s %s --cookies cookies\\ytb_Cookies.txt %s %s %s --external-downloader aria2c --external-downloader-args \"-c -x16 -s16 -k1M --file-allocation=none\"\n", Downloader_Use,config_proxy, head,play_list, config_dir,config_url);//官方已修复，已添加aria2c支持
			fclose(ytb_Download);
		}
		else if (config_media == 2) {
			Bilibili_Download = fopen("temp\\Bilibili_Download.bat", "w");
			fprintf(Bilibili_Download, "@echo off\n");
			fprintf(Bilibili_Download, "%s %s -c cookies\\Bilibili_Cookies.txt %s %s %s\n", config_proxy, Downloader_Use, play_list, config_dir, config_url);
			fclose(Bilibili_Download);
			
		}
		else if (config_media == 3) {
			QQVideo_Download = fopen("temp\\QQVideo_Download.bat", "w");
			fprintf(QQVideo_Download, "@echo off\n");
			fprintf(QQVideo_Download, "%s %s -c cookies\\QQVideo_Cookies.txt %s %s %s\n", config_proxy, Downloader_Use, play_list, config_dir, config_url);
			fclose(QQVideo_Download);

		}
		else if (config_media == 4) {
			iqiyi_Download = fopen("temp\\iqiyi_Download.bat", "w");
			fprintf(iqiyi_Download, "@echo off\n");
			fprintf(iqiyi_Download, "%s %s -c cookies\\iqiyi_Cookies.txt %s %s %s\n", config_proxy, Downloader_Use, play_list, config_dir, config_url);
			fclose(iqiyi_Download);
		}
		else {
			Youku_Download = fopen("temp\\Youku_Download.bat", "w");
			fprintf(Youku_Download, "@echo off\n");
			fprintf(Youku_Download, "%s %s -c cookies\\Youku_Cookies.txt %s %s %s\n", config_proxy, Downloader_Use, play_list, config_dir, config_url);
			fclose(Youku_Download);
		}
	}
	else if (downloadmode == 4) {
		Download = fopen("temp\\Download.bat", "w");
		fprintf(Download, "@echo off\n");
		fprintf(Download, "%s -c -x%d -s%d -k%s -j %d %s %s %s %s %s --max-tries=0 --file-allocation=none --content-disposition-default-utf8=true %s\n", Downloader_Use, config_thread, config_thread, split, Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
		fclose(Download);
	}
	else if (downloadmode == 5) {
		if (magnet_mode == 2) {
			Download = fopen("temp\\Download.bat", "w");
			fprintf(Download, "@echo off\n");
			fprintf(Download, "%s --conf-path=config\\bt.conf %s %s\n", Downloader_Use, config_proxy, config_url);
			fclose(Download);
		}
		else {
			Download = fopen("temp\\Download.bat", "w");
			fprintf(Download, "@echo off\n");
			fprintf(Download, "%s --conf-path=config\\bt.conf %s %s\n", Downloader_Use, config_proxy, config_url);
			fclose(Download);
		}
	}
	system("cls");
	printf("------------------------------------------------------------------\n");
	printf("代理地址：%s(为空代表没有设置代理)\n", config_proxy);
	printf("下载线程数：%d\n", config_thread);
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
		system("del /f /s /q temp\\*.bat");
		system("cls");
		return 0;
	}
	else {
		return 1;
	}
}
