SOURCEDIR ?= src
BUILD_DIR ?= build

SOURCES := $(shell find $(SOURCEDIR) -name '*.sgc')
OBJECTS := $(SOURCES:src/%.sgc=build/%.o)

RAY_MACOS_FLAGS ?= -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

driver: test.sgc lib/libsgccplots.a
	sgcc $^ -o $@ --echo $(RAY_MACOS_FLAGS)

build/%.o: src/%.sgc
	mkdir -p build
	sgcc -c $^ -o $@

lib/libsgccplots.a: $(OBJECTS) | RAYLIB
	mkdir -p lib
	ar x ext/raylib/src/libraylib.a
	ar cr $@ $^ *.o
	rm *.o
	rm __*

RAYLIB:
	cd ext/raylib/src && $(MAKE) PLATFORM=PLATFORM_DESKTOP
	
.PHONY: clean

clean:
	rm -rf lib
	rm driver
	rm -rf build