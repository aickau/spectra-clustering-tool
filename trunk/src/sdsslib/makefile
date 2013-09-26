# choose compiler
# gcc or g++
CC=g++

# define compiler flags
#
CFLAGS       = -fPIC -g -Wall -m64 -D_FILE_OFFSET_BITS=64  -fopenmp

# define linker flags
# 
LDFLAGS      = -shared

# define debug flags
#
DEBUGFLAGS	 = 

# define any directories containing header files other than /usr/include
# i.e. -I/home/mylib/include  -I../include
INCLUDES = 

# define any libraries to link into executable:
#   e.g. -lmylib -lm will link in libmylib.so and libm.so
LIBS = 


TARGET  = ../libsdss.so
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.cpp=.o)


all: $(TARGET)
	
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(INCLUDES) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LIBS)


.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(TARGET)


depend: $(SOURCES)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it