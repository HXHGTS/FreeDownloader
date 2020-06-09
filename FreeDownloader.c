#include<stdio.h>
#include<stdlib.h>
#include<io.h>
int downloadmode, magnet_mode,config_thread, config_media, anti_shutdown, Download_Task, IsCheckSum,Auto_ChangeUA;
char config_proxy[65], config_url[200], config_dir[35], config_cookie[40], cmd[1450], Downloader_Use[15];
char reference[216], head[300], head_show[30];
char location[200],split[7],torrent_loca[200],play_list[15];
int mark,proxymode, download_result2,  shutdown, filecheck,use_list;
FILE* * conf,*save,*power_ini,*space,*dic,*Media_conf;

int CreateFolder() {
	if (!access("Downloads", 0)) {

	}
	else {
		system("mkdir Downloads");
	}
		Media_conf = fopen("Media.conf", "w");
		fprintf(Media_conf, "dir=Downloads\n");
		fprintf(Media_conf, "continue=true\n");
		fprintf(Media_conf, "max-concurrent-downloads=1\n");
		fprintf(Media_conf, "max-connection-per-server=16\n");
		fprintf(Media_conf, "min-split-size=2M\n");
		fprintf(Media_conf, "split=16\n");
		fprintf(Media_conf, "enable-rpc=true\n");
		fprintf(Media_conf, "rpc-allow-origin-all=true\n");
		fprintf(Media_conf, "rpc-listen-all=true\n");
		fprintf(Media_conf, "rpc-listen-port=6800\n\n");
		fclose(Media_conf);
		return 0;
}
int main() {
	download_result2 = 0;
	filecheck = 0;
	anti_shutdown = shutdown = 0;
	system("title FreeDownloader");
	CreateFolder();
p_3:printf("------------------------------------------------\n");
	printf("---------------- FreeDownloader ----------------\n");
	printf("------------------------------------------------\n");
	printf("请选择下载功能：\n1.普通下载模式\n2.百度网盘下载\n3.视频下载模式\n4.高级下载模式\n5.导入下载模式\n6.磁力链下载模式（实验性）\n7.文件完整性测试\n8.Github上的软件帮助\n9.打开下载文件夹\n0.退出\n");
	printf("------------------------------------------------\n");
	printf("请输入：");
	scanf("%d", &downloadmode);
	system("cls");
	proxyswitcher();
	if ((power_ini = fopen("power.ini", "r")) == NULL) {
		if (downloadmode != 8 && downloadmode != 9 && downloadmode != 0) {
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
		ExportDownloader();
	}
	else if (downloadmode == 6) {
		MagnetDownloader();
	}
	else if (downloadmode == 7) {
		IsCheckSum = 1;
		CheckSum(IsCheckSum);
		system("cls");
		goto p_3;
	}
	else if (downloadmode == 8) {
		printf("正在打开帮助界面. . .\n");
		system("explorer.exe \"https://hxhgts.github.io/FreeDownloader/\"");
		system("cls");
		goto p_3;
	}
	else if (downloadmode == 9) {
		system("explorer.exe Downloads");
		system("cls");
		goto p_3;
	}
	else {
		exit(0);
	}
p_2:download_result2 = downloadengine();
	if (download_result2 == 0) {
		printf("\n-----------------------------------------------------\n");
		printf("----------------------下载成功!----------------------\n");
		printf("-----------------------------------------------------\n");
		AutoShutdown(shutdown);
		printf("\n正在打开下载文件夹. . .\n");
		if (downloadmode == 2) {
			printf("百度网盘下载的文件名称可能会有乱码，改为正确名称即可正常使用！\n");
		}
		system("explorer.exe Downloads");
		system("cls");
		goto p_3;
	}
	else {
		printf("\n-----------------------------------------------------\n");
		printf("----------------------下载失败!----------------------\n");
		printf("-----------------------------------------------------\n");
		system("Timeout /T 3");
		if (downloadmode == 1 || downloadmode == 3 || downloadmode == 4 || downloadmode == 5 || downloadmode == 6) {
			system("cls");
			goto p_2;
		}
		else {
			if (mark != 3 && Auto_ChangeUA != 0) {
				sprintf(reference, "%s", "--referer=\"http://pan.baidu.com/disk/home\"");
				mark = 3;
				sprintf(head, "--header=\"User-Agent:%s\"", "netdisk;6.8.9.1;PC;PC-Windows;10.0.18363;WindowsBaiduYunGuanJia");//百度网盘
				sprintf(head_show, "%s", "百度网盘（兼容模式）");
				if (mark != 3)config_thread = 5;
				else config_thread = 3;
				sprintf(split, "1M");
				system("cls");
				goto p_2;
			}
			else {
				if (mark != 3)config_thread = 5;
				else config_thread = 3;
				if (mark != 3)sprintf(split, "1M");
				else sprintf(split, "1M");
				system("cls");
				goto p_2;
			}
		}
	}
}//下载工具主程序

int NormalDownloader() {
	url();
	dir();
	threader();
	BroswerMark();
}

int ExportDownloader() {
	url();
	config_thread = 1;
	sprintf(Downloader_Use, "%s", "aria2c.exe");
	sprintf(head_show, "%s", "用户自定义");
	downloadengine();
	return 0;
}

int MagnetDownloader() {
	int tracker_update;
	tracker_update = 0;
	printf("\n请选择下载模式：\n1.种子文件导入\n2.输入磁力链\n请输入：");
	scanf("%d", &magnet_mode);
	if (magnet_mode == 2) {
		dir();
	}
	threader();
	printf("\n正在本地建立BT配置文件. . .\n");
	if ((conf = fopen("bt.conf", "r")) == NULL) {
		conf = fopen("bt.conf", "w");
		fprintf(conf, "##bt-tracker=server1,server2,server3\n");
		fprintf(conf, "continue=true\n");
		fprintf(conf, "max-concurrent-downloads=1\n");
		fprintf(conf, "max-connection-per-server=16\n");
		fprintf(conf, "min-split-size=2M\n");
		fprintf(conf, "split=16\n");;
		fprintf(conf, "dir=Downloads/\n");
		fclose(conf);
	}
	printf("\n请在弹出窗口中修改BT配置文件,");
	system("notepad bt.conf");
	system("pause");
	url();
	return 0;
}

int url() {
	FILE* url;
	if (downloadmode == 1) {
		if ((url = fopen("normal.download", "r"))==NULL) {
			url = fopen("normal.download", "w");
			fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址. . .\n\n");
		system("notepad.exe normal.download");
		sprintf(config_url, "%s", "-i normal.download");
	}
	else if (downloadmode == 2) {
		if ((url = fopen("netdisk.download", "r")) == NULL) {
			url = fopen("netdisk.download", "w");
			fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址. . .\n\n");
		system("notepad.exe netdisk.download");
		sprintf(config_url, "%s", "-i netdisk.download");
	}
	else if (downloadmode == 3) {
		if (config_media == 1) {
			if ((url = fopen("ytb.download", "r")) == NULL) {
				url = fopen("ytb.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe ytb.download");
			sprintf(config_url, "%s", "-a ytb.download");
		}
		else if (config_media == 2) {
			if ((url = fopen("Bilibili.download", "r")) == NULL) {
				url = fopen("Bilibili.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe Bilibili.download");
			sprintf(config_url, "%s", "-F Bilibili.download");
		}
		else if (config_media == 3) {
			if ((url = fopen("QQVideo.download", "r")) == NULL) {
				url = fopen("QQVideo.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe QQVideo.download");
			sprintf(config_url, "%s", "-F QQVideo.download");
		}
		else if (config_media == 4) {
			if ((url = fopen("iqiyi.download", "r")) == NULL) {
				url = fopen("iqiyi.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe iqiyi.download");
			sprintf(config_url, "%s", "-F iqiyi.download");
		}
		else {
			if ((url = fopen("Pornhub.download", "r")) == NULL) {
				url = fopen("Pornhub.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe Pornhub.download");
			sprintf(config_url, "%s", "-F Pornhub.download");
		}
		}
	else if (downloadmode == 4) {
		if ((url = fopen("advance.download", "r")) == NULL) {
			url = fopen("advance.download", "w");
			fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址. . .\n\n");
		system("notepad.exe advance.download");
		sprintf(config_url, "%s", "-i advance.download");
	}
	else if (downloadmode == 5) {
		if ((url = fopen("Export.bat", "r")) == NULL) {
			url = fopen("Export.bat", "w");
			fprintf(url, "%s", "%Input command below,only support aria2c%\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址. . .\n\n");
		system("notepad.exe Export.bat");
	}
	else if (downloadmode == 6) {
		if (magnet_mode == 2) {
			if ((url = fopen("magnet.download", "r")) == NULL) {
				url = fopen("magnet.download", "w");
				fprintf(url, "%s", "## Input URL below (Don't delete this line)##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址. . .\n\n");
			system("notepad.exe magnet.download");
			sprintf(config_url, "%s", "-i magnet.download");
		}
		else {
			printf("\n请将种子文件以拖拽至本窗口中：");
			scanf("%s", torrent_loca);
			sprintf(config_url, "\"%s\"",torrent_loca);
		}
	}
	return 0;
}//网址输入插件

int threader() {
	int threader_fix;
	if (downloadmode == 1 || downloadmode == 4) {
		printf("\n同时下载任务数：");
		scanf("%d", &Download_Task);//同时下载任务数
			config_thread = 16;
			sprintf(Downloader_Use, "%s", "aria2c.exe");
			if (downloadmode != 2)sprintf(split, "1M");
			else sprintf(split, "1M");
	}
	else if (downloadmode == 2) {
		Download_Task = 1;//同时下载任务数
		sprintf(Downloader_Use, "%s", "aria2c.exe");
		if (mark == 1)config_thread = 5;
		else if (mark == 2)config_thread = 5;
		else if (mark == 3)config_thread = 3;
		if (mark == 1)sprintf(split, "1M");
		else if (mark == 2)sprintf(split, "1M");
		else if (mark == 3)sprintf(split, "1M");
	}
	else if (downloadmode == 3) {
		Download_Task = 1;//同时下载任务数
		if (config_media != 1) {
			sprintf(Downloader_Use, "%s", "annie.exe");
		}
		else {
			sprintf(Downloader_Use, "%s", "youtube-dl.exe");
		}
		config_thread = 16;
	}
	else if (downloadmode == 6) {
		Download_Task = 1;//同时下载任务数
		sprintf(Downloader_Use, "%s", "aria2c.exe");
		config_thread = 16;
		if (magnet_mode == 2)sprintf(split, "1M");
	}

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
		if (downloadmode == 6 && magnet_mode == 1) {

		}
		else sprintf(config_dir, "%s", "--dir=\"Downloads\"");
	}
	return 0;
}

int proxyswitcher() {
	char proxy[50];
	FILE* proxy_ini;
	if ((fopen("proxy.ini", "r")) != NULL) {
		sprintf(config_proxy, "%s", " ");
	}
	else {
		if (downloadmode == 1 || downloadmode == 2 || downloadmode == 3 || downloadmode == 4 || downloadmode == 6) {
			printf("\n是否设置网络代理（是=1，否=0）：");
			scanf("%d", &proxymode);
		}
		if (downloadmode != 3) {
			if (proxymode == 0) {
				sprintf(config_proxy, "%s", " ");
			}
			else {
				printf("\n请输入代理参数，如http://127.0.0.1:1080：");
				scanf("%s", proxy);
				sprintf(config_proxy, "--all-proxy=%s", proxy);
			}
		}
		else {
			if (proxymode == 0) {
				sprintf(config_proxy, "%s", " ");
			}
			else {
				printf("\n请输入代理参数，如http://127.0.0.1:1080：");
				scanf("%s", proxy);
				sprintf(config_proxy, "--proxy %s", proxy);
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
		printf("\n应用id请尝试250528或778750，下载失败请切换UA或应用id！\n");
		printf("\n请选择浏览器标识：\n1.Wii版Opera浏览器（速度较快，不易出错）\n2.爱奇艺（速度最快，有的文件会报错）\n3.百度网盘客户端（兼容模式，速度慢）\n请输入：");
		scanf("%d", &mark);
		if (mark == 1) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Opera/9.30 (Nintendo Wii; U; ; 2071; Wii Shop Channel/1.0; en)");//Wii游戏机版Opera
			sprintf(head_show, "Wii游戏机版Opera（通用模式）");
		}
		else if (mark == 2) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Linux; Android 5.0; SM-N9100 Build/LRX21V) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/37.0.0.0 Mobile Safari/537.36 NetType/WIFI Amoeba/1.0");//爱奇艺
			sprintf(head_show, "%s", "爱奇艺（急速模式）");
		}
		else if (mark == 3) {
			sprintf(head, "--header=\"User-Agent:%s\"", "netdisk;6.8.9.1;PC;PC-Windows;10.0.18363;WindowsBaiduYunGuanJia");//百度网盘
			sprintf(head_show, "%s", "百度网盘（兼容模式）");
		}
		else {
			mark = 3;
			sprintf(head, "--header=\"User-Agent:%s\"", "netdisk;6.8.9.1;PC;PC-Windows;10.0.18363;WindowsBaiduYunGuanJia");//百度网盘
			sprintf(head_show, "%s", "百度网盘（兼容模式）");
		}
		if (mark != 3) {
			printf("\n若下载失败是否尝试切换用户代理（是=1，否=0）：");
			scanf("%d", &Auto_ChangeUA);
		}
		else {
			Auto_ChangeUA = 0;
		}
	}
	else if(downloadmode==1 || downloadmode == 4||downloadmode==6){
		printf("\n请选择浏览器标识：\n1.IE浏览器（增强模式）（普通下载推荐1）\n2.Windows版Opera浏览器\n3.Windows版Chrome（普通下载推荐2）\n4.Edge浏览器\n5.IE浏览器（兼容模式）\n6.自定义UserAgent\n请输入：");
		scanf("%d", &mark);
		if (mark == 1) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko");//IE浏览器（增强模式）
			sprintf(head_show, "IE浏览器（增强模式）");
		}
		else if (mark == 2) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 6.0; Wow64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.117 Safari/537.36 OPR/66.0.3515.36");//Windows版Opera
			sprintf(head_show, "Windows版Opera");
		}
		else if (mark == 3) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.87 Safari/537.36");//Windows版Chrome
			sprintf(head_show, "Windows版Chrome");
		}
		else if (mark == 4) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36 Edge/18.18363");//Edge浏览器
			sprintf(head_show, "Edge浏览器");
		}
		else if (mark == 5) {
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 10.0; WOW64; Trident/7.0; .NET4.0C; .NET4.0E; .NET CLR 2.0.50727; .NET CLR 3.0.30729; .NET CLR 3.5.30729)");//IE浏览器（兼容模式）
			sprintf(head_show, "IE浏览器（兼容模式）");
		}
		else if (mark == 6) {
			printf("\n请输入UserAgent的值：\n");
			scanf("%s", UserAgent_DIY);
			sprintf(head, "--header=\"User-Agent:%s\"", UserAgent_DIY);//自定义
			sprintf(head_show, "自定义");
		}
		else {
			mark = 1;
			sprintf(head, "--header=\"User-Agent:%s\"", "Mozilla/5.0 (Windows NT 6.0; Trident/7.0; rv:11.0) like Gecko");//IE浏览器（增强模式）
			sprintf(head_show, "IE浏览器（增强模式）");
		}
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
	if ((cookie = fopen("Cookies.txt", "r")) == NULL) {
	p_4:cookie = fopen("Cookies.txt", "w");
		fprintf(cookie, "##请在浏览器登录并导出Cookie的值到本文件内\n");
		fprintf(cookie, "##请将EditThisCookie插件设置中将导出格式设置为Netscape HTTP Cookie\n");
		fprintf(cookie, "##Cookies的形式如下：\n");
		fprintf(cookie, "##.jianshu.com	TRUE	/	TRUE	xxxx	_m7e_session_core	xxxxxxxxxxxxxxxxxxx\n");
		fprintf(cookie, "##.jianshu.com	TRUE    /   FALSE	0	    xxxxxxxxxxxxxxxxx	xxxxxxx\n");
		fprintf(cookie, "##导入完成记得保存\n\n");
		fclose(cookie);
		printf("\n请在弹出窗口中导入Cookies信息，");
		system("notepad Cookies.txt");
		system("pause");
		space = fopen("Cookies.txt", "a");
		fprintf(space, "\n\n");
		fclose(space);
	}
	else {
		printf("\n检测到存在Cookies信息，是否继续使用上次的信息登录（是=1 否=0）：");
		scanf("%d", &cookieuse);
		if (cookieuse != 1) {
			goto p_4;
		}
	}
	sprintf(config_cookie, "--load-cookies=\"Cookies.txt\"");
	url();
	dir();
	threader();
	return 0;
}

int Netdisk() {
	int cookieuse;
	FILE* cookie;
	BroswerMark();
	if(mark == 2)sprintf(reference, "%s", "--referer=\"http://pan.baidu.com/wap/home?#/?path=%2F&vmode=list\"");
	else {
		sprintf(reference, "%s", "--referer=\"http://pan.baidu.com/disk/home?#/all?path=%2F&vmode=list\"");
	}
	if ((cookie = fopen("Netdisk_Cookies.txt", "r")) == NULL) {
	p_4:cookie = fopen("tmp.txt", "w");
		fprintf(cookie, "##请在浏览器登录并导出百度网盘主页与pcs.baidu.com的所有Cookies的值到本文件内\n");
		fprintf(cookie, "##请将EditThisCookie插件设置中将导出格式设置为Netscape HTTP Cookie\n");
		fprintf(cookie, "##Cookies的形式如下：\n");
		fprintf(cookie, "##.jianshu.com	TRUE	/	TRUE	xxxx	_m7e_session_core	xxxxxxxxxxxxxxxxxxx\n");
		fprintf(cookie, "##.jianshu.com	TRUE    /   FALSE	0	    xxxxxxxxxxxxxxxxx	xxxxxxx\n");
		fprintf(cookie, "##导入完成记得保存\n\n");
		fclose(cookie);
		printf("\n请在弹出窗口中导入百度网盘Cookies信息，");
		system("notepad tmp.txt");
		system("pause");
		system("type tmp.txt | find \"BDUSS\">Netdisk_Cookies.txt");
		system("type tmp.txt | find \"pcsett\">>Netdisk_Cookies.txt");
		system("del tmp.txt");
		space = fopen("Netdisk_Cookies.txt", "a");
		fprintf(space, "\n\n");
		fclose(space);
	}
	else {
		printf("\n检测到存在Cookies信息，是否继续使用上次的信息登录（是=1 否=0）：");
		scanf("%d", &cookieuse);
		if (cookieuse != 1) {
			goto p_4;
		}
	}
	sprintf(config_cookie, "--load-cookies=\"Netdisk_Cookies.txt\"");
	url();
	dir();
	threader();
	return 0;
}

int CheckSum(int mode) {
	if (mode == 1) {
		printf("\n如果需要检测文件完整性，可以将文件拖拽至本窗口内！\n");
		printf("\n文件名称中不允许空格！若md5不一致，则文件损坏，需重新下载！\n\n文件路径：");
		scanf("%s", location);
		sprintf(cmd, "CertUtil -hashfile \"%s\" MD5", location);
		printf("正在计算MD5值. . .\n");
		system(cmd);
		sprintf(cmd, "CertUtil -hashfile \"%s\" SHA1", location);
		printf("正在计算SHA1值. . .\n");
		system(cmd);
		sprintf(cmd, "CertUtil -hashfile \"%s\" SHA256", location);
		printf("正在计算SHA256值. . .\n");
		system(cmd);
		sprintf(cmd, "CertUtil -hashfile \"%s\" SHA512", location);
		printf("正在计算SHA512值. . .\n");
		system(cmd);
		system("pause");
	}
	return 0;
}

int AutoShutdown(int mode) {
	if (mode == 1) {
		system("shutdown -s -t 60");
		printf("是否阻止系统关机(是=1)：");
		scanf("%d", anti_shutdown);
		if (anti_shutdown == 1) {
			system("shutdown -a");
		}
	}
	return 0;
}

int MediaDownloader() {
	FILE* Bilibili_Cookies,*ytb_Cookies,* QQVideo_Cookies,*iqiyi_Cookies,*Pornhub_Cookies;
	int StartAt,EndAt;
	printf("\n下载视频来源：\n\n1.Youtube\n\n2.B站（兼容番剧下载）\n\n3.腾讯视频\n\n4.爱奇艺\n\n5.Pornhub\n\n请输入：");
	scanf("%d", &config_media);
	if (config_media == 1) {
		printf("\n是否下载整个列表内所有视频（是=1，否=0）：");
		scanf("%d", &use_list);
		if (use_list == 0) {
			sprintf(play_list, "--no-playlist");
		}
		else {
			sprintf(play_list, "--yes-playlist");
		}
	}
	else {
		printf("\n是否下载整个列表内所有视频（是=1，只下载当前视频=0，选择集数=3）：");
		scanf("%d", &use_list);
		if (use_list == 0) {
			sprintf(play_list, "");
		}
		else if (use_list == 1) {
			sprintf(play_list, "-p");
		}
		else {
			printf("\n请按照开始集数-结束集数的格式输入下载范围，如1-5：");
			scanf("%d-%d", &StartAt,&EndAt);
			sprintf(play_list, "-p -items %d-%d",StartAt,EndAt);
		}
	}
	if (config_media == 1) {
		if ((ytb_Cookies = fopen("ytb_Cookies.txt", "r")) == NULL) {
			ytb_Cookies = fopen("ytb_Cookies.txt", "w");
			fprintf(ytb_Cookies, "# Input Cookie below#\n");
			fclose(ytb_Cookies);
		}
		system("notepad ytb_Cookies.txt");
		printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
	}
	else if (config_media == 2) {
		if ((Bilibili_Cookies = fopen("Bilibili_Cookies.txt", "r")) == NULL) {
			Bilibili_Cookies = fopen("Bilibili_Cookies.txt", "w");
			fprintf(Bilibili_Cookies, "# Input Cookie below#\n");
			fclose(Bilibili_Cookies);
		}
		system("notepad Bilibili_Cookies.txt");
		printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
	}
	else if (config_media == 3) {
		if ((QQVideo_Cookies = fopen("QQVideo_Cookies.txt", "r")) == NULL) {
			QQVideo_Cookies = fopen("QQVideo_Cookies.txt", "w");
			fprintf(QQVideo_Cookies, "# Input Cookie below#\n");
			fclose(QQVideo_Cookies);
		}
		system("notepad QQVideo_Cookies.txt");
		printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
	}
	else if (config_media == 4) {
		if ((iqiyi_Cookies = fopen("iqiyi_Cookies.txt", "r")) == NULL) {
			iqiyi_Cookies = fopen("iqiyi_Cookies.txt", "w");
			fprintf(iqiyi_Cookies, "# Input Cookie below#\n");
			fclose(iqiyi_Cookies);
		}
		system("notepad iqiyi_Cookies.txt");
		printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
	}
	else{
		if ((Pornhub_Cookies = fopen("Pornhub_Cookies.txt", "r")) == NULL) {
			Pornhub_Cookies = fopen("Pornhub_Cookies.txt", "w");
			fprintf(Pornhub_Cookies, "# Input Cookie below#\n");
			fclose(Pornhub_Cookies);
		}
		system("notepad Pornhub_Cookies.txt");
		printf("\n请在弹出窗口中导入Cookies，同一账号可以反复使用\n\n");
	}
	url();
	threader();
	dir();
	BroswerMark();
	return 0;
}

int downloadengine() {
	FILE* Bilibili_Download,*ytb_Download,*QQVideo_Download,*iqiyi_Download,*Pornhub_Download;
	int download_result1;
	if (downloadmode == 1) {
		sprintf(cmd, "%s -c -x%d -k%s -j %d %s %s %s %s", Downloader_Use, config_thread, split, Download_Task, config_dir, config_proxy, head, config_url);
	}
	else if (downloadmode == 2) {
		sprintf(cmd, "%s -c -x%d -k%s -j %d %s %s %s %s %s %s", Downloader_Use, config_thread, split, Download_Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
	}
	else if (downloadmode == 3) {
		if (config_media == 1) {
			ytb_Download = fopen("ytb_Download.bat", "w");
			fprintf(ytb_Download, "%s -f bestvideo+bestaudio --write-sub --all-subs --cookies ytb_Cookies.txt %s %s %s --external-downloader aria2c --external-downloader-args \"-x 16 -k 2M\"\n", Downloader_Use, play_list, config_dir,config_url);
			fprintf(ytb_Download, "exit\n");
			fclose(ytb_Download);
		}
		else if (config_media == 2) {
			Bilibili_Download = fopen("Bilibili_Download.bat", "w");
			fprintf(Bilibili_Download, "TIMEOUT /T 3\n");
			fprintf(Bilibili_Download, "%s -c Bilibili_Cookies.txt %s %s -aria2 %s\n", Downloader_Use, play_list, config_dir, config_url);
			fprintf(Bilibili_Download, "exit\n");
			fclose(Bilibili_Download);
			
		}
		else if (config_media == 3) {
			QQVideo_Download = fopen("QQVideo_Download.bat", "w");
			fprintf(QQVideo_Download, "TIMEOUT /T 3\n");
			fprintf(QQVideo_Download, "%s -c QQVideo_Cookies.txt %s %s -aria2 %s\n", Downloader_Use, play_list, config_dir, config_url);
			fprintf(QQVideo_Download, "exit\n");
			fclose(QQVideo_Download);

		}
		else if (config_media == 4) {
			iqiyi_Download = fopen("iqiyi_Download.bat", "w");
			fprintf(iqiyi_Download, "TIMEOUT /T 3\n");
			fprintf(iqiyi_Download, "%s -c iqiyi_Cookies.txt %s %s -aria2 %s\n", Downloader_Use, play_list, config_dir, config_url);
			fprintf(iqiyi_Download, "exit\n");
			fclose(iqiyi_Download);

		}
		else {
			Pornhub_Download = fopen("Pornhub_Download.bat", "w");
			fprintf(Pornhub_Download, "TIMEOUT /T 3\n");
			fprintf(Pornhub_Download, "%s -c Pornhub_Cookies.txt %s %s -aria2 %s\n", Downloader_Use, play_list, config_dir, config_url);
			fprintf(Pornhub_Download, "exit\n");
			fclose(Pornhub_Download);

		}
	}
	else if (downloadmode == 4) {
		sprintf(cmd, "%s -c -x%d -k%s -j %d %s %s %s %s %s %s", Downloader_Use, config_thread,split, Download_Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
	}
	else if (downloadmode == 5) {
		sprintf(cmd, "%s","Export.bat");
	}
	else if (downloadmode == 6) {
		if (magnet_mode == 2) {
			sprintf(cmd, "%s -c -x%d -k%s -j %d %s %s %s --conf-path=bt.conf %s", Downloader_Use, config_thread, split, Download_Task, config_dir, config_proxy, head, config_url);
		}
		else {
			sprintf(cmd, "%s --conf-path=bt.conf %s", Downloader_Use,config_url);
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
			system("ytb_Download.bat");
		}
		else if (config_media == 2) {
			system("start "" /min Bilibili_Download.bat");
		}
		else if (config_media == 3) {
			system("start "" /min QQVideo_Download.bat");
		}
		else if (config_media == 4) {
			system("start "" /min iqiyi_Download.bat");
		}
		else {
			system("start "" /min Pornhub_Download.bat");
		}
		if (config_media != 1) {
			printf("\n正在新建弹出窗口并发送下载任务. . .\n");
			system("start aria2c --conf-path=Media.conf");
			printf("\n由于系统限制，下载进程无法自动停止，若弹出窗口一直显示complete，或需停止下载，");
			system("pause");
			printf("\n\n");
			system("taskkill /f /im aria2c.exe");
			system("del /f /s /q *.bat");
		}
		
		download_result1 = 0;
	}
	else {
		download_result1 = system(cmd);
	}
	if (download_result1 == 0) {
		return 0;
	}
	else {
		return 1;
	}
}
