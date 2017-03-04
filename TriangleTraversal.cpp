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
   bool disjoint;
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
#define UCAST(__x) (static_cast<uint32_t>(__x))
#define FCAST(__x) (static_cast<float>(__x))
#define COORD_COUNT 4
#define RED_OFFSET 24
#define GRN_OFFSET 16
#define BLU_OFFSET 8

unsigned TestScanline(const TriList &geometry, FragList &fragments);
unsigned TestZigZag(const TriList &geometry, FragList &fragments);
unsigned TestBacktrack(const TriList &geometry, FragList &fragments);
void SnapToGrid(TriList &geometry, SystemInfo sys);
Color HexToColor(uint32_t hex);
TriList GetTriangles(ifstream &file, SystemInfo &sys);
void CheckTriangleCoordinates(TriList &geometry);
void CheckArgs(int argc, char *argv[]);
#pragma endregion

#pragma region Main
int main(int argc, char *argv[]) {
   CheckArgs(argc, argv);
   clock_t t1 = clock();
   SystemInfo sys = {UCAST(atoi(argv[2])), UCAST(atoi(argv[3])), false};
   ifstream testFile(argv[1]);

   TriList geometry = GetTriangles(testFile, sys);
   CheckTriangleCoordinates(geometry);
   cout << "Screen: " << sys.screenW << " x " << sys.screenH << "\n"
        << "Triangle Type: " << (sys.disjoint ? "Disjoint\n" : "Strips\n");

   printf("\n----------INPUT----------\n");
   int i = 1;
   for (auto& tri : geometry) {
      cout << "Triangle " << i++ << "\n";
      int j = 1;
      for (auto& vert : tri.v) {
         printf("---Vertex %d: %10.6f %10.6f %02X %02X %02X %02X\n",
                j++, vert.x, vert.y, vert.color.r, vert.color.g, vert.color.b,
                vert.color.a);
      }
   }

   SnapToGrid(geometry, sys);
   printf("\n----------Snapped to Grid----------\n");
   i = 1;
   for (auto& tri : geometry) {
      cout << "Triangle " << i++ << "\n";
      int j = 1;
      for (auto& vert : tri.v) {
         printf("---Vertex %d: %9.6f %9.6f %02X %02X %02X %02X\n",
            j++, vert.x, vert.y, vert.color.r, vert.color.g, vert.color.b,
            vert.color.a);
      }
   }

   printf("%0.3fs: Done.\n", TIME(t1));

   return 0;
}
#pragma endregion

#pragma region Tests
//
//
unsigned TestScanline(const TriList &geometry, FragList &fragments) {
   unsigned overdraw = 0;



   return overdraw;
}

//
//
unsigned TestZigZag(const TriList &geometry, FragList &fragments) {
   unsigned overdraw = 0;

   return overdraw;
}

//
//
unsigned TestBacktrack(const TriList &geometry, FragList &fragments) {
   unsigned overdraw = 0;

   return overdraw;
}
#pragma endregion

#pragma region Helper Functions
// Changes the given list of triangles so that each vertex lies on a grid location
// each pixel location has 25 allowed vertex coordinates
// X---X---X---X---X
// |               |
// X   X   X   X   X
// |               |
// X   X   X   X   X
// |               |
// X   X   X   X   X
// |               |
// X---X---X---X---X
// Input vertex coordinates expected to be in the range 0.0 - 1.0
// Output vertex coordinates will be on the scale 0.0 - ScreenWidth for x
// and 0.0 - ScreenHeight for y, both rounded to the nearest 0.25 pixels
void SnapToGrid(TriList &geometry, SystemInfo sys) {
   const float snapFactor = 4.0; // the maximum allowed fraction of a pixel length
   for (auto& tri : geometry) {
      for (auto& vert : tri.v) {
         uint32_t scaledXCoord = UCAST(vert.x * snapFactor * sys.screenW + 0.5);
         uint32_t scaledYCoord = UCAST(vert.y * snapFactor * sys.screenH + 0.5);
         vert.x = FCAST(scaledXCoord) / snapFactor;
         vert.y = FCAST(scaledYCoord) / snapFactor;
      }
   }
}

// converts a 32-bit integer representation of a coor value to a Color type
Color HexToColor(uint32_t hex) {
   Color color;

   color.r = (hex >> RED_OFFSET) & 0xFF;
   color.g = (hex >> GRN_OFFSET) & 0xFF;
   color.b = (hex >> BLU_OFFSET) & 0xFF;
   color.a = hex & 0xFF;

   return color;
}

// reads all the triangle vertex coordinates from a file
// expects disjoint format (3 vertices at a time): 
// 0.0456 0.1789 FF0000FF, 0.2452 0.3123 00FF00FF, 0.4789 0.5675 0000FFFF
// and strips format (1 vertex at a time):
// 0.0456 0.1789 FF0000FF
// 0.2452 0.3123 00FF00FF
// 0.4789 0.5675 0000FFFF
TriList GetTriangles(ifstream &file, SystemInfo &sys) {
   TriList triangles;
   uint32_t colorHex;
   string line;
   getline(file, line);
   sys.disjoint = toupper(line[0]) == 'D';
   
   if (sys.disjoint) {
      while (getline(file, line)) {
         if (line.find(",") == string::npos) {
            fprintf(stderr, "No commas found in list of disjoint triangles.\n");
            exit(-1);
         }
         stringstream ss;
         Triangle tri;
         ss.str(line);
         ss >> tri.v[0].x >> tri.v[0].y >> std::hex >> colorHex;
         tri.v[0].color = HexToColor(colorHex);
         ss.ignore(2, ',');
         ss >> tri.v[1].x >> tri.v[1].y >> std::hex >> colorHex;
         tri.v[1].color = HexToColor(colorHex);
         ss.ignore(2, ',');
         ss >> tri.v[2].x >> tri.v[2].y >> std::hex >> colorHex;
         tri.v[2].color = HexToColor(colorHex);
         triangles.push_back(tri);
      }
   }
   else {
      unsigned count = 0;
      while (getline(file, line)) {
         if (line.find(",") != string::npos) {
            fprintf(stderr, "Commas found in list of triangle strips.\n");
            exit(-1);
         }
         stringstream ss;
         Triangle tri;
         ss.str(line);
         if (count > 1) {
            tri.v[0] = tri.v[1];
            tri.v[1] = tri.v[2];
            ss >> tri.v[2].x >> tri.v[2].y >> std::hex >> colorHex;
            tri.v[2].color = HexToColor(colorHex);
            triangles.push_back(tri);
         }
         else {
            ss >> tri.v[count].x >> tri.v[count].y >> std::hex >> colorHex;
            tri.v[count].color = HexToColor(colorHex);
         }
         ++count;
      }
   }

   return triangles;
}

//checks that the x and y vertex coordinates are in the range 0.0 - 1.0
void CheckTriangleCoordinates(TriList &geometry) {
   for (auto& tri : geometry) {
      for (auto& vert : tri.v) {
         if (vert.x < 0.0 || vert.x > 1.0) {
            fprintf(stderr, "Vertex coordinate %f outside the range 0.0-1.0\n",
                    vert.x);
            exit(-1);
         }
         if (vert.y < 0.0 || vert.y > 1.0) {
            fprintf(stderr, "Vertex coordinate %f outside the range 0.0-1.0\n",
                    vert.y);
            exit(-1);
         }
      }
   }
}

// checks command-line arguments for validity
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