# Top level makefile, order of lower-level makes matters

WARN := -Wall
CXXFLAGS := ${WARN} 
JOBSERVER := -j 4

all:
	make -C method1		$(JOBSERVER)
	make -C method2		$(JOBSERVER)

clean:
	make -C method1		clean
	make -C method2		clean
