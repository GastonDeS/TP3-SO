include Makefile.inc

SOURCES_CLIENT := $(wildcard cliente/*.c)
SOURCES_SERVER := $(wildcard serve/*.c)

all: client server

client:
	@echo "Creating files for client"
	@$(GCC) $(GCCFLAGS) $(SOURCES_CLIENT) -o client

server: 
	@echo "Creating files for server"
	@$(GCC) $(GCCFLAGS) -lm $(SOURCES_SERVER) $(OBJECTS_ASM) -o server

clean:
	@echo "Deleting objects"
	@rm -rf server quine client ./serve/*.o

.PHONY: all clean server client