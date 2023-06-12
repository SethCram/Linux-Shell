# Seth Cram
# 10/4/2021
# hw2: Linux Shell Framework
# File: makefile
#
# notes:
#  make files need to be name 'Makefile' or 'makefile' to allow make to find
#  include the header files as dependencies to recompile them w/ changed
# description:
#  file to create the executable 'msh' and remove it

cc = gcc
cflags = -Wall -g
tgts = msh 
deps = SethShell.h

all:
	make msh

%.o: %.c $(deps)
	$(cc) -c -o $@ $< $(cflags)

msh: SethShell_Builtins.o SethShell.o
	$(cc) -o msh SethShell_Builtins.o SethShell.o


clean:
	rm $(tgts)
