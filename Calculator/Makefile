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


LAB10_EXE  = Calculator
LAB10_OBJS = Calculator.o

###

$(LAB10_EXE): $(LAB10_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(LAB10_OBJS)

clean:
	rm -f  $(LAB10_EXE) $(LAB10_OBJS)

###

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

###

Calculator.o: Calculator.cpp Calculator.h Utility.h Lkstack.h
