echo delete old files
if exist *.sdf   del   /F /Q *.sdf
rem if exist *.sln   del   /F /Q *.sln
if exist *.user   del   /F /Q *.user
if exist debug   rmdir /S /Q debug
if exist release rmdir /S /Q release

echo ע��VC�Ļ�������
call "%VS100COMNTOOLS%vsvars32.bat"

set dst_debug=..\..\..\10-common\lib\debug\vs2010
set dst_release=..\..\..\10-common\lib\release\vs2010
set dst_map=..\..\..\10-common\version\release\vs2010\map
set compileinfo=..\..\..\10-common\version\compileinfo

echo �������Ŀ¼
if not exist %compileinfo%  mkdir %compileinfo%
if not exist %dst_debug%    mkdir %dst_debug%
if not exist %dst_release%  mkdir %dst_release%
if not exist %dst_map%      mkdir %dst_map%

if "%1"=="" (
echo ����debug��release�汾 win32_vs2010
devenv topicmngctrl.vcxproj /ReBuild "Debug|win32"  > %compileinfo%\topicmngctrl_win32_vs2010_d.txt
devenv topicmngctrl.vcxproj /ReBuild "Release|win32"  > %compileinfo%\topicmngctrl_win32_vs2010_r.txt

echo ����dll
copy /Y Debug\topicmngctrl.dll      %dst_debug%
copy /Y Debug\topicmngctrl.lib      %dst_debug%
copy /Y Release\topicmngctrl.dll    %dst_release%
copy /Y Release\topicmngctrl.lib    %dst_release%
copy /Y Release\topicmngctrl.map    %dst_map%
copy /Y Release\topicmngctrl.pdb    %dst_map%

) else if "%1"=="debug" (
echo ����debug�汾 win32_vs2010
devenv topicmngctrl.vcxproj /ReBuild "Debug|win32"  > %compileinfo%\topicmngctrl_win32_vs2010_d.txt

echo ����dll
copy /Y Debug\topicmngctrl.dll      %dst_debug%
copy /Y Debug\topicmngctrl.lib      %dst_debug%
) else if "%1"=="release" (
echo ����release�汾 win32_vs2010
devenv topicmngctrl.vcxproj /ReBuild "Release|win32"  > %compileinfo%\topicmngctrl_win32_vs2010_r.txt

echo ����dll
copy /Y Release\topicmngctrl.dll    %dst_release%
copy /Y Release\topicmngctrl.lib    %dst_release%
copy /Y Release\topicmngctrl.map    %dst_map%
copy /Y Release\topicmngctrl.pdb    %dst_map%

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
