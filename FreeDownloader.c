#include<stdio.h>
#include<stdlib.h>

int AdvanceDownloader(),AutoShutdown(),BroswerMark(),CheckSum(),dir(),downloadengine(),ExportDownloader(),WindowSkin();
int MagnetDownloader(),MediaDownloader(),Netdisk(),NormalDownloader(),proxyswitcher(),threader(),url();
int downloadmode, magnet_mode,config_thread, config_media, anti_shutdown, Download_Task, IsCheckSum;
char config_proxy[65], config_url[260], config_dir[35], config_cookie[40], smallcmd[20],cmd[1500], Downloader_Use[15];
char reference[216], head[300], head_show[30];
char location[200],split[7],torrent_loca[250],play_list[30], color[4];
char rpctoken[21] = "UpxBsuNq2rHVKxdJh9Tp";
int mark,proxymode, redownload_result,  shutdown, filecheck,use_list,OpenDir;
FILE* * conf,*save,*power_ini,*dic,*Media_conf,*dir_mark, *skin;

int WindowSkin() {
	if (fopen("config\\skin.ini", "r") == NULL) {
		printf("重置窗体与字体颜色请直接删除config目录下skin.ini文件！\n");
		printf("请按照\"窗口颜色+字体颜色\"的格式设置皮肤，对应关系如下：\n");
		printf("---------------------------------------\n");
		printf("0=黑色   8=灰色\n");
		printf("1=蓝色   9=淡蓝色\n");
		printf("2=绿色   A=淡绿色\n");
		printf("3=浅绿色 B=淡浅绿色\n");
		printf("4=红色   C=淡红色\n");
		printf("5=紫色   D=淡紫色\n");
		printf("6=黄色   E=淡黄色\n");
		printf("7=白色   F=亮白色\n");
		printf("---------------------------------------\n");
		printf("请输入以配置界面与字体颜色，如0A：");
		scanf("%s", color);
		skin = fopen("config\\skin.ini", "w");
		fprintf(skin,"%s",color);
		fclose(skin);
	}
	else {
		skin = fopen("config\\skin.ini", "r");
		fread(color, 1, 2, skin);
		fclose(skin);
	}
	sprintf(smallcmd, "color %s", color);
	system(smallcmd);
	system("cls");
	return 0;
}

int CreateFolder() {
	if (fopen("Downloads\\dir.md", "r") == NULL){
		system("mkdir Downloads");
		dir_mark = fopen("Downloads\\dir.md", "w");
		fprintf(dir_mark, "##This file is auto-created by FreeDownloader,don't move or delete!##\n");
		fclose(dir_mark);
	}
	if (fopen("config\\dir.md", "r") == NULL) {
		system("mkdir config");
		dir_mark = fopen("config\\dir.md", "w");
		fprintf(dir_mark, "##This file is auto-created by FreeDownloader,don't move or delete!##\n");
		fclose(dir_mark);
	}
	if (fopen("cookies\\dir.md", "r") == NULL) {
		system("mkdir cookies");
		dir_mark = fopen("cookies\\dir.md", "w");
		fprintf(dir_mark, "##This file is auto-created by FreeDownloader,don't move or delete!##\n");
		fclose(dir_mark);
	}
	if (fopen("temp\\dir.md", "r") == NULL) {
		system("mkdir temp");
		dir_mark = fopen("temp\\dir.md", "w");
		fprintf(dir_mark, "##This file is auto-created by FreeDownloader,don't move or delete!##\n");
		fclose(dir_mark);
	}
		system("del Downloads\\best_aria2.txt");
		system("cls");
		Media_conf = fopen("config\\Media.conf", "w");
		fprintf(Media_conf, "dir=Downloads\n");
		fprintf(Media_conf, "continue=true\n");
		fprintf(Media_conf, "user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36\n");
		fprintf(Media_conf, "max-concurrent-downloads=1\n");
		fprintf(Media_conf, "max-connection-per-server=16\n");
		fprintf(Media_conf, "min-split-size=2M\n");
		fprintf(Media_conf, "disk-cache=128M\n");
		fprintf(Media_conf, "split=64\n");
		fprintf(Media_conf, "file-allocation=none\n");
		fprintf(Media_conf, "enable-rpc=true\n");
		fprintf(Media_conf, "rpc-secret=%s\n",rpctoken);
		fprintf(Media_conf, "rpc-allow-origin-all=true\n");
		fprintf(Media_conf, "rpc-listen-all=true\n");
		fprintf(Media_conf, "rpc-listen-port=6800\n\n");
		fclose(Media_conf);
		return 0;
}

