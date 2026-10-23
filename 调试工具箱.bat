@echo off
chcp 65001 >nul 2>&1
setlocal EnableExtensions EnableDelayedExpansion

rem ============================================================
rem  FreeDownloader 调试工具箱
rem  1. 更新 FreeDownloader 到最新版
rem  2. 强制结束相关进程
rem  3. 清除数据（保留程序本体）
rem  - 脚本先复制到临时目录执行，避免更新时覆盖自身中断
rem  - 更新时 7za.exe 先复制到临时目录再调用，避免覆盖自身被占用
rem  - 更新完成后删除临时 7za 和 Downloads\FreeDownloader.zip
rem ============================================================

set "ROOT=%~dp0"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"
if /i "%~1"=="--inner" set "ROOT=%~2"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"

if /i not "%~1"=="--inner" (
    set "TMPSCRIPT=%TEMP%\fd-toolbox-%RANDOM%-%RANDOM%.bat"
    copy /y "%~f0" "!TMPSCRIPT!" >nul
    if not exist "!TMPSCRIPT!" (
        echo [错误] 无法创建临时脚本。
        echo 请按任意键退出. . .
        pause >nul
        exit /b 1
    )
    start "" /b cmd /c ""!TMPSCRIPT!" --inner "%ROOT%""
    exit /b 0
)

:menu
cls
echo ----------------------------------
echo             调试工具箱
echo ----------------------------------
echo   1. 更新 FreeDownloader 到最新版
echo   2. 强制结束相关进程
echo   3. 清除数据（保留程序本体）
echo   0. 退出
echo ----------------------------------
set "CHOICE="
set /p "CHOICE=请输入选项: "
if not defined CHOICE exit /b 0
if "%CHOICE%"=="1" goto update
if "%CHOICE%"=="2" goto forcequit
if "%CHOICE%"=="3" goto cleardata
if "%CHOICE%"=="0" exit /b 0
goto menu

:forcequit
cls
echo 正在结束相关进程. . .
taskkill /f /im lux* >nul 2>nul
taskkill /f /im yt-dlp* >nul 2>nul
taskkill /f /im aria2c* /im aria2-next* >nul 2>nul
taskkill /f /im FreeDownloader* >nul 2>nul
echo 完成。
echo.
echo 请按任意键返回菜单. . .
pause >nul
goto menu

:cleardata
cls
echo 正在结束相关进程. . .
taskkill /f /im lux* >nul 2>nul
taskkill /f /im yt-dlp* >nul 2>nul
taskkill /f /im aria2c* /im aria2-next* >nul 2>nul
taskkill /f /im FreeDownloader* >nul 2>nul
echo 正在清除数据. . .
if exist "%ROOT%\temp\*" del /f /s /q "%ROOT%\temp\*" >nul 2>nul
if exist "%ROOT%\cookies\*" del /f /s /q "%ROOT%\cookies\*" >nul 2>nul
if exist "%ROOT%\Downloads" rd /s /q "%ROOT%\Downloads" >nul 2>nul
if exist "%ROOT%\config\proxy.ini" del /f /q "%ROOT%\config\proxy.ini" >nul 2>nul
if exist "%ROOT%\config\rpc.conf" del /f /q "%ROOT%\config\rpc.conf" >nul 2>nul
if exist "%ROOT%\config\uuid" del /f /q "%ROOT%\config\uuid" >nul 2>nul
if exist "%ROOT%\.version" del /f /q "%ROOT%\.version" >nul 2>nul
mkdir "%ROOT%\Downloads" >nul 2>nul
echo 数据已清除。
echo.
echo 请按任意键返回菜单. . .
pause >nul
goto menu

:update
cls
echo ----------------------------------
echo         FreeDownloader 更新
echo ----------------------------------
echo.

if not exist "%ROOT%\curl.exe" (
    echo [错误] 当前目录缺少 curl.exe。
    goto update_failed
)
if not exist "%ROOT%\7za.exe" (
    echo [错误] 当前目录缺少 7za.exe。
    goto update_failed
)

