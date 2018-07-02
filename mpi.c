#include</home/yicheng/Desktop/openmpi/ompi-master/ompi/include/mpi.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#define MIN_LENGTH 1024
#define MAX_LENGTH 1024*1024

void double_pack(int count){
	double *inbuf = (double*)malloc(sizeof(double)*count*10);
	double *obuf = (double*)malloc(sizeof(double)*(count * 10));
	int start = 0;
	double st, et;

	printf("This is mpi pack:\n");
	for(unsigned int i = 0; i < 10; i++){
		st = MPI_Wtime();
		MPI_Pack(inbuf, 10000, MPI_DOUBLE, obuf, sizeof(double) * count, &start, MPI_COMM_WORLD);
		et = MPI_Wtime();

		start++;
		if(i == 8)
			start = 0;
		printf("%d used time: %.7f mm\n", i, (et - st) * 1000);
	}

	free(inbuf);
	free(obuf);
}

void double_manual(int count){
	double *inbuf = (double*)malloc(sizeof(double) * count);
	double *obuf = (double*)malloc(sizeof(double) * count);
	int start = 0;
	double st, et;

	printf("This is manual pack double:\n");
	for(unsigned int i = 0; i < 10; i++){
		st = MPI_Wtime();
		for(unsigned int j = 0; j < 10000; j++){
			obuf[start] = inbuf[start];
			start++;
		}		
		et = MPI_Wtime();
		printf("%d used time: %.7f mm\n", i, (et - st) * 1000);
	}

	free(inbuf);
	free(obuf);
}

void do_pack(){
	double_pack(100000);
	printf("\n");
	double_manual(100000);
}

int
main(int argc, char **argv){
	int ierr, procid, numprocs, count = 0;
	double speed = 0;

	ierr = MPI_Init(&argc, &argv);
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &procid);
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	if(procid != 0){
		MPI_Finalize();
		exit(0);
	}

	do_pack();	

	MPI_Finalize();
	return 0;

}
