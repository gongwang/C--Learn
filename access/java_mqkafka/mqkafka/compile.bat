echo delete old files
if exist target   rmdir /S /Q target
set dst_dir=..\..\10-common\java_lib
set src_name=mqkafka.jar
set dst_name=mqkafka.jar
::�����callָ�������ж���mvn������
call mvn clean install
::echo ����Ŀ��
::copy /Y target\%src_name% 	%dst_dir%\%dst_name%
call mvn clean