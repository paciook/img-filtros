#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#define ONE_OVER_BILLION 1E-9


// Un esqueleto como base, modificar a gusto pero respetando la consigna

int main(int argc , char* argv[]){
	
	// Pasamos 0 en caso de no utilizar p2 o  "" en img2
	if(string(argv[1]) == "-help")
	{
		cout << "Uso: ./tp <filtro> <nthreads> <p1> <p2> <img1> <img2>" << endl;
		return 0; 
	}

	cout << "Aplicando filtro" << endl;
	// Start the clock
	struct timespec start, stop;
    double accum;
	clock_gettime(CLOCK_REALTIME, &start);
	
	if(string(argv[1])=="blackWhite")
	{

		string img1(argv[3]);
		ppm img(img1);
		blackWhite(img);

	}else if(string(argv[1])=="frame"){	

		int pix(argv[3]);
		int val(argv[4]);
		string img1(argv[5]);
		ppm img(img1);
		frame(img,pixel(pix, pix, pix), val);

	}else if(string(argv[1])=="brightness"){

		float br(argv[3]);
		brightness(img,br,0,0);

	}else if(string(argv[1])=="merge"){

		string img1(argv[4]);
		ppm img(img1);
		string imgb(argv[5]);
		ppm img2(imgb);
		float p1=(float)(argv[3]);
		merge(img, img2, p1);

	}else if(string(argv[1])=="edgeDetection"){

		string img1(argv[3]);
		ppm img(img1);
		ppm img_target(img.width-2, img.height-2);
		edgeDetection(img,img_target);
	}else{
		cout << "Error";
		return 1;
	}
	string img1(argv[3]);
	ppm img(img1);

	// Stop the clock
	clock_gettime(CLOCK_REALTIME, &stop);
	accum = ( stop.tv_sec - start.tv_sec )
	        + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf( "Duración del filtrado: %lf s\n", accum);
	
	cout << "Escribiendo imagen" << endl;

	// Write the image
	string out = "out/salida.ppm";
	img.write(out); 

	cout << "Listo" << endl;

	return 0;
}