@echo off
chcp 65001 >nul 2>&1
rem 统一使用 UTF-8 代码页，避免 PowerShell/终端按 UTF-8 解码批处理输出时中文乱码

echo ----------------------------------

echo curl更新工具

echo ----------------------------------

set INSTALLPATH=%CD%

echo 正在关闭curl. . .

taskkill /f /im curl.exe

cls

echo 正在下载curl. . .

aria2-next --connect-timeout=10 -s8 -x8 -k1M --allow-overwrite=true --out=win64-mingw.zip https://curl.se/windows/latest.cgi?p=win64-mingw.zip

echo 正在更新curl. . .

7za x -aoa win64-mingw.zip

cd curl-*

move /y bin\curl.exe %INSTALLPATH%\curl.exe

move /y bin\curl-ca-bundle.crt %INSTALLPATH%\curl-ca-bundle.crt

move /y bin\libcurl-x64.def %INSTALLPATH%\libcurl-x64.def

move /y bin\libcurl-x64.dll %INSTALLPATH%\libcurl-x64.dll

echo 正在清理安装包. . .

cd %INSTALLPATH%

del /s /q win64-mingw.zip

for /d %%i in (curl*) do ( echo Deleting folder: %%i && rd /s /q "%%i")

echo curl已更新!

cls

echo ----------------------------------

echo curl更新工具

echo ----------------------------------

echo 请按任意键继续. . .
pause >nul

exit 0
