## Team project ART

### Install:
```sh
sudo apt-get install libopencv-dev
```

#### Install gtest:
```sh
wget http://googletest.googlecode.com/files/gtest-1.7.0.zip
unzip gtest-1.7.0.zip
cd gtest-1.7.0/
./configure
make
sudo cp -a include/gtest /usr/include
sudo cp -a lib/.libs/* /usr/lib/
```

#### Install gmock:
```sh
wget https://googlemock.googlecode.com/files/gmock-1.7.0.zip
unzip gmock-1.7.0.zip 
cd gmock-1.7.0/
./configure
make
sudo cp -a include/gmock /usr/include
sudo cp -a lib/.libs/* /usr/lib/
```

### Building app
```sh
git clone https://github.com/Klapeyron/Team_project_ART
cd Team_project_ART/
mkdir build/
cd build/
cmake ..
make
```

#### Using clang
If u want use clang to compile application, type in build/ directory:
```sh
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..
```