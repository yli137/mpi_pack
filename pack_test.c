#include</home/yli137/openmpi/ompi/ompi/include/mpi.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdalign.h>
#include "pack_test.h"

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

	double *buf = (double*)malloc(sizeof(double) * count);
	double *obuf = (double*)malloc(sizeof(double) * count);

	st = MPI_Wtime();
	MPI_Pack(buf, 1, ddt, obuf, sizeof(double) * count, &start, MPI_COMM_WORLD);
	et = MPI_Wtime();
	printf("Used %.7f mm\n", (et - st) * 1000);

	free(buf);
	free(obuf);

	MPI_Type_free(&ddt);
}

void double_noncontig_pack_2c(int count, int num){
	//pick one double every two cache line
	//for cluster one cache line = 64 bytes = 8 doubles
	//one double every 16 doubles
	
	double *inbuf = (double*)malloc(sizeof(double) * count * 16);
	int i = 0;
	for(i; i < count * 16; i++)
		inbuf[i] = (double)i;

	double *obuf = (double*)malloc(sizeof(double) * count);

	MPI_Datatype ddt;
	MPI_Type_vector(count, 1, 16, MPI_DOUBLE, &ddt);
	MPI_Type_commit(&ddt);

	double st, et;
	int position = 0;
	st = MPI_Wtime();
	MPI_Pack(inbuf, 1, ddt, obuf, sizeof(double) * count, &position, MPI_COMM_WORLD);
	et = MPI_Wtime();
	//if(num != 0)
		printf("%d used %.7fms\n", num, (et - st) * 1000);

	MPI_Type_free(&ddt);
	free(inbuf);
	free(obuf);
}

void double_noncontig_pack_manual_2c(int count, int num){
	double *inbuf = (double*)malloc(sizeof(double) * count * 16);
	int i = 0, j;
	for(i; i < count * 16; i++)
		inbuf[i] = (double)i;
	double *obuf = (double*)malloc(sizeof(double) * count);

	double st, et;
	j = 0;
	
	st = MPI_Wtime();
	for(i = 0; i < count * 16; i+=16){
		obuf[j] = inbuf[i];
		j++;
	}
	et = MPI_Wtime();
	printf("%d used %.7fms\n", num, (et - st) * 1000);
	
	free(inbuf);
	free(obuf);
}

void double_noncontig_pack_1c(int count, int num){
	double *inbuf = (double*)malloc(sizeof(double) * count * 8);
	int i = 0; 
	for(i; i < count * 8; i++)
		inbuf[i] = (double)i;
	double *obuf = (double*)malloc(sizeof(double) * count);

	MPI_Datatype ddt;
	MPI_Type_vector(count, 1, 8, MPI_DOUBLE, &ddt);
	MPI_Type_commit(&ddt);

	double st, et;
	int position = 0;
	st = MPI_Wtime();
	MPI_Pack(inbuf, 1, ddt, obuf, sizeof(double) * count, &position, MPI_COMM_WORLD);
	et = MPI_Wtime();
	printf("%d used %.7fms\n", num, (et - st) * 1000);

	MPI_Type_free(&ddt);
	free(inbuf);
	free(obuf);
}

void double_noncontig_pack_manual_1c(int count, int num){
	double *inbuf = (double*)malloc(sizeof(double) * count * 8);
        int i = 0, j;
        for(i; i < count * 8; i++)
                inbuf[i] = (double)i;
        double *obuf = (double*)malloc(sizeof(double) * count);

        double st, et;
        j = 0;

        st = MPI_Wtime();
        for(i = 0; i < count * 8; i+=8){
                obuf[j] = inbuf[i];
                j++;
        }
        et = MPI_Wtime();
        printf("%d used %.7fms\n", num, (et - st) * 1000);

        free(inbuf);
        free(obuf);

}

void double_noncontig_pack_3c(int count, int num){
        double *inbuf = (double*)malloc(sizeof(double) * count * 24);
        int i = 0;
        for(i; i < count * 24; i++)
                inbuf[i] = (double)i;
        double *obuf = (double*)malloc(sizeof(double) * count);

        MPI_Datatype ddt;
        MPI_Type_vector(count, 1, 24, MPI_DOUBLE, &ddt);
        MPI_Type_commit(&ddt);

        double st, et;
        int position = 0;
        st = MPI_Wtime();
        MPI_Pack(inbuf, 1, ddt, obuf, sizeof(double) * count, &position, MPI_COMM_WORLD);
        et = MPI_Wtime();
        printf("%d used %.7fms\n", num, (et - st) * 1000);

        MPI_Type_free(&ddt);
        free(inbuf);
        free(obuf);
}

