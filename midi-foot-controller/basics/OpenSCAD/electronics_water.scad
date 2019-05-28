$fn=128; 

radius = 4; 
inner_radius = 3.5; 

bend_radius = 30; 


// angle_1 = 0; angle_2 = 360; colorTube = "green"; colorParticle = "blue"; particle_rad = 360; showBattery = false; 
// angle_1 = 0; angle_2 = 360; colorTube = [1, 1, 1, 0.35]; colorParticle = "blue"; particle_rad = 5; showBattery = false;
// angle_1 = -90; angle_2 = 90; colorTube = [1, 1, 1, 0.20]; colorParticle = "blue"; particle_rad = 12; showBattery = false;
// angle_1 = -90; angle_2 = 90; colorTube = [1, 1, 1, 0.20]; colorParticle = [0.2, 0.2, 0.2]; particle_rad = 12; showBattery = false;
// angle_1 = -90; angle_2 = 90; colorTube = [1, 1, 1, 0.20]; colorParticle = "white"; particle_rad = 12; showBattery = true; showCharge = false;
angle_1 = -90; angle_2 = 90; colorTube = [1, 1, 1, 0.20]; colorParticle = "white"; particle_rad = 12; showBattery = true; showCharge = true;



module module_Pie(radius, angle, height, spin=0) {
    // Negative angles shift direction of rotation
    clockwise = (angle < 0) ? true : false;
    // Support angles < 0 and > 360
    normalized_angle = abs((angle % 360 != 0) ? angle % 360 : angle % 360 + 360);
    // Select rotation direction
    rotation = clockwise ? [0, 180 - normalized_angle] : [180, normalized_angle];
    // Render
    if (angle != 0) {
        rotate([0,0,spin]) linear_extrude(height=height)
        difference() {
            circle(radius);

            if (normalized_angle < 180) {
                union() {
                    for(a = rotation) {
                        rotate(a) translate([-radius, 0, 0]) square(radius * 2);
                    }
                }
            } else if (normalized_angle != 360) {
                intersection_for(a = rotation) {
                    rotate(a) translate([-radius, 0, 0]) square(radius * 2);
                }
            }
        }
    }
}

module module_ParticleCharged() {
    color(colorParticle)
    render() {
        union() {
            difference() {
                translate([bend_radius + radius/2, 0, 0])
                sphere(inner_radius - 0.5);

                translate([bend_radius + radius/2, 0, inner_radius])
                cube([inner_radius*2, inner_radius*2, inner_radius], true);

                translate([bend_radius + radius/2, 0, inner_radius/2])
                cube([inner_radius - 0.25, inner_radius - 0.25, inner_radius - 0.25], true);
            }
        }
    }
    
    color("black")
    translate([bend_radius + radius/2, 0, inner_radius/2 - 0.3])
    cube([inner_radius - 1, inner_radius - 1, inner_radius - 1], true);
}

module module_Battery() 
{
    c = radius * 3.5;
    difference() {
        union() {
            color("red")
            translate([bend_radius, -c/2, 0])
            cube([c, c, c], true);
            color("black")
            translate([bend_radius, +c/2, 0])
            cube([c, c, c], true);
        }
        
        // torus 
        rotate_extrude() 
        translate([bend_radius + radius/2, 0, 0]) 
        circle(r=radius+0.25); 
    }
}


// ----------------------------------------------------------------------------------------------------------------------------------


for (a = [0:particle_rad:360]) {
    rotate([0, 0, a])
    if (showBattery && showCharge) {
        module_ParticleCharged();
    } else {
        color(colorParticle)
        translate([bend_radius + radius/2, 0, 0])
        sphere(inner_radius - 0.5);
    }
}





// bend 
color(colorTube)
difference() { 
    // torus 
    rotate_extrude() 
    translate([bend_radius + radius/2, 0, 0]) 
    circle(r=radius); 
    // torus cutout 
    rotate_extrude() 
    translate([bend_radius + radius/2, 0, 0]) 
    circle(r=inner_radius); 
    // piecutout
    rotate([0, 0, -angle_1])
    translate([0, 0, -radius*1.5]) 
    module_Pie(bend_radius + radius*2, 360 - abs(angle_2 - angle_1), radius*3);
}



if (showBattery) {
    module_Battery();
}




