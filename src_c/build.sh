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

make -f makefileFeature
make -f makefilePyToC
make -f makefileClassifier
make -f makefileDetector

# g++ `MagickWand-config --cflags` source/teste.cpp source/basic.cpp source/image.c -Llib -lpytoc -lfeature -lpytoc -ldetector -lclassifier `MagickWand-config --ldflags` -O3 -w

g++ `MagickWand-config --cflags` source/config.cpp source/log.cpp source/mainTrainer.cpp source/basic.cpp source/image.c -Llib -lpytoc -lpytoc -ldetector -lclassifier -lfeature `MagickWand-config --ldflags` -O3 -w

