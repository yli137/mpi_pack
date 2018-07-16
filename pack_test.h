#ifndef _PACKTEST_
#define _PACKTEST_

void double_pack(int count);
void double_pack_10(int count);
void double_manual(int count);
void double_contig(int count);
void double_vector(int count);

void double_noncontig_pack_2c(int count, int num);
void double_noncontig_pack_manual_2c(int count, int num);
void double_noncontig_pack_1c(int count, int num);
void double_noncontig_pack_manual_1c(int count, int num);
void double_noncontig_pack_3c(int count, int num);
void double_noncontig_pack_manual_3c(int count, int num);

void double_noncontig_pack_c(int count, int num, int c);
void double_noncontig_pack_manual_c(int count, int num, int c);

double num_double_noncontig_pack_c(int count, int num, int c, int n);
double num_double_noncontig_pack_manual_c(int count, int num, int c, int n);


#endif
