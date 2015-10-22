/*
 *  Taylor Andrews HM2
 *
 *  Modified example 6 code given in class to produce axes.
 *  Modified exapmle 11 code given on Moodle to produce a tracing of the lorenz effect. 
 *
 *  Key bindings:
 *  1      Decrement lorenz s value by .25
 *  2      Increment lorenz s value by .25
 *  3      Decrement lorenz b value by .05
 *  4      Increment lorenz b value by .05
 *  5      Decrement lorenz r value by .5
 *  6      Increment lorenz r value by .5
 *  7      Reset Lorenz params to default values
 *  8      Reset view angle to y,z
 *  9      Reset view angle to x,z
 *  0      Reset view angle to x,y
 *  arrows Change view angle
 *  t      Toggle trace on and off
 *  ESC    Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//  Globals
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle
double dim=2;   // Dimension of orthogonal box

/* Lorzen params, may be changed */
double s  = 10;
double b  = 2.6666;
double r  = 28;

int t; //Holds time
int tracer = 0; //Should I trace boolean

int q = 50000; //Effects how often the tracer redisplays

typedef struct {
   double x;
   double y;
   double z;
} points_t;

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
 *  Display the scene
 *  Used lorenz.c to compute the lorzen coordinants
 */
void display()
{
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT);
   //  Reset previous transforms
   glLoadIdentity();
   //  Set view angle
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   //  Draw 10 pixel yellow points
   glColor3f(1.0,1.0,1.0);
   glPointSize(10);

   glBegin(GL_LINES);

   int i; //For loop iterator

   /* Lorenz x, y, and z variables */
   double lx = 1;
   double ly = 1;
   double lz = 1;

   double dt = 0.001; //Time step
   int scale = 30; //Scaling factor for the drawing

   glColor3f(0.0, 0.0, 1.0); //Set color to blue

   glVertex4d(lx, ly, lz, scale);

   points_t lorenz[q];

   for (i=0;i<50000;i++)
   {
      /*
       *  Calculate the next point in the lorenz attractor
       *  Adapted from the lorenz.c program given on Moodle
       */

       if((i % (50000/q)) == 0) {
         points_t p = {lx, ly, lz};
         lorenz[i/(50000/q)] = p;
       }
       

       // Switch the color every 5000 steps
      if(i < 5000) {
         glColor3f(1.0, 0.0, 0.0);
      }
      else if (i < 10000) {
         glColor3f(0.0, 1.0, 0.0);
      }
      else if (i < 15000) {
         glColor3f(0.0, 0.0, 1.0);         
      }
      else if (i < 20000) {
         glColor3f(1.0, 0.0, 0.0);         
      }
      else if (i < 25000) {
         glColor3f(0.0, 1.0, 0.0);         
      }
      else if (i < 30000) { 
         glColor3f(0.0, 0.0, 1.0);        
      }
      else if (i < 35000) {
         glColor3f(1.0, 0.0, 0.0);         
      }
      else if (i < 40000) {
         glColor3f(0.0, 1.0, 0.0);          
      }
      else if (i < 45000) {  
         glColor3f(0.0, 0.0, 1.0);       
      }

      double dx = s*(ly-lx);
      double dy = lx*(r-lz)-ly;
      double dz = lx*ly - b*lz;
      lx += dt*dx;
      ly += dt*dy;
      lz += dt*dz;

      glVertex4d(lx, ly, lz, scale);
      glVertex4d(lx, ly, lz, scale);
   }
   glEnd();

   if(tracer%2 == 0) {
      glColor3f(1.0,1.0,1.0); //Set color to white
      /*glPointSize(10);
      glBegin(GL_POINTS);

      glVertex4d(lorenz[t].x,lorenz[t].y,lorenz[t].z, scale);
      glEnd();*/
      glPushMatrix();
      glTranslatef(lorenz[t].x/scale,lorenz[t].y/scale,lorenz[t].z/scale);
      glutSolidSphere(.033, 50, 50);
      glPopMatrix();
   }

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
   glWindowPos2i(10, 22);
   Print("s=%.02f b=%.04f r=%.02f",s,b,r);
   
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
   //Decrement s
   else if (ch == '1') {
      s-=.25;
   }
   //Increment s
   else if (ch == '2') {
      s+=.25;
   }
   //Decrement b
   else if (ch == '3') {
      b-=.05; 
   }
   //Increment b
   else if (ch == '4') {
      b+=.05;
   }
   //Decrement r
   else if (ch == '5') {
      r-=.5;
   }
   //Increment r
   else if (ch == '6') {
      r+=.5;
   }
   //Reset to default params for lorenz
   else if (ch == '7') {
      s  = 10;
      b  = 2.6666;
      r  = 28;
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
   else if (ch == 't') {
      tracer++;
   }

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
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
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
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 *  Modified from example 11 on Moodle
 */
void idle()
{
   t = glutGet(GLUT_ELAPSED_TIME) % q;
   glutPostRedisplay();
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
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("Coordinates");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Tell GLUT to call "idle" when nothing else is going on
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}
