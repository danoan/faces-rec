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
bin_path="../bin"

make -f makefileUtil
make -f makefileFeature
make -f makefilePyToC
make -f makefileClassifier
make -f makefileDetector

# g++ `MagickWand-config --cflags` source/teste.cpp source/basic.cpp source/image.c -Llib -lpytoc -lfeature -lpytoc -ldetector -lclassifier `MagickWand-config --ldflags` -O3 -w

g++ `MagickWand-config --cflags` $source_path/mainTrainer.cpp -L$lib_path -ldetector -lclassifier -lfeature -lutil `MagickWand-config --ldflags` -O3 -w -o $bin_path/trainer.out

g++ `MagickWand-config --cflags` $source_path/mainDetector.cpp -L$lib_path -ldetector -lclassifier -lfeature -lutil `MagickWand-config --ldflags` -O3 -w -o $bin_path/detector.out

# g++ `MagickWand-config --cflags` drawFeature.cpp image.c config.cpp basic.cpp log.cpp -L../lib -lfeature -o drawFeature.out -w `MagickWand-config --ldflags`
