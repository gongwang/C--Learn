@echo off
if "%gwvsvars%"=="" (
set gwvsvars=true
echo ע��VC�Ļ�������
call "%VS100COMNTOOLS%vsvars32.bat"
)

rem �����������Ŀ¼
echo �������Ŀ¼
set dst_debug=..\..\..\10-common\lib\debug\vs2010\db
set dst_release=..\..\..\10-common\lib\release\vs2010\db
set compileinfo=..\..\..\10-common\version\compileinfo
if not exist %compileinfo%  mkdir %compileinfo%
if not exist %dst_debug%    mkdir %dst_debug%
if not exist %dst_release%  mkdir %dst_release%


if "%1"=="" (
echo "��ʼ���� dbix.vcxproj Debug|Win32"
devenv dbix.vcxproj /REBUILD "Debug|Win32"  /OUT %compileinfo%\dbix_vs2010_d.txt
echo "������� dbix.vcxproj Debug|Win32"

echo "��ʼ���� dbix.vcxproj Release|Win32"
devenv dbix.vcxproj /REBUILD "Release|Win32"  /OUT %compileinfo%\dbix_vs2010_r.txt
echo "������� dbix.vcxproj Release|Win32"

echo �����⵽10-common
copy /Y Debug\dbix.lib    %dst_debug%
copy /Y Release\dbix.lib  %dst_release%


) else if "%1"=="debug" (
echo "��ʼ���� dbix.vcxproj Debug|Win32"
devenv dbix.vcxproj /REBUILD "Debug|Win32"  /OUT %compileinfo%\dbix_vs2010_d.txt
echo "������� dbix.vcxproj Debug|Win32"

echo �����⵽10-common
copy /Y Debug\dbix.lib    %dst_debug%


) else if "%1"=="release" (
echo "��ʼ���� dbix.vcxproj Release|Win32"
devenv dbix.vcxproj /REBUILD "Release|Win32"  /OUT %compileinfo%\dbix_vs2010_r.txt
echo "������� dbix.vcxproj Release|Win32"

echo �����⵽10-common
copy /Y Release\dbix.lib  %dst_release%


) else (
echo ����Ĳ�����"%1"
)

echo ɾ���м��ļ�
if exist dbix.vcxproj.user del /Q dbix.vcxproj.user
if exist Debug      rmdir /S /Q Debug
if exist Release    rmdir /S /Q Release
if exist *.sdf      del /Q *.sdf
if exist *.sln      del /Q *.sln
if exist *.suo      del /Q /A:H *.suo

if not %errorlevel% == 0 ( 
	goto :eof
)

