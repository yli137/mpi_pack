#include</home/yli137/openmpi/ompi/ompi/include/mpi.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdalign.h>
#include"pack_test.h"

void do_pack(){
	//tested contig data
	//found contig data for mpi pack is simply just memcpy
	//hardly anything to optimize
	
	/************
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
	*************/

	//from here are non-contig data test
	/********************
	int i;
	printf("Two cache line per one double using MPI_vector\n");
	for(i = 0; i < 11; i++){
		double_noncontig_pack_1c(1000000, i);
	}

	printf("\nManually packing 1 double every 2 cache line\n");
	for(i = 0; i < 11; i++){
		double_noncontig_pack_manual_1c(1000000, i);
	}

	printf("\n1 double per cache line using MPI_vector\n");
	for(i = 0; i < 11; i++)
		double_noncontig_pack_2c(1000000, i);

	printf("\n1 double per cache line manually\n");
	for(i = 0; i < 11; i++)
		double_noncontig_pack_manual_2c(1000000, i);

	printf("\n1 double per 3 cache line\n");
	for(i = 0; i < 11; i++)
		double_noncontig_pack_3c(1000000, i);

	printf("\n1 double per 3 cache line manually\n");
	for(i = 0; i < 11; i++)
		double_noncontig_pack_manual_3c(1000000, i);

	*********************/

	/*****************
	int i, j;
	for(i = 1; i < 5; i++){
		printf("\n1 double per %d cache line\n", i);
		for(j = 0; j < 11; j++)
			double_noncontig_pack_c(1000000, j, i);
	}

	for(i = 1; i < 5; i++){
		printf("\nManually 1 double per %d cache line\n", i);
		for(j = 0; j < 11; j++)
			double_noncontig_pack_manual_c(1000000, j, i);
	}

	for(i = 1; i < 5; i++){
		printf("\n2 double per %d cache line\n", i);
		for(j = 0; j < 11; j++)
			num_double_noncontig_pack_c(1000000, j, i, 2);
	}

	for(i = 1; i < 5; i++){
		printf("\nManually 2 double per %d cache line\n", i);
		for(j = 0; j < 11; j++)
			two_double_noncontig_pack_manual_c(1000000, j, i);
	}
	************/

	int i, j, z;
	double average, out, med;
	//number of doubles
	/**********
	for(i = 1; i < 5; i++){
		//number of cache line
		for(j = 1; j < 4; j++){
			printf("%d doubles in each cache line\n", i);
			printf("%d cache lines\n", j);

			out = 0;
			//number of repetition
			for(z = 0; z < 21; z++){
				med = num_double_noncontig_pack_c(1000000, z, j, i);
				if(med != 0)
					out += med;
			}
			printf("Average: %.7f ms\n", out / 10);
			printf("\n");
		}
		printf("\n");
	}
*******/
	printf("\nManual test:\n");
	for(i = 1; i < 9; i++){
		//number of cache line
		for(j = 1; j < 4; j++){
			printf("%d doubles in each cache line\n", i);
			printf("%d cache lines\n", j);

			out = 0;
			//number of repetition
			for(z = 0; z < 21; z++){
				med = num_double_noncontig_pack_manual_c(1000000, z, j, i);
				if(med != 0)
					out += med;
			}
			printf("Average: %.7f ms\n", out / 20);
			printf("\n");
		}
		printf("\n");
	}

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
