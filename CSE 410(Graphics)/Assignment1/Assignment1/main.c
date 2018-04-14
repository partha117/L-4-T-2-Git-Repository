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
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double a,r;
point pos;
point look;
point up;
point right;
double angle;





void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSphere(double radius,int slices,int stacks,double red,double green,double blue)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=(radius)*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)(4*slices))*2*pi);
			points[i][j].y=r*sin(((double)j/(double)(4*slices))*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
       // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

				    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(0,0,points[i][j].z);
                    glVertex3f(0,0,points[i][j].z);

			}glEnd();
		}
	}
}
void drawSquare()
{
    glBegin(GL_QUADS);{
            glVertex3f( a, r,r);
            glVertex3f( a,r,a-r);
            glVertex3f(a,a-r,a-r);
            glVertex3f(a, a-r,r);
        }glEnd();
}

void drawCylinder(double radius,double height,double segments)
{
    int i;
    double x,y,prevx,prevy;
    struct point points[100];
    glTranslatef(0,0,-height/2);
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)(4*segments))*2*pi);
        points[i].y=radius*sin(((double)i/(double)(4*segments))*2*pi);
    }
    glColor3f(0,1,0);
    for(i=0;i<segments;i++)
    {
        glBegin(GL_QUADS);{
			glVertex3f( points[i].x,points[i].y,0);
			glVertex3f( points[i].x,points[i].y,height);
			glVertex3f( points[i+1].x,points[i+1].y,height);
			glVertex3f( points[i+1].x,points[i+1].y,0);
        }glEnd();

    }
    glColor3f(1,0,0);
    prevx=radius;
    prevy=0;
    for(i=0;i<=segments;i++)
    {
        x=radius*cos(((double)i/(double)(4*segments))*2*pi);
        y=radius*sin(((double)i/(double)(4*segments))*2*pi);
        glBegin(GL_TRIANGLES);
        {
              glVertex3f(0,0,0);
              glVertex3f(x,y,0);
              glVertex3f(prevx,prevy,0);
              glVertex3f(0,0,height);
              glVertex3f(x,y,height);
              glVertex3f(prevx,prevy,height);
              prevx=x;
              prevy=y;

        }glEnd();
    }




}
void drawMySquare(double x, double y, double z)
{
    glTranslatef(x,y,z);
    drawSquare();
    glPushMatrix();
    glTranslatef(-a,0,0);
    drawSquare();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90,0,1,0);
    glTranslatef(-a,0,0);
    drawSquare();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90,0,1,0);
    glTranslatef(-2*a,0,0);
    drawSquare();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90,0,0,1);
    glTranslatef(0,-a,0);
    drawSquare();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90,0,0,1);
    glTranslatef(-a,-a,0);
    drawSquare();
    glPopMatrix();


}
void drawMyCylinder()
{

     glPushMatrix();
     glTranslatef(a-r,a*cos((60*pi)/180),0+r);
     glRotatef(90,1,0,0);
     glRotatef(-90,0,0,1);
     drawCylinder(r,a-2*r,80);
     glPopMatrix();

     glPushMatrix();
     glTranslatef(a-r,a*cos((60*pi)/180),a-r);
     glRotatef(90,1,0,0);
     drawCylinder(r,a-2*r,80);
     glPopMatrix();

     glPushMatrix();
     glTranslatef(r,a*cos((60*pi)/180),r);
     glRotatef(90,1,0,0);
     glRotatef(180,0,0,1);
     drawCylinder(r,a-2*r,80);
     glPopMatrix();

     glPushMatrix();
     glTranslatef(r,a*cos((60*pi)/180),a-r);
     glRotatef(90,1,0,0);
     glRotatef(90,0,0,1);
     drawCylinder(r,a-2*r,80);
     glPopMatrix();

     glPushMatrix();
     glTranslatef(a*cos((60*pi)/180),a-r,a-r);
     glRotatef(90,1,0,0);
     glRotatef(90,0,1,0);
     drawCylinder(r,a-2*r,80);
     glPopMatrix();



     glPushMatrix();
     glTranslatef(a*cos((60*pi)/180),r,a-r);
     glRotatef(90,0,1,0);
     glRotatef(180,0,0,1);
     drawCylinder(r,a-2*r,80);
     glPopMatrix();


     glPushMatrix();
     glTranslatef(r,r,a*cos((60*pi)/180));
     glRotatef(180,0,0,1);
     drawCylinder(r,a-2*r,80);
     glPopMatrix();



     glPushMatrix();
     glTranslatef(r,a-r,a*cos((60*pi)/180));
     glRotatef(90,0,0,1);
     drawCylinder(r,a-2*r,80);
     glPopMatrix();



     glPushMatrix();
     glTranslatef(a-r,r,a*cos((60*pi)/180));
     glRotatef(270,0,0,1);
     drawCylinder(r,a-2*r,80);
     glPopMatrix();


     glPushMatrix();
     glTranslatef(a-r,a-r,a*cos((60*pi)/180));
     drawCylinder(r,a-2*r,80);
     glPopMatrix();


     glPushMatrix();
     glTranslatef(a*cos((60*pi)/180),r,r);
     glRotatef(90,1,0,0);
     glRotatef(90,0,1,0);
     glRotatef(180,0,0,1);
     drawCylinder(r,a-2*r,80);
     glPopMatrix();



     glPushMatrix();
     glTranslatef(a*cos((60*pi)/180),a-r,r);
     glRotatef(-90,1,0,0);
     glRotatef(-90,0,1,0);
     drawCylinder(r,a-2*r,80);
     glPopMatrix();

}
void drawMySphere()
{
    glColor3f(1,0,0);
     glPushMatrix();
     glTranslatef(r,r,a-r);
     glRotatef(180,0,0,1);
     drawSphere(r,80,80,0.3,0,0);
     glPopMatrix();


     glPushMatrix();
     glTranslatef(a-r,r,a-r);
     glRotatef(270,0,0,1);
     drawSphere(r,80,80,0.3,0,0);
     glPopMatrix();


     glPushMatrix();
     glTranslatef(a-r,a-r,a-r);
     drawSphere(r,80,80,0.3,0,0);
     glPopMatrix();

     glPushMatrix();
     glTranslatef(r,a-r,a-r);
     glRotatef(90,0,0,1);
     drawSphere(r,80,80,0.3,0,0);
     glPopMatrix();


     glPushMatrix();
     glTranslatef(r,r,r);
     glRotatef(180,1,0,0);
     glRotatef(90,0,0,1);
     drawSphere(r,80,80,0.3,0,0);
     glPopMatrix();


     glPushMatrix();
     glTranslatef(r,a-r,r);
     glRotatef(180,0,1,0);
     drawSphere(r,80,80,0.3,0,0);
     glPopMatrix();

     glPushMatrix();
     glTranslatef(a-r,r,r);
     glRotatef(180,1,0,0);
     drawSphere(r,80,80,0.3,0,0);
     glPopMatrix();


     glPushMatrix();
     glTranslatef(a-r,a-r,r);
     glRotatef(90,0,1,0);
     drawSphere(r,80,80,0.3,0,0);
     glPopMatrix();
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){



        case '1':

            look.x*=cos(angle*DEG2RAD);
			look.y*=cos(angle*DEG2RAD);
			look.z*=cos(angle*DEG2RAD);
			look.x+=right.x*sin(-angle*DEG2RAD);
			look.y+=right.y*sin(-angle*DEG2RAD);
			look.z+=right.z*sin(-angle*DEG2RAD);
            break;


        case '2':


			look.x*=cos(angle*DEG2RAD);
			look.y*=cos(angle*DEG2RAD);
			look.z*=cos(angle*DEG2RAD);
			look.x+=right.x*sin(angle*DEG2RAD);
			look.y+=right.y*sin(angle*DEG2RAD);
			look.z+=right.z*sin(angle*DEG2RAD);
			break;


        case '3':



			look.x*=cos(angle*DEG2RAD);
			look.y*=cos(angle*DEG2RAD);
			look.z*=cos(angle*DEG2RAD);
			look.x+=up.x*sin(angle*DEG2RAD);
			look.y+=up.y*sin(angle*DEG2RAD);
			look.z+=up.z*sin(angle*DEG2RAD);
			break;
        case '4':



			look.x*=cos(angle*DEG2RAD);
			look.y*=cos(angle*DEG2RAD);
			look.z*=cos(angle*DEG2RAD);
			look.x+=up.x*sin(-angle*DEG2RAD);
			look.y+=up.y*sin(-angle*DEG2RAD);
			look.z+=up.z*sin(-angle*DEG2RAD);
            break;
		case '5':



			up.x*=cos(angle*DEG2RAD);
			up.y*=cos(angle*DEG2RAD);
			up.z*=cos(angle*DEG2RAD);
			up.x+=right.x*sin(angle*DEG2RAD);
			up.y+=right.y*sin(angle*DEG2RAD);
			up.z+=right.z*sin(angle*DEG2RAD);
			break;
        case '6':


            up.x*=cos(-angle*DEG2RAD);
			up.y*=cos(-angle*DEG2RAD);
			up.z*=cos(-angle*DEG2RAD);
			up.x+=right.x*sin(-angle*DEG2RAD);
			up.y+=right.y*sin(-angle*DEG2RAD);
			up.z+=right.z*sin(-angle*DEG2RAD);

            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
			pos.x+=3;
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;
			pos.x-=3;
			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			pos.y+=3;
			break;
		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;
			pos.y-=3;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.z+=3;
            break;
		case GLUT_KEY_PAGE_DOWN:
		    pos.z-=3;
            break;


		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			if(r!=0)
            {
                r-=0.5;
                break;
            }
            break;
		case GLUT_KEY_END:
			if(r<a/2)
            {
                r+=0.5;
                break;
            }
            break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
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
	gluLookAt(pos.x,pos.y,pos.z,	pos.x+look.x,pos.y+look.y,pos.z+look.z,	   up.x,up.y,up.z);
	//gluLookAt(pos.x,pos.y,pos.z,	-1,pos.y,pos.z,	0,1,0);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here

	****************************/
	//add objects

     drawAxes();
     drawGrid();

     glColor3f(1,1,1);
     glPushMatrix();
     drawMySquare(-a/2,-a/2,-a/2);
     drawMyCylinder();
     drawMySphere();
     glPopMatrix();
     printf("posx %lf posy %lf posz %lf\n",pos.x,pos.y,pos.z);
     printf("lookx %lf looky %lf lookz %lf\n",look.x,look.y,look.z);
     printf("upx %lf upy %lf upz %lf\n",up.x,up.y,up.z);
     printf("rightx %lf righty %lf rightz %lf\n",right.x,right.y,right.z);
	 glutSwapBuffers();
}


void animate(){

	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	a=75;
	r=10;
	angle=3;
    pos=(point){.x=200,.y=200,.z=50};
    look=(point){.x=-1/(sqrt(2)),.y=-1/(sqrt(2)),.z=0};
    up=(point){.x=0,.y=0,.z=1};
    right=(point){.x=-1/(sqrt(2)),.y=1/(sqrt(2)),.z=0};
    //look=(point){.x=-1/(sqrt(2)),.y=-1/(sqrt(2)),.z=0};
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

	glutCreateWindow("Assignment Program1");

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
