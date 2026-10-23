@echo off
chcp 65001 >nul 2>&1
rem 统一使用 UTF-8 代码页，避免 PowerShell/终端按 UTF-8 解码批处理输出时中文乱码

echo ----------------------------------

echo aria2-next更新工具

echo ----------------------------------
echo.
echo 提示:当前为直连模式,无需代理软件,请确保网络可访问 GitHub。
echo.
echo 请按任意键继续. . .
pause >nul

cls

echo 正在下载. . .

aria2-next --connect-timeout=10 -s8 -x8 -k1M --allow-overwrite=true --check-certificate=false --out=aria2-next.exe.download https://github.com/HXHGTS/Update-MainLine/releases/latest/download/aria2-next-windows-x86_64.exe

echo 正在结束进程. . .

taskkill /f /im aria2c* /im aria2-next*

move /y aria2-next.exe.download aria2-next.exe

cls

echo ----------------------------------

echo aria2-next更新工具

echo ----------------------------------

echo 刷新完成!请按任意键退出. . .

pause > nul