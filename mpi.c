#include</home/yli137/openmpi/ompi/ompi/include/mpi.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void double_pack(int count){
	double *inbuf = (double*)malloc(sizeof(double)*count);
	double *obuf = (double*)malloc(sizeof(double)*count);

	int start = 0;
	double st, et;
	unsigned int i;

	for(i = 0; i < 10; i++){
		st = MPI_Wtime();
		MPI_Pack(inbuf, 100000, MPI_DOUBLE, obuf, sizeof(double) * count, &start, MPI_COMM_WORLD);
		et = MPI_Wtime();
		printf("%d used time: %.7f mm\n", i, (et - st) * 1000);
	}
	et = MPI_Wtime();

	free(inbuf);
	free(obuf);
}

void double_manual(int count){
	double *inbuf = (double*)malloc(sizeof(double) * count);
	double *obuf = (double*)malloc(sizeof(double) * count);
	int start = 0;
	double st, et;
	unsigned int i, j;

	for(i = 0; i < 10; i++){
		st = MPI_Wtime();
		for(j = 0; j < 100000; j++){
			obuf[start] = inbuf[start];
			start++;
		}		
		et = MPI_Wtime();
		printf("%d used time: %.7f mm\n", i, (et - st) * 1000);
	}

	free(inbuf);
	free(obuf);
}

void double_contig(int count){
	double st, et;
	int start = 0;
	double *buf = (double*)malloc(sizeof(double) * count);
	double *obuf = (double*)malloc(sizeof(double) * count);

	MPI_Datatype ddt;
	MPI_Type_contiguous(count/10, MPI_DOUBLE, &ddt);
	MPI_Type_commit(&ddt);

	unsigned int i;
	for(i = 0; i < 10; i++){
		st = MPI_Wtime();
		MPI_Pack(buf, 1, ddt, obuf, sizeof(double) * count, &start, MPI_COMM_WORLD);
		et = MPI_Wtime();
		printf("Used %.7f mm\n", (et - st) * 1000);
	}

	free(buf);
	free(obuf);
	MPI_Type_free(&ddt);
}

void double_vector(int count){
	int start = 0;
	double st, et;	

	unsigned int i;
	for(i = 0; i < 10; i++){
		double *buf = (double*)malloc(sizeof(double) * count);
		double *obuf = (double*)malloc(sizeof(double) * count);
		
		MPI_Datatype ddt;
		MPI_Type_vector(1, count, count, MPI_DOUBLE, &ddt);
		MPI_Type_commit(&ddt);

		st = MPI_Wtime();
		MPI_Pack(buf, 1, ddt, obuf, sizeof(double) * count, &start, MPI_COMM_WORLD);
		et = MPI_Wtime();
		printf("Used %.7f mm\n", (et - st) * 1000);

		MPI_Type_free(&ddt);
		free(buf);
		free(obuf);
	}
}

void try_contig(){
	double *buf = (double*)malloc(sizeof(double) * 50);
	int i = 0;
	for(; i < 50; i++){
		buf[i] = i;
	}

	for(i = 0; i < 50; i++)
		printf("%.f ", buf[i]);

	double *obuf = (double*)malloc(sizeof(double) * 50);

	MPI_Datatype ddt;
	MPI_Type_contiguous(1, MPI_DOUBLE, &ddt);

	printf("\n");
	int start = 0;
	MPI_Pack(buf, 1, ddt, obuf, sizeof(double) * 50, &start, MPI_COMM_WORLD);
	for(i = 0; i < 50; i++){
		printf("%.f ", obuf[i]);
	}

	MPI_Type_free(&ddt);
	free(buf);
	free(obuf);

}

void do_pack(){
	try_contig();

/***********
	printf("This is mpi pack only:\n");
	double_pack(1000000);

	printf("\nThis is mpi vector:\n");
	double_vector(1000000);
	
	printf("\nThis is mpi pack contig:\n");
	double_contig(1000000);

	printf("\nThis is manual pack:\n");
	double_manual(1000000);
**********/
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
