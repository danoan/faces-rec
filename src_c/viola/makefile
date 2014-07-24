source_path:=source
lib_path:=lib
bin_path:=bin


all: libutil.a libfeature.a libclassifier.a libtrainer.a libdetector.a 	
	g++ `MagickWand-config --cflags` $(source_path)/mainDetector.cpp -L$(lib_path) -ldetector -ltrainer -lclassifier -lfeature -lutil `MagickWand-config --ldflags` -O3 -w -o $(bin_path)/detector.out
	g++ `MagickWand-config --cflags` $(source_path)/mainFeatureViewer.cpp -L$(lib_path) -ldetector -ltrainer -lclassifier -lfeature -lutil `MagickWand-config --ldflags` -O3 -w -o $(bin_path)/featureViewer.out	

teste.out:
	g++ `MagickWand-config --cflags` $(source_path)/mainTest.cpp -L$(lib_path) -ldetector -ltrainer -lclassifier -lfeature -lutil `MagickWand-config --ldflags` -O3 -w -o $(bin_path)/teste.out	

trainer.out:
	nvcc -I/usr/include/ImageMagick -arch=sm_20 $(source_path)/mainTrainer.cpp -L$(lib_path) -ldetector -ltrainer -lclassifier -lfeature -lutil `MagickWand-config --ldflags` -O3 -w -o $(bin_path)/trainer.out

libutil.a:
	$(MAKE) -f "../util/makefile" -C "../util"
	cp -r ../util/lib/*.* $(lib_path)

libfeature.a:
	$(MAKE) -f "../feature/makefile" -C "../feature"
	cp -r ../feature/lib/*.* $(lib_path)

# make -f "../pyToC/makefile" -C "../pyToC"
# cp -r ../pyToC/lib/*.* $lib_path

libclassifier.a:
	$(MAKE) -f "../classifier/makefile" -C "../classifier"
	cp -r ../classifier/lib/*.* $(lib_path)

libtrainer.a:
	$(MAKE) -f "../trainer/makefile" -C "../trainer"
	cp -r ../trainer/lib/*.* $(lib_path)

libdetector.a:
	$(MAKE) -f "../detector/makefile" -C "../detector"
	cp -r ../detector/lib/*.* $(lib_path)
