magick_left:=`MagickWand-config --cflags`
magick_right:=`MagickWand-config --ldflags`

lib_path:="/home/daniel/Projects/faces-rec/src_c/lib"
bin_path:="/home/daniel/Projects/faces-rec/analysis/bin/"

all:
	g++ $(magick_left) source/featureAnalysis.cpp -L$(lib_path) -ldetector -lclassifier -lfeature -lutil $(magick_right) -o $(bin_path)/featureAnalysis.out -w