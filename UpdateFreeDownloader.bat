@echo off
chcp 65001 >nul 2>&1
setlocal EnableExtensions EnableDelayedExpansion

rem ============================================================
rem  下载最新版 FreeDownloader.zip，解压后覆盖当前目录
rem  - 脚本先复制到临时目录执行，避免覆盖自身时中断
rem  - 7za.exe 先复制到临时目录再调用，避免覆盖自身时被占用
rem  - 完成后删除临时 7za 和 Downloads\FreeDownloader.zip
rem ============================================================

set "ROOT=%~dp0"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"
if /i "%~1"=="--inner" set "ROOT=%~2"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"

if /i not "%~1"=="--inner" (
    set "TMPSCRIPT=%TEMP%\fd-update-%RANDOM%-%RANDOM%.bat"
    copy /y "%~f0" "!TMPSCRIPT!" >nul
    if not exist "!TMPSCRIPT!" (
        echo [错误] 无法创建临时更新脚本。
        echo 请按任意键退出. . .
        pause >nul
        exit /b 1
    )
    start "" /b cmd /c ""!TMPSCRIPT!" --inner "%ROOT%""
    exit /b 0
)

set "DLDIR=%ROOT%\Downloads"
set "ZIP=%DLDIR%\FreeDownloader.zip"
set "ZIPURL=https://github.com/HXHGTS/FreeDownloader/releases/latest/download/FreeDownloader.zip"
set "TMPDIR=%TEMP%\fd-update-%RANDOM%-%RANDOM%"
set "TMP7Z=%TMPDIR%\7za.exe"
set "EXTRACT=%TMPDIR%\extract"

echo ----------------------------------
echo         FreeDownloader 更新
echo ----------------------------------
echo.

if not exist "%ROOT%\curl.exe" (
    echo [错误] 当前目录缺少 curl.exe。
    goto :fail
)
if not exist "%ROOT%\7za.exe" (
    echo [错误] 当前目录缺少 7za.exe。
    goto :fail
)

tasklist /fi "imagename eq FreeDownloader.exe" 2>nul | find /i "FreeDownloader.exe" >nul
if not errorlevel 1 (
    echo [错误] FreeDownloader 正在运行，请先关闭后再更新。
    goto :fail
)

if not exist "%DLDIR%" mkdir "%DLDIR%"

echo 正在下载最新版 FreeDownloader.zip ...
echo 保存到: %ZIP%
"%ROOT%\curl.exe" -fL --retry 3 --retry-delay 5 --connect-timeout 30 -o "%ZIP%" "%ZIPURL%"
if errorlevel 1 (
    echo.
    echo [错误] 下载失败，请检查网络连接。
    goto :fail
)

set "ZIPSIZE="
for %%A in ("%ZIP%") do set "ZIPSIZE=%%~zA"
if not defined ZIPSIZE (
    echo [错误] 下载的文件不存在。
    goto :fail
)
if !ZIPSIZE! LEQ 0 (
    echo [错误] 下载的文件为空。
    goto :fail
)

rem 解压前把 7za 复制到临时路径，避免覆盖 7za.exe 自身时被占用
mkdir "%TMPDIR%" >nul 2>nul
copy /y "%ROOT%\7za.exe" "%TMP7Z%" >nul
if not exist "%TMP7Z%" (
    echo [错误] 无法复制 7za.exe 到临时目录。
    goto :fail
)

echo 正在校验压缩包...
"%TMP7Z%" t "%ZIP%" >nul
if errorlevel 1 (
    echo [错误] 压缩包校验失败，可能下载不完整。
    goto :fail
)

echo 正在解压...
"%TMP7Z%" x -aoa -o"%EXTRACT%" "%ZIP%" >nul
if errorlevel 1 (
    echo [错误] 解压失败。
    goto :fail
)

set "SRC=%EXTRACT%\FreeDownloader"
if not exist "%SRC%\" set "SRC=%EXTRACT%"

echo 正在覆盖当前目录...
xcopy "%SRC%\*" "%ROOT%" /E /I /Y /Q /R /H >nul
if errorlevel 1 (
    echo.
    echo [警告] 部分文件未能覆盖，可能有程序正在占用。
    echo 请关闭 FreeDownloader、aria2、yt-dlp、ffmpeg 后重试。
    goto :fail
)

echo.
echo 更新完成。
goto :cleanup

:fail
echo.
echo 更新未完成。

:cleanup
rem 清理：临时 7za、解压目录、下载的 FreeDownloader.zip
if exist "%TMP7Z%" del /f /q "%TMP7Z%" >nul 2>nul
if exist "%TMPDIR%" rd /s /q "%TMPDIR%" >nul 2>nul
if exist "%ZIP%" del /f /q "%ZIP%" >nul 2>nul

echo.
echo 请按任意键退出. . .
pause >nul
exit /b 0