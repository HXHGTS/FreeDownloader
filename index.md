## 标准下载器官网

### 一、安装方法

1.下载压缩包[Windows64位](https://lanzous.com/iczgw6b)  [Windows32位]()并解压

2.在chrome浏览器[国内离线安装包](https://www.lanzous.com/i9r1beh)  [Chrome扩展商店](https://chrome.google.com/webstore/detail/editthiscookie/fngmhnnpilhplaeedifhccceomclgfbg?hl=zh-CN)中安装EditThisCookie插件与油猴脚本(本地安装方法：[下载插件](https://www.lanzous.com/i9mqftg)并解压，打开chrome浏览器更多工具-扩展程序，打开开发者模式后加载已解压的扩展程序）（ps:火狐与Opera浏览器也可以安装，但需要自己百度下载插件)

3.[安装网盘助手](https://greasyfork.org/scripts/378301-%E7%BD%91%E7%9B%98%E5%8A%A9%E6%89%8B/code/%E7%BD%91%E7%9B%98%E5%8A%A9%E6%89%8B.user.js)(务必先安装油猴脚本再安装这个！！！)

4.鼠标右击EditThisCookie插件（饼干🍪图标），单击“选项”-“选择Cookies的导出格式”，将格式改为“Netscape HTTP Cookie File”

### 二、百度网盘功能使用方法
1.打开标准下载器，不用代理，按提示操作，当程序弹窗要求导入Cookies信息时，开始第2步操作（夜间挂机下载可以将自动关机功能打开）（程序默认屏蔽自动关机功能与代理服务器功能，如果需要，请在进入软件前删除power.ini与proxy.ini文件）

2.在chrome浏览器中登录百度网盘，点击右上角饼干图标（EditThisCookie插件），点击“导出Cookies”，复制粘贴填入标准下载器弹出的文本文件中

3.在插件的输入框中输入pcs.baidu.com，再点击“导出Cookies”，也复制粘贴填入标准下载器弹出的文本文件中(2、3步中数据只用导入1次，之后如果不再切换百度账号或修改密码可直接使用第一次导入的数据)

4.选择你要下载的一个或多个文件，单击生成链接，将生成的链接复制粘贴到标准下载器弹出的文本输入框内，多个链接间回车隔开，如果设置的应用id与软件的模式不匹配，需要Ctrl+H自己替换（推荐应用ID：250528或778750）

5.关于用户代理（UserAgent）：不同的用户代理可以模拟不同软件的下载行为，在不稳定的情况下尽量选择百度网盘客户端的用户代理，必要时可以自行导入

### 三、视频下载功能使用方法（实验性功能）
1.复制视频所在网址，可以下载油管和B站（由于B站部分番剧加密，暂时无法下载），部分视频网站选择B站模式貌似也可以下载，具体可以自己试试

2.如果需要下载会员专享高清或独播视频，需要导入会员账号Cookies信息，可在浏览器中先用会员账号登录，再导出当前页面所有Cookies信息

注意：以后仅更新程序关键文件，下载后直接覆盖即可！

### 四、导入下载功能
支持外部插件导入生成的aria2下载命令

### 五、关于BT种子与磁力链下载（实验性功能）
本软件支持种子与磁力链下载功能，并内置tracker服务器列表帮助加速下载，必要时可以自行导入

### 六、关于文件校验
由于网络波动因素的存在，有时候下载下来的文件（尤其是百度网盘）可能会存在文件错误，此功能通过计算文件校验和（MD5与SHAXXX等）帮助用户判断文件完整性

### 七、第三方软件
本软件使用了[aria2官方版（稳定性更好，但限制16线程）](https://aria2.github.io/)、[youtube-dl](https://github.com/ytdl-org/youtube-dl)、[ffmpeg](https://ffmpeg.org/)等开源工具作为下载或解码工具
