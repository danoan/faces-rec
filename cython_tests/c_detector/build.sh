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

g++ `MagickWand-config --cflags` teste.cpp source/basic.cpp source/c_loader.c source/detector.cpp source/window_gen.cpp source/loader.cpp source/classifier.cpp source/integral.cpp  source/featureMask.cpp source/featureFactory.cpp source/image.c  `MagickWand-config --ldflags` -lm -O3 -w
./a.out