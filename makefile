CC=/home/yicheng/Desktop/openmpi/ompi-master/after_config/bin/mpicc

all: mpi

mpi: mpi.c pack_test.o
	$(CC) -o mpi pack_test.o mpi.c

pack_test.o: pack_test.c
	$(CC) -c pack_test.c

clear:
	rm *.o
