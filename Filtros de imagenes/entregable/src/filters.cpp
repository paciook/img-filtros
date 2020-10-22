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
	/* Greyscales an image */

	// Declare the needed variable
	unsigned short int aux;

	// Read the image
	for(int y = 0; y < img.height; y++){
		for(int x = 0; x < img.width; x++){
			// Process every pixel value
			aux = img.getPixel(y,x).cumsum() / 3;
			
			// Set the result
			img.setPixel(y,x,pixel(aux,aux,aux));
		}
	}
	return;
}

void threadedBlackWhitePt(ppm* img, int fila_start, int fila_end) {

	for (size_t i = fila_start; i < fila_end; i++)
	{
		for (size_t j = 0; j < img->width; j++)
		{
			pixel p = img->getPixel(i, j);
			pixel* pixelResultante = new pixel();
			unsigned int gr = (p.r + p.g + p.b) / 3;
			pixelResultante->r = gr;
			pixelResultante->g = gr;
			pixelResultante->b = gr;
			img->setPixel(i, j, *pixelResultante);
		}
	}
}

void threadedBlackWhite(ppm& img, int nThreads) {
	// calculo como separar las filas
	int c_filas_por_thread = (int)(img.height / nThreads);
	int offset = img.height - (c_filas_por_thread * nThreads);
	// hago el pool de threads
	vector<thread> thread_pool;
	for (size_t i = 0; i < nThreads; i++)
	{
		// calculo donde empieza y termina cada thread
		int start = i * c_filas_por_thread;
		int end = (i + 1) * c_filas_por_thread;
		if (i == nThreads - 1) {
			end += offset;
		}
		// Creo el thread
		thread_pool.push_back(thread(threadedBlackWhitePt, &img, start, end));
	}
	for (size_t i = 0; i < nThreads; i++)
	{
		// los espero
        thread_pool[i].join();
	}
	// Si, colgué y me dió fiaca
}

void merge(ppm& img1, ppm& img2, float alpha)
{
	/* Merges two images */

	// Declare the needed variables and the new image
	unsigned short int r,g,b;

	// Read the images
	for(int y = 0; y < img1.height; y++){
		
		for(int x = 0; x < img1.width; x++){
			// Process every pixel values
			r = (img1.getPixel(y,x).r * alpha) + ( img2.getPixel(y,x).r * (1- alpha));
			g = (img1.getPixel(y,x).g * alpha) + ( img2.getPixel(y,x).g * (1- alpha));
			b = (img1.getPixel(y,x).b * alpha) + ( img2.getPixel(y,x).b * (1- alpha));
			
			// Set the result
			img1.setPixel(y,x,pixel(r,g,b));
		}
	}
	return;
}

void brightness(ppm& img, float b, int start, int end)
{
	/* Changes the brightness of an image */
	// Read the image
	for(int y = 0; y < img.height; y++){		
		for(int x = 0; x < img.width; x++){
			// Process every pixel values
			img.getPixel(y,x).add(255*b).truncate();
		}
	}
	return;
}

void contrast(ppm& img, float contrast)
{
	/* Changes the contrast of an image */

	// Declare the needed variable
	float f = (259*(contrast+255)) / (255*(259-contrast));

	// Read the image

	for(int y = 0; y < img.height; y++){
		for(int x = 0; x < img.width; x++){
			// Process every pixel values
			img.getPixel(y,x).sub(128).mult(f).add(128).truncate();
		}
	}
	return;
}

void convolution(ppm& img,ppm& img_target, short int ker[])
{
	/* Generic convolution algorithm */

	// Declare the needed variables
	int r,g,b;

	// Read the image
	for(int y = 1; y < img.height - 1; y++){
		for(int x = 1; x < img.width - 1; x++){
			r=g=b=0;

			// Run the kernel over the image
			for(int ky = 0; ky < 3; ky++){
				for(int kx = 0; kx < 3; kx++){

					r += img.getPixel(y+ky-1,x+kx-1).r * ker[ky*3+kx];
					g += img.getPixel(y+ky-1,x+kx-1).g * ker[ky*3+kx];
					b += img.getPixel(y+ky-1,x+kx-1).b * ker[ky*3+kx];
				}
			}

			// Set the result
			img_target.setPixel(y-1,x-1,pixel(r,g,b).truncate());
		}
	}
	return;
}

void sobel(ppm &img1, ppm &img2){
	// Declare the needed vairables
	unsigned short int r,g,b;

	// Read the image
	for(int y = 0; y < img1.height; y++){
		for(int x = 0; x < img1.width; x++){
			// Process each pixel value
			r = sqrt(pow(img1.getPixel(y,x).r,2) + pow(img2.getPixel(y,x).r,2));
			g = sqrt(pow(img1.getPixel(y,x).g,2) + pow(img2.getPixel(y,x).g,2));
			b = sqrt(pow(img1.getPixel(y,x).b,2) + pow(img2.getPixel(y,x).b,2));

			// Set the value
			img1.setPixel(y,x,pixel(r,g,b));
		}
	}
}

