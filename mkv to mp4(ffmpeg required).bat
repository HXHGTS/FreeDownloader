@echo off
for /R %%a IN ("Downloads\*.mkv") do ( ffmpeg -i %%a -vcodec copy -acodec copy "Downloads\%%~na.mp4")
echo Finished!
pause