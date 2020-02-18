ver_type=cloud2
ismp_version=1.0.2
if [ -n "$1" ]; then
    ver_type=$1
	echo $ver_type
fi
if [ -n "$2" ]; then
    ismp_version=$2
	echo $ismp_version
fi
echo delete old files
rm -rf target
DIR1="{BASH_SOURCE[0]}"   #脚本相对于当前目录的路径，是相对路径
DIR2="$( dirname $DIR1 )"   #得到脚本的目录名，也是相对路径
#cd命令切换到脚本所在目录，再执行pwd命令得到脚本绝对路径
path="$( cd $DIR2 && pwd )"
ver_unit=${path##*/}
dst_dir=../../10-common/version/release/linux64/ismp/j1cbb
src_name=${ver_unit}-${ismp_version}.jar
dst_name=${ver_unit}-${ismp_version}.jar

echo "创建相关目录"
if [ ! -d "$dst_dir" ]; then  
    mkdir -p "$dst_dir"  
fi

#必须加call指令，否则会中断在mvn操作上
mvn clean install -Dismp.version=${ismp_version}>../../10-common/version/compileinfo/ismp_${ver_type}_linux64_j1cbb_${ver_unit}_r.txt 2>&1
echo "拷贝目标"
cp -f target/$src_name 	$dst_dir/$dst_name
mvn clean
echo "----end"
