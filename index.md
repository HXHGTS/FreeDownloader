## FreeDownloader


<h1 align="center">
<img src="https://github.com/HXHGTS/FreeDownloader/raw/master/icon.ico">
</h1>


### 一、安装方法

1.下载压缩包[Windows64位](https://github.com/HXHGTS/FreeDownloader/releases/latest)并解压

2.在chrome浏览器[Chrome国内下载地址](https://www.google.cn/chrome/)中安装EditThisCookie插件与油猴脚本(国内网盘集合包安装方法：[下载插件](https://www.lanzous.com/i9mqftg)并解压，打开chrome浏览器更多工具-扩展程序，打开开发者模式后加载已解压的扩展程序）（谷歌商店安装方法：[Chrome扩展商店EditThisCookie](https://chrome.google.com/webstore/detail/editthiscookie/fngmhnnpilhplaeedifhccceomclgfbg?hl=zh-CN) [Chrome扩展商店油猴插件](https://chrome.google.com/webstore/detail/tampermonkey/dhdgffkkebhmkfjojejmpbldmpobfkfo)）（ps:火狐与Chromium核心的大部分浏览器也可以安装，但需要自己百度下载插件)

3.[安装网盘助手](https://greasyfork.org/scripts/378301-%E7%BD%91%E7%9B%98%E5%8A%A9%E6%89%8B/code/%E7%BD%91%E7%9B%98%E5%8A%A9%E6%89%8B.user.js)(务必先安装油猴脚本再安装这个！！！)

4.鼠标右击EditThisCookie插件（饼干🍪图标），单击“选项”-“选择Cookies的导出格式”，将格式改为“Netscape HTTP Cookie File”

### 二、百度网盘功能使用方法

1.打开FreeDownloader，不用代理，按提示操作，当程序弹窗要求导入Cookies信息时，开始第2步操作（夜间挂机下载可以将自动关机功能打开）（程序默认屏蔽自动关机功能与代理服务器功能，如果需要，请在进入软件前删除config\power.ini(屏蔽自动关机)与config\proxy.ini(屏蔽代理服务器)文件）

2.在chrome浏览器中登录[百度网盘](https://pan.baidu.com/disk/home?#/all?path=%2F&vmode=list)，点击右上角饼干图标（EditThisCookie插件），点击“导出Cookies”，复制粘贴填入FreeDownloader弹出的文本文件中(只用导入1次，之后如果不再切换百度账号或修改密码可直接使用第一次导入的数据)，或者直接点击网址输入框旁边的锁🔒，找到"Cookie"，打开，找到BDUSS的值，粘贴到软件输入框中亦可(不推荐，这种方法每次需要重新导入)

3.选择你要下载的一个或多个文件，单击生成链接，替换应用id为778750（必须！！！），将生成的链接复制粘贴到FreeDownloader弹出的文本输入框内，多个链接间回车隔开，保存

### 三、视频下载功能使用方法

1.打开FreeDownloader，按提示操作

2.复制视频所在网址，可以下载油管、哔哩哔哩、腾讯视频、爱奇艺、优酷视频

3.下载普通视频只需登录网站并导出自己的Cookies到FreeDownloader，如果需要下载会员专享高清或独播视频，则需要导入会员账号Cookies信息，可在浏览器中先用会员账号登录，再仿照百度网盘的Cookies导出方式导出当前页面Cookies信息


### 四、关于BT种子与磁力链下载

1.本软件支持种子与磁力链下载功能，并在每次启用BT下载时自动更新tracker服务器列表帮助加速下载（感谢[trackerslist.com](https://trackerslist.com/#/zh)公开tracker列表），若因网络问题更新失败需要自行导入

2.如果需要加速BT下载，需要使用全局加速工具，普通http与socks5代理对BT下载加速无效！推荐使用Clash的Tap模式或SStap或Netch等

### 五、关于文件校验

由于网络波动因素的存在，有时候下载下来的文件（尤其是百度网盘）可能会存在文件错误，此功能通过计算文件校验和（包括MD5、SHA1、SHA256、SHA512）帮助用户判断文件完整性，可结合网页提供的信息判断，具体可百度

### 六、第三方软件

本软件使用了[aria2](https://aria2.github.io/)、[youtube-dl](https://github.com/ytdl-org/youtube-dl)、[annie](https://github.com/iawia002/annie)、[ffmpeg](https://ffmpeg.org/)等开源工具作为下载或解码工具，在此向以上开源软件开发者致谢！

### 七、其它事项

由于更新软件工程量巨大，所以源代码的更新速度远远先于release文件，如需使用最新版本可以自己下载[软件源码](https://github.com/HXHGTS/FreeDownloader/archive/master.zip)文件，使用vistual studio2019编译！


