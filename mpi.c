#include</home/yicheng/Desktop/openmpi/ompi-master/ompi/include/mpi.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#define MIN_LENGTH 1024
#define MAX_LENGTH 1024*1024

typedef struct buffer{
	double dv[10];
} buffer;

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

	MPI_Datatype type;
	MPI_Type_vector(1000, 1, 10, MPI_DOUBLE, &type );
	MPI_Type_commit(&type);

	double *buf = (double*)malloc(sizeof(double) * 10000);
	for(int i = 0; i < 10000; i++)
		buf[i] = i + 100;
	double *obuf = (double*)malloc(sizeof(double) * 10000);

	for(int i = 0; i < 20; i++)
		printf("%.2f ", buf[i]);
	
	double st, et;
	int position = 0;
	printf("\nUsing MPI_Pack:\n");
	for(int j = 0; j < 10; j++){
		st = MPI_Wtime();
		for(unsigned int i = 0; i < 1000; i++){
			MPI_Pack(buf, 1, MPI_DOUBLE, obuf, sizeof(double) * 10000, &i, MPI_COMM_WORLD);
			//position += sizeof(double);
		}
		et = MPI_Wtime();
		printf("%d Used time: %.7f mm\n", j, (et - st) * 1000);
	}

/*****
	printf("\nPack by for loop\n");
	for(int j = 0; j < 10; j++){
		int count = 0;
		st = MPI_Wtime();
		for(unsigned int i = 0; i < 10000; i++){
			obuf[count] = buf[count];
			count+=1;
		}

		et = MPI_Wtime();
		printf("%d Used time: %.7f mm\n", j, (et-st)*1000);
	}
*****/
	for(int i = 0; i < 20; i++)
		printf("%.2f ", obuf[i]);

	printf("\n");	
	MPI_Type_free(&type);
	MPI_Finalize();
	return 0;

}
