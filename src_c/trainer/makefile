magick_left:=`MagickWand-config --cflags`
magick_right:=`MagickWand-config --ldflags`

source:=source
object:=object
lib:=lib

module:=trainer

object_path:=$(object)
source_path:=$(source)
lib_path:=$(lib)

all: classificationTable.o imageSet.o tableItem.o trainer.o trainingImage.o trainingImageRepository.o trainingSet.o validationSet.o trainingImageDescriptor.o
	ar rc $(lib_path)/libtrainer.a $(object_path)/classificationTable.o $(object_path)/imageSet.o $(object_path)/tableItem.o $(object_path)/trainer.o $(object_path)/trainingImage.o $(object_path)/trainingImageRepository.o $(object_path)/trainingSet.o $(object_path)/validationSet.o $(object_path)/trainingImageDescriptor.o

classificationTable.o:
	g++ $(magick_left) -o $(object_path)/classificationTable.o -c $(source_path)/classificationTable.cpp $(magick_right) -O3 -w	

imageSet.o:
	g++ $(magick_left) -o $(object_path)/imageSet.o -c $(source_path)/imageSet.cpp $(magick_right) -O3 -w

tableItem.o:
	g++ $(magick_left) -o $(object_path)/tableItem.o -c $(source_path)/tableItem.cpp $(magick_right) -O3 -w	

trainer.o:
	g++ $(magick_left) -o $(object_path)/trainer.o -c $(source_path)/trainer.cpp $(magick_right) -Llib -lfeature -lutil -O3 -w

trainingImage.o:
	g++ $(magick_left) -o $(object_path)/trainingImage.o -c $(source_path)/trainingImage.cpp $(magick_right) -Llib -lfeature -lutil -O3 -w

trainingImageDescriptor.o:
	g++ $(magick_left) -o $(object_path)/trainingImageDescriptor.o -c $(source_path)/trainingImageDescriptor.cpp $(magick_right) -Llib -lfeature -O3 -w	

trainingImageRepository.o:
	g++ $(magick_left) -o $(object_path)/trainingImageRepository.o -c $(source_path)/trainingImageRepository.cpp $(magick_right) -Llib -lfeature -O3 -w	

trainingSet.o:
	g++ $(magick_left) -o $(object_path)/trainingSet.o -c $(source_path)/trainingSet.cpp $(magick_right) -O3 -w

validationSet.o:
	g++ $(magick_left) -o $(object_path)/validationSet.o -c $(source_path)/validationSet.cpp $(magick_right) -O3 -w




