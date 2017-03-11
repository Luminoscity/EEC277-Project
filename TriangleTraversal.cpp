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

/* Crow's Algorithm Credit: Michigan State University
  www.cse.msu.edu/~cse872/rasterization */
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

typedef enum PrintType {
  P_HUMAN, P_DEBUG, P_CSV
} PrintType;

typedef struct SystemInfo {
  uint32_t screenW;
  uint32_t screenH;
  bool disjoint;
  PrintType pnt;
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

typedef struct Inside {
  float l_Ei, l_dX, l_dY, r_Ei, r_dX, r_dY;
} Inside;

typedef vector<Triangle> TriList;
typedef vector<Fragment> FragList;
typedef unsigned (*TestPtr)(const TriList &, vector<FragList> &, SystemInfo);
#pragma endregion

#pragma region Declarations
#define TIME(__startTime) (((float)clock() - (float)(__startTime)) / CLOCKS_PER_SEC)
#define UCAST(__x) (static_cast<uint32_t>(__x))
#define FCAST(__x) (static_cast<float>(__x))
#define ICAST(__x) (static_cast<int>(__x))
#define BCAST(__x) (static_cast<uint8_t>(__x))
#define NUM_TESTS 3
#define RED_OFFSET 24
#define GRN_OFFSET 16
#define BLU_OFFSET 8

unsigned TestScanline(const TriList &geometry, vector<FragList> &fragments,
                      SystemInfo sys);
unsigned TestZigZag(const TriList &geometry, vector<FragList> &fragments,
                    SystemInfo sys);
unsigned TestBacktrack(const TriList &geometry, vector<FragList> &fragments,
                       SystemInfo sys);
/*float dX(Vertex v1, Vertex v2);
float dY(Vertex v1, Vertex v2);*/
void decrement(Vertex &vert, Vertex &dV);
float Ei(const Vertex &v1, const Vertex &v2, float x, float y);
unsigned backtrack(const Vertex &left, const Vertex &right, int &x, int y,
                   Inside &inside, FragList &out);
unsigned zigzag(const Vertex &left, const Vertex &right, int &x, int y,
                Inside &Ei, bool toRight, FragList &out);
void Ei(const Vertex &v1, const Vertex &v2, Vertex &Ei, Vertex &dEi, float d,
        float f);
void increment(Vertex &vert, Vertex &dV);
unsigned scanline(const Vertex &left, const Vertex &right, int y, FragList &out);
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
  vector<string> testFileNames;
  SystemInfo sys = { UCAST(atoi(argv[2])), UCAST(atoi(argv[3])), false,
               P_HUMAN };
  string testStrings[] = { "Scanline", "Backtrack", "ZigZag" };

  if (argc > 5) {
    char printing = toupper(argv[5][0]);
    sys.pnt = printing == 'D' ? P_DEBUG : printing == 'H' ? P_HUMAN : P_CSV;
  }

  if (sys.pnt == P_CSV)
     cout << "Screen," << sys.screenW << "," << sys.screenH << "\n"
          << "Data File,Triangle Type,Scanline,Backtrack,ZigZag\n";
  else
     cout << "Screen: " << sys.screenW << " x " << sys.screenH << "\n"
          << "Debug Output: " << (sys.pnt ? "Yes\n" : "No\n");

  string dataset;
  testFileNames.push_back(argv[1]);
  int fileIdx = 0;
  ifstream testFile(testFileNames[fileIdx]);
  getline(testFile, dataset);
  TriList origGeometry = GetTriangles(testFile, sys);
  TriList geometry = origGeometry;
  CheckTriangleCoordinates(origGeometry);

  SnapToGrid(geometry, sys);   //geometry modified


  if (sys.pnt == P_DEBUG) {
    printf("\n-----------------INPUT------------------------"
         "----------------Snapped to Grid--------------\n");
    TriList *og = &origGeometry;
    TriList *g = &geometry;
    for (unsigned i = 0; i < g->size(); ++i) {
      printf("Triangle %-35d Triangle %d    ", i, i);
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
  geometry.erase(std::remove_if(geometry.begin(), geometry.end(), isAreaZero),
            geometry.end());

  TestPtr tests[] = {&TestScanline, &TestBacktrack, &TestZigZag};
  int test = atoi(argv[4]);
  vector<FragList> outputs[NUM_TESTS];
  unsigned results[NUM_TESTS];
  if (sys.pnt == P_CSV)
    cout << dataset << ","
         << (sys.disjoint ? "Disjoint," : "Strips,");
  else
    cout << "---------" << dataset << "---------\n"
         << "Triangle Type: " << (sys.disjoint ? "Disjoint\n" : "Strips\n");

  if (test == 0) {
    for (int i = 0; i < NUM_TESTS; ++i) {
      if (sys.pnt == P_DEBUG) {
        printf("%0.3fs: Running Test: ", TIME(t1));
        cout << testStrings[i] << "\n";
      }
      results[i] = tests[i](geometry, outputs[i], sys);    //run the test
      if (sys.pnt == P_DEBUG)
        printf("%0.3fs: ", TIME(t1));
      if (sys.pnt == P_CSV)
        cout << results[i] << (i < NUM_TESTS - 1 ? "," : "\n");
      else
        cout << "Overdraw for " << testStrings[i] << ": " << results[i]
             << "\n"; 
      if (sys.pnt == P_DEBUG) {
        printf("------------Fragments------------\n");
        for (unsigned t = 0; t < outputs[i].size(); ++t) {
          printf("Triangle %d\n", t);
          for (unsigned frag = 0; frag < outputs[i][t].size(); ++frag)
            printf("---Fragment %3d: x=%-3u y=%-3u c=%02X %02X %02X %02X"
                   "\n", frag, outputs[i][t][frag].x, outputs[i][t][frag].y,
                   outputs[i][t][frag].color.r, outputs[i][t][frag].color.g,
                   outputs[i][t][frag].color.b, outputs[i][t][frag].color.a);
        }
        printf("\n");
      }
    }
  }
  else {
    --test;
    if (sys.pnt == P_DEBUG) {
      printf("%0.3fs: Running Test: ", TIME(t1));
      cout << testStrings[test] << "\n";
    }
    results[test] = tests[test](geometry, outputs[test], sys);  //run test
    if (sys.pnt == P_DEBUG)
      printf("%0.3fs:  ", TIME(t1));
    if (sys.pnt == P_CSV)
      cout << testStrings[test] << "\n" << results[test] << "\n";
    else
      cout << "Overdraw for " << testStrings[test] << ": " << results[test]
           << "\n"; 
  }

  if (sys.pnt != P_CSV)
    printf("%0.3fs: Done.\n", TIME(t1));

  return 0;
}
#pragma endregion

#pragma region Tests
// Traverses the triangle from bottom (or bottom-left)
// to top using left-to-right scanlines beginning at the left edges
// The first vertex is already in place to be determine the first pixel we
// check because of MakeRrightHandedTriangle.
// Code based on Michigan State University's psudocode
// for Crow's Algorithm  www.cse.msu.edu/~cse872/rasterization
unsigned TestScanline(const TriList &geometry, vector<FragList> &fragments,
               SystemInfo sys) {
  unsigned overdraw = 0;
  int li, ri;   //left and right upper endpoint indices
  int ly, ry;   //left and right upper endpoint y values
  Vertex l, dl;  //current left edge and dX
  Vertex r, dr;  //current right edge and dX
  int y;      //current scanline

  for (auto& tri : geometry) {  //compute fragments for every triangle
    int i = 0;    //lowest vertex index (setup in MakeRightHandedTriangle)
    li = ri = i;
    ly = ry = y = ICAST(tri.v[i].y) + 1;
    FragList thisTriangle;            //fragments for this triangle
    fragments.push_back(thisTriangle);    //add to list of all fragments

    //scanline algorithm
    for (int remain = 3; remain > 0;) {
      while (ly <= y &&  remain > 0) {   //find next left edge
        --remain;
        //clockwise since triangle is specified in right-handed order
        i = (li - 1 < 0) ? 2 : li - i;
        ly = ICAST(tri.v[i].y) + 1;
        if (ly > y)   //replace left edge
          Ei(tri.v[li], tri.v[i], l, dl, tri.v[i].y - tri.v[li].y,
            y - tri.v[li].y);
        li = i;
      }
      while (ry <= y &&  remain > 0) {   //find next right edge
        --remain;
        //counter-clockwise since triangle is in right-handed order
        i = (ri + 1) % 3;
        ry = ICAST(tri.v[i].y) + 1;
        if (ry > y)   //replace right edge
          Ei(tri.v[ri], tri.v[i], r, dr, tri.v[i].y - tri.v[ri].y,
            y - tri.v[ri].y);
        ri = i;
      }

      for (; y < ly && y < ry; ++y) {    //draw spans
        //scan and interpolate by edges
        overdraw += scanline(l, r, y-1, fragments.back());
        increment(l, dl);
        increment(r, dr);
      }
    }
  }

  return overdraw;
}

// Traverses the triangle from bottom to top, left to right only in each row
// Must find the far left edge every time a new row is processed
unsigned TestBacktrack(const TriList &geometry, vector<FragList> &fragments,
                SystemInfo sys) {
  unsigned overdraw = 0;
  int li, ri;   //left and right upper endpoint indices
  int ly, ry;   //left and right upper endpoint y values
  Vertex l,    //color interpolant for left edge
       dl;    //dY for color
  Vertex r,    //color interpolant for right edge
       dr;    //dY for color
  int x, y;    //current colomn and row
  Inside inside;

  for (auto& tri : geometry) {  //compute fragments for every triangle
    int i = 0;    //lowest vertex index (setup in MakeRightHandedTriangle)
    li = ri = i;
    ly = ry = y = ICAST(tri.v[i].y) + 1;
    x = ICAST(tri.v[i].x) + 1;
    FragList thisTriangle;            //fragments for this triangle
    fragments.push_back(thisTriangle);    //add to list of all fragments

    for (int remain = 3; remain > 0;) {
      while (ly <= y &&  remain > 0) {   //find next left edge
        --remain;
        //clockwise since triangle is specified in right-handed ordr
        i = (li - 1 < 0) ? 2 : li - i;
        ly = ICAST(tri.v[i].y) + 1;
        if (ly > y) {   //replace left edge
          inside.l_Ei = Ei(tri.v[i], tri.v[li], x - FCAST(0.5),
                      y - FCAST(0.5));
          inside.l_dX = tri.v[li].x - tri.v[i].x;
          inside.l_dY = tri.v[li].y - tri.v[i].y;
          Ei(tri.v[li], tri.v[i], l, dl, -inside.l_dY, y - tri.v[li].y);
        }
        li = i;
      }
      while (ry <= y &&  remain > 0) {   //find next right edge
        --remain;
        //counter-clockwise since triangle is in right-handed ordr
        i = (ri + 1) % 3;
        ry = ICAST(tri.v[i].y) + 1;
        if (ry > y) {  //replace right edge
          inside.r_Ei = Ei(tri.v[ri], tri.v[i], x - FCAST(0.5),
                      y - FCAST(0.5));
          inside.r_dX = tri.v[i].x - tri.v[ri].x;
          inside.r_dY = tri.v[i].y - tri.v[ri].y;
          Ei(tri.v[ri], tri.v[i], r, dr, inside.r_dY, y - tri.v[ri].y);
        }
        ri = i;
      }

      for (; y < ly && y < ry; ++y) { //while left edge and right edge are
                            //endpoints of current horizonal row
                            //scan and interpolate by edges
        overdraw += backtrack(l, r, x, y, inside, fragments.back());
        increment(l, dl);        //increment left color interpolant
        increment(r, dr);        //increment right color interpolant
        inside.l_Ei -= inside.l_dX;  //increment left edge Ei(x, y+1)
        inside.r_Ei -= inside.r_dX;  //increment right edge Ei(x, y+1)
      }
    }
  }

  return overdraw;
}

// Traverses the triangle from bottom to top, first right, then left,
// then right, and so on, changing directions for each new row.
// Seems more complicated thn the Scanline algorithm, but that is only because
// we kept all the color interpolation logic the same and added the
// insidedness testing logic, ignoring the pre-calculated x edge boundaries
// that come with the color interpolation logic
unsigned TestZigZag(const TriList &geometry, vector<FragList> &fragments,
              SystemInfo sys) {
  unsigned overdraw = 0;
  int li, ri;   //left and right upper endpoint indices
  int ly, ry;   //left and right upper endpoint y values
  Vertex l,    //color interpolant for left edge
       dl;    //dY for color
  Vertex r,    //color interpolant for right edge
       dr;    //dY for color
  int x, y;    //current colomn and row
  Inside inside;

  for (auto& tri : geometry) {  //compute fragments for every triangle
    int i = 0;    //lowest vertex index (setup in MakeRightHandedTriangle)
    li = ri = i;
    ly = ry = y = ICAST(tri.v[i].y) + 1;
    x = ICAST(tri.v[i].x) + 1;
    bool toRight = true;
    FragList thisTriangle;            //fragments for this triangle
    fragments.push_back(thisTriangle);    //add to list of all fragments

    for (int remain = 3; remain > 0;) {
      while (ly <= y &&  remain > 0) {   //find next left edge
        --remain;
        //clockwise since triangle is specified in right-handed ordr
        i = (li - 1 < 0) ? 2 : li - i;
        ly = ICAST(tri.v[i].y) + 1;
        if (ly > y) {   //replace left edge
          inside.l_Ei = Ei(tri.v[i], tri.v[li], x - FCAST(0.5),
                      y - FCAST(0.5));
          inside.l_dX = tri.v[li].x - tri.v[i].x;
          inside.l_dY = tri.v[li].y - tri.v[i].y;
          Ei(tri.v[li], tri.v[i], l, dl, -inside.l_dY, y - tri.v[li].y);
        }
        li = i;
      }
      while (ry <= y &&  remain > 0) {   //find next right edge
        --remain;
        //counter-clockwise since triangle is in right-handed ordr
        i = (ri + 1) % 3;
        ry = ICAST(tri.v[i].y) + 1;
        if (ry > y) {  //replace right edge
          inside.r_Ei = Ei(tri.v[ri], tri.v[i], x - FCAST(0.5),
                      y - FCAST(0.5));
          inside.r_dX = tri.v[i].x - tri.v[ri].x;
          inside.r_dY = tri.v[i].y - tri.v[ri].y;
          Ei(tri.v[ri], tri.v[i], r, dr, inside.r_dY, y - tri.v[ri].y);
        }
        ri = i;
      }

      for (; y < ly && y < ry; ++y) {//while left edge and right edge are
                           //the endpoints of current horizonal row
        //scan and interpolate by edges
        overdraw += zigzag(l, r, x, y, inside, toRight, fragments.back());
        increment(l, dl);        //increment left color interpolant
        increment(r, dr);        //increment right color interpolant
        inside.l_Ei -= inside.l_dX;  //increment left edge Ei(x, y+1)
        inside.r_Ei -= inside.r_dX;  //increment right edge Ei(x, y+1)
        toRight = !toRight;
      }
    }
  }

  return overdraw;
}
#pragma endregion

#pragma region Test Related Functions
// Calulates the insidedness of pixel location x,y for the edge
// connecting vertices v1 and v2
float Ei(const Vertex &v1, const Vertex &v2, float x, float y) {
  return (x - v1.x) * (v2.y - v1.y) - (y - v1.y) * (v2.x - v1.x);
}

unsigned backtrack(const Vertex &left, const Vertex &right, int &x, int y,
             Inside &inside, FragList &out) {
  unsigned overdraw = 0;    //overdraw for 
  int lx = ICAST(left.x) + 1,
     rx = ICAST(right.x) + 1;
  Vertex s, ds;    //Interpolated color

  if (inside.l_Ei <= 0.0) {  //within the bounds of the left edge
    //find the x coordinate just inside of the left edge
    for (; inside.l_Ei <= 0.0; --x) {
      inside.l_Ei -= inside.l_dY;    //Ei(x-1, y) left
      inside.r_Ei -= inside.r_dY;    //Ei(x-1, y) right
      ++overdraw;  //checking insidedness, but not yet outputting fragments
    }
    inside.l_Ei += inside.l_dY;      //Ei(x+1, y) left
    inside.r_Ei += inside.r_dY;      //Ei(x+1, y) right
    ++x;    //go right to the first pixel center inside the triangle
  }

  //Interpolate color
  Ei(left, right, s, ds, right.x - left.x, lx - left.x);
  for (; inside.r_Ei < 0.0; ++x) {  //within the bounds of the right edge
    if (inside.l_Ei <= 0.0) {
      Fragment frag = { UCAST(x - 1), UCAST(y - 1), s.color };
      out.push_back(frag);
      increment(s, ds);        //increment color interpolants
    }
    else
      ++overdraw; //started from pixel location to the left of the triangle
    inside.l_Ei += inside.l_dY;
    inside.r_Ei += inside.r_dY;
  }
  ++overdraw;   //stopped on a pixel we are not generating a fragment for

  return overdraw;
}

unsigned zigzag(const Vertex &left, const Vertex &right, int &x, int y,
           Inside &inside, bool toRight, FragList &out) {
  unsigned overdraw = 0;
  int lx = ICAST(left.x) + 1,
     rx = ICAST(right.x) + 1;
  Vertex s, ds;    //Interpolated color

  if (toRight) {         //traversing to the right
    if (inside.l_Ei <= 0.0) {  //within the bounds of the left edge
      //find the x coordinate just inside of the left edge
      for (; inside.l_Ei <= 0.0; --x) {
        inside.l_Ei -= inside.l_dY;
        inside.r_Ei -= inside.r_dY;
        ++overdraw;
      }
      inside.l_Ei += inside.l_dY;
      inside.r_Ei += inside.r_dY;
      ++x;
    }

    //Interpolate color
    Ei(left, right, s, ds, right.x - left.x, lx - left.x);
    for (; inside.r_Ei < 0.0; ++x) {  //within the bounds of the right edge
      if (inside.l_Ei <= 0.0) {
        Fragment frag = {UCAST(x-1), UCAST(y-1), s.color};
        out.push_back(frag);
        increment(s, ds);       //increment color interpolants
      }
      else
        ++overdraw;
      inside.l_Ei += inside.l_dY;
      inside.r_Ei += inside.r_dY;
    }
    ++overdraw;   //stopped on a pixel we are not generating a fragment for
  }
  else {              //traversing to the left
    if (inside.r_Ei < 0.0) {   //within the bounds of the right edge
      //find the x coordinate just inside of the right edge
      for (; inside.r_Ei < 0.0; ++x) {
        inside.l_Ei += inside.l_dY;
        inside.r_Ei += inside.r_dY;
        ++overdraw;
      }
      inside.l_Ei -= inside.l_dY;
      inside.r_Ei -= inside.r_dY;
      --x;
    }

    //Interpolate color
    Ei(left, right, s, ds, right.x - left.x, rx - right.x);
    for (; inside.l_Ei <= 0.0; --x) {  //within the bounds of the left edge
      if (inside.r_Ei < 0.0) {
        Fragment frag = {UCAST(x-1), UCAST(y-1), s.color};
        out.push_back(frag);
        decrement(s, ds);   //increment color interpolants
      }
      else
        ++overdraw;
      inside.l_Ei -= inside.l_dY;
      inside.r_Ei -= inside.r_dY;
    }
    ++overdraw;   //stopped on a pixel we are not generating a fragment for
  }

  return overdraw;
}

void Ei(const Vertex &v1, const Vertex &v2, Vertex &Ei, Vertex &dEi, float d,
      float f) {
  dEi.x = (v2.x - v1.x) / d;
  Ei.x = v1.x + f * dEi.x;
  dEi.color.r = BCAST((v2.color.r - v1.color.r) / d);
  Ei.color.r = BCAST(v1.color.r + f * dEi.color.r);
  dEi.color.g = BCAST((v2.color.g - v1.color.g) / d);
  Ei.color.g = BCAST(v1.color.g + f * dEi.color.g);
  dEi.color.b = BCAST((v2.color.b - v1.color.b) / d);
  Ei.color.b = BCAST(v1.color.b + f * dEi.color.b);
  dEi.color.a = BCAST((v2.color.a - v1.color.a) / d);
  Ei.color.a = BCAST(v1.color.a + f * dEi.color.a);
}

// used by ZigZag to interpolate colors from right to left
void decrement(Vertex &vert, Vertex &dV) {
  vert.x -= dV.x;
  vert.color.r -= dV.color.r;
  vert.color.g -= dV.color.g;
  vert.color.b -= dV.color.b;
  vert.color.a -= dV.color.a;
}

// interpolate colors and insidedness one pixel position up or right
// from current position
void increment(Vertex &vert, Vertex &dV) {
  vert.x += dV.x;
  vert.color.r += dV.color.r;
  vert.color.g += dV.color.g;
  vert.color.b += dV.color.b;
  vert.color.a += dV.color.a;
}

unsigned scanline(const Vertex &left, const Vertex &right, int y, FragList &out) {
  unsigned overdraw = 0;
  int x,
     lx = ICAST(left.x),
     rx = ICAST(right.x);
  Vertex s, ds;

  if (lx < rx) {
    Ei(left, right, s, ds, right.x - left.x, lx - left.x);
    for (x = lx; x < rx; ++x) {
      Fragment frag = {UCAST(x), UCAST(y), s.color};
      out.push_back(frag);
      increment(s, ds);
    }
  }
  else
    ++overdraw;

  return overdraw;
}
#pragma endregion

#pragma region Helper Functions
// Changes the given list of triangles so that each vertex lies on a grid
// location each pixel location has 25 allowed vertex coordinates
// X---X---X---X---X
// |          |
// X  X  X  X  X
// |          |
// X  X  X  X  X
// |          |
// X  X  X  X  X
// |          |
// X---X---X---X---X
// Input vertex coordinates expected to be in the range 0.0 - 1.0
// Output vertex coordinates will be on the scale 0.0 - ScreenWidth for x
// and 0.0 - ScreenHeight for y, both rounded to the nearest 0.25 pixels
void SnapToGrid(TriList &geometry, SystemInfo sys) {
  const float snapFactor = 4.0; // maximum allowed fraction of a pixel length
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
          " [debug_human_or_csv]\n%s", argv[0], tests);
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
  char printChar = toupper(argv[5][0]);
  if (argc > 5 && !(printChar == 'D' || printChar == 'H' || printChar == 'C')) {
    fprintf(stderr, "Invalid output printing type: %s\nValid arguments:\n"
          "D(debug): full debug output\n"
          "H(human): abbriviated human-readable output\n"
          "C(csv): comma-separated-value output\n", argv[5]);
    exit(-1);
  }
}
#pragma endregion
