echo delete old files
if exist *.sdf   del   /F /Q *.sdf
rem if exist *.sln   del   /F /Q *.sln
if exist *.user   del   /F /Q *.user
if exist debug   rmdir /S /Q debug
if exist release rmdir /S /Q release

if "%gwvsvars%"=="" (
set gwvsvars=true
echo 注册VC的环境变量
call "%VS100COMNTOOLS%vsvars32.bat"
)

set dst_debug=..\..\..\10-common\version\debug\vs2010
set dst_release=..\..\..\10-common\version\release\vs2010
set dst_debug_lib=..\..\..\10-common\lib\debug\vs2010\mcredis
set dst_release_lib=..\..\..\10-common\lib\release\vs2010\mcredis
set dst_map=..\..\..\10-common\version\release\vs2010\map
set compileinfo=..\..\..\10-common\version\compileinfo

echo 创建相关目录
if not exist %compileinfo%  mkdir %compileinfo%
if not exist %dst_debug%    mkdir %dst_debug%
if not exist %dst_release%  mkdir %dst_release%
if not exist %dst_debug_lib%    mkdir %dst_debug_lib%
if not exist %dst_release_lib%  mkdir %dst_release_lib%
if not exist %dst_map%      mkdir %dst_map%

if "%1"=="" (
echo 编译debug和release版本 win32_vs2010
devenv mcredis.vcxproj /ReBuild "Debug|win32"  > %compileinfo%\mcredis_vs2010_d.txt
devenv mcredis.vcxproj /ReBuild "Release|win32"  > %compileinfo%\mcredis_vs2010_r.txt

echo 拷贝dll
copy /Y Debug\mcredis.dll      %dst_debug%
copy /Y Debug\mcredis.dll      %dst_debug_lib%
copy /Y Debug\mcredis.lib      %dst_debug_lib%
copy /Y Release\mcredis.dll    %dst_release%
copy /Y Release\mcredis.dll    %dst_release_lib%
copy /Y Release\mcredis.lib    %dst_release_lib%
copy /Y Release\mcredis.map    %dst_map%
copy /Y Release\mcredis.pdb    %dst_map%
) else if "%1"=="debug" (
echo 编译debug版本 win32_vs2010
devenv mcredis.vcxproj /ReBuild "Debug|win32"  > %compileinfo%\mcredis_vs2010_d.txt

echo 拷贝dll
copy /Y Debug\mcredis.dll      %dst_debug%
copy /Y Debug\mcredis.dll      %dst_debug_lib%
copy /Y Debug\mcredis.lib      %dst_debug_lib%

) else if "%1"=="release" (
echo 编译release版本 win32_vs2010
devenv mcredis.vcxproj /ReBuild "Release|win32"  > %compileinfo%\mcredis_vs2010_r.txt

echo 拷贝dll
copy /Y Release\mcredis.dll    %dst_release%
copy /Y Release\mcredis.dll    %dst_release_lib%
copy /Y Release\mcredis.lib    %dst_release_lib%
copy /Y Release\mcredis.map    %dst_map%
copy /Y Release\mcredis.pdb    %dst_map%

) else (
echo 错误的参数："%1"
)

:clean
echo 删除编译中间文件
if exist debug   rmdir /S /Q Debug
if exist release rmdir /S /Q Release
if exist *.sdf   del   /F /Q *.sdf
rem if exist *.sln   del   /F /Q *.sln
if exist *.user   del   /F /Q *.user
