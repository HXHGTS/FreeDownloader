#include<stdio.h>
#include<stdlib.h>
int downloadmode, magnet_mode,config_thread, config_media, anti_shutdown, Download_Task, IsCheckSum,Auto_ChangeUA;
char config_proxy[65], config_url[200], config_dir[35], config_cookie[40], cmd[1450], Downloader_Use[15];
char reference[216], head[300], head_show[30];
char location[200],split[7],torrent_loca[200],play_list[15];
int mark,proxymode, download_result2,  shutdown, filecheck,use_list;
FILE* log_gen,* conf,*save,*power_ini;


int main() {
	download_result2 = 0;
	filecheck = 0;
	anti_shutdown = shutdown = 0;
	system("title 标准下载器");
p_3:printf("------------------------------------------------\n");
	printf("-------------------标准下载器-------------------\n");
	printf("------------------------------------------------\n");
	printf("请选择下载功能：\n1.普通下载模式\n2.百度网盘下载\n3.视频下载模式\n4.高级下载模式（自定义下载）\n5.导入下载模式\n6.磁力链下载模式（实验性）\n7.继续上一次的下载\n8.文件完整性测试\n9.Github上的软件帮助\n0.打开下载文件夹\n");
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
		if ((save = fopen("startdownload.bat", "r")) != NULL) {
			printf("正在进行上一次的下载. . .\n");
			system("startdownload.bat");
			system("pause");
			system("cls");
			goto p_3;
		}
		{
			printf("找不到上一次的下载. . .\n");
			system("pause");
			system("cls");
			goto p_3;
		}
	}
	else if (downloadmode == 8) {
		IsCheckSum = 1;
		CheckSum(IsCheckSum);
		system("cls");
		goto p_3;
	}
	else if (downloadmode == 9) {
		printf("正在打开帮助界面. . .\n");
		system("explorer.exe \"https://hxhgts.github.io/FreeDownloader/\"");
		system("cls");
		goto p_3;
	}
	else if (downloadmode == 0) {
		system("explorer.exe Downloads");
		system("cls");
		goto p_3;
	}
	else {
		system("cls");
		goto p_3;
	}
