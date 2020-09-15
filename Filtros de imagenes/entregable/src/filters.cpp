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
	// Declare the needed variables
	unsigned short int r,g,b,aux;

	// Read the image
	for(y = 0; y < img.height; y++){

		for(x = 0; x < img.width; x++){
			
			// Process every pixel values
			r = img.getPixel(y,x).r;
			g = img.getPixel(y,x).g;
			b = img.getPixel(y,x).b;
			
			aux = (r+g+b)/3;
			
			// Set the result
			img.setPixel(y,x,pixel(aux,aux,aux));
		}
	}

	return;
}

void merge(ppm& img1, ppm& img2, float alpha)
{
	// Declare the needed variables and the new image
	unsigned short int r,g,b;
	ppm newImg(img.width, img.height);

	// Read the images
	for(y = 0; y < img.height; y++){
		
		for(x = 0; x < img.width; x++){
			
			// Process every pixel values
			r = (img1.getPixel(y,x).r * alpha) + ( img2.getPixel(y,x).r * (1- alpha));
			g = (img1.getPixel(y,x).g * alpha) + ( img2.getPixel(y,x).g * (1- alpha));
			b = (img1.getPixel(y,x).b * alpha) + ( img2.getPixel(y,x).b * (1- alpha));
			
			// Set the result
			newImg.setPixel(y,x,pixel(r,g,b));
		}
	}

	return;
}

void brightness(ppm& img, float b, int start, int end)
{
	// Declare the needed variables and the new image
	short int r,g,b;

	// Read the image
	for(y = 0; y < img.height; y++){
		
		for(x = 0; x < img.width; x++){
			
			// Process every pixel values
			r = img.getPixel(y,x).r + ( 255 * b );
			g = img.getPixel(y,x).g + ( 255 * b ):
			b = img.getPixel(y,x).b + ( 255 * b );
			
			ppm& pix = pixel(r,g,b);

			// Set the result
			img.setPixel(y,x,pix.truncate());
		}
	}

	return;
}

void contrast(ppm& img, float contrast)
{
	// Declare the needed variables and the new image
	short int r,g,b,f;
	f = 259*(contrast+255) / 255*(259-contrast);

	// Read the image
	for(y = 0; y < img.height; y++){

		for(x = 0; x < img.width; x++){

			// Process every pixel values
			r = f * (img.getPixel(y,x).r - 128) + 128;
			g = f * (img.getPixel(y,x).g - 128) + 128;
			b = f * (img.getPixel(y,x).b - 128) + 128;

			// Set the result
			img.setPixel(y,x,pixel(r,g,b));
		}
	}

	return;
}