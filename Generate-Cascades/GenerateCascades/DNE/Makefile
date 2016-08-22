#
# Makefile for non-Microsoft compilers
#

## Linux  (uncomment the 2 lines below for compilation on Linux)
CXXFLAGS += -std=c++98 -Wall -ggdb
LDFLAGS +=

## CygWin (uncomment the 2 lines below for compilation on CygWin)
#CXXFLAGS += -Wall
#LDFLAGS +=

MAIN = DNS

all: $(MAIN) DNS GenNets

opt: CXXFLAGS += -O4
opt: LDFLAGS += -O4
opt: $(MAIN) DNS 
#GenNets

# COMPILE
$(MAIN): $(MAIN).cpp Snap.o
	g++ $(LDFLAGS) -o DNS $(MAIN).cpp Cascade.cpp GenCascs.cpp Sampling.cpp Criteria.cpp Snap.o -I./../glib -I./../snap
	
GenNets: GenNets.cpp Snap.o 
	g++ $(LDFLAGS) -o GenNets GenNets.cpp Snap.o -I./../glib -I./../snap

#GenCascs: GenCascs.cpp Snap.o
#	g++ $(LDFLAGS) -o GenCascs GenCascs.cpp Cascade.cpp Snap.o -I./../glib -I./../snap


Snap.o:
	g++ -c $(CXXFLAGS)  ./../snap/Snap.cpp -I./../glib -I./../snap

clean:
	rm -f  $(MAIN) Snap.o  $(MAIN).exe
