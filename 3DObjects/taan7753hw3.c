/*
 *  Taylor Andrews HW3
 *
 *  Modified example 7 code to abstract the cylinder creation to a function
 *
 *  Key bindings:
 *  1      Zoom Out
 *  2      Zoom in
 *  3      Reset to default zoom
 *  8      Reset view angle to y,z
 *  9      Reset view angle to x,z
 *  0      Reset view angle to x,y
 *  arrows Change view angle
 *  ESC    Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.1416

//  Globals
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle
double dim=2;   // Dimension of orthogonal box
double w2h;


/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Adapted from Example 7 given on Moodle
 *  Draw a cylinder
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the z axis
 *     given top & bottom color
 *     given side color
 */

void drawCylinder(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, char lcol, char scol) {
   double i;
   double radius = 0.3, height = 0.2;

   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);

   //Set colors
   if (scol == 'r'){
      glColor3f(1.0, 0.0, 0.0); //Set color to red
   } else if (scol == 'g') {
      glColor3f(0.0, 1.0, 0.0); //Set color to green
   } else if (scol == 'b') {
      glColor3f(0.0, 0.0, 1.0); //Set color to blue
   } else if (scol == 'w') {
      glColor3f(1.0, 1.0, 1.0); //Set color to white
   } else {
      glColor3f(0.8, 0.8, 0.8); //Default color to gray
   }


   /* Tube of Cylinder */
   glBegin(GL_QUAD_STRIP);
      for(i = 0.0; i <= 2.1*PI; i+=.05){
         glVertex3d(radius * cos(i), height, radius * sin(i));
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
   glEnd();

   //Set colors
   if (lcol == 'r'){
      glColor3f(1.0, 0.0, 0.0); //Set color to red
   } else if (lcol == 'g') {
      glColor3f(0.0, 1.0, 0.0); //Set color to green
   } else if (lcol == 'b') {
      glColor3f(0.0, 0.0, 1.0); //Set color to blue
   } else if (lcol == 'w') {
      glColor3f(1.0, 1.0, 1.0); //Set color to white
   } else {
      glColor3f(0.8, 0.8, 0.8); //Default color to gray
   }

   /* Top of Cylinder */
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, height, 0.0);

      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), height, radius * sin(i));
      }
   glEnd();

   /* Bottom of Cylinder */
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, -height, 0.0);

      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
   glEnd();

   glPopMatrix();
}

/*
 *  Display the scene
 */
void display()
{
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //  Reset previous transforms
   glLoadIdentity();
   //  Set view angle
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);

   glEnable(GL_DEPTH_TEST); //Enable z buffering

   /* Draw 4 Clyinders */   
   drawCylinder(0, 0, 0, 1, 1, 1, 0, 'r', 'b');
   drawCylinder(.5, .5, .5, 1, 3, 1, 20, 'b', 'q');
   drawCylinder(-.9, 0, 0, 2, 2, 1, 90, 'w', 'g');
   drawCylinder(-.6, -.6, -.8, .3, .5, .3, -45, 'g', 'r');

   //  Draw axes in white
   glColor3f(1,1,1);
   glBegin(GL_LINES);
   glVertex3d(0,0,0);
   glVertex3d(1,0,0);
   glVertex3d(0,0,0);
   glVertex3d(0,1,0);
   glVertex3d(0,0,0);
   glVertex3d(0,0,1);
   glEnd();
   //  Label axes
   glRasterPos3d(1,0,0);
   Print("X");
   glRasterPos3d(0,1,0);
   Print("Y");
   glRasterPos3d(0,0,1);
   Print("Z");
   //  Display parameters
   glWindowPos2i(5,5);
   Print("View Angle=%d,%d",th,ph);
   
   //  Flush and swap
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   else if (ch == '1') {
      dim+=.05;
   }
   else if (ch == '2') {
      dim-=.05;
   }
   else if (ch == '3') {
      dim=2;
   }
   //  Reset view angle to y,z
   else if (ch == '8') {
      th = 90;
      ph = 0;
   }
   //  Reset view angle to x,z
   else if (ch == '9') {
      th = 0;
      ph = -90;
   }
   //  Reset view angle to x,y
   else if (ch == '0')
      th = ph = 0;

   if (dim > 3) dim = 3; //Max scaling
   if (dim < 1) dim = 1; 

   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph -= 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph += 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   /*if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);*/
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do
 *  Modified from example 6 on Moodle
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window 
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("Taylor Andrews");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Tell GLUT to call "idle" when nothing else is going on
   //glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}
