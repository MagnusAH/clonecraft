TARGET=WINDOWS
CXX=x86_64-w64-mingw32-g++
EXECUTABLE = clonecraft.exe

PPFLAGS =

CWD:=$(shell pwd)
SUBDIRS = src/ $(sort $(dir $(wildcard src/*/ ) ) )
BUILD=$(CWD)/windows/build

CXXFLAGS = -L$(CWD)/windows/ -lglfw3 -lgl3w
CXXFLAGS += -I$(CWD)/include
CXXFLAGS += -msse4.2 -mavx2 -mfma

VPATH = $(CWD)/src:$(BUILD)

export CXX CXXFLAGS PPFLAGS EXECUTABLE BUILD VPATH

.PHONY: build release compile clean run

build: CXXFLAGS += -O2 -Wall
build: PPFLAGS += -DDEBUG
build: compile

release: CXXFLAGS += -O3 -Werror
release: PPFLAGS += -DRELEASE
release: clean compile

compile:
	@clear
	$(shell mkdir -p $(BUILD))
	$(MAKE) -j -C "src/" && cd $(CWD)
#	$(CXX) $(BUILD)/*.o -o windows/$(EXECUTABLE) $(CXXFLAGS)
#for compiling when libstdc++-6.dll is not in executable directory included
	$(CXX) $(BUILD)/*.o -o windows/$(EXECUTABLE) $(CXXFLAGS) -static-libgcc -static-libstdc++

clean:
	@rm -rf $(BUILD)
	@rm -f windows/$(EXECUTABLE)
	@clear

run:
	@clear
	-@cd windows && ./$(EXECUTABLE)