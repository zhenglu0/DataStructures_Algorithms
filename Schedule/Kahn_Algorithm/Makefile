#
# Makefile for lab10
#

##############################################################
# Compiler and flags to use
##############################################################

CXX       = g++
CXXFLAGS  = -g -Wall 
#-Werror

##############################################################
# You shouldn't have to change anything below this line.
##############################################################


LAB10_EXE  = Range_course
LAB10_OBJS = Range_course.o

###

$(LAB10_EXE): $(LAB10_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(LAB10_OBJS)

clean:
	rm -f  $(LAB10_EXE) $(LAB10_OBJS)

###

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

###

Range_course.o: Range_course.cpp Range_course.h Utility.h Lk_list.h Adjacency_lists.h Sq_queue.h
