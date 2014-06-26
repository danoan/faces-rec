magick_left:=`MagickWand-config --cflags`
magick_right:=`MagickWand-config --ldflags`

source:=source
object:=object
lib:=lib

module:=classifier

object_path:=$(object)
source_path:=$(source)
lib_path:=$(lib)


all: cascadeClassifier.o classifier.o integralImage.o loader.o 
	ar rc $(lib_path)/libclassifier.a $(object_path)/cascadeClassifier.o $(object_path)/classifier.o $(object_path)/integralImage.o $(object_path)/loader.o

cascadeClassifier.o:
	g++ $(magick_left) -o $(object_path)/cascadeClassifier.o -c $(source_path)/cascadeClassifier.cpp $(magick_right) -O3 -w

classifier.o:
	g++  $(magick_left) -o $(object_path)/classifier.o -c $(source_path)/classifier.cpp $(magick_right) -Llib -lfeature -O3 -w

integralImage.o:
	g++ $(magick_left) -o $(object_path)/integralImage.o -c $(source_path)/integralImage.cpp $(magick_right) -Llib -lfeature -O3 -w

loader.o:
	g++ $(magick_left) -o $(object_path)/loader.o -c $(source_path)/loader.cpp $(magick_right) -O3 -w
