#
# Makefile for lab10
#

##############################################################
# Compiler and flags to use
##############################################################

CXX       = g++
CXXFLAGS  = -g -Wall -Werror -std=c++0x

##############################################################
# You shouldn't have to change anything below this line.
##############################################################


LAB10_EXE  = Editor
LAB10_OBJS = Editor.o

###

$(LAB10_EXE): $(LAB10_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(LAB10_OBJS)

clean:
	rm -f  $(LAB10_EXE) $(LAB10_OBJS)

###

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

###

Editor.o: Editor.cpp Editor.h Utility.h List.h Dlist.h String.h