#pragma region Info
/*
 * TriangleTraversal.cpp
 *
 * Tim Ambrose and Angela Tobin
 * March 2017
 * Tests the following rasterization paths (triangle traversal methods)
 * against an inputted set of triangle vertex coordinates:
 * Scanline, Backtrack, ZigZag
 */
#pragma endregion

#pragma region Includes
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <limits>
#include <vector>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using std::string;
using std::vector;
using std::cout;
using std::setw;
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
   bool printDebug;
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
typedef unsigned (*TestPtr)(const TriList &, FragList &, SystemInfo);
#pragma endregion

#pragma region Declarations
#define TIME(__startTime) (((float)clock() - (float)(__startTime)) / CLOCKS_PER_SEC)
#define UCAST(__x) (static_cast<uint32_t>(__x))
#define FCAST(__x) (static_cast<float>(__x))
#define NUM_TESTS 3
#define RED_OFFSET 24
#define GRN_OFFSET 16
#define BLU_OFFSET 8

unsigned TestScanline(const TriList &geometry, FragList &fragments,
                      SystemInfo sys);
unsigned TestZigZag(const TriList &geometry, FragList &fragments,
                    SystemInfo sys);
unsigned TestBacktrack(const TriList &geometry, FragList &fragments,
                       SystemInfo sys);
float dX(Vertex v1, Vertex v2);
float dY(Vertex v1, Vertex v2);
float Ei(Vertex triV1, Vertex triV2, Vertex pixel);
void SnapToGrid(TriList &geometry, SystemInfo sys);
Color HexToColor(uint32_t hex);
void MakeRightHandedTriangle(Triangle &tri);
double TriangleArea(Triangle tri);
bool isAreaZero(Triangle);
TriList GetTriangles(ifstream &file, SystemInfo &sys);
void CheckTriangleCoordinates(TriList &geometry);
void CheckArgs(int argc, char *argv[]);
#pragma endregion

#pragma region Main
int main(int argc, char *argv[]) {
   CheckArgs(argc, argv);
   clock_t t1 = clock();
   SystemInfo sys = {UCAST(atoi(argv[2])), UCAST(atoi(argv[3])), false, false};
   ifstream testFile(argv[1]);

   if (argc > 5)
      sys.printDebug = toupper(argv[5][0]) == 'Y';

   TriList origGeometry = GetTriangles(testFile, sys);
   TriList geometry = origGeometry;
   CheckTriangleCoordinates(origGeometry);
   cout << "Screen: " << sys.screenW << " x " << sys.screenH << "\n"
        << "Triangle Type: " << (sys.disjoint ? "Disjoint\n" : "Strips\n")
        << "Debug Output: " << (sys.printDebug ? "Yes\n" : "No\n");

   SnapToGrid(geometry, sys);    //geometry modified
   

   if (sys.printDebug) {
      printf("\n-----------------INPUT------------------------"
             "----------------Snapped to Grid--------------\n");
      TriList *og = &origGeometry;
      TriList *g = &geometry;
      for (unsigned i = 0; i < g->size(); ++i) {
         printf("Triangle %-37d Triangle %d    ", i, i);
         cout << "Area: " << TriangleArea((*g)[i]) << "\n";
         for (int j = 0; j < 3; ++j)
            printf("---Vertex %d: %8.6f %8.6f %02X %02X %02X %02X   "
                   "---Vertex %d: %10.6f %10.6f %02X %02X %02X %02X\n",
                   j, (*og)[i].v[j].x, (*og)[i].v[j].y, (*og)[i].v[j].color.r,
                   (*og)[i].v[j].color.g, (*og)[i].v[j].color.b,
                   (*og)[i].v[j].color.a, j, (*g)[i].v[j].x, (*g)[i].v[j].y,
                   (*g)[i].v[j].color.r, (*g)[i].v[j].color.g,
                   (*g)[i].v[j].color.b, (*g)[i].v[j].color.a);
      }
      printf("\n");
   }

   // remove all zero area triangles
   geometry.erase(std::remove_if(geometry.begin(), geometry.end(), isAreaZero), geometry.end());

   TestPtr tests[] = {&TestScanline, &TestBacktrack, &TestZigZag};
   string testStrings[] = {"Scanline", "Backtrack", "ZigZag"};
   int test = atoi(argv[4]);
   FragList outputs[NUM_TESTS];
   unsigned results[NUM_TESTS];
   if (test == 0) {
      for (int i = 0; i < NUM_TESTS; ++i) {
         results[i] = tests[i](geometry, outputs[i], sys);
         cout << "Overdraw for " << testStrings[i] << ": " << results[i]
              << "\n";
      }
   }
   else {
      --test;
      results[test] = tests[test](geometry, outputs[test], sys);
      cout << "Overdraw for " << testStrings[test] << ": " << results[test]
           << "\n";
   }

   printf("%0.3fs: Done.\n", TIME(t1));

   return 0;
}
#pragma endregion

