WIN86_CPP=/mingw32/bin/i686-w64-mingw32-g++
WIN64_CPP=/mingw64/bin/x86_64-w64-mingw32-g++
OUT=dark

CPP_LD_FLAGS=-Wall -Wextra -O2
CPP_FLAGS=-I ../include -std=c++17
LD_FLAGS=-static
SOURCE_FILES=$(shell find ../source -name *.cpp)
OBJECT_FILES=$(addsuffix .o, $(SOURCE_FILES))

build-win:
	rm -f *.exe
	$(MAKE) build-win-x86
	$(MAKE) build-win-x64

build-win-x86:
	$(MAKE) BUILD_ARCH=x86 BUILD_CPP=$(WIN86_CPP) $(OBJECT_FILES) link
	$(MAKE) clean

build-win-x64:
	$(MAKE) BUILD_ARCH=x64 BUILD_CPP=$(WIN64_CPP) $(OBJECT_FILES) link
	$(MAKE) clean

build-for-env:
	$(MAKE) BUILD_ARCH=env BUILD_CPP=g++ $(OBJECT_FILES) link
	$(MAKE) clean

link:
	$(BUILD_CPP) $(OBJECT_FILES) $(CPP_LD_FLAGS) $(LD_FLAGS) -o $(OUT)-$(BUILD_ARCH)

clean:
	rm -f $(OBJECT_FILES)

%.cpp.o:	%.cpp
	$(BUILD_CPP) -c $< $(CPP_LD_FLAGS) $(CPP_FLAGS) -o $@