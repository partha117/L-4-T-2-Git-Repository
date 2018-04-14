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
#define RAD2DEG 180.0/3.14159
struct point
{
	double x,y,z;
};
typedef struct point point;


double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;


double arm1;
double arm1Length;
double arm1Scale;
double arm2;
double arm2Length;
double arm2Scale;
double arm3;
double arm3Length;
double arm3Scale;
double arm4;
double arm4Length;
double arm4Scale;
double arm5;
double arm1Rot2;




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











void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
		    if((arm1<45))
            {
                arm1++;
            }
			break;
		case '2':
             if((arm1>-45))
            {
                arm1--;
            }
			break;
        case '3':
		    if(arm2<180)
            {
                arm2++;
            }
			break;
		case '4':
		    if(arm2>90)
            {
                arm2--;
            }
			break;

		case '5':
            if((arm3<0))
            {
                arm3++;
            }
			break;
        case '6':
            if((arm3>-180))
            {
                arm3--;
            }
			break;
        case '7':
            if((arm5<0))
            {
                arm5++;
            }
			break;
        case '8':
            if((arm5>-90))
            {
                arm5--;
            }
			break;
        case '9':

            if((arm4<0))
            {
                arm4++;
            }
			break;
        case '0':
            if((arm4>-90))
            {
                arm4--;
            }
			break;
        case 'w':
			if(arm1Rot2>0)
            {
                arm1Rot2--;
            }
			break;
        case 'q':
			if(arm1Rot2<90)
            {
                arm1Rot2++;
            }
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
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
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/

	drawAxes();
	drawGrid();
	glColor3f(1,1,1);


    glRotatef(arm1Rot2,0,1,0);

    //****************************************************ARM 1 **************************************************/
	glPushMatrix();
	{
	    glTranslatef(0,arm1Length*arm1Scale*sin(arm1*DEG2RAD),-arm1Length*arm1Scale*cos(arm1*DEG2RAD));
        glRotatef(arm1,1,0,0);
        glScalef(1,1,arm1Scale);
        glutWireSphere(arm1Length,10,10);
	}glPopMatrix();

        //****************************************************ARM 1  END  **************************************************/


        //****************************************************ARM 2 **************************************************/
        double d=sqrt(pow(arm1Length*arm1Scale*2,2)+pow(arm2Length*arm2Scale,2)-2*arm1Length*arm1Scale*2*arm2Length*arm2Scale*cos((arm2)*DEG2RAD));
        double y=(asin(((arm2Length*arm2Scale)/(d))*sin((arm2)*DEG2RAD)))*(RAD2DEG);
        double totalAngle=y-arm1;
        //printf("d %lf y %lf totalAngle %lf\n",d,y,totalAngle);

        glPushMatrix();
        {

            glTranslatef(0,-d*sin(totalAngle*DEG2RAD),-d*cos(totalAngle*DEG2RAD) );
            glRotatef(arm1,1,0,0);
            glRotatef(arm2,1,0,0);
            glScalef(1,1,arm2Scale);
            glutWireSphere(arm2Length,10,10);
        }glPopMatrix();

        //****************************************************ARM 2 END **************************************************/


        //****************************************************ARM 3 **************************************************/

         d=sqrt(pow(arm1Length*arm1Scale*2,2)+pow(arm2Length*arm2Scale*2,2)-2*arm1Length*arm1Scale*2*arm2Length*arm2Scale*2*cos((arm2)*DEG2RAD));
         y=(asin(((arm2Length*arm2Scale*2)/(d))*sin((arm2)*DEG2RAD)))*(RAD2DEG);
         totalAngle=y-arm1;
            glTranslatef(0,-d*sin(totalAngle*DEG2RAD),-d*cos(totalAngle*DEG2RAD) );
            glRotatef(arm1,1,0,0);
            glRotatef(arm3,0,1,0);
            glRotatef(arm2,1,0,0);
            glScalef(1,1,arm3Scale);
            glColor3f(1,0,0);
            glBegin(GL_TRIANGLES);{
            glVertex3f(0,0,0);
            glVertex3f(-arm3Length,0,arm3Length);
            glVertex3f(arm3Length,0,arm3Length);
            }glEnd();

        //****************************************************ARM 3 END **************************************************/



        //****************************************************ARM 4 **************************************************/


        glColor3f(1,1,1);
        glPushMatrix();
        {
            glTranslatef(-arm3Length,arm4Length*arm4Scale*sin(-arm4*DEG2RAD),arm3Length+arm4Length*arm4Scale-arm4Length*arm4Scale*sin(-arm4*DEG2RAD));
;
            glRotatef(arm4,1,0,0);
            glScalef(1,1,arm4Scale);
            glutWireSphere(arm4Length,10,10);
        }glPopMatrix();

        //****************************************************ARM 4 END **************************************************/



        //****************************************************ARM 5(Rotate) **************************************************/
        glPushMatrix();
        {

            glTranslatef(arm3Length,0,(arm3Length+arm4Length*arm4Scale));
            glRotatef(arm5,0,0,1);
            glScalef(1,1,arm4Scale);
            glutWireSphere(arm4Length,10,10);
        }glPopMatrix();

        //****************************************************ARM 5(Rotate) END **************************************************/

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){

	//codes for any changes in Models, Camera
//	printf("temp1: %lf temp2: %lf temp3: %lf\n",temp1,temp2,temp3);
//	printf("p1: %lf p2: %lf p3: %lf\n",p1,p2,p3);
//	printf("arm1: %lf  arm2 %lf  arm3 %lf\n",arm1,arm2,arm3);
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;




	arm1=0;
	arm1Length=25;
	arm1Scale=4;


	arm2=180;
	arm2Length=25;
	arm2Scale=4;

	arm3=0;
	arm3Length=40;
	arm3Scale=1;

	arm4=0;
	arm4Scale=2;
	arm4Length=10;

	arm5=0;
	arm1Rot2=0;


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
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Assignment program3");

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
