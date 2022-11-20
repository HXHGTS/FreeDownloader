## FreeDownloader

一款个人使用的轻量下载工具

### 一、安装方法

1.下载压缩包并解压

下载地址: [Windows64位(Github)](https://github.com/HXHGTS/FreeDownloader/releases/latest/download/FreeDownloader.X64.zip)

2.在chrome浏览器[Chrome下载](https://chrome.google.com)中安装[EditThisCookie](https://chrome.google.com/webstore/detail/editthiscookie/fngmhnnpilhplaeedifhccceomclgfbg?hl=zh-CN),[网盘助手](https://chrome.google.com/webstore/detail/%E7%BD%91%E7%9B%98%E5%8A%A9%E6%89%8B/cfaglmggimoleoclaghmmboppofffdda) (注意:需要科学上网)

4.鼠标右击EditThisCookie插件(饼干🍪图标),单击"选项"-"选择Cookies的导出格式",将格式改为"Netscape HTTP Cookie File"

### 二、百度网盘功能使用方法

1.打开FreeDownloader,不用代理,按提示操作,当程序弹窗要求导入Cookies信息时,开始第2步操作

2.在chrome浏览器中登录[百度网盘](https://pan.baidu.com/disk/home?#/all?path=%2F&vmode=list),点击右上角饼干图标(EditThisCookie插件),点击"导出Cookies",复制粘贴填入FreeDownloader弹出的文本文件中(只用导入1次,之后如果不再切换百度账号或修改密码可直接使用第一次导入的数据)

2.5.或者直接点击网址输入框旁边的锁🔒,找到"Cookie",打开,找到BDUSS的值,粘贴到软件输入框中;然后用浏览器访问[pcs.baidu.com](https://pcs.baidu.com),找到pcsett的值,粘贴到软件输入框中(只用导入1次,之后如果不再切换百度账号或修改密码可直接使用第一次导入的数据)

3.选择你要下载的一个或多个文件,单击生成链接,将生成的链接复制粘贴到FreeDownloader弹出的文本输入框内,多个链接间回车隔开,保存

4.由于百度网盘增强了限速机制，建议使用VIP或SVIP账号尝试

### 三、视频下载功能使用方法

1.打开FreeDownloader,按提示操作

2.下载普通视频只需登录网站并导出自己的Cookies到FreeDownloader,如果需要下载会员专享高清或独播视频,则需要导入会员账号Cookies信息,可在浏览器中先用会员账号登录,再仿照百度网盘的Cookies导出方式导出当前页面Cookies信息

3.复制视频所在网址,可以下载油管、哔哩哔哩、腾讯视频、爱奇艺、优酷视频，未来可能增加更多

### 四、关于BT种子与磁力链下载

1.本软件支持种子与磁力链下载功能,并在每次启用BT下载时自动更新tracker服务器列表帮助加速下载(基于[trackerslist.com](https://trackerslist.com/#/zh?id=aria2)公开tracker列表),若因网络问题更新失败需要自行导入

2.如果需要加速BT下载,需要使用Tap/Tun模式接管全局流量,普通http(s)与socks5代理对BT下载加速无效!
推荐使用:
(1)Clash的Tap/Tun模式
(2)Surge网关模式
(3)采用路由器解决方案

### 五、挂机下载(下载完成自动关机)

1.夜间挂机下载可以将自动关机功能打开(程序默认屏蔽自动关机功能,如果需要,请在进入软件后在安装目录新建config\power.ini文件)

⚠注意：一定在进入软件后!!!

2.在自动关机被执行一次后，该文件会被自动删除，需要请重新创建

### 六、关于文件校验

由于网络波动因素的存在,有时候下载下来的文件(尤其是百度网盘)可能会存在文件错误,此功能通过计算文件校验和(包括MD5、SHA1、SHA256、SHA512)帮助用户判断文件完整性,可结合网页提供的信息判断

### 七、第三方软件

本软件采用[aria2](https://aria2.github.io/)、[yt-dlp](https://github.com/yt-dlp/yt-dlp)、[lux](https://github.com/iawia002/lux)、[ffmpeg](https://ffmpeg.org/)等开源工具作为下载或合成工具,在此向以上开源软件开发者致谢!

### 八、其它事项

由于更新软件工程量巨大,所以源代码的更新速度远远先于release文件,如需使用最新版本可以自己下载[软件源码](https://github.com/HXHGTS/FreeDownloader/archive/master.zip)文件并解压缩,使用vistual studio2022打开.sln文件编译!


