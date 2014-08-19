#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>
#include "png.h"

/*
 * Analyze a PNG file.
 * If it is a PNG file, print out all relevant metadata and return 0.
 * If it isn't a PNG file, return -1 and print nothing.
 */

int check(int size, int i){
	if ((i >= size) || (i <0)) {
		return 1;
	}
	return 0;
}

uLong compute_checksum(unsigned char* buf, unsigned int length) {
  uLong crc = crc32(0L, Z_NULL, 0);
  return crc32(crc, buf, length);
}

int analyze_png(FILE *f) {
    /* YOU WRITE THIS PART */
   
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f,0,SEEK_SET);


	if (size <= 7) {
		return -1;
	}

	unsigned char* file = malloc(size);

	fread(file, 1, size, f);


	// if file doesn't have first same 8 bytes as what PNG file should have, return -1
	if (! ( (file[0] == 0x89) && (file[1] == 0x50) && (file[2] == 0x4e) && (file[3] == 0x47) && (file[4] == 0x0d)  && (file[5] == 0x0a) && (file[6] == 0x1a) && (file[7] == 0x0a) )){
		return -1;
	}

	int length = 0;
	int i = 0;
	for (i = 8; i < size;){

		if (check(size, i+3)) return -1;
		if (check(size, i+2)) return -1;
		if (check(size, i+1)) return -1;
		if (check(size, i+0)) return -1;
		if (check(size, i-1)) return -1;

		length = file[i+3] + (file[i+2] << 8) + (file[i+1] << 16) + (file[i] << 24);

		if (check(size, i+4)) return -1;
		if (check(size, i+5)) return -1;
		if (check(size, i+6)) return -1;
		if (check(size, i+7)) return -1;

		if ( (file[i+4] ==  0x74) && (file[i+5] ==  0x45) && (file[i+6] ==  0x58) && (file[i+7] ==  0x74)) {
			int j = 0;
			for (j = 1; j <= length; j++) {
				if (check(size, i+7+j)) return -1;
				if(file[i+7+j] == 0x00){
					printf(": ");
				}
				else{
					printf("%c", file[i+7+j]);
				}
			}
			printf("\n");
		} else if ( (file[i+4] ==  0x7a) && (file[i+5] ==  0x54) && (file[i+6] ==  0x58) && (file[i+7] ==  0x74)){
			int j = 0;
			int count = 0;
			if (check(size, i+7+j)) return -1;
			for (j = 1;  (!(file[i+j+7] == 0x00 )); j++) {
				printf("%c", file[i+j+7]);
				count = count+1;
			}
			printf(": ");
			count= count+3;


			int newversion = Z_BUF_ERROR;
			uLongf dest_len = 0;
			int datalength = 1000;
			while(newversion == Z_BUF_ERROR){
				dest_len = datalength;
				unsigned char* dest = (unsigned char *) calloc(dest_len, sizeof(unsigned char));
				unsigned char* src = file+i+count+7;
				uLong src_len = (length - count) + 1;
				newversion = uncompress((Bytef *) dest, &dest_len, (Bytef *) src, src_len);
				datalength = datalength *2;
				if (newversion != Z_BUF_ERROR){
					int z = 0;
					for (z = 0; z < dest_len; z++){
						printf("%c", dest[z]);
					}
				}
				free(dest);
			}
			j = j  + newversion;
			printf("\n");
		} else if ( (file[i+4] ==  0x74) && (file[i+5] ==  0x49) && (file[i+6] ==  0x4d) && (file[i+7] ==  0x45) ){
			if (check(size, i+8)) return -1;
			if (check(size, i+9)) return -1;
			if (check(size, i+10)) return -1;
			if (check(size, i+11)) return -1;
			if (check(size, i+12)) return -1;
			if (check(size, i+13)) return -1;
			if (check(size, i+14)) return -1;
			if (length != 0x7) return -1;
			int year = (file[i+8] << 8) + (file[i+9]);
			int month = file[i+10];
			int day = file[i+11];
			int hour = file[i+ 12];
			int minute = file[i+13];
			int second = file[i+14];
			printf("Timestamp: %d/%d/%d %d:%d:%d" , month, day, year, hour, minute, second);
			printf("\n");
		}
		if (check(size, i+8+length)) return -1;
		if (check(size, i+9+length)) return -1;
		if (check(size, i+10+length)) return -1;
		if (check(size, i+11+length)) return -1;
		uLong checksum = ((uLong)file[i + 8 + length] << 24) + ((uLong)file[i + 9 + length] << 16) + ((uLong)file[i + 10 + length] << 8) + ((uLong)file[i + 11 + length]);
		uLong crcCheck = compute_checksum((Bytef*)(file+i+4), length+4);
		if (checksum!=crcCheck){
			return -1;
		}
		i = i + length + 12;
	}
	free(file);
	return 0;
}