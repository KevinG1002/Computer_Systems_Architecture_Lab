/* 
Author: Kevin Golan
Student ID: 9942991
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int main() {

	
	

	/*Initialise list of file-strings. This will be used to iterate through the different trace files at hand, 
	and execute the cache simulation at different block sizes. 
	*/
	const char *file_names[2] = {"cross_correlation_trace_036.trc", "bubble_sort_trace_036.trc"};

	//Useful variables - pretty self-explanatory in terms of the purpose they serve. 

	int Cache_Line_ID;
	int Tag_Bits;
	int short list_of_masks [7] = {0x007F, 0x007E, 0x007C, 0x0078, 0x0070, 0x0060, 0x0040};
	char Read_OR_Write;
	


	//Start of Simulation: For loop; 2 iterations for 2 trace files. 
	for (int j=0; j<2; j++){
		printf("File at hand is: %s \n", file_names[j]);
		int Cache_Initial_Size = 256;
		float Percentage_Hits, Percentage_Write_Hits, Percentage_Read_Hits = 0;
		char s, n;
		
		//7 iterations for the cache performance simulations on the 7 possible configurations. [128, 64, 32, 16, 8, 4, 2]
		for (int i=1; i<8; i++){
			
			int Memory_Address;	

			Cache_Initial_Size = Cache_Initial_Size/2; //Used to define cache size for each iterations
			//Opening of target trace file defined by for-loop above. 
			FILE *in_file = fopen(file_names[j], "r");
			
			int Read_Count = 0;
			int Write_Count = 0;
			int Cache_Read_Hit=0;
			int Cache_Read_Miss=0;
			int Cache_Write_Hit=0;
			int Cache_Write_Miss=0;
				
			//Initialisation of Cache Memory and Validation Bits array. Each block has 16 bits (unsigned int short)
			unsigned int short Cache_Array[Cache_Initial_Size];
			int valid_bits[Cache_Initial_Size];

			for (int k=0; k<Cache_Initial_Size; k++){
				Cache_Array[k] = 0;
				valid_bits[k] = 0;
			}


			printf("Number of Cache Lines: %d \n", Cache_Initial_Size);
				
				while(fscanf(in_file, "%c%c%x%c", &Read_OR_Write, &s, &Memory_Address, &n) != EOF){
			
					//Number of reads, number of writes. 
					if (Read_OR_Write == 'R'){
						Read_Count++;
					}
					if (Read_OR_Write == 'W'){
						Write_Count++;
					}

					//Defining Cache Line ID and Tag Bits. 
					Cache_Line_ID = Memory_Address & list_of_masks[i-1];
					Cache_Line_ID = Cache_Line_ID >> (i-1);
					//Tag Bit identification method never changes. 
					Tag_Bits = Memory_Address & 0xFF80;
					Tag_Bits = Tag_Bits >> 7;

					//Cache Memory Logic when Empty
					if (valid_bits[Cache_Line_ID] == 0 && Read_OR_Write == 'W') {
						Cache_Array[Cache_Line_ID] = Tag_Bits;
						Cache_Write_Miss++;
						valid_bits[Cache_Line_ID] = 1;
					}
					else if (valid_bits[Cache_Line_ID] == 0 && Read_OR_Write == 'R') {
						Cache_Array[Cache_Line_ID] = Tag_Bits;
						Cache_Read_Miss++;
						valid_bits[Cache_Line_ID] = 1;
					}

					//Cache Memory Logic when non-empty. 
					else if (valid_bits[Cache_Line_ID] !=0 && Read_OR_Write == 'W' && Cache_Array[Cache_Line_ID] == Tag_Bits){
						Cache_Write_Hit++;
					}
					else if (valid_bits[Cache_Line_ID] !=0 && Read_OR_Write == 'W' && Cache_Array[Cache_Line_ID] != Tag_Bits){
						Cache_Array[Cache_Line_ID] = Tag_Bits;
						Cache_Write_Miss++;
					}

					else if (valid_bits[Cache_Line_ID] !=0 && Read_OR_Write == 'R' && Cache_Array[Cache_Line_ID] == Tag_Bits){
						Cache_Read_Hit++;
					}
					else if (valid_bits[Cache_Line_ID] !=0 && Read_OR_Write == 'R' && Cache_Array[Cache_Line_ID] != Tag_Bits){
						Cache_Array[Cache_Line_ID] = Tag_Bits;
						Cache_Read_Miss++;
					}

					
				}
			//Close file after each for loop iteration - then re-open it. 
			fclose(in_file);
			
			// Print statements hereunder show the performance and the metrics for each cache memory configuration. 
			
			Percentage_Hits = ((float)Cache_Read_Hit+Cache_Write_Hit)/(Read_Count+Write_Count);
			Percentage_Write_Hits = ((float)Cache_Write_Hit)/(Write_Count);
			Percentage_Read_Hits = ((float)Cache_Read_Hit)/(Read_Count);

			printf("Total number of read accesses to the external memory: %d \n", Read_Count);
			printf("Total number of write accesses to the external memory: %d \n", Write_Count);
			printf("\n");
			printf("Total number of Write to Cache Hits: %d \n", Cache_Write_Hit);
			printf("Total number of Write to Cache Misses: %d \n", Cache_Write_Miss);
			printf("\n");
			printf("Total number of Read from Cache Hits: %d \n", Cache_Read_Hit);
			printf("Total number of Read from Cache Misses: %d \n", Cache_Read_Miss);
			printf("\n");

			printf("Cache Memory Hit-rate: %f \n", Percentage_Hits*100);
			printf("Cache Memory Write Hit-rate: %f \n", Percentage_Write_Hits*100);
			printf("Cache Memory Read Hit-rate: %f \n", Percentage_Read_Hits*100);

			printf("\n");
			
			
		}

	printf("End of Cache Simulation on file: %s \n", file_names[j]);
	sleep(3);

	}

	
	sleep(3);

	printf("End of Cache Simulation of Bubble-Sort and Cross-Correlation files\n");
	
	return 0;
}
