CFLAGS = -std=c++17
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

vulkanshmulkan: main.cpp
	g++ $(CFLAGS) -o vulkanshmulkan main.cpp $(LDFLAGS)

.PHONY: test clean

test: vulkanshmulkan
	./vulkanshmulkan

clean:
	rm -f vulkanshmulkan
