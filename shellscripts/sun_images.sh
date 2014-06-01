function domath(){
        eq=$1

        echo -e "scale=4 \n" $eq > temp.txt
        echo `bc -l < temp.txt`
}


count=0
path=SUN2012/Images
destiny=SUN2012/AllImages
for i in `ls $path`
do
    #1o Nivel de profundidade
    `mv $path/$i/*.* $destiny/`

    path_2=$path/$i
    for j in `ls $path_2`
    do
        #2o Nivel de profundidade
        `mv $path_2/$j/*.* $destiny/`
        path_3=$path_2/$j

        for k in `ls $path_3`
        do
            #3o Nivel de profundidade
            `mv $path_3/$k/*.* $destiny/`
        done

    done
    
done