int main() {
	redownload_result = 0;
	filecheck = 0;
	anti_shutdown = shutdown = 0;
	system("title FreeDownloader");
	CreateFolder();
	WindowSkin();
p_3:system("cls");
	printf("------------------------------------------------\n");
	printf("---------------- FreeDownloader ----------------\n");
	printf("------------------------------------------------\n");
	printf("请选择下载功能：\n1.普通下载模式\n2.百度网盘下载\n3.视频下载模式\n4.高级下载模式\n5.磁力链下载模式\n6.文件完整性测试\n7.Github上的软件帮助\n8.打开下载文件夹\n9.清空下载记录\n0.退出\n");
	printf("------------------------------------------------\n");
	printf("请输入：");
	scanf("%d", &downloadmode);
	system("cls");
	if (fopen("config\\power.ini", "r") == NULL) {
		if (downloadmode != 7 && downloadmode != 8 && downloadmode != 9 && downloadmode != 0) {
			printf("\n是否设置下载完成自动关机（是=1，否=0）：");
			scanf("%d", &shutdown);
		}
	}
	else {
		shutdown = 0;
	}
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
		goto p_3;
	}
	else if (downloadmode == 7) {
		printf("正在打开帮助界面. . .\n");
		system("explorer.exe \"https://hxhgts.github.io/FreeDownloader/\"");
		system("cls");
		goto p_3;
	}
	else if (downloadmode == 8) {
		system("explorer.exe Downloads");
		system("cls");
		goto p_3;
	}
	else if (downloadmode == 9) {
		system("del /f /s /q temp\\*");
		system("cls");
		goto p_3;
	}
	else {
		exit(0);
	}
p_2:redownload_result = downloadengine();
	if (redownload_result == 0) {
		system("cls");
		printf("-----------------------------------------------------\n");
		printf("----------------------下载成功!----------------------\n");
		printf("-----------------------------------------------------\n");
		AutoShutdown(shutdown);
		printf("\n是否打开下载文件夹：\n\n1.是\n\n0.否\n\n请输入：");
		scanf("%d", &OpenDir);
		if (OpenDir != 0) {
			system("explorer Downloads");
		}
		system("cls");
		goto p_3;
	}
	else {
		system("cls");
		printf("-----------------------------------------------------\n");
		printf("----------------------下载失败!----------------------\n");
		printf("-----------------------------------------------------\n");
		system("Timeout /T 3");
		if (downloadmode == 1 || downloadmode == 3 || downloadmode == 4 || downloadmode == 5 ) {
			system("cls");
			goto p_2;
		}
		else {
				system("cls");
				goto p_2;
			}
	}
}//下载工具主程序

int NormalDownloader() {
	url();
	dir();
	threader();
	proxyswitcher();
	BroswerMark();
}

