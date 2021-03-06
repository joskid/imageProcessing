// The programe reads out the image data from an image file and write into another 
// last updated on 5/2/2007 by May-chen Kuo                                                               


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>


int main(int argc, char *argv[]){
	FILE *file;
	int i,j;
	
	if(argc < 4){
		printf( "usage: program_name input_img output_img1 output_img1 \n" );
		return 0;
	}
	
	unsigned char in[512][512];		
	unsigned char out[512][512];		
	unsigned char out1[512][512];		
	
	if (!(file=fopen(argv[1],"rb"))){
		printf("Cannot open file: %s\n" , argv[1]);
		exit(1);
	}
	fread(in, sizeof(unsigned char), 512*512, file);
	fclose(file);
//////////////////////////////////////////////////
    /*double A[6][6] = {
		{-1517.062439, -1525.015625, -1523.035126, 1526.015625, 1523.035126, 1517.062439 },
		{6.929810, 6.976562, 6.957092, -6.976562, -6.957092, -6.929810 },
		{4.984375, 4.988266, 5.000062, -5.000062, -4.984375, -4.988266 },
		{-0.007751, -0.007812, -0.007782, 0.007812, 0.007782, 0.007751}, 
		{-0.007812, -0.007812, -0.007812, 0.007812, 0.007812, 0.007812}, 
		{-0.007812, -0.007843, -0.007874, 0.007874, 0.007812, 0.007843 }	
	};
	int u[6] = {255, 383, 511, 436, 255, 383};
	int v[6] = {255, 383, 511, 181, 255, 383};
	
	int a[6];
	int b[6];
	

	for(i =0; i < 6; ++i){
		a[i] = 0;
		b[i] = 0;
		for(j =0; j < 6; ++j){
			a[i] += A[i][j]*u[j];
			b[i] += A[i][j]*v[j];
		}
	}*/   
	for(i = 0; i < 512; ++i){
		for(j = 0; j < 512; ++j){
			double dist = sqrt((255-i)*(255-i) + (255-j)*(255-j));
			if(dist > 256) {
				int d = (int)((dist - 256)/1.414 + 0.5); 			
				dist = sqrt((255-i - d)*(255-i-d) + (255-j-d)*(255-j-d));
				if(dist < 256) 	out[i + d][j + d] = in[i][j] ;			
			}	
			else{
				out[i][j] = in[i][j] ;			
			}
		}
	}

//////////////////////////////////////////////////
	if (!(file=fopen(argv[2],"wb")))
	{
		printf( "Cannot open file: %s\n" , argv[2] );
		exit(1);
	}
	fwrite(out, sizeof(unsigned char), 512*512, file);
	fclose(file);
	for(i = 0; i < 512; ++i){
		for(j = 0; j < 512; ++j){
			double dist = sqrt((255-i)*(255-i) + (255-j)*(255-j));
			if(dist > 256) {
				int d = (int)((dist - 256)/1.414 + 0.5); 			
				dist = sqrt((255-i - d)*(255-i - d) + (255-j - d)*(255-j - d));
				out1[i][j] = in[i][j] ;			
			}	
			else{
				out1[i][j] = in[i][j] ;			
			}
		}
	}

	
	if (!(file=fopen(argv[3],"wb")))
	{
		printf( "Cannot open file: %s\n" , argv[2] );
		exit(1);
	}
	fwrite(out1, sizeof(unsigned char), 512*512, file);
	fclose(file);
	return 0;
}



