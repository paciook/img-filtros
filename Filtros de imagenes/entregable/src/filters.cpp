#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>  

#define BLACK 0

using namespace std;

// COMPLETAR :)

void blackWhite(ppm& img)
{
	vector<int> v{1};
	short int r,g,b,aux;
	for(line = 0; line < img.height; line++){
		for(row = 0; row < img.width; row++){
			
			r = img.getPixel(line,row).r;
			g = img.getPixel(line,row).g;
			b = img.getPixel(line,row).b;
			
			aux = (r+g+b)/3;
			
			img.setPixel(line,row,pixel(aux,aux,aux));
		}
	}

	return;
}

void merge(ppm& img1, ppm& img2, float alpha)
{
	vector<int> v{1};
	short int r,g,b;

	for(line = 0; line < img.height; line++){
		
		for(row = 0; row < img.width; row++){
			
			r = ( img1.getPixel(line,row).r + img2.getPixel(line,row).r ) / 2;
			g = ( img1.getPixel(line,row).g + img2.getPixel(line,row).g ) / 2;
			b = ( img1.getPixel(line,row).b + img2.getPixel(line,row).b ) / 2;
			
			newImg.setPixel(line,row,pixel(r,g,b));
		}
	}

	return;
}

void brightness(ppm& img, float b, int start, int end)
{
	vector<int> v{1};
	short int r,g,b;

	for(line = 0; line < img.height; line++){
		
		for(row = 0; row < img.width; row++){
			
			r = img.getPixel(line,row).r + ( 255 * b );
			g = img.getPixel(line,row).g + ( 255 * b ):
			b = img.getPixel(line,row).b + ( 255 * b );
			
			ppm& pix = pixel(r,g,b);

			img.setPixel(line,row,pix.truncate());
		}
	}

	return;
}