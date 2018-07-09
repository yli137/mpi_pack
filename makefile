CC=/home/yicheng/Desktop/openmpi/ompi-master/after_config/bin/mpicc

all: mpi

mpi: mpi.c
	$(CC) -o mpi mpi.c


