## POWER-OpenCV: Open Source Computer Vision Library Optimized for IBM Power Architecture

### Resources

* Homepage: <http://opencv.org>
* Docs: <http://docs.opencv.org/master/>
* Q&A forum: <http://answers.opencv.org>
* Issue tracking: <https://github.com/opencv/opencv/issues>

### Contributing

* Please read before starting work on a pull request: <https://github.com/opencv/opencv/wiki/How_to_contribute>
* Build and other information, see: <https://github.com/IBM/opencv-power/wiki>

#### Summary of guidelines:

* One pull request per issue;
* Choose the right base branch;
* Include tests and documentation;
* Clean up "oops" commits before submitting;
* Follow the [coding style guide](https://github.com/opencv/opencv/wiki/Coding_Style_Guide).


#### Compile and install
```
$ cd opencv
$ mkdir build
$ cd build
$ cmake -DWITH_JPEG=ON -DWITH_OPENCL=OFF \
-DWITH_OPENMP=ON -DWITH_PTHREADS_PF=OFF \
-DCMAKE_C_FLAGS="-mcpu=power9 -mtune=power9" -DCMAKE_CXX_FLAGS="-mcpu=power9 -mtune=power9" \
-DCMAKE_VERBOSE_MAKEFILE=ON \
-DCMAKE_C_COMPILER=/opt/at11.0/bin/gcc -DCMAKE_CXX_COMPILER=/opt/at11.0/bin/g++ \
$ make -j20
$ make install
```
====================================
```
a possible compile options for python3 libs

cmake -DWITH_JPEG=ON -DWITH_OPENCL=OFF \
-DWITH_OPENMP=OFF -DWITH_PTHREADS_PF=OFF \
-DWITH_CUDA=OFF \
-DCMAKE_C_FLAGS="-mcpu=power9 -mtune=power9" -DCMAKE_CXX_FLAGS="-mcpu=power9 -mtune=power9" \
-DCMAKE_VERBOSE_MAKEFILE=ON \
-DCMAKE_C_COMPILER=$(which gcc) -DCMAKE_CXX_COMPILER=$(which g++) \
-DPYTHON3_EXECUTABLE=$(which python3) \
-DPYTHON3_INCLUDE_DIR=$(python3 -c "from distutils.sysconfig import get_python_inc; print(get_python_inc())") \
-DPYTHON3_LIBRARY=$(python3 -c "from distutils.sysconfig import get_config_var;from os.path import dirname,join ; print(join(dirname(get_config_var('LIBPC')),get_config_var('LDLIBRARY')))") \
-DPYTHON3_NUMPY_INCLUDE_DIRS=$(python3 -c "import numpy; print(numpy.get_include())") \
-DPYTHON3_PACKAGES_PATH=$(python3 -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())") \
```
* The example compilation guide is using AT which is bind to Python3. Use other compilers for python2.
* You could specify the compiler as your default GCC/G++;
* It is highly recommend to turn OpenMP on for performance consideration, and it is better to set the environment  OMP_PROC_BIND=true and proper OMP_NUM_THREADS for affinity;
* If you are compiling the code on a POWER8 machine, please replace the cmake c/c++ flags as power8.
