# Makefile to build lbstime.a library and driver routines 
#
# Andrew J. Pounds, Ph.D.
# Departments of Chemistry and Computer Science
# Mercer University
# Spring 2007 (last edit Spring 2022)
# 

F77 = gfortran    
CC  = g++ 
CFLAGS = -O3
FFLAGS = -O3

TIMINGLIBS =  -L./ -llbstime 
CLIBS = -lm

OBJS = cputime.o walltime.o  

all: dusty77 dusty95 dusty lib

cputime.o : cputime.cc   
	$(CC) $(CFLAGS) -c cputime.cc  

walltime.o : walltime.cc   
	$(CC) $(CFLAGS) -c walltime.cc  

######### F77

dusty77.o : dusty.f   
	$(F77) $(FFLAGS) -c dusty.f 
	mv dusty.o dusty77.o  

# Don't forget the -lstdc++
dusty77 : dusty77.o lib  $(OBJS) 
	$(F77) -o dusty77 dusty77.o  $(TIMINGLIBS) -lstdc++  


######## F95


dusty95.o : dusty.f95   
	$(F77) $(FFLAGS) -c dusty.f95
	mv dusty.o dusty95.o   

# Don't forget the -lstdc++
dusty95 : dusty95.o lib  $(OBJS) 
	$(F77) -o dusty95 dusty95.o  $(TIMINGLIBS) -lstdc++  


####### C

dusty.o : dusty.cc   
	$(CC) $(CFLAGS) -c dusty.cc   

# Don't forget the -lstdc++
dusty : dusty.o lib  $(OBJS) 
	$(CC) -o dusty dusty.o  $(TIMINGLIBS) -lstdc++  




# Default Targets for Cleaning up the Environment
clean :
	rm *.o
	rm *.a

pristine :
	rm *.o
	rm *.a
	touch *.cc *.f  
	test -f dusty && rm dusty
	test -f dusty77 && rm dusty77
	test -f dusty95 && rm dusty95	

ctags :
	ctags  *.cc *.f

# Target for making the library

lib: $(OBJS) 
	ar -rc liblbstime.a $(OBJS) 
	ranlib liblbstime.a
