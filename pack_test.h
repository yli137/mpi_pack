#ifndef _PACKTEST_
#define _PACKTEST_

void double_pack(int count);
void double_pack_10(int count);
void double_manual(int count);
void double_contig(int count);
void double_vector(int count);

void double_noncontig_pack(int count, int num);
void double_noncontig_pack_manual(int count, int num);
void double_noncontig_pack_1c(int count, int num);
void double_noncontig_pack_manual_1c(int count, int num);

#endif
