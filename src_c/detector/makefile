magick_left:=`MagickWand-config --cflags`
magick_right:=`MagickWand-config --ldflags`

source:=source
object:=object
lib:=lib

module:=detector

object_path:=$(object)
source_path:=$(source)
lib_path:=$(lib)


all: detector.o subwindowGenerator.o
	ar rc $(lib_path)/libdetector.a $(object_path)/detector.o $(object_path)/subwindowGenerator.o

detector.o:
	g++ $(magick_left) -o $(object_path)/detector.o -c $(source_path)/detector.cpp $(magick_right) -L$(lib) -lclassifier -lutil -O3 -w

subwindowGenerator.o:
	g++ $(magick_left) -o $(object_path)/subwindowGenerator.o -c $(source_path)/subwindowGenerator.cpp $(magick_right) -L$(lib) -lutil -O3 -w
