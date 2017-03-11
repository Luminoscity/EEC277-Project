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


-------------Example HUMAN run of TriangleTraversal------------------  
TriangleTraversal.exe D:\Documents\EEC277-Project\exampleDisjointTriangles.txt 640 480 0 human
Screen: 640 x 480
Debug Output: No
---------Example Disjoint Triangles---------
Triangle Type: Disjoint
Overdraw for Scanline: 4
Overdraw for Backtrack: 27477
Overdraw for ZigZag: 1213
0.092s: Done.


-------------Example DEBUG run of TriangleTraversal------------------  
TriangleTraversal.exe D:\Documents\EEC277-Project\exampleDisjointTriangles.txt 50 50 0 debug
Screen: 50 x 50
Debug Output: Yes

-----------------INPUT----------------------------------------Snapped to Grid--------------
Triangle 0                                   Triangle 0    Area: 14.4375
---Vertex 0: 0.045612 0.178945 FF 00 00 FF   ---Vertex 0:   2.250000   9.000000 FF 00 00 FF
---Vertex 1: 0.578945 0.467845 00 00 FF FF   ---Vertex 1:  29.000000  23.500000 00 00 FF FF
---Vertex 2: 0.245612 0.312345 00 FF 00 FF   ---Vertex 2:  12.250000  15.500000 00 FF 00 FF
Triangle 1                                   Triangle 1    Area: 197.719
---Vertex 0: 0.678159 0.732841 00 00 FF FF   ---Vertex 0:  34.000000  36.750000 00 00 FF FF
---Vertex 1: 0.815926 0.915926 00 FF 00 FF   ---Vertex 1:  40.750000  45.750000 00 FF 00 FF
---Vertex 2: 0.000001 0.999999 FF 00 00 FF   ---Vertex 2:   0.000000  50.000000 FF 00 00 FF

---------Example Disjoint Triangles---------
Triangle Type: Disjoint
0.009s: Running Test: Scanline
0.010s: Overdraw for Scanline: 2
------------Fragments------------
Triangle 0
... <fragments>
Triangle 1
... <fragments>

0.012s: Running Test: Backtrack
0.012s: Overdraw for Backtrack: 288
------------Fragments------------
Triangle 0
... <fragments>
Triangle 1
... <fragments>

0.014s: Running Test: ZigZag
0.014s: Overdraw for ZigZag: 100
------------Fragments------------
Triangle 0
... <fragments>
Triangle 1
... <fragments>

0.016s: Done.