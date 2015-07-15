
CXX = /usr/bin/g++
CXX_FLAGS += -O3 -Wall
CXX_INC = -Iinc -I/usr/include/X11
CXX_LIB = -L/usr/lib/x86_64-linux-gnu -lX11


TARGET  = bin/helloX
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
INCLUDES = $(wildcard inc/*.h)


$(TARGET):$(OBJECTS)
	$(CXX) $(CXX_FLAGS)  $(OBJECTS)  $(CXX_LIB)  -o  $(TARGET)


src/%.o: src/%.cpp $(INCLUDES)
	$(CXX) -c $(CXX_FLAGS) $(CXX_INC) $< -o $@


clean:
	rm -f src/*.o $(TARGET)
	

