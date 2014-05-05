gcc `MagickWand-config --cflags` c_image.c `MagickWand-config --ldflags`
gcc `MagickWand-config --cflags` c_window_gen.c c_image.c `MagickWand-config --ldflags` -lm
gcc `MagickWand-config --cflags` c_detector.c c_window_gen.c c_image.c `MagickWand-config --ldflags` -lm


gcc `MagickWand-config --cflags` teste.c c_detector.c c_image.c c_integral.c c_loader.c c_window_gen.c `MagickWand-config --ldflags` -lm

g++ `MagickWand-config --cflags` teste.cpp c_basic.cpp features.cpp c_window_gen.c c_image.c  `MagickWand-config --ldflags` -lm

g++ `MagickWand-config --cflags` teste.c c_detector.c c_basic.cpp features.cpp c_integral.c c_loader.c c_window_gen.c c_image.c  `MagickWand-config --ldflags` -lm


Hierarquia de dependencias

basic
image
featureMask
integral
featureFactory
classifier
loader
window_gen
detector


g++ `MagickWand-config --cflags` detector.cpp window_gen.cpp classifier.cpp featureFactory.cpp  integral.cpp  featureMask.cpp image.c  `MagickWand-config --ldflags` -lm


g++ `MagickWand-config --cflags` ../main.cpp loader.cpp classifier.cpp integral.cpp  featureMask.cpp image.c  `MagickWand-config --ldflags` -lm

g++ `MagickWand-config --cflags` teste.cpp source/basic.cpp source/detector.cpp source/window_gen.cpp source/loader.cpp source/classifier.cpp source/integral.cpp  source/featureMask.cpp source/featureFactory.cpp source/image.c  `MagickWand-config --ldflags` -lm