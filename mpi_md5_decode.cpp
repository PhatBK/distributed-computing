#include<stdio.h>
#include<string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <openssl/md5.h>
#include<mpi.h>
char khong_gian_khoa[26]   = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
char khong_gian_khoa_1[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char khong_gian_khoa_2[10] = {'1','2','3','4','5','6','7','8','9'};
char bang1[3] = { 'a','b','c' };


//Ham ma hoa md5
void ma_hoa_MD5(const char *string, char *mdString) {
	unsigned char digest[16];
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, string, strlen(string));
	MD5_Final(digest, &ctx);
	
	for (int i = 0; i < 16; i++)
		sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);
}
//end hash
//Ham kiem tra 2 chuoi hash co khop nhau khong
bool so_sanh_hash(char *md1, char *md2) {
	for (int i = 0; i < 32; i++) {
		if (md1[i] != md2[i]) {
			return false;
		}
	}
	return true;
}
//end check
void print_one_string( char *output, int size,char *hash) {
	char hash_output[33];
	output[size]='\0';
	ma_hoa_MD5(output,hash_output);	
	if(so_sanh_hash(hash_output,hash)==true){
		printf("Tìm thấy password:");
		for(int i=0;i<size;i++){
			printf("%c",output[i]);
		}
		printf("\n");
		exit();
	}
}
void print_util(char set[],int size, int k, char * output, int count,char *hash){
	count++;
	if (count <= k) {
		for (int i = 0; i < size; i++) {
			output[count - 1] = set[i];
			print_util(set, size, k, output, count,hash);
			if (count == k) {
				print_one_string(output,k,hash);
			}
		}
	}
}
void print_string_all(char set[],char set_one[],const int size,int size_one,int k,char *hash) {
	char *output = (char *) malloc(k * sizeof(char));
	for (int i = 0; i < size_one; i++) {
		output[0] = set_one[i];
		print_util(set, size, k, output, 1,hash);
	}
	
}
//Ham chia du lieu cho cac tien trinh
int  chia_du_lieu(int so_tien_trinh,int rank,char *mang){

	int count = (26/(so_tien_trinh + 1));
	int index = count * rank;
	for(int i=0;i<count;i++){
		if(index>=26){
			break;
		}
		mang[i]=khong_gian_khoa[index];
		index++;
	}
	int temp1;
	if((26%so_tien_trinh>0) && (so_tien_trinh - rank == 1)){
		int temp=count;
		for(int j=0;j<(26%so_tien_trinh);j++){
			index;
			mang[temp]=khong_gian_khoa[index];
			temp++;
			index++;
		}
		temp1=temp;
		return temp1;
	}else{
		return count;
		
	}
}
// kết thúc hàm chia dữ liệu

//Ham xu ly cua rank0
void rank0(int so_tien_trinh,char *hash){
	char mang[27];
	int size_one=chia_du_lieu(so_tien_trinh,0,mang);
	for(int i=1;i<=7;i++){
		print_string_all(bang_chu_cai,mang,26,size_one,i,hash);	
	}
	
}
//end rank0


//Ham xu ly cua rank i
void ranki(int so_tien_trinh,int rank,char *hash){
	char mang[27];
	int size_one=chia_du_lieu(so_tien_trinh,rank,mang);
	for(int i=1; i<=7;i++){
		print_string_all(bang_chu_cai,mang,26,size_one,i,hash);	
	}
		
}
//end rank i

int main(int argc, char ** argv) {
	char mdString[33];
    int len_password;
    if(argc != 2) return -1;
    sscanf(argv[1], "%s", mdString);	
	MPI_Init(NULL,NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD,&world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
	if(world_rank==0){
		rank0(world_size,mdString);
	}else{
		ranki(world_size,world_rank,mdString);
	}
	MPI_Finalize();
	return 0;
}

