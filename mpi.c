#include</home/yicheng/Desktop/openmpi/ompi-master/ompi/include/mpi.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

void double_pack(int count){
	double *inbuf = (double*)malloc(sizeof(double)*count);
	double *obuf = (double*)malloc(sizeof(double)*count);

	int start = 0;
	double st, et;
	st = MPI_Wtime();
	MPI_Pack(inbuf, count, MPI_DOUBLE, obuf, sizeof(double) * count, &start, MPI_COMM_WORLD);
	et = MPI_Wtime();
	printf("Used time: %.7f mm\n", (et - st) * 1000);
	et = MPI_Wtime();

	free(inbuf);
	free(obuf);
}

void double_pack_10(int count){
	double *inbuf = (double*)malloc(sizeof(double) * count);
	int i, j;
	double *obuf = (double*)malloc(sizeof(double) * count);

	int start = 0;
	double st, et;
	st = MPI_Wtime();
	for(i = 0; i < count; i++){
		MPI_Pack(inbuf, 1, MPI_DOUBLE, obuf, sizeof(double) * count, &start, MPI_COMM_WORLD);
	}
	et = MPI_Wtime();
	printf("Used: %.7f mm\n", (et - st) * 1000);

	free(inbuf);
	free(obuf);
}

void double_manual(int count){
	double *inbuf = (double*)malloc(sizeof(double) * count);
	double *obuf = (double*)malloc(sizeof(double) * count);
	double st, et;
	unsigned int i, j;

	st = MPI_Wtime();
	for(j = 0; j < count; j++){
		obuf[j] = inbuf[j];
	}		
	et = MPI_Wtime();
	printf("Used time: %.7f mm\n", (et - st) * 1000);

	free(inbuf);
	free(obuf);
}

void double_contig(int count){
	double st, et;
	int start = 0;
	double *buf = (double*)malloc(sizeof(double) * count);
	double *obuf = (double*)malloc(sizeof(double) * count);

	MPI_Datatype ddt;
	MPI_Type_contiguous(count, MPI_DOUBLE, &ddt);
	MPI_Type_commit(&ddt);

	st = MPI_Wtime();
	MPI_Pack(buf, 1, ddt, obuf, sizeof(double) * count, &start, MPI_COMM_WORLD);
	et = MPI_Wtime();
	printf("Used %.7f mm\n", (et - st) * 1000);

	free(buf);
	free(obuf);
	MPI_Type_free(&ddt);
}

void double_vector(int count){
	int start = 0;
	double st, et;	
	MPI_Datatype ddt;
	MPI_Type_vector(1, count, count, MPI_DOUBLE, &ddt);
	MPI_Type_commit(&ddt);

	unsigned int i;
	//	for(i = 0; i < 10; i++){
	double *buf = (double*)malloc(sizeof(double) * count);
	double *obuf = (double*)malloc(sizeof(double) * count);

	st = MPI_Wtime();
	MPI_Pack(buf, 1, ddt, obuf, sizeof(double) * count, &start, MPI_COMM_WORLD);
	et = MPI_Wtime();
	printf("Used %.7f mm\n", (et - st) * 1000);

	free(buf);
	free(obuf);
	//	}

	MPI_Type_free(&ddt);
}

void do_pack(){
	//	try_contig();
	unsigned int i;

	printf("This is mpi pack only:\n");
	for(i = 0; i < 10; i++)
		double_pack(1000000);

	printf("\nThis is mpi vector:\n");
	for(i = 0; i < 10; i++)
		double_vector(1000000);

	printf("\nThis is mpi pack contig:\n");
	for(i = 0; i < 10; i++)
		double_contig(1000000);

	printf("\nThis is manual pack:\n");
	for(i = 0; i < 10; i++)
		double_manual(1000000);
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