p_2:download_result2 = downloadengine();
	if (download_result2 == 0) {
		printf("-----------------------------------------------------\n");
		printf("----------------------下载成功!----------------------\n");
		printf("-----------------------------------------------------\n");
		AutoShutdown(shutdown);
		printf("\n正在打开下载文件夹. . .\n");
		if (downloadmode == 2) {
			printf("百度网盘下载的文件名称可能会有乱码，改为正确名称即可正常使用！\n");
		}
		system("explorer.exe Downloads");
		system("pause");
		system("cls");
		goto p_3;
	}
	else {
		printf("-----------------------------------------------------\n");
		printf("----------------------下载失败!----------------------\n");
		printf("-----------------------------------------------------\n");
		system("Timeout /T 3");
		if (downloadmode == 1 || downloadmode == 3 || downloadmode == 4 || downloadmode == 5 || downloadmode == 6) {
			log_gen = fopen("log.txt", "a");
			fprintf(log_gen, "由于下载失败，软件默认自动重试\n");
			fclose(log_gen);
			system("cls");
			goto p_2;
		}
		else {
			if (mark != 3 && Auto_ChangeUA != 0) {
				sprintf(reference, "%s", "--referer=\"http://pan.baidu.com/disk/home\"");
				log_gen = fopen("log.txt", "a");
				fprintf(log_gen, "由于下载失败，软件默认采用了更换UserAgent的操作，并自动重试\n");
				fclose(log_gen);
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
				log_gen = fopen("log.txt", "a");
				fprintf(log_gen, "由于下载失败，软件默认自动重试\n");
				fclose(log_gen);
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
	if ((conf = fopen("bt.conf", "r")) != NULL) {
		fclose(conf);
		system("del bt.conf");
	}
	printf("\n正在更新traker服务器列表. . .\n");
	p_5:tracker_update=system("aria2c -o bt.conf \"https://github.com/HXHGTS/FreeDownloader/raw/master/bt.conf\"");
	if (tracker_update != 0) {
		printf("\n列表更新失败，正在重试. . .\n");
		goto p_5;
	}
	printf("\ntraker服务器列表更新成功！\n");
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
			fprintf(url, "%s", "\n##请在本行文字删除，并将下载地址粘贴在这里##\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址，输入完成后");
		system("notepad.exe normal.download");
		system("pause");
		sprintf(config_url, "%s", "-i normal.download");
	}
	else if (downloadmode == 2) {
		if ((url = fopen("netdisk.download", "r")) == NULL) {
			url = fopen("netdisk.download", "w");
			fprintf(url, "%s", "\n##请在本行文字删除，并将下载地址粘贴在这里##\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址，输入完成后");
		system("notepad.exe netdisk.download");
		system("pause");
		sprintf(config_url, "%s", "-i netdisk.download");
	}
	else if (downloadmode == 3) {
		if ((url = fopen("media.download", "r")) == NULL) {
			url = fopen("media.download", "w");
			fprintf(url, "%s", "\n##请在本行文字删除，并将下载地址粘贴在这里##\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址，输入完成后");
		system("notepad.exe media.download");
		system("pause");
		sprintf(config_url, "%s", "-a media.download");
	}
	else if (downloadmode == 4) {
		if ((url = fopen("advance.download", "r")) == NULL) {
			url = fopen("advance.download", "w");
			fprintf(url, "%s", "\n##请在本行文字删除，并将下载地址粘贴在这里##\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址，输入完成后");
		system("notepad.exe advance.download");
		system("pause");
		sprintf(config_url, "%s", "-i advance.download");
	}
	else if (downloadmode == 5) {
		if ((url = fopen("startdownload.bat", "r")) == NULL) {
			url = fopen("startdownload.bat", "w");
			fprintf(url, "%s", "\n%请在本行文字删除，并将下载地址粘贴在这里%\n");
			fclose(url);
		}
		printf("\n请在弹出页输入下载地址，输入完成后");
		system("notepad.exe startdownload.bat");
		system("pause");
	}
	else if (downloadmode == 6) {
		if (magnet_mode == 2) {
			if ((url = fopen("magnet.download", "r")) == NULL) {
				url = fopen("magnet.download", "w");
				fprintf(url, "%s", "\n##请在本行文字删除，并将磁力链粘贴在这里##\n");
				fclose(url);
			}
			printf("\n请在弹出页输入下载地址，输入完成后");
			system("notepad.exe magnet.download");
			system("pause");
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
		sprintf(Downloader_Use, "%s", "youtube-dl.exe");
		config_thread = 1;
	}
	else if (downloadmode == 6) {
		Download_Task = 1;//同时下载任务数
		sprintf(Downloader_Use, "%s", "aria2c.exe");
		config_thread = 128;
		if (magnet_mode == 2)sprintf(split, "1M");
	}

}//线程数修改与引擎选择        

int dir() {
	if (downloadmode == 3) {
		sprintf(config_dir, "%s", "-o \"/Downloads/\%(title)s.\%(ext)s\"");
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
	FILE* Media_Cookies,*ytb_Cookies;
	printf("\n下载视频来源(1.Youtube 2.B站（暂不支持番剧下载）)：");
	scanf("%d", &config_media);
	printf("\n是否下载整个列表内所有视频（是=1，否=0）：");
	scanf("%d", &use_list);
	if (use_list == 0) {
		sprintf(play_list, "--no-playlist");
	}
	else {
		sprintf(play_list, "");
	}
	if (config_media == 1) {
		if ((ytb_Cookies = fopen("ytb_Cookies.txt", "r")) == NULL) {
			ytb_Cookies = fopen("ytb_Cookies.txt", "w");
			fprintf(ytb_Cookies, "##请将当前视频页面的Cookie导出粘贴到下面##\n\n");
			fclose(ytb_Cookies);
		}
		printf("\n请在弹出窗口中导入视频站的Cookies信息以便获取高清视频，同一账号可以反复使用，保存完成后");
		system("notepad ytb_Cookies.txt");
		system("pause");
		url();
		threader();
	}
	else {
		if ((Media_Cookies = fopen("Media_Cookies.txt", "r")) == NULL) {
			Media_Cookies = fopen("Media_Cookies.txt", "w");
			fprintf(Media_Cookies, "##请将当前视频页面的Cookie导出粘贴到下面##\n\n");
			fclose(Media_Cookies);
		}
		printf("\n请在弹出窗口中导入视频站的Cookies信息以便获取高清视频，同一账号可以反复使用，保存完成后");
		system("notepad Media_Cookies.txt");
		system("pause");
		url();
		threader();
	}
	dir();
	BroswerMark();
	return 0;
}

int downloadengine() {
	int download_result1;
	if (downloadmode == 1) {
		sprintf(cmd, "%s -c -x%d -k%s -j %d %s %s %s %s", Downloader_Use, config_thread, split, Download_Task, config_dir, config_proxy, head, config_url);
	}
	else if (downloadmode == 2) {
		sprintf(cmd, "%s -c -x%d -k%s -j %d %s %s %s %s %s %s", Downloader_Use, config_thread, split, Download_Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
	}
	else if (downloadmode == 3) {
		if (config_media == 1) {
			sprintf(cmd, "%s %s -c --cookies ytb_Cookies.txt -f bestvideo+bestaudio %s --write-sub --all-subs %s %s %s", Downloader_Use, head,play_list, config_proxy, config_dir, config_url);
		}
		else {
			sprintf(cmd, "%s %s -c %s %s --cookies Media_Cookies.txt %s %s", Downloader_Use, head,play_list, config_proxy, config_dir, config_url);
		}
	}
	else if (downloadmode == 4) {
		sprintf(cmd, "%s -c -x%d -k%s -j %d %s %s %s %s %s %s", Downloader_Use, config_thread,split, Download_Task, config_dir, config_proxy, reference, head, config_cookie, config_url);
	}
	else if (downloadmode == 5) {
		sprintf(cmd, "%s","startdownload.bat");
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
	log_gen = fopen("log.txt","a");
	fprintf(log_gen,"用户执行了如下命令：\n%s\n", cmd);
	fclose(log_gen);
	if (downloadmode != 5) {
		save = fopen("startdownload.bat", "w");
		fprintf(save, "%s", cmd);
		fclose(save);
	}
	download_result1 = system(cmd);
	if (download_result1 == 0) {
		log_gen = fopen("log.txt", "a");
		fprintf(log_gen, "命令执行：成功\n\n");
		fclose(log_gen);
		return 0;
	}
	else {
		log_gen = fopen("log.txt", "a");
		fprintf(log_gen, "命令执行：失败\n");
		fclose(log_gen);
		return 1;
	}
}
