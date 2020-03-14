echo delete old files
if exist *.sdf   del   /F /Q *.sdf
rem if exist *.sln   del   /F /Q *.sln
if exist *.user   del   /F /Q *.user
if exist debug   rmdir /S /Q debug
if exist release rmdir /S /Q release
if "%gwvsvars%"=="" (
set gwvsvars=true
echo ע��VC�Ļ�������
call "%VS100COMNTOOLS%vsvars32.bat"
)

set dst_debug_lib=..\..\..\10-common\lib\debug\win32\communication
set dst_release_lib=..\..\..\10-common\lib\release\win32\communication
set compileinfo=..\..\..\10-common\version\compileinfo

echo �������Ŀ¼
if not exist %compileinfo%  mkdir %compileinfo%
if not exist %dst_debug_lib%    mkdir %dst_debug_lib%
if not exist %dst_release_lib%  mkdir %dst_release_lib%

if "%1"=="" (
echo ����debug��release�汾 win32_vs2010
devenv communication.vcxproj /ReBuild "Debug|win32"  > %compileinfo%\communication_win32_vs2010_d.txt
devenv communication.vcxproj /ReBuild "Release|win32"  > %compileinfo%\communication_win32_vs2010_r.txt

echo ����lib
copy /Y Debug\communication.lib      %dst_debug_lib%
copy /Y Release\communication.lib      %dst_release_lib%

) else if "%1"=="debug" (
echo ����debug�汾 win32_vs2010
devenv communication.vcxproj /ReBuild "Debug|win32"  > %compileinfo%\communication_win32_vs2010_d.txt

echo ����lib
copy /Y Debug\communication.lib      %dst_debug_lib%

) else if "%1"=="release" (
echo ����release�汾 win32_vs2010
devenv communication.vcxproj /ReBuild "Release|win32"  > %compileinfo%\communication_win32_vs2010_r.txt

echo ����lib
copy /Y Release\communication.lib      %dst_release_lib%

) else (
echo ����Ĳ�����"%1"
)

:clean
echo ɾ�������м��ļ�
if exist debug   rmdir /S /Q Debug
if exist release rmdir /S /Q Release
if exist *.sdf   del   /F /Q *.sdf
rem if exist *.sln   del   /F /Q *.sln
if exist *.user   del   /F /Q *.user
