// 3D Scene Example

//Katherine Delisle

import processing.opengl.*;

float time = 0;  // keep track of passing of time
float camzoom = 80; //simulated camera zoom
float camrotateY = 0; //horizontal camera rotation
float camrotateX = 0; //vertical camera rotation

void setup() {
  size(600, 600, P3D);  // must use OPENGL here !!!
  noStroke();     // do not draw the edges of polygons
}

// Draw a scene with a cylinder, a sphere and a box
void draw() {
  noStroke();
  resetMatrix();  // set the transformation matrix to the identity (important!)

  background(255, 206, 0);  // clear the screen to black
  
  // set up for perspective projection
  perspective (PI * 0.333, 1.0, 0.01, 1000.0);
  
  // place the camera in the scene (just like gluLookAt())
  camera (0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
  
  scale (1.0, -1.0, 1.0);  // change to right-handed coordinate system
  
  // create an ambient light source
  ambientLight(102, 102, 102);
  
  // create two directional light sources
  lightSpecular(204, 204, 204);
  directionalLight(102, 102, 102, -0.7, -0.7, -1);
  directionalLight(152, 152, 152, 0, 0, -1);
  
  //drawcoordinate axis to measure current scene rotation.
  pushMatrix();
  translate(-.5, -.5, -1);
  rotateY(-camrotateY); //horizontal rotation
  rotateX(-camrotateX); //vertical rotation
  drawAxis(.1);
  popMatrix();
  
  //simulate a mobile camera by moving the scene around the camera.
  translate(0,0,-camzoom);
  rotateY(-camrotateY); //horizontal rotation
  rotateX(-camrotateX); //vertical rotation
  
  pushMatrix();
  translate(0,-30,0);
  //Draw Panda
  pushMatrix();
  
  rotate (time, 0.0, 1.0, 0.0);      // rotate based on "time"
  
  fill(255,255,255);//white
  cylinder(10,20,20); //bod
  translate(0,25,0);
  sphere(12); //head
  translate(7,-20,4);
  rotateX(PI/2.0);
  fill(2, 2, 8);
  translate(-2,0,0);
  cylinder(5, 10, 20);//leg
  translate(-10,0,0);
  cylinder(5, 10, 20);//leg
  translate(0,10,0);
  sphere(5);
  translate(10,0,0);
  sphere(5);
  fill(51,153,255);
  scale(1,1,2);
  translate(-10,-3,-12);
  sphere(2); //left pupil
  
  pushMatrix();
  fill(0,0,0);
  translate(0,-1,0);
  sphere(2.75); //left eye
  translate(8,0,0);
  sphere(2.75); //right eye
  popMatrix();
  fill(51,153,255);
  translate(8,0,0);
  sphere(2); //right pupil
  popMatrix();
  
  pushMatrix(); //ears
  rotate (time, 0.0, 1.0, 0.0);      // rotate based on "time"
  fill(0,0,0);
  translate(-8,35,0);
  sphere(5); //left ear
  translate(15,0,0);
  sphere(5); //right ear
  popMatrix();
  
  pushMatrix(); //tail
  fill(0,0,0);
  rotate (time, 0.0, 1.0, 0.0);      // rotate based on "time"
  translate(0,5,-12);
  
  sphere(4);
  popMatrix();
  rotate (time, 0.0, 1.0, 0.0);      // rotate based on "time"
  translate(10,15,10);
  arm(); //left arm
  translate(-20,0,0);
  arm(); //right arm
    
  fill(255,0,0);
  translate(9,10,1);

  sphere(2); //nose
  
  popMatrix(); //end panda
  
  // step forward in time
  time += 0.005;
}

void arm() { //arm is horizontal to ground
  pushMatrix();
  fill(0,0,0);
  rotateX(PI/2);
  rotateZ(PI);
  cylinder(3, 8, 20);//arm
  sphere(3);
  popMatrix();
}

// Draw a cylinder of a given radius, height and number of sides.
// The base is on the y=0 plane, and it extends vertically in the y direction.
void cylinder (float radius, float height, int sides) {
  int i,ii;
  float []c = new float[sides];
  float []s = new float[sides];

  for (i = 0; i < sides; i++) {
    float theta = TWO_PI * i / (float) sides;
    c[i] = cos(theta);
    s[i] = sin(theta);
  }
  
  // bottom end cap
  
  normal (0.0, -1.0, 0.0);
  for (i = 0; i < sides; i++) {
    ii = (i+1) % sides;
    beginShape(TRIANGLES);
    vertex (c[ii] * radius, 0.0, s[ii] * radius);
    vertex (c[i] * radius, 0.0, s[i] * radius);
    vertex (0.0, 0.0, 0.0);
    endShape();
  }
  
  // top end cap

  normal (0.0, 1.0, 0.0);
  for (i = 0; i < sides; i++) {
    ii = (i+1) % sides;
    beginShape(TRIANGLES);
    vertex (c[ii] * radius, height, s[ii] * radius);
    vertex (c[i] * radius, height, s[i] * radius);
    vertex (0.0, height, 0.0);
    endShape();
  }
  
  // main body of cylinder
  for (i = 0; i < sides; i++) {
    ii = (i+1) % sides;
    beginShape();
    normal (c[i], 0.0, s[i]);
    vertex (c[i] * radius, 0.0, s[i] * radius);
    vertex (c[i] * radius, height, s[i] * radius);
    normal (c[ii], 0.0, s[ii]);
    vertex (c[ii] * radius, height, s[ii] * radius);
    vertex (c[ii] * radius, 0.0, s[ii] * radius);
    endShape(CLOSE);
  }
}

//draw a coordinate axis
void drawAxis(float s){
  //X-axis
  fill(255,0,0);
  pushMatrix();
  translate(s/2.0,0,0);
  scale(1.0, 0.1, 0.1);
  box(s);
  popMatrix();
  //Y-axis
  fill(0,255,0);
  pushMatrix();
  translate(0,s/2.0,0);
  scale(0.1, 1.0, 0.1);
  box(s);
  popMatrix();
  //Z-axis
  fill(0,0,255);
  pushMatrix();
  translate(0,0,s/2.0);
  scale(0.1, 0.1, 1.0);
  box(s);
  popMatrix();
}

//draw a radius 1 half-sphere
// d is the detail level of the sphere (number of faces per rotation)
//note: seam will show if detail is not sufficiently large. Recommend d>30
void half_sphere(int d, boolean draw_endcap){
  if(d<1)
    println("Nope, half_sphere(>=1) required");
    
  float s = PI/(d*2);
  float[][] v = new float[(int)(d*d*4)][3];
  int vn = 0;
 
  for(int z=1; z<=d; z++){
    float phi = (PI/2) + s *(z-1);
    for(int c=0; c<d*4; c++){
       float theta = c*s;
       v[vn][0] = cos(theta)*sin(phi);
       v[vn][1] = sin(theta)*sin(phi);
       v[vn++][2] = cos(phi);
    }
  }
  
  //draw end cap
  if(draw_endcap){
    beginShape(TRIANGLE_FAN);
    vertex(0,0,0);
    for(int p=0; p<d*4; p++)
      vertex(v[p][0],v[p][1],v[p][2]);
    vertex(v[0][0],v[0][1],v[0][2]);
    endShape();
  }
  
  //draw top cap
  beginShape(TRIANGLE_FAN); 
  vertex(0,0,-1);
  for(int p=0; p<d*4; p++)
    vertex(v[vn-p-1][0],v[vn-p-1][1],v[vn-p-1][2]);
  vertex(v[vn-1][0],v[vn-1][1],v[vn-1][2]);
  endShape();
  
  //draw body quads
  
  for(int c = 0; c<d-1; c++){
    int nc = (c+1)*d*4;
    int cc = (c)*d*4;
    beginShape(QUAD_STRIP);
    for(int p=0; p<d*4; p++){
      vertex(v[nc+p][0],v[nc+p][1],v[nc+p][2]);
      vertex(v[cc+p][0],v[cc+p][1],v[cc+p][2]);
    }
    vertex(v[nc][0],v[nc][1],v[nc][2]);
    vertex(v[cc][0],v[cc][1],v[cc][2]);
    endShape();
  }
  
  
}

//rotate the virtual camera based on mouse movement by changing the rotation angles.
void mouseDragged(){
  camrotateY += (pmouseX-mouseX)*((PI)/90.0);
  camrotateX += (pmouseY-mouseY)*((PI)/90.0);
}

//mouse wheel to zoom
void mouseWheel(MouseEvent event){
  //int delta = event.getCount();
  //if(keyPressed && keyCode == SHIFT && key==CODED)
    //delta *=4;
  //camzoom += delta;
}

