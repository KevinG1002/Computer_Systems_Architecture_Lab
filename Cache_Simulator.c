/* 
Author: Kevin Golan
Student ID: 9942991
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

	
	int Memory_Address;

	/*Initialise list of file-strings. This will be used to iterate through the different trace files at hand, 
	and execute the cache simulation at different block sizes. 
	*/
	const char *file_names[2] = {"cross_correlation_trace_036.trc", "bubble_sort_trace_036.trc"};

	//Useful variables - pretty self-explanatory in terms of the purpose they serve. 
	int Cache_Line_ID;
	int Tag_Bits;
	float Percentage_Hits;
	char Read_OR_Write;
	

	//Start of Simulation: For loop; 2 iterations for 2 trace files. 
	for (int j=0; j<2; j++){
		printf("File at hand is: %s \n", file_names[j]);
		int Cache_Initial_Size = 256;
		char s, n;
		
		//7 iterations for the cache performance simulations on the 7 possible configurations. [128, 64, 32, 16, 8, 4, 2]
		for (int i=1; i<8; i++){
			
			Cache_Initial_Size = Cache_Initial_Size/2; //Used to define cache size for each iterations
			//Opening of target trace file defined by for-loop above. 
			FILE *in_file = fopen(file_names[j], "r");
			
			int Read_Count = 0;
			int Write_Count = 0;
			
			int Cache_Read_Hit=0;
			int Cache_Read_Miss=0;
			int Cache_Write_Hit=0;
			int Cache_Write_Miss=0;
				
			//Initialisation of Cache Memory. Each block has 16 bits (unsigned int short)
			unsigned int short Cache_Array[Cache_Initial_Size];
			
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
					Cache_Line_ID = Memory_Address % Cache_Initial_Size; //128 bits = 0x007F

					//Tag Bit identification method never changes. 
					Tag_Bits = Memory_Address & 0xFF80;
					Tag_Bits = Tag_Bits >> 7;

					//Cache Memory Logic when Empty
					if (Cache_Array[Cache_Line_ID] == 0 && Read_OR_Write == 'W') {
						Cache_Array[Cache_Line_ID] = Tag_Bits;
						Cache_Write_Miss++;
					}
					else if (Cache_Array[Cache_Line_ID] == 0 && Read_OR_Write == 'R') {
						Cache_Array[Cache_Line_ID] = Tag_Bits;
						Cache_Read_Miss++;
					}

					//Cache Memory Logic when non-empty. 
					else if (Cache_Array[Cache_Line_ID] !=0 && Read_OR_Write == 'W' && Cache_Array[Cache_Line_ID] == Tag_Bits){
						Cache_Write_Hit++;
					}
					else if (Cache_Array[Cache_Line_ID] !=0 && Read_OR_Write == 'W' && Cache_Array[Cache_Line_ID] != Tag_Bits){
						Cache_Array[Cache_Line_ID] = Tag_Bits;
						Cache_Write_Miss++;
					}

					else if (Cache_Array[Cache_Line_ID] !=0 && Read_OR_Write == 'R' && Cache_Array[Cache_Line_ID] == Tag_Bits){
						Cache_Read_Hit++;
					}
					else if (Cache_Array[Cache_Line_ID] !=0 && Read_OR_Write == 'R' && Cache_Array[Cache_Line_ID] != Tag_Bits){
						Cache_Array[Cache_Line_ID] = Tag_Bits;
						Cache_Read_Miss++;
					}

					
				}
			//Close file after each for loop iteration - then re-open it. 
			fclose(in_file);
			
			// Print statements hereunder show the performance and the metrics for each cache memory configuration. 
			
			Percentage_Hits = ((float)Cache_Read_Hit+Cache_Write_Hit)/(Read_Count+Write_Count);

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

			printf("\n");
			
			
		}

	printf("End of Cache Simulation on file: %s \n", file_names[j]);
	sleep(3);

	}

	
	sleep(3);

	printf("End of Cache Simulation of Bubble-Sort and Cross-Correlation files\n");
	
	return 0;
}
