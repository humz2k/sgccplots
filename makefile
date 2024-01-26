RAY_MACOS_FLAGS ?= -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

driver: test.sgc lib/libsgccplots.a
	sgcc $^ -o $@ --echo $(RAY_MACOS_FLAGS)

%.o: %.sgc
	sgcc -c $^ -o $@

lib/libsgccplots.a: src/sgccplots.o
	mkdir -p lib
	ar x ext/raylib/src/libraylib.a
	ar cr $@ $^ *.o
	rm *.o
	rm __*
	
.PHONY: clean

clean:
	rm -rf lib
	rm *.o
	rm driver
	