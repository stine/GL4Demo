CC=g++
CFLAGS=-c -g -O0 -Wall -I./glm/ -DGL_GLEXT_LEGACY -DGLX_GLXEXT_LEGACY -DGL_GLEXT_PROTOTYPES -DGLX_GLXEXT_PROTOTYPES
LDFLAGS=-L/usr/lib/fglrx -lGL
SOURCES=main.C Cube.C Shader.C Stopwatch.C
OBJECTS=$(SOURCES:.C=.o)
EXECUTABLE=GL4Demo

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.C.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *~ shaders/*~ glm/*~