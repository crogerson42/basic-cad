# comment

plane plane_0
p 0 1 0
n 0 1 0
c 1.0 0.5 0.0
/plane

sketch sketch_0
p plane_0
v 1 -1
v 1 -2
v 1.5 -1.4
v 2 -2
v 2 -1
/sketch

extrude extrude_0
s sketch_0
l 0.5
/extrude