void edgeDetection(ppm &img, ppm &img_target){
	/* Detects the edges */

	// B&W Filter
	blackWhite(img);

	// Horizontal convolution
	short int kernel[] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
	convolution(img,img_target,kernel);

	// Vertical convolution
	short int ker[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
	ppm img2(img_target.width,img_target.height);
	convolution(img, img2, kernel);

	// Sobel
	sobel(img_target,img2);

	img = img_target;

	return;
}

void threadedConvolution(ppm* img, ppm* img_target, short int ker[], int start, int end)
{
	/* Generic convolution algorithm */

	// Declare the needed variables
	int r,g,b;

	// Read the image
	for(int y = start; y < end; y++){
		for(int x = 1; x < img->width - 1; x++){
			r=g=b=0;

			// Run the kernel over the image
			for(int ky = 0; ky < 3; ky++){
				for(int kx = 0; kx < 3; kx++){

					r += img->getPixel(y+ky-1,x+kx-1).r * ker[ky*3+kx];
					g += img->getPixel(y+ky-1,x+kx-1).g * ker[ky*3+kx];
					b += img->getPixel(y+ky-1,x+kx-1).b * ker[ky*3+kx];
				}
			}

			// Set the result
			img_target->setPixel(y-1,x-1,pixel(r,g,b).truncate());
		}
	}
	return;
}

void threadedEdgeDetection(ppm &img, int nThreads){
	/* Detects the edges */
	// B&W Filter
	threadedBlackWhite(img, nThreads);

	ppm img1(img.width - 2,img.height - 2);
	ppm img2(img.width - 2,img.height - 2);

	int c_filas_por_thread = (int)(img.height / nThreads);
	int offset = img.height - (c_filas_por_thread * nThreads);

	vector<thread> thread_pool;
	for (size_t i = 0; i < nThreads; i++)
	{
		// calculo donde empieza y termina cada thread
		int start = (i * c_filas_por_thread) + 1;
		int end = ((i + 1) * c_filas_por_thread);
		if (i == nThreads - 1) {
			end += offset - 1;
		}
		// Horizontal convolution
		short int kernel[] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
		// Creo el thread
		thread_pool.push_back(thread(threadedConvolution, &img, &img2, kernel, start, end));
	}
	for (size_t i = 0; i < nThreads; i++)
	{
		// los espero
        thread_pool[i].join();
	}

	vector<thread> threads;
	for (size_t i = 0; i < nThreads; i++)
	{
		// calculo donde empieza y termina cada thread
		int start = i * c_filas_por_thread;
		int end = (i + 1) * c_filas_por_thread;
		if (i == nThreads - 1) {
			end += offset;
		}
		// Horizontal convolution
		short int ker[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
		// Creo el thread
		threads.push_back(thread(threadedConvolution, &img, &img1, ker, start, end));
	}
	for (size_t i = 0; i < nThreads; i++)
	{
		// los espero
        threads[i].join();
	}

	// Sobel
	sobel(img1,img2);

	img = img1;

	return;
}

void frame(ppm& img, pixel color, int p){
	/* Makes a frame for the image */

	// Read the image
	for(int y = 0; y < img.height; y++){
		for(int x = 0; x < img.width; x++){
			// Check the pixel
			if( ((y < p+1)|(y>img.height-p-1)) | ((x<p+1)|(x>img.width-p-1)) )
				// Set the result
				img.setPixel(y,x,color);
		}
	}
	return;
}

void dither(ppm& img){
	/* Represents grayscales just using b&w */

	// Declare the needed variables
	short int c;
	float err;
	
	// Greyscale the image
	blackWhite(img);

	// Read the image
	
	for(int y = 0; y < img.height-1; y++){
		for(int x = 1; x < img.width-1; x++){
			// Process every pixel values
			c = (round((float)img.getPixel(y,x).r / 255)) * (255);

			// Calculate the error
			err = img.getPixel(y,x).r - c;

			// Set the result
			img.setPixel(y,x,pixel(c,c,c));

			// Spread the error
			img.getPixel(y  ,x+1).add(err*7/16.0).truncate();
			img.getPixel(y+1,x-1).add(err*3/16.0).truncate();
			img.getPixel(y+1,x  ).add(err*5/16.0).truncate();
			img.getPixel(y+1,x+1).add(err*1/16.0).truncate();
		}
	}
	return;
}

void zoom(ppm &img, ppm &img_zoomed, int n){
	/* Zoom xd */

	// Read the image
	for(int y = 0; y < img.height; y++){
		for(int x = 1; x < img.width; x++){
		
		// Write the aux
			for(int i = 0; i < n; i++){
				for(int j = 0; j < n; j++){
					img_zoomed.setPixel((y*n) + i, (x*n) + j, img.getPixel(y,x));
					}
			}
		}
	}
	img = img_zoomed;
	return;

}