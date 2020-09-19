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
	string filtro = argv[1];
	cout << "Abriendo imagen..." << endl;
	ppm img(argv[1]);

	cout << "Aplicando filtro..." << endl;
	struct timespec start, stop;
	double accum;
	clock_gettime(CLOCK_REALTIME, &start);
/*
	if (filtro == "b&w"){
		if (singleThread)
			blackWhite(img);
	}
	else if (filtro == "contrast"){
		contrast(img, atof(argv[4]));
	}
	else if (filtro == "bright"){
		brightness(img, atof(argv[4]), 0, 0);
	}
	else if (filtro == "merge"){
		ppm img2(argv[4]);
		float alpha = stof(argv[5]);
		merge(img, img2, alpha);
	}
	else if (filtro == "frame"){
		pixel p;
		p.r = atoi(argv[4]);
		p.g = atoi(argv[4]);
		p.b = atoi(argv[4]);
		frame(img, p, atoi(argv[5]));
	}
	else if (filtro == "edgeDetect"){
			ppm img2((string)argv[6]);
			edgeDetection(img, img2);
	}
*/
	dither(img);

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