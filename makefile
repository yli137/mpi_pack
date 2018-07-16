CC=/home/yli137/openmpi/ompi/after_config/bin/mpicc

all: mpi

mpi: mpi.c pack_test.o
	$(CC) -o mpi pack_test.o -O3 mpi.c

pack_test.o: pack_test.c
	$(CC) -c pack_test.c

clear:
	rm *.o
