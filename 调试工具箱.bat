@echo off
chcp 65001 >nul 2>&1
setlocal EnableExtensions EnableDelayedExpansion

rem ============================================================
rem  FreeDownloader 调试工具箱
rem  1. 强制结束相关进程
rem  2. 清除数据（保留程序本体）
rem  - 本地升级功能已移除（不可用），升级请手动下载 Release 压缩包覆盖解压
rem  - 脚本先复制到临时目录执行: cmd 执行批处理时是边读边跑, 覆盖解压时
rem    直接替换正在运行的 .bat 会让脚本中途失效, 跑副本更稳
rem  - 退出时删除本次运行的脚本副本
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
echo   1. 强制结束相关进程
echo   2. 清除数据（保留程序本体）
echo   0. 退出
echo ----------------------------------
set "CHOICE="
set /p "CHOICE=请输入选项: "
if not defined CHOICE goto done
if "%CHOICE%"=="1" goto forcequit
if "%CHOICE%"=="2" goto cleardata
if "%CHOICE%"=="0" goto done
goto menu

:killprocs
taskkill /f /im lux* >nul 2>nul
taskkill /f /im yt-dlp* >nul 2>nul
taskkill /f /im aria2c* /im aria2-next* >nul 2>nul
taskkill /f /im ffmpeg* /im ffprobe* >nul 2>nul
taskkill /f /im deno* /im qjs* /im invidious_companion* >nul 2>nul
taskkill /f /im FreeDownloader* >nul 2>nul
exit /b 0

:forcequit
cls
echo 正在结束相关进程. . .
call :killprocs
echo 完成。
echo.
echo 请按任意键返回菜单. . .
pause >nul
goto menu

:cleardata
cls
echo 正在结束相关进程. . .
call :killprocs
echo 正在清除数据. . .
if exist "%ROOT%\temp\*" del /f /s /q "%ROOT%\temp\*" >nul 2>nul
if exist "%ROOT%\cookies\*" del /f /s /q "%ROOT%\cookies\*" >nul 2>nul
if exist "%ROOT%\Downloads" rd /s /q "%ROOT%\Downloads" >nul 2>nul
if exist "%ROOT%\config\proxy.ini" del /f /q "%ROOT%\config\proxy.ini" >nul 2>nul
if exist "%ROOT%\config\rpc.conf" del /f /q "%ROOT%\config\rpc.conf" >nul 2>nul
if exist "%ROOT%\config\uuid" del /f /q "%ROOT%\config\uuid" >nul 2>nul
if exist "%ROOT%\config\dht.dat__temp" del /f /q "%ROOT%\config\dht.dat__temp" >nul 2>nul
if exist "%ROOT%\config\dht6.dat__temp" del /f /q "%ROOT%\config\dht6.dat__temp" >nul 2>nul
if exist "%ROOT%\.version" del /f /q "%ROOT%\.version" >nul 2>nul
mkdir "%ROOT%\Downloads" >nul 2>nul
echo 数据已清除。
echo.
echo 请按任意键返回菜单. . .
pause >nul
goto menu

:done
rem 删除本次运行的临时脚本副本: 自己正被 cmd 读取, 交给分离进程延迟删除
if defined TMPSCRIPT (
    start "" /b cmd /c "ping -n 3 127.0.0.1 >nul & del /f /q ""!TMPSCRIPT!"" >nul 2>nul"
)
endlocal
exit /b 0
