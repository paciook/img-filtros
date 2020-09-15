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
	for(int y = 0; y < img.height; y++){

		for(int x = 0; x < img.width; x++){
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
	ppm newImg(img1.width, img1.height);

	// Read the images
	for(int y = 0; y < img1.height; y++){
		
		for(int x = 0; x < img1.width; x++){
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

void brightness(ppm& img, float br, int start, int end)
{
	// Declare the needed variables and the new image
	short int r,g,b;

	// Read the image
	for(int y = 0; y < img.height; y++){
		
		for(int x = 0; x < img.width; x++){
			// Process every pixel values
			r = img.getPixel(y,x).r + ( 255 * br );
			g = img.getPixel(y,x).g + ( 255 * br );
			b = img.getPixel(y,x).b + ( 255 * br );
			
			pixel pix = pixel(r,g,b);

			// Set the result
			img.setPixel(y,x,pix.truncate());
		}
	}

	return;
}

void contrast(ppm& img, float contrast)
{
	// Declare the needed variables and the new image
	unsigned short int r,g,b,f;
	f = 259*(contrast+255) / 255*(259-contrast);

	// Read the image

	for(int y = 0; y < img.height; y++){

		for(int x = 0; x < img.width; x++){
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

ppm& convolution(ppm& img, short int ker[])
{
	// Declare the needed variables
	ppm newImg(img.height-2, img.width-2);
	unsigned short int r,g,b=0;

	// Read the image
	for(int y = 1; y < img.height - 1; y++){

		for(int x = 1; x < img.width - 1; x++){
			r,g,b=0;

			// Run the kernel over the image
			for(int ky = 0; ky < 4; ky++){

				for(int kx = 0; kx < 4; kx++){
					r += img.getPixel(y+ky-1,x+kx-1).r * ker[ky*3+kx];
					g += img.getPixel(y+ky-1,x+kx-1).g * ker[ky*3+kx];
					b += img.getPixel(y+ky-1,x+kx-1).b * ker[ky*3+kx];
				}
			}

			// Set the result
			newImg.setPixel(y,x,pixel(r,g,b));
		}
	}

	return newImg;
}

void edgeDetection(ppm &img, ppm &img_target){
	// Declare the kernel
	short int kernel[] = {1, 0, -1, 2, 0, -2, 1, 0, -1};

	// Set the result
	ppm& aux = convolution(img,kernel);

	for(int y = 1; y < img_target.height - 1; y++){

		for(int x = 1; x < img_target.width - 1; x++){
			img_target.setPixel(y,x,aux.getPixel(y,x));
		}
	}

	return;
}