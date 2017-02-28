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

#define TIME() (((float)clock() - (float)t1) / CLOCKS_PER_SEC)

using std::string;
using std::vector;
using std::cout;
using std::ifstream;
using std::ofstream;

void CheckArgs(int argc, char *argv[]);
#pragma endregion

#pragma region Main
int main(int argc, char *argv[]) {
   CheckArgs(argc, argv);
   
   const char geometryString[] = "glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB";
   const char coordString[] = "0x";
   ifstream inFile(argv[1]);
   ofstream outFile(argv[2]);
   string line;
   clock_t t1 = clock();

   outFile << std::hex;
   int i = 0;
   while (getline(inFile, line)) {
      size_t pos = line.find(geometryString, 0);
      if (pos != string::npos) {
         while ((pos = line.find(coordString, pos + 1)) != string::npos) {
            auto number = std::stoul(line.substr(pos), (size_t*)0, 16);
            outFile << number << " ";
         }
         outFile << "\n";
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
	if(argc != 3) {
		fprintf(stderr, "Usage: %s traceFile output_file_name\n", argv[0]);
		exit(-1);
	}
	ifstream file1(argv[1]);
	if(!file1.good()) {
		fprintf(stderr, "Could not open file: %s\n", argv[1]);
		exit(-1);
	}
}
#pragma endregion