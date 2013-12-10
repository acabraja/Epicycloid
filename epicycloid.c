#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <utility>

using namespace std;

#define PI 3.1415926535897932384626433832795
#define STEP 30
#define SPEED 2

float angel = 0;
vector<pair<float,float> > tocke;
pair<float,float> centerBasicCircle;
pair<float,float> centerRotateCircle;
pair<float,float> forRadius;
double radiusBasicCircle = 0;
double radiusRotateCircle = 0;
vector<pair<float,float> > epicycloid;
double Xmin = -1.0, Xmax = 1.0;
double Ymin = -1.0, Ymax = 1.0;
int WindowHeight = 700;
int WindowWidth = 700;
int xP = 300, yP = 100;
float t = 0;
bool forStart = false;
bool run = false;

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key ) 
    {
        case 27:
            exit(1);
        case 's':
        {
            run = true;
            forStart = true;
            glutPostRedisplay();
        }
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

float beginPoint()
{
    float x = tocke[1].first - tocke[0].first;
    float y = tocke[1].second - tocke[0].second;

    float k = y/x;
    float ks = radiusBasicCircle/radiusRotateCircle;
    return  atan(k)*180/PI;
}

void myMouseFunc( int button, int state, int x, int y )
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && tocke.size() < 2)
    {
        float xPos = Xmin + (Xmax-Xmin)*(float)x/(float)(WindowWidth - 1);
        float yPos = Ymax - (Ymax-Ymin)*(float)y/(float)(WindowHeight - 1);
        tocke.push_back(make_pair(xPos, yPos));
        glutPostRedisplay();

        if( tocke.size() == 2)
        {
            float x = tocke[1].first - tocke[0].first;
            float y = tocke[1].second - tocke[0].second;
            float k = y/x;
            float pomX, pomY;
            if(tocke[0].first < tocke[1].first)
            {
                pomX = tocke[1].first + 0.1;
                pomY = k*0.1+tocke[1].second;
            }
            if(tocke[0].first > tocke[1].first)
            {
                pomX = tocke[1].first - 0.1;
                pomY = -k*0.1+tocke[1].second;
            }
            tocke.push_back(make_pair(pomX,pomY));
            glutPostRedisplay();
            findRadius();
        }
    }
}

void resizeWindow(int w, int h)
{
    double scale, center;
    double windowXmin, windowXmax, windowYmin, windowYmax;

    glViewport( 0, 0, w, h );
    w = (w==0) ? 1 : w;
    h = (h==0) ? 1 : h;
    WindowWidth = w;
    WindowHeight = h;
    if ( (Xmax-Xmin)/w < (Ymax-Ymin)/h ) {
        scale = ((Ymax-Ymin)/h)/((Xmax-Xmin)/w);
        center = (Xmax+Xmin)/2;
        windowXmin = center - (center-Xmin)*scale;
        windowXmax = center + (Xmax-center)*scale;
        windowYmin = Ymin;
        windowYmax = Ymax;
    }
    else {
        scale = ((Xmax-Xmin)/w)/((Ymax-Ymin)/h);
        center = (Ymax+Ymin)/2;
        windowYmin = center - (center-Ymin)*scale;
        windowYmax = center + (Ymax-center)*scale;
        windowXmin = Xmin;
        windowXmax = Xmax;
    }

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1 );
    Xmax = windowXmax;
    Xmin = windowXmin;
    Ymax = windowYmax;
    Ymin = windowYmin;
}

