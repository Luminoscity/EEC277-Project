# Makefile for EEC277-Project
# for compiling outside of Visual Studio

CC = g++
CFLAGS = -g -Wall -Wno-unknown-pragmas -Werror -std=c++0x

LIBS += -lstdc++

ALL = TriangleTraversal
#ALL = TriangleTraversal genTris GrabTriangles

all: $(ALL)

genTris: genTris.cpp
	@echo "-------------------------------"
	@echo "*** Building $@ ***"
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "*** Complete! ***"
	@echo "-------------------------------"

GrabTriangles: GrabTriangles.cpp
	@echo "-------------------------------"
	@echo "*** Building $@ ***"
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "*** Complete! ***"
	@echo "-------------------------------"

TriangleTraversal: TriangleTraversal.cpp
	@echo "-------------------------------"
	@echo "*** Building $@ ***"
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "*** Complete! ***"
	@echo "-------------------------------"

# clean targets
clean:
	@echo "-------------------------------"
	@echo "*** Cleaning Files..."
	rm -rf *.o *.dSYM $(ALL)
	@echo "-------------------------------"
