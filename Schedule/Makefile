# Top level makefile, order of lower-level makes matters

WARN := -Wall
CXXFLAGS := ${WARN} 
JOBSERVER := -j 4

all:
	make -C DFS_Algorithm		$(JOBSERVER)
	make -C Kahn_Algorithm		$(JOBSERVER)

clean:
	make -C DFS_Algorithm		clean
	make -C Kahn_Algorithm		clean
