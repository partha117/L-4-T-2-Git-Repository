#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>
#include <GL/glut.h>
#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#define pi (2*acos(0.0))
#define DEG2RAD 3.14159/180.0
struct point
{
	double x,y,z;
};
typedef struct point point;
double a,r;
double angle;
point pos;
point direction;
double increment;




void drawSquare()
{
    glColor3f(1,1,1);
    glBegin(GL_LINES);{
            glVertex3f( 0,0,0);
            glVertex3f( a,0,0);
            glVertex3f(a,0,0);
            glVertex3f(a,a,0);
            glVertex3f(a,a,0);
            glVertex3f(0,a,0);
            glVertex3f(0,a,0);
            glVertex3f(0,0,0);
        }glEnd();
}
void drawArrow()
{
    glColor3f(0,1,0);
    glBegin(GL_LINES);{
            glVertex3f(pos.x  ,pos.y,pos.z);
            glVertex3f(pos.x+direction.x   ,pos.y+direction.y  ,0);

        }glEnd();
}

void drawHead()
 {
    glColor3f(1,0,0);
     point backPos=(point){.x=pos.x-direction.x*0.3+direction.x, .y=pos.y-direction.y*0.3+direction.y,.z=0};
     point ppdirection=(point){.x=-direction.y,.y=direction.x,.z=0};
     point pos1=(point){.x=backPos.x+ppdirection.x*0.15,.y=backPos.y+ppdirection.y*0.15,.z=0};
     point pos2=(point){.x=backPos.x-ppdirection.x*0.15,.y=backPos.y-ppdirection.y*0.15,.z=0};
     glBegin(GL_TRIANGLES);{
        glVertex3f(pos.x+direction.x,pos.y+direction.y,pos.z);
        glVertex3f(pos1.x,pos1.y,pos1.z);
        glVertex3f(pos2.x,pos2.y,pos2.z);


     }glEnd();
 }

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':

			break;
        case '2':


            break;
		default:
			break;
	}
}
void specialKeyListener(int key, int x,int y){
    double temp;
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;

			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;

			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			temp=cos(-angle*DEG2RAD)*direction.x-sin(-angle*DEG2RAD)*direction.y;
            direction.y=sin(-angle*DEG2RAD)*temp+cos(-angle*DEG2RAD)*direction.y;
            direction.x=temp;

			break;
		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;

            temp=cos(angle*DEG2RAD)*direction.x-sin(angle*DEG2RAD)*direction.y;
            direction.y=sin(angle*DEG2RAD)*temp+cos(angle*DEG2RAD)*direction.y;
            direction.x=temp;
			break;

		case GLUT_KEY_PAGE_UP:

            break;
		case GLUT_KEY_PAGE_DOWN:

            break;


		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:

            break;
		case GLUT_KEY_END:

            break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:

			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}
void collisionChecker()
{
    double constant;
    double val=sqrt(direction.x*direction.x+direction.y*direction.y+direction.z*direction.z);
    point unitDirection;
    int change=0;
    printf("direction x %lf direction y %lf direction z %lf\n",direction.x,direction.y,direction.z);
    printf("pos x %lf pos y %lf pos z %lf\n",pos.x,pos.y,pos.z);
    if((pos.x+direction.x>a))
    {
        unitDirection=(point){.x=-1,.y=0,.z=0};
        constant=(unitDirection.x*direction.x);
        change=1;
        pos.x=a;
    }
    else if(pos.x+direction.x<0)
    {
        unitDirection=(point){.x=1,.y=0,.z=0};
        constant=(unitDirection.x*direction.x);
        change=1;
        pos.x=0;

    }
    else if(pos.y+direction.y>a)
    {
        unitDirection=(point){.x=0,.y=-1,.z=0};
        constant=(unitDirection.y*direction.y);
        change=1;
        pos.y=a;
    }
    else if(pos.y+direction.y<0)
    {
        unitDirection=(point){.x=0,.y=1,.z=0};
        constant=(unitDirection.y*direction.y);
        change=1;
        pos.y=0;
    }
    if(change==1)
    {
        printf("contant %lf\n",constant);
        printf("unit x %lf unit y %lf unit z %lf\n",unitDirection.x,unitDirection.y,unitDirection.z);
        printf("here\n");
        unitDirection.x*=(constant*2);
        unitDirection.y*=(constant*2);

        printf("AFTER unit x %lf unit y %lf unit z %lf\n",unitDirection.x,unitDirection.y,unitDirection.z);

        direction.x-=unitDirection.x;
        direction.y-=unitDirection.y;
        printf("AFTER direction x %lf direction y %lf direction z %lf\n",direction.x,direction.y,direction.z);


    }

}




void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(pos.x,pos.y,pos.z,	pos.x+look.x,pos.y+look.y,pos.z+look.z,	   up.x,up.y,up.z);
	//gluLookAt(pos.x,pos.y,pos.z,	-1,pos.y,pos.z,	0,1,0);
	gluLookAt(a/2,a/2,200,	a/2,a/2,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here

	****************************/
	//add objects

     //drawAxes();
     drawSquare();
     collisionChecker();
     drawArrow();
     drawHead();
	 glutSwapBuffers();
}


void animate(){

	//codes for any changes in Models, Camera
	pos.x+=direction.x*0.01;
	pos.y+=direction.y*0.01;
//	if(direction.x>0)
//    {
//
//        pos.x+=increment;
//    }
//    else{
//        pos.x-=increment;
//    }
//    if(direction.y>0)
//    {
//        pos.y+=increment;
//    }
//    else{
//        pos.y-=increment;
//    }

	glutPostRedisplay();
}

void init(){
	//codes for initialization

	a=200;
	angle=5;
    increment=0.01;
    pos=(point){.x=0,.y=0,.z=0};
    direction=(point){.x=10,.y=10,.z=0};
	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Assignment Program2");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
