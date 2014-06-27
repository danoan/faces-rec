magick_left:=`MagickWand-config --cflags`
magick_right:=`MagickWand-config --ldflags`

lib_path:="../src_c/viola/lib"
bin_path:="bin"

all:
	g++ $(magick_left) source/featureAnalysis.cpp -L$(lib_path) -ldetector -lclassifier -ltrainer -lfeature -lutil $(magick_right) -o $(bin_path)/featureAnalysis.out -w
