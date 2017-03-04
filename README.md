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
TriangleTraversal.exe D:\Documents\EEC277-Project\exampleDisjointTriangles.txt 10 10
Screen: 10 x 10
Triangle Type: Disjoint

----------INPUT----------
Triangle 1
---Vertex 1: 0.045612 0.178945 FF 00 00 FF
---Vertex 2: 0.245612 0.312345 00 FF 00 FF
---Vertex 3: 0.478945 0.567845 00 00 FF FF
Triangle 2
---Vertex 1: 0.678159 0.732841 00 00 FF FF
---Vertex 2: 0.815926 0.915926 00 FF 00 FF
---Vertex 3: 0.000001 0.999999 FF 00 00 FF

----------Snapped to Grid----------
Triangle 1
---Vertex 1: 0.250000 1.750000 FF 00 00 FF
---Vertex 2: 2.250000 3.000000 00 FF 00 FF
---Vertex 3: 4.750000 5.500000 00 00 FF FF
Triangle 2
---Vertex 1: 6.750000 7.250000 00 00 FF FF
---Vertex 2: 8.000000 9.000000 00 FF 00 FF
---Vertex 3: 0.000000 9.750000 FF 00 00 FF
0.022s: Done.

Will eventually take test number as input and output amount of overdraw for each test run