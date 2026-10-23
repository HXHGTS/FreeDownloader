@echo off
chcp 65001 >nul 2>&1
rem 统一使用 UTF-8 代码页，避免 PowerShell/终端按 UTF-8 解码批处理输出时中文乱码

echo ----------------------------------

echo ffmpeg更新工具

echo ----------------------------------

set INSTALLPATH=%CD%

echo 正在关闭ffmpeg. . .

taskkill /f /im ffmpeg.exe

cls

echo 正在下载ffmpeg. . .

@REM aria2-next --connect-timeout=10 -s8 -x8 -k1M --allow-overwrite=true --out=ffmpeg-master-latest-win64-gpl.zip https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-master-latest-win64-gpl.zip

aria2-next --connect-timeout=10 -s8 -x8 -k1M --allow-overwrite=true --out=ffmpeg-master-latest-win64-gpl.zip https://github.com/yt-dlp/FFmpeg-Builds/releases/download/latest/ffmpeg-master-latest-win64-gpl.zip

echo 正在更新ffmpeg. . .

7za x -aoa ffmpeg-master-latest-win64-gpl.zip

cd ffmpeg-*

move /y bin\ffmpeg.exe %INSTALLPATH%\ffmpeg.exe

move /y bin\ffprobe.exe %INSTALLPATH%\ffprobe.exe

move /y bin\ffplay.exe %INSTALLPATH%\ffplay.exe

echo 正在清理安装包. . .

cd %INSTALLPATH%

del /s /q ffmpeg-master-latest-win64-gpl.zip

for /d %%i in (ffmpeg*) do ( echo Deleting folder: %%i && rd /s /q "%%i")

echo ffmpeg已更新!

cls

echo ----------------------------------

echo ffmpeg更新工具

echo ----------------------------------

echo 请按任意键继续. . .
pause >nul

exit 0