int MagnetDownloader() {
	int tracker_update;
	printf("请选择下载模式：\n\n1.种子文件导入\n\n2.输入磁力链\n\n请输入：");
	scanf("%d", &magnet_mode);
	if (magnet_mode == 2) {
		dir();
	}
	threader();
	printf("\n是否联网更新tracker：\n\n1.是\n\n0.否\n\n请输入：");
	scanf("%d", &tracker_update);
	system("cls");
	if (tracker_update == 1) {
		printf("正在尝试连接到trackerslist.com服务器. . .\n\n");
		if (system("aria2c --dir=Downloads https://trackerslist.com/best_aria2.txt") != 0) {
			printf("\n更新失败，正在本地建立BT配置文件. . .\n");
			conf = fopen("config\\bt.conf", "w");
			fprintf(conf, "##bt-tracker=server1,server2,server3\n");
			fprintf(conf, "listen-port=20331\n");
			fprintf(conf, "continue=true\n");
			fprintf(conf, "max-concurrent-downloads=1\n");
			fprintf(conf, "max-connection-per-server=16\n");
			fprintf(conf, "bt-max-peers=999\n");
			fprintf(conf, "min-split-size=2M\n");
			fprintf(conf, "disk-cache=128M\n");
			fprintf(conf, "split=64\n");;
			fprintf(conf, "dir=Downloads/\n");
			fprintf(conf, "file-allocation=none\n");
			fprintf(conf, "enable-peer-exchange=true\n");
			fprintf(conf, "seed-ratio=0.0\n");
			fprintf(conf, "user-agent=qBittorrent/4.2.5\n");
			fprintf(conf, "peer-agent=qBittorrent/4.2.5\n");
			fprintf(conf, "peer-id-prefix=-qB4250-\n");
			fclose(conf);
		}
			else {
				printf("\n更新成功，正在本地建立BT配置文件. . .\n");
				conf = fopen("config\\bt.conf", "w");
				fprintf(conf, "bt-tracker=");
				fclose(conf);
				system("type Downloads\\best_aria2.txt>>config\\bt.conf");
				conf = fopen("config\\bt.conf", "a");
				fprintf(conf, "\nlisten-port=20331\n");
				fprintf(conf, "continue=true\n");
				fprintf(conf, "max-concurrent-downloads=1\n");
				fprintf(conf, "max-connection-per-server=16\n");
				fprintf(conf, "bt-max-peers=999\n");
				fprintf(conf, "min-split-size=2M\n");
				fprintf(conf, "disk-cache=128M\n");
				fprintf(conf, "split=64\n");;
				fprintf(conf, "dir=Downloads/\n");
				fprintf(conf, "file-allocation=none\n");
				fprintf(conf, "enable-peer-exchange=true\n");
				fprintf(conf, "seed-ratio=0.0\n");
				fprintf(conf, "user-agent=qBittorrent/4.2.5\n");
				fprintf(conf, "peer-agent=qBittorrent/4.2.5\n");
				fprintf(conf, "peer-id-prefix=-qB4250-\n");
				fclose(conf);
			}
		}
	else {
		printf("正在本地建立BT配置文件，建议手动导入tracker服务器列表以加快BT下载速度. . .\n");
		if (fopen("config\\bt.conf", "r") == NULL) {
			conf = fopen("config\\bt.conf", "w");
			fprintf(conf, "##bt-tracker=server1,server2,server3\n");
			fprintf(conf, "listen-port=20331\n");
			fprintf(conf, "continue=true\n");
			fprintf(conf, "max-concurrent-downloads=1\n");
			fprintf(conf, "max-connection-per-server=16\n");
			fprintf(conf, "bt-max-peers=999\n");
			fprintf(conf, "min-split-size=2M\n");
			fprintf(conf, "disk-cache=128M\n");
			fprintf(conf, "split=64\n");;
			fprintf(conf, "dir=Downloads/\n");
			fprintf(conf, "file-allocation=none\n");
			fprintf(conf, "enable-peer-exchange=true\n");
			fprintf(conf, "seed-ratio=0.0\n");
			fprintf(conf, "user-agent=qBittorrent/4.2.5\n");
			fprintf(conf, "peer-agent=qBittorrent/4.2.5\n");
			fprintf(conf, "peer-id-prefix=-qB4250-\n");
			fclose(conf);
		}
	}
	printf("\n请在弹出窗口中修改BT配置文件. . .\n");
	system("notepad config\\bt.conf");
	BroswerMark();
	proxyswitcher();
	url();
	return 0;
}

