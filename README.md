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


-------------Example run of TriangleTraversal------------------  
TriangleTraversal.exe D:\Documents\EEC277-Project\exampleDisjointTriangles.txt 640 480 0 yes
Screen: 640 x 480
Triangle Type: Disjoint
Debug Output: Yes

-----------------INPUT----------------------------------------Snapped to Grid--------------
Triangle 0                                     Triangle 0
---Vertex 0: 0.045612 0.178945 FF 00 00 FF   ---Vertex 0:  29.250000  86.000000 FF 00 00 FF
---Vertex 1: 0.245612 0.312345 00 FF 00 FF   ---Vertex 1: 157.250000 150.000000 00 FF 00 FF
---Vertex 2: 0.478945 0.567845 00 00 FF FF   ---Vertex 2: 306.500000 272.500000 00 00 FF FF
Triangle 1                                     Triangle 1
---Vertex 0: 0.678159 0.732841 00 00 FF FF   ---Vertex 0: 434.000000 351.750000 00 00 FF FF
---Vertex 1: 0.815926 0.915926 00 FF 00 FF   ---Vertex 1: 522.250000 439.750000 00 FF 00 FF
---Vertex 2: 0.000001 0.999999 FF 00 00 FF   ---Vertex 2:   0.000000 480.000000 FF 00 00 FF

Overdraw for Scanline: 0
Overdraw for Backtrack: 0
Overdraw for ZigZag: 0
0.008s: Done.