#pragma region Tests
//
//
unsigned TestScanline(const TriList &geometry, FragList &fragments,
                      SystemInfo sys) {
   unsigned overdraw = 0;
   
   
   
   return overdraw;
}

//
//
unsigned TestBacktrack(const TriList &geometry, FragList &fragments,
                       SystemInfo sys) {
   unsigned overdraw = 0;
   
   return overdraw;
}

//
//
unsigned TestZigZag(const TriList &geometry, FragList &fragments,
                    SystemInfo sys) {
   unsigned overdraw = 0;
   
   return overdraw;
}
#pragma endregion

#pragma region Test Related Functions
float dX(Vertex v1, Vertex v2) {
   return v2.x - v1.x;
}

float dY(Vertex v1, Vertex v2) {
   return v2.y - v1.y;
}

float Ei(Vertex triV1, Vertex triV2, Vertex pixel) {
   return (pixel.x - triV1.x) * (triV2.y - triV1.y) -
          (pixel.y - triV1.y) * (triV2.x - triV1.x);
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
   vector<TriList::iterator> toErase;
   for (TriList::iterator it = geometry.begin(); it != geometry.end(); it++) {
      for (auto& vert : it->v) {
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

// rearranges the order of the vertices so that going from vertex 1 to 2 to 3
// forms a triangle in a counter-clockwise direction (right-handed)
// and so that the first vertex has the lowest y value
// for a tie, the vertex closest to the origin will used for the first vertex
void MakeRightHandedTriangle(Triangle &tri) {
   Vertex vert;
   int lowestV = 0;

   for (int i = 1; i < 3; ++i)
      if (tri.v[i].y < tri.v[lowestV].y ||
         (tri.v[i].y == tri.v[lowestV].y && tri.v[i].x < tri.v[lowestV].x)) {
         lowestV = i;
      }

   // make the first vertex the lowest vertex
   vert = tri.v[0];
   tri.v[0] = tri.v[lowestV];
   tri.v[lowestV] = vert;

   float ax = tri.v[1].x - tri.v[0].x,
         ay = tri.v[1].y - tri.v[0].y,
         bx = tri.v[2].x - tri.v[1].x,
         by = tri.v[2].y - tri.v[1].y;

   if (ax * by < ay * bx) {  //cross product
      vert = tri.v[1];
      tri.v[1] = tri.v[2];
      tri.v[2] = vert;
   }

   assert((tri.v[1].x - tri.v[0].x) * (tri.v[2].y - tri.v[1].y) >=
          (tri.v[1].y - tri.v[0].y) * (tri.v[2].x - tri.v[1].x));
}

// returns area of the triangle
double TriangleArea(Triangle tri) {
   return abs((tri.v[0].x * (tri.v[1].y - tri.v[2].y) +
               tri.v[1].x * (tri.v[2].y - tri.v[0].y) +
               tri.v[2].x * (tri.v[0].y - tri.v[1].y)) / 2.0);
}

bool isAreaZero(Triangle tri) {
   return TriangleArea(tri) == 0.0;
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
         MakeRightHandedTriangle(tri);
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
            MakeRightHandedTriangle(tri);
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
   char tests[] = "Tests:\n1: Scanline\n2: Backtrack\n3: ZigZag\n"
                  "0: All\n";
   const int numTests = 3;
   if (argc < 5 || argc > 6) {
      fprintf(stderr, "Usage: %s geometryFile screenWidth screenHeight testNum"
              "[debugY/N]\n%s", argv[0], tests);
      exit(-1);
   }
   ifstream file1(argv[1]);
   if (!file1.good()) {
      fprintf(stderr, "Could not open file: %s\n", argv[1]);
      exit(-1);
   }
   if (atoi(argv[2]) < 1 || atoi(argv[3]) < 1) {
      fprintf(stderr, "Screen Width and Height must be greater than 0.\n");
      exit(-1);
   }
   if (atoi(argv[4]) < 0 || atoi(argv[4]) > numTests) {
      fprintf(stderr, "Invalid test number: %d\n%s", atoi(argv[4]), tests);
      exit(-1);
   }
}
#pragma endregion