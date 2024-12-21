@echo off

taskkill /f /im lux*

taskkill /f /im yt-dlp*

taskkill /f /im aria2c*

taskkill /f /im FreeDownloader*

del /f /s /q temp\*

del /f /s /q cookies\*

rd /s /q Downloads

del /f /s /q config\proxy.ini

del /f /s /q config\rpc.conf

del /f /s /q config\uuid

mkdir Downloads

cls

echo Finished!

pause > nul

exit 0