void coord(void)
{
    glColor3f(0.4,0.4,0.4);

    glBegin(GL_LINES);
        glVertex2f(0.0, Ymax);
        glVertex2f(0.0, Ymin);
        glVertex2f(Xmax, 0.0);
        glVertex2f(Xmin, 0.0);
    glEnd();
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    coord();
    glPointSize(4);
    glColor3f(1.0,1.0,0.0);

    if(tocke.size() <= 3 && forStart == false)
    {
        glBegin(GL_POINTS);
            for(int i = 0; i < tocke.size(); i++)
                glVertex2f(tocke[i].first,tocke[i].second);
        glEnd();
        glBegin(GL_LINE_STRIP);
            for(int i = 0; i < tocke.size(); i++)
                glVertex2f(tocke[i].first,tocke[i].second);
        glEnd();
    }

    if(tocke.size() == 3)
    {
        glMatrixMode( GL_MODELVIEW );
        glBegin(GL_POLYGON);
            for(double i = 0; i < 2 * PI; i += PI / STEP)
                    glVertex3f(cos(i) * radiusBasicCircle + centerBasicCircle.first, sin(i) * radiusBasicCircle + centerBasicCircle.second, 0.0);
        glEnd();
        if(forStart)
        {
            angel = float((angel) + SPEED);
            float ks = radiusBasicCircle/radiusRotateCircle;
            t = t + SPEED*PI/(180*ks);
            float r = radiusRotateCircle;
            pair<float,float> vr = centerBasicCircle;
            epicycloid.push_back(make_pair(r*(ks+1)*cos(t) - r*cos((ks+1)*t) + vr.first ,r*(ks+1)*sin(t) - r*sin((ks+1)*t)+vr.second));
            glTranslatef( centerBasicCircle.first, centerBasicCircle.second, 0.0 );
            glRotatef( angel/ks, 0.0, 0.0, 1.0 );
            glTranslatef( -centerBasicCircle.first, -centerBasicCircle.second, 0.0 );
        }
        glBegin(GL_LINE_LOOP);
            for(double i = 0; i < 2 * PI; i += PI / STEP)
                    glVertex3f(cos(i) * radiusRotateCircle + centerRotateCircle.first, sin(i) * radiusRotateCircle + centerRotateCircle.second, 0.0);
        glEnd();
        if(forStart)
        {
            glTranslatef( centerRotateCircle.first, centerRotateCircle.second, 0.0 );
            glRotatef( angel, 0.0, 0.0, 1.0 );
            glTranslatef( -centerRotateCircle.first, -centerRotateCircle.second, 0.0 );
            glBegin(GL_LINES);
                glVertex2f(centerRotateCircle.first,centerRotateCircle.second);
                glVertex2f(forRadius.first,forRadius.second);
            glEnd();
            glLoadIdentity();
            glTranslatef( centerBasicCircle.first, centerBasicCircle.second, 0.0 );
            glRotatef( beginPoint(), 0.0, 0.0, 1.0 );
            glTranslatef( -centerBasicCircle.first, -centerBasicCircle.second, 0.0 );
            glColor3f(0.0,1.0,1.0);
            glBegin(GL_LINE_STRIP);
                for(int k = 0; k < epicycloid.size(); k++)
                    glVertex2f(epicycloid[k].first,epicycloid[k].second);
            glEnd();
        }
        glLoadIdentity();
    }
    glFlush();
    glutSwapBuffers();

    if(run == true)
        glutPostRedisplay();
}

void init() {
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
}

void Motion(int x, int y)
{
    float eps = 0.0000001;
    float xPos = Xmin + (Xmax-Xmin)*(float)x/(float)(WindowWidth - 1);
    float yPos = Ymax - (Ymax-Ymin)*(float)y/(float)(WindowHeight - 1); 

    float sx = xPos- tocke[0].first;
    float sy = yPos - tocke[0].second;
    float k = sy/sx;
    sx = tocke[1].first- tocke[0].first;
    sy = tocke[1].second - tocke[0].second;
    float pomRadius = sqrt((xPos - tocke[0].first)*(xPos - tocke[0].first) + (yPos - tocke[0].second)*(yPos - tocke[0].second));

    int i = 2;
    if(/*(sy/sx - k < eps) && */pomRadius > radiusBasicCircle)
    {
        tocke[i].first=xPos; 
        tocke[i].second=yPos; 
    }
    if(forStart == false)
    {
        findRadius();         
        glutPostRedisplay();
    }
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
    glutMotionFunc(Motion);
    glutReshapeFunc( resizeWindow );
    glutDisplayFunc( display);
    glutMainLoop(  );

    return 0;
}