#pragma region Info
/*
 * TriangleTraversal.cpp
 *
 * Tim Ambrose and Angela Tobin
 * March 2017
 * Tests the following rasterization paths (triangle traversal methods)
 * against an inputted set of triangle vertex coordinates:
 * Scanline, ZigZag, Backtrack
 */
#pragma endregion

#pragma region Includes
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

using std::string;
using std::vector;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::stringstream;
#pragma endregion

#pragma region Types
/*typedef struct Coordinate {
   float x;
   float y;
} Coordinate;*/

typedef struct SystemInfo {
   uint32_t screenW;
   uint32_t screenH;
} SystemInfo;

typedef struct Color {
   uint8_t r;
   uint8_t g;
   uint8_t b;
   uint8_t a;
} Color;

typedef struct Vertex {
   float x;
   float y;
   Color color;
} Vertex;

typedef struct Fragment {
   uint32_t x;
   uint32_t y;
   Color color;
} Fragment;

typedef struct Triangle {
   Vertex v[3];
} Triangle;

typedef vector<Triangle> TriList;
typedef vector<Fragment> FragList;
#pragma endregion

#pragma region Declarations
#define TIME(__startTime) (((float)clock() - (float)(__startTime)) / CLOCKS_PER_SEC)
#define COORD_COUNT 4
#define RED_OFFSET 6
#define GRN_OFFSET 4
#define BLU_OFFSET 2

unsigned TestScanline(const TriList &geometry, FragList &fragments);
unsigned TestZigZag(const TriList &geometry, FragList &fragments);
unsigned TestBacktrack(const TriList &geometry, FragList &fragments);
void SnapToGrid(TriList &geometry, SystemInfo sys);
Color HexToColor(uint32_t hex);
void CheckArgs(int argc, char *argv[]);
#pragma endregion

#pragma region Main
int main(int argc, char *argv[]) {
   CheckArgs(argc, argv);
   clock_t t1 = clock();
   SystemInfo sys = {(uint32_t)atoi(argv[2]), (uint32_t)atoi(argv[3])};

   cout << "Screen: " << sys.screenW << " x " << sys.screenH << "\n";

   printf("%0.3fs: Done.\n", TIME(t1));

   return 0;
}
#pragma endregion

#pragma region Tests
unsigned TestScanline(const TriList &geometry, FragList &fragments) {
   unsigned overdraw = 0;



   return overdraw;
}

unsigned TestZigZag(const TriList &geometry, FragList &fragments) {
   unsigned overdraw = 0;

   return overdraw;
}

unsigned TestBacktrack(const TriList &geometry, FragList &fragments) {
   unsigned overdraw = 0;

   return overdraw;
}
#pragma endregion

#pragma region Helper Functions
// changes the given list of triangles so that each vertex lies on a grid location
// 
void SnapToGrid(TriList &geometry, SystemInfo sys) {

}

Color HexToColor(uint32_t hex) {
   Color color;

   color.r = (hex >> RED_OFFSET) & 0xFF;
   color.g = (hex >> GRN_OFFSET) & 0xFF;
   color.b = (hex >> BLU_OFFSET) & 0xFF;
   color.a = hex & 0xFF;

   return color;
}

void CheckArgs(int argc, char *argv[]){
   if(argc != 4) {
      fprintf(stderr, "Usage: %s geometryFile screenWidth screenHeight\n", argv[0]);
      exit(-1);
   }
   ifstream file1(argv[1]);
   if(!file1.good()) {
      fprintf(stderr, "Could not open file: %s\n", argv[1]);
      exit(-1);
   }
   if(atoi(argv[2]) < 1 || atoi(argv[3]) < 1) {
      fprintf(stderr, "Screen Width and Height must be greater than 0.\n");
      exit(-1);
   }
}
#pragma endregion