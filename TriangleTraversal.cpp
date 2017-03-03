#pragma region Info
/*
 * TriangleTraversal.cpp
 *
 * Tim Ambrose and Angela Tobin
 * March 2017
 * Tests the following rasterization paths (triangle traversal methods)
 * against an inputted set of triangle vertex coordinates:
 * 
 */
#pragma endregion

#pragma region Includes, Types, and Constants
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

#define TIME(__startTime) (((float)clock() - (float)(__startTime)) / CLOCKS_PER_SEC)
#define COORD_COUNT 4

using std::string;
using std::vector;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::stringstream;

unsigned TestScanline();
unsigned TestZigZag();
unsigned TestBacktrack();
void CheckArgs(int argc, char *argv[]);
#pragma endregion

#pragma region Main
int main(int argc, char *argv[]) {
   CheckArgs(argc, argv);
   clock_t t1 = clock();

   printf("%0.3fs: Done.\n", TIME(t1));

   return 0;
}
#pragma endregion

#pragma region Tests
unsigned TestScanline() {
   unsigned overdraw = 0;

   return overdraw;
}

unsigned TestZigZag() {
   unsigned overdraw = 0;

   return overdraw;
}

unsigned TestBacktrack() {
   unsigned overdraw = 0;

   return overdraw;
}
#pragma endregion

#pragma region Helper Functions
void CheckArgs(int argc, char *argv[]){
   if(argc != 4) {
      fprintf(stderr, "Usage: %s traceFile output_file_name duplicate_line_range\n", argv[0]);
      exit(-1);
   }
   ifstream file1(argv[1]);
   if(!file1.good()) {
      fprintf(stderr, "Could not open file: %s\n", argv[1]);
      exit(-1);
   }
   if(atoi(argv[3]) < 0) {
      fprintf(stderr, "duplicate_line_range must be at least 0\n"
                      "0 to allow all duplicates\n"
                      "n to eliminate duplicates within n lines\n", argv[1]);
      exit(-1);
   }
}
#pragma endregion