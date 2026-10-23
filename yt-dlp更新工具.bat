@echo off
chcp 65001 >nul 2>&1
rem 统一使用 UTF-8 代码页，避免 PowerShell/终端按 UTF-8 解码批处理输出时中文乱码

echo ----------------------------------

echo yt-dlp更新工具

echo ----------------------------------
echo.
echo 提示:当前为直连模式,无需代理软件,请确保网络可访问 GitHub。
echo.
echo 请按任意键继续. . .
pause >nul

cls

echo 正在结束进程. . .

taskkill /f /im yt-dlp*

echo 正在建立data文件夹. . .

echo 正在下载. . .

aria2-next --connect-timeout=10 -s8 -x8 -k1M --allow-overwrite=true --check-certificate=false --out=yt-dlp.exe https://github.com/yt-dlp/yt-dlp-nightly-builds/releases/latest/download/yt-dlp.exe

@REM aria2-next --connect-timeout=10 -s8 -x8 -k1M --allow-overwrite=true --check-certificate=false --out=yt-dlp.exe https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp.exe

cls

echo ----------------------------------

echo yt-dlp更新工具

echo ----------------------------------

echo 刷新完成!请按任意键退出. . .

pause > nul