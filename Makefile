CXX = g++
CC = gcc
CPPOBJS = src/ui.o
COBJS =
EDCXXFLAGS = -I ./ -I ./include/ -Wall -pthread $(CXXFLAGS)
EDCFLAGS = $(CFLAGS)
EDLDFLAGS := -lpthread -lm -lncurses $(LDFLAGS)
TARGET = ui.out

all: $(COBJS) $(CPPOBJS)
	$(CXX) $(EDCXXFLAGS) $(COBJS) $(CPPOBJS) -o $(TARGET) $(EDLDFLAGS)
	./$(TARGET)

%.o: %.cpp
	$(CXX) $(EDCXXFLAGS) -o $@ -c $<

%.o: %.c
	$(CC) $(EDCFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	$(RM) *.out
	$(RM) *.o
	$(RM) src/*.o

.PHONY: spotless

spotless:
	$(RM) drivers/*.o