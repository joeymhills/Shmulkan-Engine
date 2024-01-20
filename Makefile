STB_INCLUDE_PATH = /home/joey/tools/stb
TINYOBJ_INCLUDE_PATH = /home/joey/tools/tinyobjloader

CFLAGS = -std=c++17 -O3 -I$(TINYOBJ_INCLUDE_PATH) -I$(STB_INCLUDE_PATH)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi 

SRCS = main.cpp Instance.cpp Debug.cpp Utility.cpp Device.cpp Swapchain.cpp

shmulkan: main.cpp
	g++ $(CFLAGS) -o shmulkan $(SRCS) $(LDFLAGS)

.PHONY: test clean

test: shmulkan
	./shmulkan

clean:
	rm -f shmulkan