void double_noncontig_pack_manual_3c(int count, int num){
        double *inbuf = (double*)malloc(sizeof(double) * count * 24);
        int i = 0, j;
        for(i; i < count * 24; i++)
                inbuf[i] = (double)i;
        double *obuf = (double*)malloc(sizeof(double) * count);

        double st, et;
        j = 0;

        st = MPI_Wtime();
        for(i = 0; i < count * 24; i+=24){
                obuf[j] = inbuf[i];
                j++;
        }
        et = MPI_Wtime();
        printf("%d used %.7fms\n", num, (et - st) * 1000);

        free(inbuf);
        free(obuf);

}

void double_noncontig_pack_c(int count, int num, int c){
        double *inbuf = (double*)malloc(sizeof(double) * count * 8 * c);
        int i = 0;
        for(i; i < count * 8 * c; i++)
                inbuf[i] = (double)i;
        double *obuf = (double*)malloc(sizeof(double) * count);

        MPI_Datatype ddt;
        MPI_Type_vector(count, 1, 8 * c, MPI_DOUBLE, &ddt);
        MPI_Type_commit(&ddt);

        double st, et;
        int position = 0;
        st = MPI_Wtime();
        MPI_Pack(inbuf, 1, ddt, obuf, sizeof(double) * count, &position, MPI_COMM_WORLD);
        et = MPI_Wtime();
        printf("%d used %.7fms\n", num, (et - st) * 1000);

        MPI_Type_free(&ddt);
        free(inbuf);
        free(obuf);
}

void double_noncontig_pack_manual_c(int count, int num, int c){
        double *inbuf = (double*)malloc(sizeof(double) * count * 8 * c);
        int i = 0, j;
        for(i; i < count * 8 * c; i++)
                inbuf[i] = (double)i;
        double *obuf = (double*)malloc(sizeof(double) * count);

        double st, et;
        j = 0;

        st = MPI_Wtime();
        for(i = 0; i < count * 8 * c; i+=8 * c){
                obuf[j] = inbuf[i];
                j++;
        }
        et = MPI_Wtime();
        printf("%d used %.7fms\n", num, (et - st) * 1000);

        free(inbuf);
        free(obuf);

}

//num is number of repetition
//c is the number of the cache line
//n is the number of doubles taken from each cache line
double num_double_noncontig_pack_c(int count, int num, int c, int n){
        double *inbuf = (double*)malloc(sizeof(double) * count * 8 * c);
        int i = 0;
        for(i; i < count * 8 * c; i++)
                inbuf[i] = (double)i;
        double *obuf = (double*)malloc(sizeof(double) * count * n);

        MPI_Datatype ddt;
        MPI_Type_vector(count, n, 8 * c, MPI_DOUBLE, &ddt);
        MPI_Type_commit(&ddt);

        double st, et;
        int position = 0;
        st = MPI_Wtime();
        MPI_Pack(inbuf, 1, ddt, obuf, sizeof(double) * count * n, &position, MPI_COMM_WORLD);
        et = MPI_Wtime();

        MPI_Type_free(&ddt);
        free(inbuf);
        free(obuf);

	if(num == 0)
		return 0;
	return (et - st)*1000;
}

double num_double_noncontig_pack_manual_c(int count, int num, int c, int n){
        double *inbuf = (double*)malloc(sizeof(double) * count * 8 * c);
        int i = 0, j, z;
        for(i; i < count * 8 * c; i++)
                inbuf[i] = (double)i;
        double *obuf = (double*)malloc(sizeof(double) * count * n);

        double st, et;
        j = 0;

        st = MPI_Wtime();
        for(i = 0; i < count * 8 * c; i+=8 * c){
		for(z = i; z < i+n; z++){
			obuf[j] = inbuf[z];
		}
		j+=n;
        }
        et = MPI_Wtime();
	printf("%d used %.7fms\n", num, (et - st) * 1000);

        free(inbuf);
        free(obuf);

	if(num == 0)
		return 0;
	return (et - st)*1000;
}















