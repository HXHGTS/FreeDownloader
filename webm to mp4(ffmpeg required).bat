@echo off
for /R %%a IN ("Downloads\*.webm") do ( ffmpeg -i %%a -preset ultrafast "Downloads\%%~na.mp4")
echo Finished!
pause

