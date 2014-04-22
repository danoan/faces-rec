rm integral_image.so
OPT="-O3" CFLAGS="`MagickWand-config --cflags --cppflags`" LDFLAGS="`MagickWand-config --ldflags --libs`" python setup.py build_ext
cp ~/Projects/faces-rec/cython_tests/integral_image/build/lib.linux-x86_64-2.7/integral_image.so ~/Projects/faces-rec/cython_tests/integral_image/integral_image.so
cp ~/Projects/faces-rec/cython_tests/integral_image/build/lib.linux-x86_64-2.7/integral_image.so ~/Projects/faces-rec/src/treinador/integral_image.so
python integral.py
python ~/Projects/faces-rec/src/treinador/test_features.py
