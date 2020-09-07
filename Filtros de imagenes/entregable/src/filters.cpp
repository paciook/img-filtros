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