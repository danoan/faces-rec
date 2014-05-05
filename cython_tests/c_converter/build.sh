rm c_converter.so
rm c_loader.so
python setup.py build
cp ~/Projects/faces-rec/cython_tests/c_converter/build/lib.linux-x86_64-2.7/c_converter.so ~/Projects/faces-rec/cython_tests/c_converter/c_converter.so
cp ~/Projects/faces-rec/cython_tests/c_converter/build/lib.linux-x86_64-2.7/c_loader.so ~/Projects/faces-rec/cython_tests/c_converter/c_loader.so
python pyToc.py
