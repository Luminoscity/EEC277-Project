#pragma region Info
/*
	Generates an XBINS * YBINS grid of triangles 
		(grid of squares, sliced along the SW-NE diagonal)
	Currently selects colors as vertical stripes for 5 bins/direction
 */
#pragma endregion

#pragma region Includes
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>

using std::string;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::stringstream;

#pragma endregion


#pragma region Declarations
#define _CRT_SECURE_NO_WARNINGS


#define XBINS 5
#define YBINS 5
#pragma endregion

#pragma region getColor
char const* getColor(float x) {

	int z = 10 * x;

	if (z == 0) {	
		return "FF0000FF";		//red
	}
	else if (z == 2) {	
		return "FF9900FF";		//orange
	}
	else if (z == 4) {
		return "FFFF00FF";		//yellow
	}
	else if (z == 6) {
		return "00FF00FF";		//green
	}
		else if (z == 8) {		
		return "0000FFFF";		//blue
	}
	else if (z == 10) {
		return "6600FFFF";		//purple
	}
	else {	
		return "FFFFFFFF";		//white if missed
	}

}
#pragma endregion

#pragma region Main
int main() {

	//set up vars
	FILE *fp = fopen("triStripes.txt", "wb");

	float xstep = 1.0 / XBINS;
	float ystep = 1.0 / YBINS;

	float x = 0.0;
	float y = 0.0;

	float xR = x + xstep;

	//D for disjoint
	fprintf(fp, "D\n");
	
	//loop through x and y
	//	write a whole column (loop through y)
	//	before incrementing x and writing the next column
	//	currently, getColor assigns colors in vertical stripes following a ROYGBP pattern
	while (x < 1.0) {

		while (y < 1.0) {

			fprintf(fp, "%f %f %s, %f %f %s, %f %f %s\n", x, y, getColor(x), xR, y + ystep, getColor(xR), xR, y, getColor(xR));
			fprintf(fp, "%f %f %s, %f %f %s, %f %f %s\n", x, y, getColor(x), xR, y + ystep, getColor(xR), x, y + ystep, getColor(x));

			y = y + ystep;
		}

		x = x + xstep;
		xR = xR + xstep;
		y = 0.0;

	}


	fclose(fp);
	return(0);

}
#pragma endregion
