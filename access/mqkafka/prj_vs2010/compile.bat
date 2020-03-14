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
set dst_debug_lib=..\..\..\10-common\lib\debug\vs2010\mqkafka
set dst_release_lib=..\..\..\10-common\lib\release\vs2010\mqkafka
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
devenv mqkafka.vcxproj /ReBuild "Debug|win32"  > %compileinfo%\mqkafka_vs2010_d.txt
devenv mqkafka.vcxproj /ReBuild "Release|win32"  > %compileinfo%\mqkafka_vs2010_r.txt

echo 拷贝dll
copy /Y Debug\mqkafka.dll      %dst_debug%
copy /Y Debug\mqkafka.dll      %dst_debug_lib%
copy /Y Debug\mqkafka.lib      %dst_debug_lib%
copy /Y Release\mqkafka.dll    %dst_release%
copy /Y Release\mqkafka.dll    %dst_release_lib%
copy /Y Release\mqkafka.lib    %dst_release_lib%
copy /Y Release\mqkafka.map    %dst_map%
copy /Y Release\mqkafka.pdb    %dst_map%
echo 拷贝依赖库
copy /Y ..\..\..\10-common\lib\debug\win32\pthreadVC2.dll      			%dst_debug_lib%
copy /Y ..\..\..\10-common\lib\debug\win32\mqkafka\librdkafka.dll      	%dst_debug_lib%
copy /Y ..\..\..\10-common\lib\debug\win32\mqkafka\zlibd.dll      		%dst_debug_lib%
copy /Y ..\..\..\10-common\lib\release\win32\pthreadVC2.dll    			%dst_release_lib%
copy /Y ..\..\..\10-common\lib\release\win32\mqkafka\librdkafka.dll    	%dst_release_lib%
copy /Y ..\..\..\10-common\lib\release\win32\mqkafka\zlib.dll    		%dst_release_lib%
) else if "%1"=="debug" (
echo 编译debug版本 win32_vs2010
devenv mqkafka.vcxproj /ReBuild "Debug|win32"  > %compileinfo%\mqkafka_vs2010_d.txt

echo 拷贝dll
copy /Y Debug\mqkafka.dll      %dst_debug%
copy /Y Debug\mqkafka.dll      %dst_debug_lib%
copy /Y Debug\mqkafka.lib      %dst_debug_lib%
echo 拷贝依赖库
copy /Y ..\..\..\10-common\lib\debug\win32\pthreadVC2.dll      			%dst_debug_lib%
copy /Y ..\..\..\10-common\lib\debug\win32\mqkafka\librdkafka.dll      	%dst_debug_lib%
copy /Y ..\..\..\10-common\lib\debug\win32\mqkafka\zlibd.dll      		%dst_debug_lib%
) else if "%1"=="release" (
echo 编译release版本 win32_vs2010
devenv mqkafka.vcxproj /ReBuild "Release|win32"  > %compileinfo%\mqkafka_vs2010_r.txt

echo 拷贝dll
copy /Y Release\mqkafka.dll    %dst_release%
copy /Y Release\mqkafka.dll    %dst_release_lib%
copy /Y Release\mqkafka.lib    %dst_release_lib%
copy /Y Release\mqkafka.map    %dst_map%
copy /Y Release\mqkafka.pdb    %dst_map%
echo 拷贝依赖库
copy /Y ..\..\..\10-common\lib\release\win32\pthreadVC2.dll    			%dst_release_lib%
copy /Y ..\..\..\10-common\lib\release\win32\mqkafka\librdkafka.dll    	%dst_release_lib%
copy /Y ..\..\..\10-common\lib\release\win32\mqkafka\zlib.dll    		%dst_release_lib%
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
