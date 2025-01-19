# comment

plane waist
p 0 5 0
n 0 1 0
c 1.0 0.5 0.0
/plane

plane shoulder
p 0 12 0
n 0 -1 0
/plane

plane neck
p 0 13 0
n 0 1 0
/plane

sketch foot_1
p plane_xz
v 1 -1
v 3 -1
v 3 1
v 1 1
/sketch

sketch foot_2
p plane_xz
# correct winding order
v -3 -1
v -1 -1
v -1 1
v -3 1
/sketch

sketch torso
p waist
v 3.5 -1.5
v 3.5 1.5
v -3.5 1.5
v -3.5 -1.5
/sketch

sketch arm_1
p shoulder
v 5.5 -1
v 5.5 1
v 3.5 1
v 3.5 -1
/sketch

sketch arm_2
p shoulder
v -3.5 -1
v -3.5 1
v -5.5 1
v -5.5 -1
/sketch

sketch head
p neck
v 2.5 -2
v 2.5 2
v -2.5 2
v -2.5 -2
/sketch

extrude leg_1
s foot_1
l 5
/extrude

extrude leg_2
s foot_2
l 5
/extrude

extrude torso
s torso
l 8
/extrude

extrude arm_1
s arm_1
l 6
/extrude

extrude arm_2
s arm_2
l 6
/extrude

extrude head
s head
l 4
/extrude