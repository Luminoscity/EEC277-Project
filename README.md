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
TriangleTraversal.exe D:\Documents\EEC277-Project\exampleTriangles.txt 160 90
Screen: 160 x 90
0.002s: Done.

Will eventually take test number as input and output amount of overdraw for each test run