CFLAGS = -std=c++17
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

shmulkan: main.cpp
	g++ $(CFLAGS) -o shmulkan main.cpp $(LDFLAGS)

.PHONY: test clean

test: shmulkan
	./shmulkan

clean:
	rm -f shmulkan
