CPP=g++
OUT=dark.out

CPP_FLAGS=-Wall -Wextra -I ../include -std=c++17 -O2
LD_FLAGS=-Wall -Wextra -O2
SOURCE_FILES=$(shell find ../source -name *.cpp)
OBJECT_FILES=$(addsuffix .o, $(SOURCE_FILES))

build:
	$(MAKE) $(OBJECT_FILES)
	$(CPP) $(OBJECT_FILES) $(LD_FLAGS) -o $(OUT)

clean:
	rm -f $(OUT) $(OBJECT_FILES)

%.cpp.o:	%.cpp
	$(CPP) -c $< $(CPP_FLAGS) -o $@