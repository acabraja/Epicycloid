#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <utility>

using namespace std;

#define PI 3.1415926535897932384626433832795
#define STEP 100

float angel = 0;
vector<pair<float,float> > tocke;
const double Xmin = -1.0, Xmax = 1.0;
const double Ymin = -1.0, Ymax = 1.0;
int WindowHeight = 700;
int WindowWidth = 700;
double radiusBasicCircle = 0;
double radiusRotateCircle = 0;
pair<float,float> centerBasicCircle;
pair<float,float> centerRotateCircle;
pair<float,float> forRadius;
vector<pair<float,float> > epicycloid;
int xP = 300, yP = 100;
double windowXmin, windowXmax, windowYmin, windowYmax;
float t;

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 27:
        exit(1);
    case 's':
        glutPostRedisplay();  
    }
}

void findRadius(void)
{
    centerBasicCircle = tocke[0];
    centerRotateCircle = tocke[2];
    forRadius = tocke[1];

    radiusBasicCircle = sqrt((centerBasicCircle.first - forRadius.first)*(centerBasicCircle.first - forRadius.first) +
        (centerBasicCircle.second - forRadius.second)*(centerBasicCircle.second - forRadius.second));
    radiusRotateCircle = sqrt((centerRotateCircle.first - forRadius.first)*(centerRotateCircle.first - forRadius.first) +
        (centerRotateCircle.second - forRadius.second)*(centerRotateCircle.second - forRadius.second));
}

void beginPoint()
{
    float x = tocke[1].first ;//+ tocke[2].first;
    float y = tocke[1].second ;//+ tocke[2].second;

    float k = -y/x;
    float ks = radiusBasicCircle/radiusRotateCircle;
    float t = atan(k) -  2*PI/(180*ks);;
    printf("%f\n",k );
}

void myMouseFunc( int button, int state, int x, int y ) 
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && tocke.size() < 3) 
    {
        float xPos = float( x )/(GLfloat)WindowWidth ;
        float yPos = 1-float( y )/(GLfloat)WindowHeight; 

        tocke.push_back(make_pair(xPos, yPos)); 
        glutPostRedisplay();   

        if( tocke.size() == 3)
        {
            findRadius();
            beginPoint();
        }
    }
}

void resizeWindow(int w, int h)
{
    WindowWidth = w;
    WindowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void coord(void)
{
    glColor3f(0.4,0.4,0.4);

    glBegin(GL_LINES);
        glVertex2f(0.0, 1);
        glVertex2f(0.0, -1);
        glVertex2f(1, 0.0);
        glVertex2f(-1, 0.0);
    glEnd();
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //coord();
    glPointSize(4);
    glColor3f(1.0,1.0,0.0);

    if(tocke.size() < 3)
    {
        glBegin(GL_POINTS);
            for(int i = 0; i < tocke.size(); i++)
                glVertex2f(tocke[i].first,tocke[i].second);
        glEnd();
    }

    if(tocke.size() == 3)
    {
        glMatrixMode( GL_MODELVIEW );
        glBegin(GL_LINE_LOOP);
            for(double i = 0; i < 2 * PI; i += PI / STEP) 
                    glVertex3f(cos(i) * radiusBasicCircle + centerBasicCircle.first, sin(i) * radiusBasicCircle + centerBasicCircle.second, 0.0);
        glEnd();
        angel = float((angel) + 2);
        float ks = radiusBasicCircle/radiusRotateCircle;
        t = t + 2*PI/(180*ks);
        float r = radiusRotateCircle;
        pair<float,float> vr = centerBasicCircle;
        epicycloid.push_back(make_pair(r*(ks+1)*cos(t) - r*cos((ks+1)*t) + vr.first,r*(ks+1)*sin(t) - r*sin((ks+1)*t)+vr.second));
        glTranslatef( centerBasicCircle.first, centerBasicCircle.second, 0.0 );
        glRotatef( angel/ks, 0.0, 0.0, 1.0 );
        glTranslatef( -centerBasicCircle.first, -centerBasicCircle.second, 0.0 );
        glBegin(GL_LINE_LOOP);
            for(double i = 0; i < 2 * PI; i += PI / STEP) 
                    glVertex3f(cos(i) * radiusRotateCircle + centerRotateCircle.first, sin(i) * radiusRotateCircle + centerRotateCircle.second, 0.0);
        glEnd();
        glTranslatef( centerRotateCircle.first, centerRotateCircle.second, 0.0 );
        glRotatef( angel, 0.0, 0.0, 1.0 );
        glTranslatef( -centerRotateCircle.first, -centerRotateCircle.second, 0.0 );
        glBegin(GL_LINES);
            glVertex2f(centerRotateCircle.first,centerRotateCircle.second);
            glVertex2f(forRadius.first,forRadius.second);
        glEnd();
        glLoadIdentity();
        glBegin(GL_LINE_STRIP);
            for(int k = 0; k < epicycloid.size(); k++)
                glVertex2f(epicycloid[k].first,epicycloid[k].second);
        glEnd();
    }
    glFlush();
    glutSwapBuffers();

    /*if(tocke.size() == 3)
        glutPostRedisplay();*/
}

void init() {
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.0, 5.0, -5.0, 5.0, -1.0, 1.0);*/
}

int main( int argc, char** argv )
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize(WindowWidth,WindowHeight);
    glutInitWindowPosition(xP,yP);
    glutCreateWindow( "Epicycloid");
    init();
    glutKeyboardFunc( myKeyboardFunc );
    glutMouseFunc(myMouseFunc);
    glutReshapeFunc( resizeWindow );
    glutDisplayFunc( display);
    glutMainLoop(  );

    return 0;
}