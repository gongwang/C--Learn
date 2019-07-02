SET WORK_PATH=%cd%
cd /d %VS100COMNTOOLS%
cd ..
cd IDE\
SET VS100IDE=%cd%\devenv.exe
cd /d %WORK_PATH%


echo build release version
del/f/s/q  ..\..\..\10-common\version\svroutput\ftpactive_r.txt
"%VS100IDE%" FtpActiveX.vcxproj /REBUILD "Release|Win32" /OUT ..\..\..\10-common\version\svroutput\ftpactive_r.txt 
echo build release version over


