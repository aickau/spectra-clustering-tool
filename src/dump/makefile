# choose compiler
# gcc or g++
CC=g++

# define compiler flags
#
CFLAGS       = -fPIC -g -Wall -m64 -D_FILE_OFFSET_BITS=64  -fopenmp

# define linker flags
# -Wl,-rpath=/mnt/hgfs/vmshare/mpitest
LDFLAGS      = -L../ -Wl,-rpath=/mnt/hgfs/vmshare/sdss/dump

# define debug flags
#
DEBUGFLAGS	 = 

# define any directories containing header files other than /usr/include
# i.e. -I/home/mylib/include  -I../include
INCLUDES = -I../  -I../3rdparty/api -I../sdsslib 


# define any libraries to link into executable:
#   e.g. -lmylib -lm will link in libmylib.so and libm.so
LIBS = libsdss.so


TARGET  = dump
SOURCES = $(wildcard *.cpp)
ASM		= $(wildcard *.S)
HEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.cpp=.o) $(ASM:.S=.o)


all: $(TARGET)
	
#link:
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(INCLUDES) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LIBS)

#compile:
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(TARGET)

test:
	@echo $(OBJECTS)

depend: $(SOURCES)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it