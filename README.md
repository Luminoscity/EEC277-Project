# EEC277-Project  
  
-------------Example run of GrabTriangles------------------  
GrabTriangles.exe D:\Downloads\doom3-640x480\tracefile.txt triangles.txt 20
6.420s: Processed line: 100000  
12.788s: Processed line: 200000  
19.223s: Processed line: 300000  
25.657s: Processed line: 400000  
...  
1602.447s: Processed line: 24200000  
1607.650s: Done.  

Will ignore all 0,0,0,0 entries and ignore duplicates within a single frame within n lines of each other in the tracefile



NOTE: Commented out lines (//) in your FilesToTest master file indicate datasets to skip.
You will have to modify the FilesToTest file with your name on it to fit the directories in your system.
All test files updated to include name of the dataset in the first line.

-------------Example HUMAN run of TriangleTraversal------------------  
TriangleTraversal.exe  D:\Documents\EEC277-Project\FilesToTest-Tim.txt 640 480 0 human
Screen: 640 x 480
Debug Output: No

---------------2 Easy Triangles---------------
Triangle Type: Disjoint
Overdraw for Scanline: 1
Overdraw for Backtrack: 12418
Overdraw for ZigZag: 462

---------------Vertical Stripes---------------
Triangle Type: Disjoint
Overdraw for Scanline: 15
Overdraw for Backtrack: 310450
Overdraw for ZigZag: 11550
0.860s: Done.



-------------Example DEBUG run of TriangleTraversal------------------  
TriangleTraversal.exe D:\Documents\EEC277-Project\FilesToTest-Tim.txt 50 50 0 debug
Screen: 50 x 50
Debug Output: Yes

-------------------------------------------------------------------------------------------
---------------2 Easy Triangles---------------
Triangle Type: Disjoint

-----------------INPUT----------------------------------------Snapped to Grid--------------
Triangle 0                                   Triangle 0    Area: 50
---Vertex 0: 0.000000 0.000000 FF 00 00 FF   ---Vertex 0:   0.000000   0.000000 FF 00 00 FF
---Vertex 1: 0.200000 0.000000 00 00 00 FF   ---Vertex 1:  10.000000   0.000000 00 00 00 FF
---Vertex 2: 0.200000 0.200000 00 00 00 FF   ---Vertex 2:  10.000000  10.000000 00 00 00 FF
Triangle 1                                   Triangle 1    Area: 50
---Vertex 0: 0.000000 0.000000 00 FF 00 FF   ---Vertex 0:   0.000000   0.000000 00 FF 00 FF
---Vertex 1: 0.200000 0.200000 00 00 00 FF   ---Vertex 1:  10.000000  10.000000 00 00 00 FF
---Vertex 2: 0.000000 0.200000 00 FF 00 FF   ---Vertex 2:   0.000000  10.000000 00 FF 00 FF

0.011s: Running Test: Scanline
0.011s: Overdraw for Scanline: 1
------------Fragments------------
Triangle 0
... <fragments>
Triangle 1
... <fragments>

0.012s: Running Test: Backtrack
0.012s: Overdraw for Backtrack: 121
------------Fragments------------
Triangle 0
... <fragments>
Triangle 1
... <fragments>

0.013s: Running Test: ZigZag
0.013s: Overdraw for ZigZag: 44
------------Fragments------------
Triangle 0
... <fragments>
Triangle 1
... <fragments>


-------------------------------------------------------------------------------------------
---------------Vertical Stripes---------------
Triangle Type: Disjoint

-----------------INPUT----------------------------------------Snapped to Grid--------------
Triangle 0                                   Triangle 0    Area: 50
---Vertex 0: 0.000000 0.000000 FF 00 00 FF   ---Vertex 0:   0.000000   0.000000 FF 00 00 FF
---Vertex 1: 0.200000 0.000000 FF 99 00 FF   ---Vertex 1:  10.000000   0.000000 FF 99 00 FF
---Vertex 2: 0.200000 0.200000 FF 99 00 FF   ---Vertex 2:  10.000000  10.000000 FF 99 00 FF
...
...
...
Triangle 49                                  Triangle 49    Area: 50
---Vertex 0: 0.800000 0.800000 00 00 FF FF   ---Vertex 0:  40.000000  40.000000 00 00 FF FF
---Vertex 1: 1.000000 1.000000 66 00 FF FF   ---Vertex 1:  50.000000  50.000000 66 00 FF FF
---Vertex 2: 0.800000 1.000000 00 00 FF FF   ---Vertex 2:  40.000000  50.000000 00 00 FF FF

0.024s: Running Test: Scanline
0.029s: Overdraw for Scanline: 25
------------Fragments------------
Triangle 0
... <fragments>
...
...
...
Triangle 49
... <fragments>

0.064s: Running Test: Backtrack
0.069s: Overdraw for Backtrack: 3025
------------Fragments------------
Triangle 0
... <fragments>
...
...
...
Triangle 49
... <fragments>

0.095s: Running Test: ZigZag
0.101s: Overdraw for ZigZag: 1100
------------Fragments------------
Triangle 0
... <fragments>
...
...
...
Triangle 49
... <fragments>

0.126s: Done.