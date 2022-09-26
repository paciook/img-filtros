#include<iostream>
#include<fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#define ONE_OVER_BILLION 1E-9

using namespace std;

void ShowHelp();
void ShowHelp(string s);

int main(int argc , char* argv[]){
	// cout << argc << endl;
	string out = "out/salida.ppm";
	// pidio help
	// cout << "Uso: ./tp <filtro> <nthreads> <p1> <p2> <img1> <img2>" << endl;
	if(argc <= 2){
		ShowHelp();
		return 1;
	}
	
	string filtro = argv[1];
	int nThreads = atoi(argv[2]);
	bool singleThread = nThreads == 1;
	
	cout << "Abriendo imagen..." << endl;
	
	ppm img(argv[3]);
	
	cout << "Aplicando filtro..." << endl;

	// Apply filter based on user input
	{
		if(filtro == "bw"){
			// Black and white
			// ./tp bw <nthreads> <img1>
			if (singleThread)
				blackWhite(img);
			else
				threadedBlackWhite(img, nThreads);
		}

		else if (filtro == "br"){
			// Brightness
			// ./tp br <nthreads> <img1> <brightness>
			brightness(img, atof(argv[4]), 0, 0);
		}

		else if (filtro == "ct") {
			// Contrast
			// ./tp br <nthreads> <img1> <contraste>
			contrast(img, atof(argv[4]));
		}

		else if (filtro == "mg"){
			// merge
			// ./tp br <nthreads> <img1> <img2> <alpha>
			ppm img2(argv[4]);
			float alpha = stof(argv[5]);
			merge(img, img2, alpha);
		}

		else if (filtro == "ed"){
			// Edge Detection
			// ./tp ed <nthreads> <img1>
			if(singleThread)
				edgeDetection(img);
			else
				threadedEdgeDetection(img, nThreads);
		}

		else if (filtro == "fr"){
			// Frame
			// ./tp br <nthreads> <img1> <color> <x>
			pixel p;
			p.r = atoi(argv[4]);
			p.g = atoi(argv[4]);
			p.b = atoi(argv[4]);

			frame(img, p, atoi(argv[5]));
		}

		else if (filtro == "uz"){
			// Unzoom
			// ./tp uz <nthreads> <img> <escale>
			int escale = atoi(argv[4]);

			ppm img2(img.width/escale, img.height/escale);

			unzoom(img, img2, escale);

		}

		else if (filtro == "dd"){
			dither(img);
		}
		
		else {
			ShowHelp();
			return 1;
		}
	}
	

	cout << "Escribiendo imagen..." << endl;
	
	img.write(out);
	
	cout << "Listo." << endl;
	
	return 0;
}



void ShowHelp(){
	cout << "Uso: ./tp <filtro> <nthreads> <p1> <p2> <img1> <img2>" << endl;
	exit(0);
}
