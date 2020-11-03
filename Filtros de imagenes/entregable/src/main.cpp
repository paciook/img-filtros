#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#define ONE_OVER_BILLION 1E-9

void ShowHelp();
void ShowHelp(string s);

// Un esqueleto como base, modificar a gusto pero respetando la consigna

int main(int argc , char* argv[]){
	// cout << argc << endl;
	string out = "out/salida.ppm";
	// pidio help
	// cout << "Uso: ./tp <filtro> <nthreads> <p1> <p2> <img1> <img2>" << endl;
	if(argc == 1){
		ShowHelp();
	}
	else if (argv[2] == "--help" || argv[2] == "-h"){
		ShowHelp(argv[1]);
	}
	else{
		string filtro = argv[1];
		int nThreads = atoi(argv[2]);
		cout << "Abriendo imagen..." << endl;
		ppm img(argv[3]);
		bool singleThread = nThreads == 1;
		cout << "Aplicando filtro..." << endl;
		// timeo
		struct timespec start, stop;
		double accum;
		clock_gettime(CLOCK_REALTIME, &start);
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
		else if (filtro == "ct"){
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
		else{
			ShowHelp();
		}
		clock_gettime(CLOCK_REALTIME, &stop);
			accum = ( stop.tv_sec - start.tv_sec )
					+ ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
			printf( "Duración del filtrado: %lf s\n", accum);

		cout << "Escribiendo imagen..." << endl;
		img.write(out);
		cout << "Listo." << endl;
		// n, t, w
		ofstream file;
		file.open("out/info.csv", ios::app);
		string out = to_string(nThreads) + "," + to_string(accum) + "," + to_string(img.height * img.width) + "\n";
		cout << "TAMOS ";
		file << out;
		file.close();
	}
	return 0;
}



void ShowHelp(){
	cout << "Uso: ./tp <filtro> <nthreads> <p1> <p2> <img1> <img2>" << endl;
	exit(0);
}

void ShowHelp(string command){
	if(command == "bw"){
		cout << "./tp bw <nthreads> <img>";
		exit(0);
	}
}