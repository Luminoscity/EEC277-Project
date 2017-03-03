#pragma region Info
/*
 * GrabTriangles.cpp
 *
 * Program for extracting the vertex/triangle coordinates from an Attila game trace
 * Outputs a file with easy to read triangle coordinates, ready for the main application to import
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

#define TIME() (((float)clock() - (float)t1) / CLOCKS_PER_SEC)
#define COORD_COUNT 4

using std::string;
using std::vector;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::stringstream;

void CheckArgs(int argc, char *argv[]);
#pragma endregion

#pragma region Main
int main(int argc, char *argv[]) {
   CheckArgs(argc, argv);
   
   const char geometryString[] = "glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB";
   const char coordString[] = "0x";
   ifstream inFile(argv[1]);
   ofstream outFile(argv[2]);
   unsigned dupLines = atoi(argv[3]);
   string line;
   clock_t t1 = clock();

   outFile << std::hex << std::fixed << std::setprecision(9);
   int i = 0;
   vector<uint32_t> zero(COORD_COUNT, 0);
   vector<uint32_t> coords(COORD_COUNT, 0);
   vector<vector<uint32_t> > lines;
   while (getline(inFile, line)) {
      size_t pos = line.find(geometryString, 0);
      if (pos != string::npos) {
         int idx = 0;
         while ((pos = line.find(coordString, pos + 1)) != string::npos &&
                idx < COORD_COUNT) {
            uint32_t number = (uint32_t)std::stoul(line.substr(pos), (size_t*)0, 16);
            coords[idx++] = number;
         }
         if (dupLines == 0 || (coords != zero &&
             std::find(lines.begin(), lines.end(), coords) == lines.end())) {
            for (int j = 0; j < COORD_COUNT; ++j) {
               float f = *(float *)(&coords[j]);
               outFile << f << " ";
            }
            outFile << "\n";
         }
         if (dupLines) {
            lines.push_back(coords);
            if (lines.size() > dupLines)
               lines.erase(lines.begin());
         }
      }
      if (++i % 100000 == 0) {
         printf("%0.3fs: Processed line: %d\n", TIME(), i);
      }
   }
   printf("%0.3fs: Done.\n", TIME());

   return 0;
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