int url() {
	FILE* url;
	if (downloadmode == 1) {
		if (fopen("temp\\normal.download", "r")==NULL) {
			url = fopen("temp\\normal.download", "w");
			fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址. . .\n\n");
		system("notepad.exe temp\\normal.download");
		sprintf(config_url, "%s", "-i temp\\normal.download");
	}
	else if (downloadmode == 2) {
		if (fopen("temp\\netdisk.download", "r") == NULL) {
			url = fopen("temp\\netdisk.download", "w");
			fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址. . .\n\n");
		system("notepad.exe temp\\netdisk.download");
		sprintf(config_url, "%s", "-i temp\\netdisk.download");
	}
	else if (downloadmode == 3) {
		if (config_media == 1) {
			if (fopen("temp\\ytb.download", "r") == NULL) {
				url = fopen("temp\\ytb.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe temp\\ytb.download");
			sprintf(config_url, "%s", "-a temp\\ytb.download");
		}
		else if (config_media == 2) {
			if (fopen("temp\\Bilibili.download", "r") == NULL) {
				url = fopen("temp\\Bilibili.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe temp\\Bilibili.download");
			sprintf(config_url, "%s", "-F temp\\Bilibili.download");
		}
		else if (config_media == 3) {
			if (fopen("temp\\QQVideo.download", "r") == NULL) {
				url = fopen("temp\\QQVideo.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe temp\\QQVideo.download");
			sprintf(config_url, "%s", "-F temp\\QQVideo.download");
		}
		else if (config_media == 4) {
			if (fopen("temp\\iqiyi.download", "r") == NULL) {
				url = fopen("temp\\iqiyi.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe temp\\iqiyi.download");
			sprintf(config_url, "%s", "-F temp\\iqiyi.download");
		}
		else {
			if (fopen("temp\\Youku.download", "r") == NULL) {
				url = fopen("temp\\Youku.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe temp\\Youku.download");
			sprintf(config_url, "%s", "-F temp\\Youku.download");
		}
		}
	else if (downloadmode == 4) {
		if (fopen("temp\\advance.download", "r") == NULL) {
			url = fopen("temp\\advance.download", "w");
			fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址. . .\n\n");
		system("notepad.exe temp\\advance.download");
		sprintf(config_url, "%s", "-i temp\\advance.download");
	}
	else if (downloadmode == 5) {
		if (magnet_mode == 2) {
			if (fopen("temp\\magnet.download", "r") == NULL) {
				url = fopen("temp\\magnet.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe temp\\magnet.download");
			sprintf(config_url, "%s", "-i temp\\magnet.download");
		}
		else {
			printf("\n若种子文件名过长建议重命名成简单字母或数字再拖入窗口，否则可能报错！\n\n");
			printf("请将种子文件以拖拽至本窗口中：");
			scanf("%s", torrent_loca);
			sprintf(config_url, "\"%s\"",torrent_loca);
		}
	}
	return 0;
}//网址输入插件

int threader() {
	if (downloadmode == 1 || downloadmode == 4) {
		Download_Task = 1;//同时下载任务数
		config_thread = 16;
		sprintf(Downloader_Use, "%s", "aria2c");
		sprintf(split, "1M");
	}
	else if (downloadmode == 2) {
		Download_Task = 1;//同时下载任务数
		sprintf(Downloader_Use, "%s", "aria2c");
		config_thread = 3;
		sprintf(split, "1M");
	}
	else if (downloadmode == 3) {
		Download_Task = 1;//同时下载任务数
		if (config_media != 1) {
			sprintf(Downloader_Use, "%s", "annie");
		}
		else {
			sprintf(Downloader_Use, "%s", "youtube-dl");
		}
		config_thread = 16;
	}
	else if (downloadmode == 5) {
		Download_Task = 1;//同时下载任务数
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
	if ((fopen("config\\proxy.ini", "r")) != NULL) {
		sprintf(config_proxy, "%s", "");
	}
	else {
		if (downloadmode == 1 || downloadmode == 2 || downloadmode == 3 || downloadmode == 4 || downloadmode == 5) {
			printf("\n是否设置网络代理（是=1，否=0）：");
			scanf("%d", &proxymode);
		}
		if (downloadmode != 3) {
			if (proxymode == 0) {
				sprintf(config_proxy, "%s", "");
			}
			else {
				printf("\n请输入代理参数，如http://127.0.0.1:1080：");
				scanf("%s", proxy);
				sprintf(config_proxy, "--all-proxy=%s", proxy);
			}
		}
		else {
			if (proxymode == 0) {
				sprintf(config_proxy, "%s", "");
			}
			else {
				printf("\n请输入代理参数，如http://127.0.0.1:1080：");
				scanf("%s", proxy);
				if (config_media == 1) {
					sprintf(config_proxy, "--proxy %s", proxy);
				}
				else {
					sprintf(config_proxy, "set HTTP_PROXY=\"%s/\" &", proxy);
				}
			}
		}
	}
	return 0;
}

int BroswerMark() {
	char UserAgent_DIY[275];
	if (downloadmode == 3) {
		sprintf(head, "--user-agent \"%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.87 Safari/537.36");
		sprintf(head_show, "Windows版Chrome");
	}
	else if(downloadmode==2){
		printf("\n应用id为778750，下载失败请尝试切换应用id与浏览器标识！\n");
		printf("\n请选择浏览器标识：\n\n1.爱奇艺(官方的高速通道，不过貌似对1G以上文件不友好)\n\n2.百度网盘客户端(应该是最慢的方案了)\n\n3.Pandownload(最新解决方案，可能不稳定)\n\n请输入：");
		scanf("%d", &mark);
		if (mark == 1) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Linux; Android 5.0; SM-N9100 Build/LRX21V) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/37.0.0.0 Mobile Safari/537.36 NetType/WIFI Amoeba/1.0");//爱奇艺
			sprintf(head_show, "%s", "爱奇艺");
		}
		else if (mark == 2) {
			sprintf(head, "--header=\"User-Agent:%s\"", "netdisk;6.9.10.1;PC;PC-Windows;10.0.19041;WindowsBaiduYunGuanJia");//百度网盘
			sprintf(head_show, "%s", "百度网盘客户端");
		}
		else if (mark == 3) {
			sprintf(head, "--header=\"User-Agent:%s\"", "netdisk;P2SP;2.2.60.26");//Pandownload
			sprintf(head_show, "%s", "Pandownload");
		}
		else {
			mark = 3;
			sprintf(head, "--header=\"User-Agent:%s\"", "netdisk;P2SP;2.2.60.26");//Pandownload
			sprintf(head_show, "%s", "Pandownload");
		}
	}
	else if(downloadmode==1){
		sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36");//Windows版Chrome
		sprintf(head_show, "Windows版Chrome");
	}
	else if (downloadmode == 4) {
		printf("\n请选择浏览器标识：\n\n1.IE浏览器（新版）\n\n2.Windows版Chrome\n\n3.Edge浏览器\n\n4.IE浏览器（旧版）\n\n请输入：");
		scanf("%d", &mark);
		if (mark == 1) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko");//IE浏览器（增强模式）
			sprintf(head_show, "IE浏览器（新版）");
		}
		else if (mark == 2) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36");//Windows版Chrome
			sprintf(head_show, "Windows版Chrome");
		}
		else if (mark == 3) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36 Edge/18.19041");//Edge浏览器
			sprintf(head_show, "Edge浏览器");
		}
		else if (mark == 4) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 10.0; WOW64; Trident/7.0; .NET4.0C; .NET4.0E; .NET CLR 2.0.50727; .NET CLR 3.0.30729; .NET CLR 3.5.30729)");//IE浏览器（兼容模式）
			sprintf(head_show, "IE浏览器（旧版）");
		}
		else {
			mark = 2;
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36");//Windows版Chrome
			sprintf(head_show, "Windows版Chrome");
		}
	}
	else if (downloadmode == 5) {
		sprintf(head_show, "qBittorrent/4.2.5");
	}
	return 0;
}

int AdvanceDownloader() {
	int cookieuse;
	char reference_input[200];
	FILE* cookie;
	BroswerMark();
	printf("\n请输入下载引用页地址：\n");
	scanf("%s", reference_input);
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
		scanf("%d", &cookieuse);
		if (cookieuse != 1) {
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
	int cookieuse;
	FILE* cookie;
	BroswerMark();
	if(mark == 1)sprintf(reference, "%s", "--referer=\"https://pan.baidu.com/wap/home#/\"");
	else {
		sprintf(reference, "%s", "--referer=\"https://pan.baidu.com/disk/home?#/all?path=%2F&vmode=list\"");
	}
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
		scanf("%d", &cookieuse);
		if (cookieuse != 1) {
			goto p_4;
		}
	}
	sprintf(config_cookie, "--load-cookies=\"cookies\\Netdisk_Cookies.txt\"");
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
		scanf("%s", location);
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
		scanf("%d", &anti_shutdown);
		if (anti_shutdown == 1) {
			system("shutdown -a");
		}
	}
	return 0;
}

int MediaDownloader() {
	FILE* Bilibili_Cookies,*ytb_Cookies,* QQVideo_Cookies,*iqiyi_Cookies,*Youku_Cookies;
	char chapter[14];
	printf("\n下载音视频来源：\n\n1.油管\n\n2.哔哩哔哩\n\n3.腾讯视频\n\n4.爱奇艺\n\n5.优酷\n\n请输入：");
	scanf("%d", &config_media);
	if (config_media == 1) {
		printf("\n下载整个列表内所有音视频？\n\n1.是\n\n0.否\n\n请输入：");
		scanf("%d", &use_list);
		if (use_list == 0) {
			sprintf(play_list, "--no-playlist");
		}
		else {
			sprintf(play_list, "--yes-playlist");
		}
	}
	else {
		printf("\n下载整个列表内所有音视频（哔哩哔哩仅支持选项1）？\n\n1.是\n\n2.只下载当前视频\n\n0.选择集数\n\n请输入：");
		scanf("%d", &use_list);
		if (config_media == 2) {
			use_list = 1;
		}
		if (use_list == 2) {
			sprintf(play_list, "");
		}
		else if (use_list == 1) {
			sprintf(play_list, "-p");
		}
		else {
			printf("\n请按照格式输入下载范围，如1-5,6,7,8-15：");
			scanf("%s", chapter);
			sprintf(play_list, "-p -items %s",chapter);
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
	FILE* Bilibili_Download,*ytb_Download,*QQVideo_Download,*iqiyi_Download,*Youku_Download,*Download;
	int download_result;
	if (downloadmode == 1) {
		sprintf(cmd, "%s -c -x%d -k%s --file-allocation=none -j %d %s %s %s %s", Downloader_Use, config_thread, split, Download_Task, config_dir, config_proxy, head, config_url);
	}
	else if (downloadmode == 2) {
		sprintf(cmd, "%s -c -x%d -s64 --timeout=10 --file-allocation=none --input-file=\\temp\\aria2.session --save-session=\\temp\\aria2.session --min-split-size=10M -k%s -j %d %s %s %s %s %s --check-certificate=false --content-disposition-default-utf8=true %s", Downloader_Use, config_thread, split, Download_Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
	}
	else if (downloadmode == 3) {
		if (config_media == 1) {
			ytb_Download = fopen("temp\\ytb_Download.bat", "w");
			fprintf(ytb_Download, "@echo off\n");
			fprintf(ytb_Download, "%s -f bestvideo+bestaudio --write-sub --all-subs --cookies cookies\\ytb_Cookies.txt %s %s %s\n", Downloader_Use, play_list, config_dir,config_url);//暂时不添加aria2c支持，待官方修复
			fclose(ytb_Download);
		}
		else if (config_media == 2) {
			Bilibili_Download = fopen("temp\\Bilibili_Download.bat", "w");
			fprintf(Bilibili_Download, "@echo off\n");
			fprintf(Bilibili_Download, "%s %s -c cookies\\Bilibili_Cookies.txt %s %s %s -aria2 -aria2token %s\n", config_proxy, Downloader_Use, play_list, config_dir, config_url,rpctoken);
			fclose(Bilibili_Download);
			
		}
		else if (config_media == 3) {
			QQVideo_Download = fopen("temp\\QQVideo_Download.bat", "w");
			fprintf(QQVideo_Download, "@echo off\n");
			fprintf(QQVideo_Download, "%s %s -c cookies\\QQVideo_Cookies.txt %s %s %s -aria2 -aria2token %s\n", config_proxy, Downloader_Use, play_list, config_dir, config_url, rpctoken);
			fclose(QQVideo_Download);

		}
		else if (config_media == 4) {
			iqiyi_Download = fopen("temp\\iqiyi_Download.bat", "w");
			fprintf(iqiyi_Download, "@echo off\n");
			fprintf(iqiyi_Download, "%s %s -c cookies\\iqiyi_Cookies.txt %s %s %s -aria2 -aria2token %s\n", config_proxy, Downloader_Use, play_list, config_dir, config_url, rpctoken);
			fclose(iqiyi_Download);

		}
		else {
			Youku_Download = fopen("temp\\Youku_Download.bat", "w");
			fprintf(Youku_Download, "@echo off\n");
			fprintf(Youku_Download, "%s %s -c cookies\\Youku_Cookies.txt %s %s %s -aria2 -aria2token %s\n", config_proxy, Downloader_Use, play_list, config_dir, config_url, rpctoken);
			fclose(Youku_Download);
		}
	}
	else if (downloadmode == 4) {
		sprintf(cmd, "%s -c -x%d -k%s -j %d %s %s %s %s %s --file-allocation=none --content-disposition-default-utf8=true %s", Downloader_Use, config_thread,split, Download_Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
	}
	else if (downloadmode == 5) {
		if (magnet_mode == 2) {
			sprintf(cmd, "%s -c -x%d -k%s -j %d %s %s %s --conf-path=config\\bt.conf %s", Downloader_Use, config_thread, split, Download_Task, config_dir, config_proxy, head, config_url);
		}
		else {
			sprintf(cmd, "%s --conf-path=config\\bt.conf %s", Downloader_Use,config_url);
		}
	}
	system("cls");
	printf("-----------------------------------------------------\n");
	printf("代理地址：%s(为空代表没有设置代理)\n", config_proxy);
	printf("下载线程数：%d\n", config_thread);
	printf("下载引擎：%s\n", Downloader_Use);
	printf("浏览器标识：%s\n", head_show);
	printf("下载过程中出现的红色ERROR报错可忽略，对下载没有影响！\n");
	printf("-----------------------------------------------------\n");
	printf("下载正在执行，希望中断下载建议按Ctrl+C以正常退出. . .\n");
	printf("-----------------------------------------------------\n");
	if (downloadmode == 3) {
		if (config_media != 1) {
			printf("正在弹出窗口中建立本地下载任务监听进程，请不要手动关闭. . .\n");
		}
		else {
			printf("正在执行下载任务. . .\n\n");
		}
		if (config_media == 1) {
			system("temp\\ytb_Download.bat");
		}
		if (config_media != 1) {
			printf("\n正在新建弹出窗口并发送下载任务. . .\n\n");
			system("start aria2c --conf-path=config\\Media.conf");
		if (config_media == 2) {
			system("temp\\Bilibili_Download.bat");
		}
		else if (config_media == 3) {
			system("temp\\QQVideo_Download.bat");
		}
		else if (config_media == 4) {
			system("temp\\iqiyi_Download.bat");
		}
		else {
			system("temp\\Youku_Download.bat");
		}
			printf("\n由于系统限制，下载进程无法自动停止，若弹出窗口一直显示complete，或需停止下载，");
			system("pause");
			printf("\n\n");
			system("taskkill /f /im aria2c.exe");
			system("del Downloads\\best_aria2.txt");
		}
		system("del /f /s /q temp\\*.bat");
		system("cls");
		download_result = 0;
	}
	else {
		Download = fopen("temp\\Download.bat","w");
		fprintf(Download, "@echo off\n");
		fprintf(Download, "%s\n", cmd);
		fclose(Download);
		download_result = system("temp\\Download.bat");
		system("del /f /s /q temp\\*.bat");
		system("cls");
	}
	if (download_result == 0) {
		return 0;
	}
	else {
		return 1;
	}
}
