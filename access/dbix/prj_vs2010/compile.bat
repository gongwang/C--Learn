@echo off
if "%gwvsvars%"=="" (
set gwvsvars=true
echo 注册VC的环境变量
call "%VS100COMNTOOLS%vsvars32.bat"
)

rem 创建编译输出目录
echo 创建相关目录
set dst_debug=..\..\..\10-common\lib\debug\vs2010\db
set dst_release=..\..\..\10-common\lib\release\vs2010\db
set compileinfo=..\..\..\10-common\version\compileinfo
if not exist %compileinfo%  mkdir %compileinfo%
if not exist %dst_debug%    mkdir %dst_debug%
if not exist %dst_release%  mkdir %dst_release%


if "%1"=="" (
echo "开始编译 dbix.vcxproj Debug|Win32"
devenv dbix.vcxproj /REBUILD "Debug|Win32"  /OUT %compileinfo%\dbix_vs2010_d.txt
echo "编译完成 dbix.vcxproj Debug|Win32"

echo "开始编译 dbix.vcxproj Release|Win32"
devenv dbix.vcxproj /REBUILD "Release|Win32"  /OUT %compileinfo%\dbix_vs2010_r.txt
echo "编译完成 dbix.vcxproj Release|Win32"

echo 拷贝库到10-common
copy /Y Debug\dbix.lib    %dst_debug%
copy /Y Release\dbix.lib  %dst_release%


) else if "%1"=="debug" (
echo "开始编译 dbix.vcxproj Debug|Win32"
devenv dbix.vcxproj /REBUILD "Debug|Win32"  /OUT %compileinfo%\dbix_vs2010_d.txt
echo "编译完成 dbix.vcxproj Debug|Win32"

echo 拷贝库到10-common
copy /Y Debug\dbix.lib    %dst_debug%


) else if "%1"=="release" (
echo "开始编译 dbix.vcxproj Release|Win32"
devenv dbix.vcxproj /REBUILD "Release|Win32"  /OUT %compileinfo%\dbix_vs2010_r.txt
echo "编译完成 dbix.vcxproj Release|Win32"

echo 拷贝库到10-common
copy /Y Release\dbix.lib  %dst_release%


) else (
echo 错误的参数："%1"
)

echo 删除中间文件
if exist dbix.vcxproj.user del /Q dbix.vcxproj.user
if exist Debug      rmdir /S /Q Debug
if exist Release    rmdir /S /Q Release
if exist *.sdf      del /Q *.sdf
if exist *.sln      del /Q *.sln
if exist *.suo      del /Q /A:H *.suo

if not %errorlevel% == 0 ( 
	goto :eof
)

