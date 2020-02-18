echo delete old files
if exist target   rmdir /S /Q target
set dst_dir=..\..\10-common\java_lib
set src_name=mqkafka.jar
set dst_name=mqkafka.jar
::必须加call指令，否则会中断在mvn操作上
call mvn clean install
::echo 拷贝目标
::copy /Y target\%src_name% 	%dst_dir%\%dst_name%
call mvn clean