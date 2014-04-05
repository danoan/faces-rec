function domath(){
        eq=$1

        echo -e "scale=4 \n" $eq > temp.txt
        echo `bc -l < temp.txt`
}


count=0
for i in `ls *.jpg`
do
	name=$( printf '%d%s' $count ".pgm")
	`convert $i $name`
	count=`domath $count+1`
done
