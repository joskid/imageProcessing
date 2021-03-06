#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;

int T2X2[2][2] = {{96 ,160}, {224, 32}};
int T4X4[4][4] = {
{88,  151, 104, 167},
{215,  24, 231, 40},
{120, 183,  72, 135},
{247, 56,  199,  8}};

int T1[2][2] = {{21, 42},{64,0}};
int T2[2][2] = {{106, 128},{150,85}};
int T3[2][2] = {{192, 213},{234,171}};

double gaussian(){

	double random = ((double)rand()/((unsigned)RAND_MAX+1)); 
	double tmp = (random)/0.5;
	double gauss= exp(-tmp*tmp/2.0); 

	return gauss*255;
}


// Here we assume the image is of size 256*256 and is of raw format
int main(int argc, char *argv[]){
	// file pointer
	FILE *file;
	int type = 0;
	if(argc < 4){
		cout << "usage: program_name input_image.raw output_image.raw type" << endl;
		return 0;
	}
	type = atoi(argv[3]);	
	//read image	
	unsigned char inImg[256][256];	
	if (!(file=fopen(argv[1],"rb"))){
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(inImg, sizeof(unsigned char), 256*256, file);
	fclose(file);	


	unsigned char outImg[256][256];	
	if(type == 2 || type == 3) srand( (unsigned int)time( NULL ) );

	for(int i=1; i<255; i++){
		for(int j=1; j<255; j++){
			if(type == 1){
				if(inImg[i][j] < 127){
					outImg[i][j] = 0;
				}
				else{
					outImg[i][j] = 255;
				}
			}
			else if(type == 2){
				int t = ((unsigned)rand())% 256;
				if(inImg[i][j] < t){
					outImg[i][j] = 0;
				}
				else{
					outImg[i][j] = 255;
				}
			}
			else if(type == 3){
				double t = gaussian();
				if(inImg[i][j] < t){
					outImg[i][j] = 0;
				}
				else{
					outImg[i][j] = 255;
				}
			}
			else if(type == 4){ //2X2 dithering matrix				
				if(inImg[i][j] < T2X2[i%2][j%2]){
					outImg[i][j] = 0;
				}
				else{
					outImg[i][j] = 255;
				}		
			}
			else if(type == 5){ //4X4 dithering matrix				
				if(inImg[i][j] < T4X4[i%4][j%4]){
					outImg[i][j] = 0;
				}
				else{
					outImg[i][j] = 255;
				}
			}
			
			else if(type == 6){ //4 intensity level
				if(inImg[i][j] < T1[i%2][j%2]){
					outImg[i][j] = 0;
				}
				else if(inImg[i][j] >= T1[i%2][j%2] && inImg[i][j] < T2[i%2][j%2]){
					outImg[i][j] = 63;
				}		
				
				else if(inImg[i][j] >= T2[i%2][j%2] && inImg[i][j] < T3[i%2][j%2]){
					outImg[i][j] = 127;
				}				
				else if(inImg[i][j] >= T2[i%2][j%2]){
					outImg[i][j] = 191;
				}		
			}
		}         
	}

	
	if(type == 7){ //Floyd-Steinberg
		double med[256][256];
		for(int i=1; i<255; i++){
			if(i%2){
				for(int j=1; j<255; j++){
					if(inImg[i][j] > 127) med[i][j] = 255;
					else med[i][j] = 0;					
					double error = inImg[i][j] - med[i][j];
					med[i+1][j] += (7/16.0)* error;
					med[i-1][j+1] += (3/16.0)* error;
					med[i][j+1] += (5/16.0)* error;
					med[i+1][j+1] += (1/16.0)* error;
				}
			}
			else{
				for(int j=254; j > 0; j--){
					if(inImg[i][j] > 127) med[i][j] = 255;
					else med[i][j] = 0;
					double error = inImg[i][j] - med[i][j];
					med[i+1][j] += (7/16.0)* error;
					med[i-1][j+1] += (3/16.0)* error;
					med[i][j+1] += (5/16.0)* error;
					med[i+1][j+1] += (1/16.0)* error;
				}
			}
		}
		for(int i=1; i<255; i++){
			for(int j=1; j<255; j++){
				outImg[i][j] = (int)(med[i][j] + 0.5);
			}
		}
	}
	if (!(file=fopen(argv[2],"wb")))
	{
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(outImg, sizeof(unsigned char), 256*256, file);
	fclose(file);

	return 0;
}

