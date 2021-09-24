Raytracer


## Compile
```bash
mkdir build
cd ./build
conan install ../
cmake --build . --target clean
cmake ../ -G "Unix Makefiles" 
cmake --build .
```
