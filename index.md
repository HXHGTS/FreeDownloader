## FreeDownloader


![Image](https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1591516856861&di=cf4424b94f910a9a13be0e12bf77a018&imgtype=0&src=http%3A%2F%2Fhbimg.b0.upaiyun.com%2F115c59c5f292336d52e40f21e9ade30ab39ed15c72ac-tEMn6b_fw658)


### 一、安装方法

1.下载压缩包[Windows64位](https://lanzous.com/id1lplg)  [Windows32位](https://lanzous.com/id1m2mf)并解压

2.在chrome浏览器[Chrome国内离线安装包](https://www.lanzous.com/i9r1beh)  [Chrome扩展商店EditThisCookie](https://chrome.google.com/webstore/detail/editthiscookie/fngmhnnpilhplaeedifhccceomclgfbg?hl=zh-CN)中安装EditThisCookie插件与油猴脚本(本地安装方法：[下载插件](https://www.lanzous.com/i9mqftg)并解压，打开chrome浏览器更多工具-扩展程序，打开开发者模式后加载已解压的扩展程序）（ps:火狐与Opera浏览器也可以安装，但需要自己百度下载插件)

3.[安装网盘助手](https://greasyfork.org/scripts/378301-%E7%BD%91%E7%9B%98%E5%8A%A9%E6%89%8B/code/%E7%BD%91%E7%9B%98%E5%8A%A9%E6%89%8B.user.js)(务必先安装油猴脚本再安装这个！！！)

4.鼠标右击EditThisCookie插件（饼干🍪图标），单击“选项”-“选择Cookies的导出格式”，将格式改为“Netscape HTTP Cookie File”

### 二、百度网盘功能使用方法

1.打开FreeDownloader，不用代理，按提示操作，当程序弹窗要求导入Cookies信息时，开始第2步操作（夜间挂机下载可以将自动关机功能打开）（程序默认屏蔽自动关机功能与代理服务器功能，如果需要，请在进入软件前删除power.ini与proxy.ini文件）

2.在chrome浏览器中登录百度网盘，点击右上角饼干图标（EditThisCookie插件），点击“导出Cookies”，复制粘贴填入FreeDownloader弹出的文本文件中

3.在插件的输入框中输入pcs.baidu.com，再点击“导出Cookies”，也复制粘贴填入FreeDownloader弹出的文本文件中(2、3步中数据只用导入1次，之后如果不再切换百度账号或修改密码可直接使用第一次导入的数据)

4.选择你要下载的一个或多个文件，单击生成链接，将生成的链接复制粘贴到FreeDownloader弹出的文本输入框内，多个链接间回车隔开，如果设置的应用id与软件的模式不匹配，需要Ctrl+H自己替换

应用ID：
778750

5.关于用户代理
在插件的输入框（UserAgent）：不同的用户代理可以模拟不同软件的下载行为，在不稳定的情况下尽量选择百度网盘客户端的用户代理

### 三、视频下载功能使用方法（实验性功能）

1.复制视频所在网址，可以下载油管、哔哩哔哩（支持哔哩哔哩加密番剧下载）、腾讯视频、爱奇艺、优酷视频

2.下载普通视频只需登录网站并导出自己的Cookies到FreeDownloader，如果需要下载会员专享高清或独播视频，则需要导入会员账号Cookies信息，可在浏览器中先用会员账号登录，再仿照百度网盘的Cookies导出方式导出当前页面Cookies信息

### 四、导入下载功能

支持外部插件导入生成的aria2、annie、youtube-dl下载命令，以aria2c xxx... 、annie xxx... 、youtube-dl xxx...格式构成的命令

### 五、关于BT种子与磁力链下载（实验性功能）

本软件支持种子与磁力链下载功能，并内置tracker服务器列表帮助加速下载（感谢[trackerslist.com](https://trackerslist.com/#/zh)公开tracker列表），必要时可以自行导入

### 六、关于文件校验

由于网络波动因素的存在，有时候下载下来的文件（尤其是百度网盘）可能会存在文件错误，此功能通过计算文件校验和（包括MD5、SHA1、SHA256、SHA512）帮助用户判断文件完整性，可结合网页提供的信息判断，具体可百度

### 七、第三方软件

本软件使用了[aria2](https://aria2.github.io/)、[youtube-dl](https://github.com/ytdl-org/youtube-dl)、[annie](https://github.com/iawia002/annie)、[ffmpeg](https://ffmpeg.org/)等开源工具作为下载或解码工具，在此向以上开源软件开发者致谢！

### 八、其它事项

由于更新软件工程量巨大，所以源代码的更新速度远远先于release文件，如需使用最新版本可以自己下载[master.zip](https://github.com/HXHGTS/FreeDownloader/archive/master.zip)文件，使用vistual studio编译！


