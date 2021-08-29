.PHONY: build

export CXX CXXFLAGS PPFLAGS EXECUTABLE BUILD VPATH

CXX_OBJECTS:=$(patsubst %.cpp, %.o, $(wildcard *.cpp))
SUBDIRS:=$(dir $(wildcard */))
CWD:=${shell pwd}

build: ${CXX_OBJECTS}
	$(foreach dir, ${SUBDIRS}, "${MAKE}" -j -C $(dir) &&) cd ${CWD}
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(PPFLAGS) -c -o $(BUILD)/$@ $^
%.cpp: %.hpp