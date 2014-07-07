magick_left:=`MagickWand-config --cflags`
magick_right:=`MagickWand-config --ldflags`

source:=source
object:=object
lib:=lib

module:=util

object_path:=$(object)
source_path:=$(source)
lib_path:=$(lib)


all: basic.o image.o log.o config.o directory.o drawFeature.o
	ar rc $(lib_path)/libutil.a $(object_path)/basic.o $(object_path)/image.o $(object_path)/log.o $(object_path)/config.o $(object_path)/drawFeature.o $(object_path)/directory.o

basic.o:
	g++ -o $(object_path)/basic.o -c $(source_path)/basic.cpp -O3 -w

image.o:
	g++ $(magick_left) -o $(object_path)/image.o -c $(source_path)/image.c $(magick_right) -O3 -w

log.o:
	g++ -o $(object_path)/log.o -c $(source_path)/log.cpp -O3 -w    

config.o:
	g++ -o $(object_path)/config.o -c $(source_path)/config.cpp -O3 -w    

directory.o:
	g++ -o $(object_path)/directory.o -c $(source_path)/directory.cpp -O3 -w  	

drawFeature.o:
	g++ $(magick_left) -o $(object_path)/drawFeature.o -c $(source_path)/drawFeature.cpp $(magick_right) -O3 -w 

teste.out:
	g++ $(magick_left) $(source_path)/teste.cpp -L$(lib_path) -lutil $(magick_right) -O3 -w -o teste.out