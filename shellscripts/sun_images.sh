function domath(){
        eq=$1

        echo -e "scale=4 \n" $eq > temp.txt
        echo `bc -l < temp.txt`
}


count=0
path=SUN2012/Images/files
for i in `ls $path`
do
    `mv $path/$i/*.* $path/`
done
