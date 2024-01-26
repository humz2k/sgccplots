RAY_MACOS_FLAGS ?= -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

driver: test.sgc lib/libsgccplots.a
	sgcc $^ -o $@ --echo $(RAY_MACOS_FLAGS)

lib/%.o: src/%.sgc
	mkdir -p lib
	sgcc -c $^ -o $@

lib/libsgccplots.a: lib/sgccplots.o RAYLIB
	mkdir -p lib
	ar x ext/raylib/src/libraylib.a
	ar cr $@ $< *.o
	rm *.o
	rm __*

RAYLIB:
	cd ext/raylib/src && $(MAKE) PLATFORM=PLATFORM_DESKTOP
	
.PHONY: clean

clean:
	rm -rf lib
	rm driver