#
# Makefile for lab10
#

##############################################################
# Compiler and flags to use
##############################################################

CXX       = g++
CXXFLAGS  = -g -Wall -Werror
VPATH = ../Utils

##############################################################
# You shouldn't have to change anything below this line.
##############################################################


LAB10_EXE  = Huffman
LAB10_OBJS = Huffman.o

###

$(LAB10_EXE): $(LAB10_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(LAB10_OBJS)

clean:
	rm -f  $(LAB10_EXE) $(LAB10_OBJS)

###

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

###

Huffman.o: Huffman.cpp Huffman.h Utility.h Lkstack.h
