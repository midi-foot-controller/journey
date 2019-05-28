/*
scale(0.1)
import("/_shared_/dev/dev.projects/TOC/PEDALBOARD.GIT/doc/internal/3D.GrabCad/lcd-display-20x4-with-i2c-pcf8574-module-1.snapshot.1/LCD & Module Assem.STL");
*/

blackColor = [0.1, 0.1, 0.1, 1];
greenColor = [0.2, 0.6, 0.47, 1];


difference()  {
    color(greenColor)
    translate([0, 0, 1.5/2])
    cube([98, 60, 1.5], center=true);

    translate([93/2, 55/2, -1])
    cylinder(h=3, r=2.5/2, $fn=20);

    translate([-93/2, 55/2, -1])
    cylinder(h=3, r=2.5/2, $fn=20);

    translate([93/2, -55/2, -1])
    cylinder(h=3, r=2.5/2, $fn=20);

    translate([-93/2, -55/2, -1])
    cylinder(h=3, r=2.5/2, $fn=20);
}

difference() {
    color(blackColor)
    translate([0, 0, 1.5 + 7/2])
    cube([98, 38.40, 7], center=true);
    
    translate([0, 0, 1.5 + 7/2 + 1/2 + 2.6])
    cube([76, 25.20, 1], center=true);
}

translate([0, 60/2 - 2, -2])
for (x = [0:15]) {
    translate([10 + (x * 2.54) - (98 / 2), 0, 0])
    cylinder(h = 10, r1 = 0.25, r2 = 0.25, center = true);
}
