#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <utility>

using namespace std;

vector<pair<float,float> > tocke;
const double Xmin = -1.0, Xmax = 1.0;
const double Ymin = -1.0, Ymax = 1.0;
int WindowHeight = 700;
int WindowWidth = 700;


void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 27:
        exit(1);
    }
}

void myMouseFunc( int button, int state, int x, int y ) 
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && tocke.size() < 3) 
    {
        float xPos = ((float)x)/((float)(WindowWidth-1));
        float yPos = ((float)y)/((float)(WindowHeight-1));

        yPos = 1.0f-yPos;
        tocke.push_back(make_pair(xPos, yPos)); 
        glutPostRedisplay();   
    }

    glutPostRedisplay();
}

void resizeWindow(int w, int h)
{
    /*double scale, center;
    double windowXmin, windowXmax, windowYmin, windowYmax;

    glViewport( 0, 0, w, h );
    w = (w==0) ? 1 : w;
    h = (h==0) ? 1 : h;
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
    glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1 );*/

    WindowHeight = (h>1) ? h : 2;
    WindowWidth = (w>1) ? w : 2;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);  // Always view [0,1]x[0,1].
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
    glClear (GL_COLOR_BUFFER_BIT);
    //coord();
    glPointSize(4);
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_POINTS);
        for(int i = 0; i < tocke.size(); i++)
            glVertex2f(tocke[i].first,tocke[i].second);
    glEnd();
    glFlush();
    glutSwapBuffers();
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
    glutInitWindowSize(700,700);
    glutInitWindowPosition(450,100);
    glutCreateWindow( "Epicycloid");
    init();
    glutKeyboardFunc( myKeyboardFunc );
    glutMouseFunc(myMouseFunc);
    glutReshapeFunc( resizeWindow );
    glutDisplayFunc( display);
    glutMainLoop(  );

    return 0;
}