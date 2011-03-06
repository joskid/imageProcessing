// The programe reads out the image data from an image file and write into another 
// last updated on 5/2/2007 by May-chen Kuo                                                               


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

// Here we assume the image is of size 256*256 and is of raw format
// You will need to make corresponding changes to accommodate images of different sizes and types


int main(int argc, char *argv[])

{
	// file pointer
	FILE *file;
	int Size = 256;
	
	if(argc < 3){
		cout << "usage: program_name input_image.raw output_image.raw" << endl;
		return 0;
	}
	// image data array
	unsigned char Imagedata[Size][Size];

	// read image "ride.raw" into image data matrix
	if (!(file=fopen(argv[1],"rb")))
	{
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size, file);
	fclose(file);
	///////////////////// generate histogram
	unsigned long histogram[256];
	for(int i = 0 ; i  < 256; ++i){
		histogram[i] = 0;
	}

	for(int i = 0 ; i  < Size; ++i){
		for(int j = 0 ; j  < Size; ++j){
			histogram[Imagedata[i][j]] = histogram[Imagedata[i][j]] + 1;
		}
	}
	
	int fmax = -1;
	int fmin = -1;
	for(int i = 0 ; i  < 256; ++i){
		if(histogram[i] > 0){
			if(fmin == -1){
				fmin = i;
			}
			if(fmax < i){
				fmax = i;
			}
		}
	}
	
	unsigned char OutputImagedata[Size][Size];	
		unsigned long sum = 0;
		unsigned long cdf[256];
		for(int i = 0 ; i  < 256; ++i){
			sum = sum + histogram[i];
			cdf[i] = sum;
		}	
		double mult = 255.0/(Size*Size - cdf[fmin]);
		for(int i = 0 ; i  < Size; ++i){
			for(int j = 0 ; j  < Size; ++j){
				int k = Imagedata[i][j];
				OutputImagedata[i][j] = (unsigned char)((cdf[k] - cdf[fmin])*mult + 0.5);
			}
		}

	if (!(file=fopen(argv[2],"wb")))
	{
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(OutputImagedata, sizeof(unsigned char), Size*Size, file);
	fclose(file);

	return 0;
}
