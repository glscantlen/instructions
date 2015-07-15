
CC = /usr/bin/gcc
CC_FLAGS += -O3 -Wall
CC_INC = -Iinc


TARGET  = bin/hello
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
INCLUDES = $(wildcard inc/*.h)


$(TARGET):$(OBJECTS)
	$(CC) $(CC_FLAGS)  $(OBJECTS)  -o  $(TARGET)


src/%.o: src/%.c $(INCLUDES)
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@


clean:
	rm -f src/*.o $(TARGET)
	

