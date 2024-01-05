STB_INCLUDE_PATH = /home/joey/tools/stb
TINYOBJ_INCLUDE_PATH = /home/joey/tools/tinyobjloader

CFLAGS = -std=c++17 -I$(TINYOBJ_INCLUDE_PATH) -I$(STB_INCLUDE_PATH)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi 

shmulkan: main.cpp
	g++ $(CFLAGS) -o shmulkan main.cpp $(LDFLAGS)

.PHONY: test clean

test: shmulkan
	./shmulkan

clean:
	rm -f shmulkan