tasklist /fi "imagename eq FreeDownloader.exe" 2>nul | find /i "FreeDownloader.exe" >nul
if not errorlevel 1 (
    echo [错误] FreeDownloader 正在运行，请先关闭或使用菜单 2 结束后再更新。
    goto update_failed
)

set "DLDIR=%ROOT%\Downloads"
set "ZIP=%DLDIR%\FreeDownloader.zip"
set "ZIPURL=https://github.com/HXHGTS/FreeDownloader/releases/latest/download/FreeDownloader.zip"
set "TMPDIR=%TEMP%\fd-update-%RANDOM%-%RANDOM%"
set "TMP7Z=%TMPDIR%\7za.exe"
set "EXTRACT=%TMPDIR%\extract"

if not exist "%DLDIR%" mkdir "%DLDIR%"

echo 正在下载最新版 FreeDownloader.zip ...
echo 保存到: %ZIP%
"%ROOT%\curl.exe" -fL --retry 3 --retry-delay 5 --connect-timeout 30 -o "%ZIP%" "%ZIPURL%"
if errorlevel 1 (
    echo.
    echo [错误] 下载失败，请检查网络连接。
    goto update_failed
)

set "ZIPSIZE="
for %%A in ("%ZIP%") do set "ZIPSIZE=%%~zA"
if not defined ZIPSIZE (
    echo [错误] 下载的文件不存在。
    goto update_failed
)
if !ZIPSIZE! LEQ 0 (
    echo [错误] 下载的文件为空。
    goto update_failed
)

rem 解压前把 7za 复制到临时路径，避免覆盖 7za.exe 自身时被占用
mkdir "%TMPDIR%" >nul 2>nul
copy /y "%ROOT%\7za.exe" "%TMP7Z%" >nul
if not exist "%TMP7Z%" (
    echo [错误] 无法复制 7za.exe 到临时目录。
    goto update_failed
)

echo 正在校验压缩包...
"%TMP7Z%" t "%ZIP%" >nul
if errorlevel 1 (
    echo [错误] 压缩包校验失败，可能下载不完整。
    goto update_failed
)

echo 正在解压...
"%TMP7Z%" x -aoa -o"%EXTRACT%" "%ZIP%" >nul
if errorlevel 1 (
    echo [错误] 解压失败。
    goto update_failed
)

set "SRC=%EXTRACT%\FreeDownloader"
if not exist "%SRC%\" set "SRC=%EXTRACT%"

echo 正在覆盖当前目录...
xcopy "%SRC%\*" "%ROOT%" /E /I /Y /Q /R /H >nul
if errorlevel 1 (
    echo.
    echo [警告] 部分文件未能覆盖，可能有程序正在占用。
    echo 请关闭 FreeDownloader、aria2、yt-dlp、ffmpeg 后重试。
    goto update_failed
)

echo 正在清理旧版脚本...
for %%F in ("7z更新工具.bat" "curl更新工具.bat" "ffmpeg更新工具.bat" "aria2更新工具.bat" "yt-dlp更新工具.bat" "UpdateFreeDownloader.bat" "ForceQuit.bat" "清除数据.bat") do (
    if exist "%ROOT%\%%~F" del /f /q "%ROOT%\%%~F" >nul 2>nul
)
if exist "%ROOT%\.version" del /f /q "%ROOT%\.version" >nul 2>nul

echo.
echo 更新完成。
goto update_cleanup

:update_failed
echo.
echo 更新未完成。

:update_cleanup
rem 清理：临时 7za、解压目录、下载的 FreeDownloader.zip
if exist "%TMP7Z%" del /f /q "%TMP7Z%" >nul 2>nul
if exist "%TMPDIR%" rd /s /q "%TMPDIR%" >nul 2>nul
if exist "%ZIP%" del /f /q "%ZIP%" >nul 2>nul

echo.
echo 请按任意键返回菜单. . .
pause >nul
goto menu