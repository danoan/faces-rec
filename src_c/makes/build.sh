# Hierarquia de dependencias

# basic
# image
# featureMask
# integral
# featureFactory
# classifier
# loader
# window_gen
# detector

source_path="../source" 
lib_path="../lib"

make -f makefileFeature
make -f makefilePyToC
make -f makefileClassifier
make -f makefileDetector

# g++ `MagickWand-config --cflags` source/teste.cpp source/basic.cpp source/image.c -Llib -lpytoc -lfeature -lpytoc -ldetector -lclassifier `MagickWand-config --ldflags` -O3 -w

g++ `MagickWand-config --cflags` $source_path/config.cpp $source_path/log.cpp $source_path/mainTrainer.cpp $source_path/basic.cpp $source_path/image.c -L$lib_path -lpytoc -lpytoc -ldetector -lclassifier -lfeature `MagickWand-config --ldflags` -O3 -w -o main.out

