echo "----mcredis"
ver_type=cloud2
if [ -n "$1" ]; then 
    ver_type=$1
fi

cd prj_linux64
make ver_type=$ver_type
cd ..
