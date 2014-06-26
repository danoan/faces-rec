source:=source
object:=object
lib:=lib

module:=feature

object_path:=$(object)
feature:=$(source)


all: maskBlock.o mask.o featureMask.o featureFactory.o mdf.o m3vf.o m3hf.o m2vf.o m2hf.o facesFeatureFactory.o
	ar rc $(lib)/libfeature.a $(object_path)/maskBlock.o $(object_path)/mask.o $(object_path)/featureMask.o $(object_path)/featureFactory.o $(object_path)/mdf.o $(object_path)/m3vf.o $(object_path)/m3hf.o $(object_path)/m2vf.o $(object_path)/m2hf.o $(object_path)/facesFeatureFactory.o

facesFeatureFactory.o:
	g++ -o $(object_path)/facesFeatureFactory.o -c $(feature)/facesFeatureFactory.cpp -O3 -w

mdf.o:
	g++ -o $(object_path)/mdf.o -c $(feature)/maskDiagonalFactory.cpp -O3 -w

m3vf.o:
	g++ -o $(object_path)/m3vf.o -c $(feature)/maskThreeVerticalFactory.cpp -O3 -w

m3hf.o:
	g++ -o $(object_path)/m3hf.o -c $(feature)/maskThreeHorizontalFactory.cpp -O3 -w

m2vf.o:
	g++ -o $(object_path)/m2vf.o -c $(feature)/maskTwoVerticalFactory.cpp -O3 -w

m2hf.o:
	g++ -o $(object_path)/m2hf.o -c $(feature)/maskTwoHorizontalFactory.cpp -O3 -w

featureFactory.o:
	g++ -o $(object_path)/featureFactory.o -c $(feature)/featureFactory.cpp -O3 -w

featureMask.o:
	g++ -o $(object_path)/featureMask.o -c $(feature)/featureMask.cpp -O3 -w

maskBlock.o:
	g++ -o $(object_path)/maskBlock.o -c $(feature)/maskBlock.cpp -O3 -w

mask.o:
	g++ -o $(object_path)/mask.o -c $(feature)/mask.cpp -lm -O3 -w
