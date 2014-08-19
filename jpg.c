#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jpg.h"

/*
 * Analyze a JPG file that contains Exif data.
 * If it is a JPG file, print out all relevant metadata and return 0.
 * If it isn't a JPG file, return -1 and print nothing.
 */


int checkj(int size, int i){
	if ((i >= size) || (i <0)) {
		return 1;
	}
	return 0;
}


int exif_method(int i,unsigned char* file, int s, int size){

	int pointer_offset_tiff = file[s+10] + (file[s+11] << 8) +  (file[s+12] << 16) + (file[s+13] << 24);


	//start of exif id 
	s = i+10+pointer_offset_tiff;

	int e_numoftagstructures = (file[s+1] << 8) + file[s];


	int e_counter = 0;
	int e_tag_id;
	int e_skip;


	int e_data_type;
	int e_numElements;
	int e_offset_tiff;
	int e_type_size;
	int e_list_value;

	while (e_counter < e_numoftagstructures) {
		//while (counter < 1) {
		if (checkj(size, s+3)) return -1;
		if (checkj(size, s+2)) return -1;
		e_tag_id = (file[s+3] << 8) + file[s+2];
		e_skip = 0;
		if (e_tag_id == 0x010D){
			printf("DocumentName: ");
		} else if (e_tag_id == 0x010E){
			printf("ImageDescription: ");
		} else if (e_tag_id == 0x010F){
			printf("Make: ");
		} else if (e_tag_id == 0x0110){
			printf("Model: ");
		} else if (e_tag_id == 0x0131){
			printf("Software: ");
		} else if (e_tag_id == 0x0132){
			printf("DateTime: ");
		} else if (e_tag_id == 0x013B){
			printf("Artist: ");
		} else if (e_tag_id == 0x013C){
			printf("HostComputer: ");
		} else if (e_tag_id == 0x8298){
			printf("Copyright: ");
		} else if (e_tag_id == 0xA004){
			printf("RelatedSoundFile: ");
		} else if (e_tag_id == 0x9003){
			printf("DateTimeOriginal: ");
		} else if (e_tag_id == 0x9004){
			printf("DateTimeDigitized: ");
		} else if (e_tag_id == 0x927C){
			printf("MakerNote: ");
			//e_skip = 1;
		} else if (e_tag_id == 0x9286){
			printf("UserComment: ");
		} else if (e_tag_id == 0xA420){
			printf("ImageUniqueID: ");
		} else {
			e_skip = 1;
		}

		// printf("\n%d", file[s+4]);
		// printf("\n%d", file[s+5]);
		// printf("\n%d", file[s+6]);
		// printf("\n%d", file[s+7]);
		// printf("\n%d", file[s+8]);
		// printf("\n%d", file[s+9]);
		// break;

		if (checkj(size, s+4)) return -1;
 		if (checkj(size, s+5)) return -1;
		if (checkj(size, s+6)) return -1;
		if (checkj(size, s+7)) return -1;
		if (checkj(size, s+8)) return -1;
		if (checkj(size, s+9)) return -1;
		if (checkj(size, s+10)) return -1;
		if (checkj(size, s+11)) return -1;
		if (checkj(size, s+12)) return -1;
		if (checkj(size, s+13)) return -1;



		if (e_skip == 0){
			e_data_type = (file[s+5] << 8) + file[s+4];
			e_numElements = file[s+6] + (file[s+7] << 8) +  (file[s+8] << 16) + (file[s+9] << 24);
			e_offset_tiff = file[s+10] + (file[s+11] << 8) +  (file[s+12] << 16) + (file[s+13] << 24);
			e_type_size = 0;
			e_list_value = 0;
			if( e_data_type == 0x01 || e_data_type == 0x02 || e_data_type == 0x07) { //data type of size 1 byte with <= 4 elements
				if(e_numElements <= 4) {
					e_list_value = e_offset_tiff;
				}
				else {
					e_type_size = 1;
				}
			}
			else if( (e_data_type == 0x03 || e_data_type == 0x08)){ //data type of size 2 bytes with <= 2 elements
				if(e_numElements <= 2) {
					e_list_value = e_offset_tiff;
				}
				else {
					e_type_size = 2;
				}
			}
			else if( (e_data_type == 0x04 || e_data_type == 0x09 || e_data_type == 0x0b)){ //data type of size 4 bytes with <= 1 element
				if(e_numElements <= 1) {
					e_list_value = e_offset_tiff;
				}
				else {
					e_type_size = 4;
				}
			} else if( (e_data_type == 0x05 || e_data_type == 0x0c || e_data_type == 0x0a)){ 
				e_type_size = 8;
			}

			if (!(e_type_size == 0)){
				int e_k = 0;
				int q = e_list_value;
				q = 0;
				int e_total = e_numElements * e_type_size;
				if(e_tag_id == 0x927C){ //Maker note
					if (checkj(size, i+10+e_offset_tiff)) return -1;
					printf("%.*s", e_numElements,file+i+10+e_offset_tiff);
				}
				else{
					if (e_tag_id == 0x9286){ // User Comments
						q = 8;
						e_total = e_total +1 ;
					} 
					for(e_k = q; e_k < e_total-1; e_k++){
						if (checkj(size, i+10+e_offset_tiff+e_k)) return -1;
						printf("%c", file[i+10+e_offset_tiff+e_k]);
					}
				}
				printf("\n");
			} else {
				int e_k = 0;
				int e_total = e_numElements-1;
				if (e_tag_id == 0x9286 || e_tag_id == 0x927C) {
					e_total = e_total+1;
				}
				for(e_k = 0; e_k < e_total; e_k++){
					if (checkj(size, s+10+e_k)) return -1;
					printf("%c", file[s+10+e_k]);
				}
				printf("\n");
			}


		}

		s = s+12;
		e_counter = e_counter + 1;
	}
	return 0;
}



 int analyze_jpg(FILE *f) {
    /* YOU WRITE THIS PART */

 	fseek(f, 0, SEEK_END);
 	int size = ftell(f);
 	fseek(f,0,SEEK_SET);
 	unsigned char* file = malloc(size);

 	fread(file, 1, size, f);

 	if (size <= 1){
 		return -1;
 	}


	// if file doesn't have first same 8 bytes as what PNG file should have, return -1
 	if (! ( (file[0] == 0xff) && (file[1] == 0xd8))) {
 		return -1;
 	}

 	int length = 0;
 	int i = 0;

 	for (i = 0; i < size;){
 		if (checkj(size, i+1)) return -1;
 		if (file[i+1] ==0xd9){
 			i = size;
 		} else if ( (file[i+1] >= 0xd0) && (file[i+1] <= 0xda) ){
 			int j = 2;
 			if (checkj(size, i+j)) return -1;
 			if (checkj(size, i+j+1)) return -1;
 			while (!((file[i+j] == 0xff) && (file[i+j+1] != 0x00))) {
 				j = j+1;
 				if (checkj(size, i+j)) return -1;
 				if (checkj(size, i+j+1)) return -1;
 			}
 			i = i+j;
		//	printf("super chunk");
 		} else if (file[i+1] != 0xe1){
 			if (checkj(size, i+2)) return -1;
 			if (checkj(size, i+3)) return -1;
 			length = (file[i+2] << 8) + (file[i+3]);
 			i = i + 2 + length;
		//	printf("standard chunk");
 		} else {

 			//printf("     %x      %x    %x      %x     ", file[i], file[i+1], file[i+4], file[i+5]);


			//TIFF

			//i +8 is the  first byte in TIFF File
 			if (checkj(size, i+14)) return -1;
 			if (checkj(size, i+15)) return -1;
 			if (checkj(size, i+16)) return -1;
 			if (checkj(size, i+17)) return -1;
 			int offset = (file[i+17] << 24) + (file[i+16] << 16) + (file[i+15] << 8) + file[i+14];


			//i+ 8 + offset = where 0th ifd starts


 			int s = i + 8 + offset + 2;


 			if (checkj(size, s+1)) return -1;
 			int numoftagstructures = (file[s+1] << 8) + file[s];


 			int counter = 0;
 			int tag_id;
 			int exif;
 			int skip;
 			while (counter < numoftagstructures) {
			//while (counter < 1) {
				if (checkj(size, s+3)) return -1;
 				tag_id = (file[s+3] << 8) + file[s+2];
 				exif = 0;
 				skip = 0;
 				if (tag_id == 0x010D){
 					printf("DocumentName: ");
 				} else if (tag_id == 0x010E){
 					printf("ImageDescription: ");
 				} else if (tag_id == 0x010F){
 					printf("Make: ");
 				} else if (tag_id == 0x0110){
 					printf("Model: ");
 				} else if (tag_id == 0x0131){
 					printf("Software: ");
 				} else if (tag_id == 0x0132){
 					printf("DateTime: ");
 				} else if (tag_id == 0x013B){
 					printf("Artist: ");
 				} else if (tag_id == 0x013C){
 					printf("HostComputer: ");
 				} else if (tag_id == 0x8298){
 					printf("Copyright: ");
 				} else if (tag_id == 0xA004){
 					printf("RelatedSoundFile: ");
 				} else if (tag_id == 0x9003){
 					printf("DateTimeOriginal: ");
 				} else if (tag_id == 0x9004){
 					printf("DateTimeDigitized: ");
 				} else if (tag_id == 0x927C){
 					//skip = 1;
 					printf("MakerNote: ");
 				} else if (tag_id == 0x9286){
 					printf("UserComment: ");
 				} else if (tag_id == 0xA420){
 					printf("ImageUniqueID: ");
 				} else if (tag_id == 0x8769){
 					exif = 1;
 					int exifreturn = exif_method(i, file, s, size);
 					if (exifreturn == -1) return -1;
 				} else {
 					skip = 1;
 				}

				// printf("\n%d", file[s+4]);
				// printf("\n%d", file[s+5]);
				// printf("\n%d", file[s+6]);
				// printf("\n%d", file[s+7]);
				// printf("\n%d", file[s+8]);
				// printf("\n%d", file[s+9]);
				// break;

 				if ((skip == 0) && (exif == 0) ){
 					if (checkj(size, s+4)) return -1;
 					if (checkj(size, s+5)) return -1;
 					if (checkj(size, s+6)) return -1;
 					if (checkj(size, s+7)) return -1;
 					if (checkj(size, s+8)) return -1;
 					if (checkj(size, s+9)) return -1;
 					if (checkj(size, s+10)) return -1;
 					if (checkj(size, s+11)) return -1;
 					if (checkj(size, s+12)) return -1;
 					if (checkj(size, s+13)) return -1;

 					int data_type = (file[s+5] << 8) + file[s+4];
 					int numElements = file[s+6] + (file[s+7] << 8) +  (file[s+8] << 16) + (file[s+9] << 24);
 					int offset_tiff = file[s+10] + (file[s+11] << 8) +  (file[s+12] << 16) + (file[s+13] << 24);
 					int type_size = 0;
 					int list_value = 0;
					if( data_type == 0x01 || data_type == 0x02 || data_type == 0x07) { //data type of size 1 byte with <= 4 elements
						if(numElements <= 4) {
							list_value = offset_tiff;
						}
						else {
							type_size = 1;
						}
					}
					else if( (data_type == 0x03 || data_type == 0x08)){ //data type of size 2 bytes with <= 2 elements
						if(numElements <= 2) {
							list_value = offset_tiff;
						}
						else {
							type_size = 2;
						}
					}
					else if( (data_type == 0x04 || data_type == 0x09 || data_type == 0x0b)){ //data type of size 4 bytes with <= 1 element
						if(numElements <= 1) {
							list_value = offset_tiff;
						}
						else {
							type_size = 4;
						}
					} else if( (data_type == 0x05 || data_type == 0x0c || data_type == 0x0a)){ 
						type_size = 8;
					}

					//printf("\n   tag_id = %x          data type = %x               num elements = %x        type_size = %x     \n", tag_id, data_type, numElements, type_size);

					if (!(type_size == 0)){
						int q = list_value;
						q = 0;
						int k = 0;
						int total = numElements * type_size -1;
						if(tag_id == 0x927C){
							if (checkj(size, i+10+offset_tiff)) return -1;
							printf("%.*s", numElements, file+i+10+offset_tiff);
						}
						else{
							if (tag_id == 0x9286){
								q = 8;
								total = total +1;
							} 
							for(k = q; k < total; k++){
							//for(e_k = 0; e_k <1; e_k++){
								if (checkj(size, i+10+offset_tiff+k)) return -1;
								printf("%c", file[i+10+offset_tiff+k]);
							}
						}
						printf("\n");
					} else {
						int k = 0;
						int total = numElements -1;
						if(tag_id == 0x9286 || tag_id == 0x927C){
							total = total +1;
						}
						for(k = 0; k < total; k++){
							if (checkj(size, s+10+k)) return -1;
							printf("%c", file[s+10+k]);
						}
						printf("\n");
					}

				}
				s = s+12;
				counter = counter + 1;
			}

			i = size;

		}
	}
	return 0;

}