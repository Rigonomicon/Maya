rm -rf build.2026
Mkdir build.2026
export DEVKIT_LOCATION=/Users/recombobulator/Projects/Rigonomicon/CMAKE/devkitbase.2026
export MAYA_LOCATION=/Applications/Autodesk/maya2026
cmake -H. -Bbuild.2026 -G Xcode -DCMAKE_OSX_ARCHITECTURES="arm64"
cmake --build build.2026