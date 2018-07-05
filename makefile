CC=/home/yli137/openmpi/ompi/after_config/bin/mpicc

all: mpi

mpi: mpi.c
	$(CC) -o mpi mpi.c


