STB_INCLUDE_PATH = ~/tools/stb


CFLAGS = -std=c++17
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -I$(STB_INCLUDE_PATH)

shmulkan: main.cpp
	g++ $(CFLAGS) -o shmulkan main.cpp $(LDFLAGS)

.PHONY: test clean

test: shmulkan
	./shmulkan

clean:
	rm -f shmulkan
