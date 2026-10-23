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

echo 正在获取 aria2-next 最新版本...

set "ARIA2API=https://api.github.com/repos/AnInsomniacy/aria2-next/releases/latest"
set "ARIA2JSON=%TEMP%\aria2-next-release-%RANDOM%.json"

curl.exe -sS --fail --connect-timeout 10 --retry 3 "%ARIA2API%" -o "%ARIA2JSON%"

if errorlevel 1 (
    echo.
    echo [错误] 无法访问 GitHub API。
    echo 请检查网络连接。
    del /q "%ARIA2JSON%" >nul 2>nul
    echo 请按任意键继续. . .
    pause >nul
    exit /b 1
)

set "ARIA2URL="
for /f "tokens=1,* delims=:" %%A in ('findstr /c:"browser_download_url" "%ARIA2JSON%" ^| findstr /c:"windows-x86_64.exe"') do set "ARIA2URL=%%B"

if not defined ARIA2URL (
    echo.
    echo [错误] 未找到 Windows 版 aria2-next 资产。
    del /q "%ARIA2JSON%" >nul 2>nul
    echo 请按任意键继续. . .
    pause >nul
    exit /b 1
)

set "ARIA2URL=%ARIA2URL: =%"
set ARIA2URL=%ARIA2URL:"=%
set ARIA2URL=%ARIA2URL:,=%
del /q "%ARIA2JSON%" >nul 2>nul

echo 正在下载. . .

aria2-next --connect-timeout=10 -s8 -x8 -k1M --allow-overwrite=true --check-certificate=false --out=aria2-next.exe.download "%ARIA2URL%"

echo 正在结束进程. . .

taskkill /f /im aria2c* /im aria2-next*

move /y aria2-next.exe.download aria2-next.exe

cls

echo ----------------------------------

echo aria2-next更新工具

echo ----------------------------------

echo 刷新完成!请按任意键退出. . .

pause > nul