#include <iostream>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include "helper.hpp"
#include "bitmap_image.hpp"
#include<float.h>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>
#include <GL/glut.h>
#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#define pi (2*acos(0.0))
#define DEG2RAD 3.14159/180.0
#define possine sin(pi/60)
#define poscosine cos(pi/60)
#define negsine sin(-pi/60)
#define negcosine cos(-pi/60)


#define fov 80

int image_width, image_height,window_width,window_height;//Change it

Vector3 pos(0, -100, 10);
Vector3 l(0, 1, 0);
//Vector3 l(-1/sqrt(2), -1/sqrt(2), 0);
//Vector3 r(-1/sqrt(2), 1/sqrt(2), 0);
Vector3 r(1, 0, 0);
Vector3 u(0, 0, 1);
double lookVector, upVector, rightVector;//, thick = 0.5;

void capture();

void keyboardListener(unsigned char key, int x,int y) {


    switch(key) {

    case '1':

        l.x = l.x * poscosine - l.y * possine;
        l.y = l.x * possine + l.y * poscosine;

        lookVector = sqrt(l.x*l.x + l.y*l.y + l.z*l.z);

        l.x /= lookVector;
        l.y /= lookVector;

        r.x = r.x * poscosine - r.y * possine;
        r.y = r.x * possine + r.y * poscosine;

        rightVector = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);

        r.x /= rightVector;
        r.y /= rightVector;

        break;

    case '2':

        l.x = l.x * negcosine - l.y * negsine;
        l.y = l.x * negsine + l.y * negcosine;

        lookVector = sqrt(l.x*l.x + l.y*l.y + l.z*l.z);

        l.x /= lookVector;
        l.y /= lookVector;

        r.x = r.x * negcosine - r.y * negsine;
        r.y = r.x * negsine + r.y * negcosine;

        rightVector = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);

        r.x /= rightVector;
        r.y /= rightVector;

        break;

    case '3':

        l.x = l.x * poscosine + u.x * possine;
        l.y = l.y * poscosine + u.y * possine;
        l.z = l.z * poscosine + u.z * possine;

        u.x = u.x * poscosine - l.x * possine;
        u.y = u.y * poscosine - l.y * possine;
        u.z = u.z * poscosine - l.z * possine;

        break;

    case '4':

        l.x = l.x * negcosine + u.x * negsine;
        l.y = l.y * negcosine + u.y * negsine;
        l.z = l.z * negcosine + u.z * negsine;

        u.x = u.x * negcosine - l.x * negsine;
        u.y = u.y * negcosine - l.y * negsine;
        u.z = u.z * negcosine - l.z * negsine;


        break;

    case '5':

        r.y = r.y * poscosine - r.z * possine;
        r.z = r.y * possine + r.z * poscosine;

        rightVector = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);

        r.y /= rightVector;
        r.z /= rightVector;

        u.y = u.y * poscosine - u.z * possine;
        u.z = u.y * possine + u.z * poscosine;

        upVector = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);

        u.y /= upVector;
        u.z /= upVector;

        break;

    case '6':

        r.y = r.y * negcosine - r.z * negsine;
        r.z = r.y * negsine + r.z * negcosine;

        rightVector = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);

        r.y /= rightVector;
        r.z /= rightVector;

        u.y = u.y * negcosine - u.z * negsine;
        u.z = u.y * negsine + u.z * negcosine;

        upVector = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);

        u.y /= upVector;
        u.z /= upVector;

        break;

    case '0':
        capture();
        break;

    case 'q':
        exit(0);
        break;

    default:
        break;
    }
}


void specialKeyListener(int key, int x,int y) {
    switch(key) {
    case GLUT_KEY_DOWN:
        pos.x+=2;
        break;
    case GLUT_KEY_UP:
        pos.x-=2;
        break;

    case GLUT_KEY_RIGHT:
        pos.y+=2;
        break;
    case GLUT_KEY_LEFT:
        pos.y-=2;
        break;

    case GLUT_KEY_PAGE_UP:
        pos.z+=2;
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos.z-=2;
        break;

    default:
        break;
    }
}
void drawAxes() {
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    {
        glVertex3f( 100,0,0);
        glVertex3f(-100,0,0);

        glVertex3f(0,-100,0);
        glVertex3f(0, 100,0);

        glVertex3f(0,0, 100);
        glVertex3f(0,0,-100);
    }
    glEnd();
}
void capture()
{   cout<<"start"<<endl;
    bitmap_image image(image_width, image_height);

    for (int i=0; i<image_width; i++) {
        for (int j=0; j<image_height; j++) {
            image.set_pixel(j, i,BLACK);
        }
    }

    double plane_distance = (window_height/2)/tan(fov*DEG2RAD/2);
    Vector3 top_left = pos + l * plane_distance - r * (window_width/2) + u * (window_height/2);
    double du = double(window_width) / image_width;
    double dv = double(window_height) / image_height;
    for (int i=0; i<image_width; i++) {
        for (int j=0; j<image_height; j++) {

            Vector3 corner = top_left + r*j*du - u*i*dv;
            Ray ray(pos, corner - pos);
            int nearestObjectID=-10;
            double minIntersectionPoint = DBL_MAX;
            double returnColor[3];
            //cout<<"corner "<<corner<<endl;

            for (int k=0; k < (int)(objects.size()); k++)
            {
                //cout<<i<<","<<j<<","<<k<<endl;

                double t = objects[k]->intersect(&ray,returnColor,0);
                //return;
                if(t <= 0) {
                    continue;
                } else if (t < minIntersectionPoint) {
                    minIntersectionPoint = t;
                    nearestObjectID = k;
                }
            }

                if(nearestObjectID!=-10)
                {

                    objects[nearestObjectID]->intersect(&ray, returnColor, 1);
                    image.set_pixel(j,i,returnColor[0]*255,returnColor[1]*255,returnColor[2]*255);

                }
            }
        }

    image.save_image("out.bmp");
    cout<<"end"<<endl;
}
void ActualData()
{
    int objectCount;
    freopen("scene.txt", "r", stdin);
    cin>>recursion_level;
    cin>>image_width;
    image_height=image_width;
    cin>>objectCount;
    string objectType;
    double radius,ambient,diffuse,specular,reflection,red,green,blue,x,y,z,shine;
    Object *temp;
    for(int i=0;i<objectCount;i++)
    {
        cin>>objectType;
        if(objectType=="sphere")
        {
            cin>>x>>y>>z;
            cin>>radius;
            cin>>red>>green>>blue;
            cin>>ambient>>diffuse>>specular>>reflection;
            cin>>shine;
            temp=new Sphere(Vector3(x,y,z),radius);
            temp->setColor(red,green,blue);
            temp->setCoEfficients(ambient,diffuse,specular,reflection);
            temp->setShine(shine);
            objects.push_back(temp);

        }
        else if(objectType=="triangle")
        {
            cin>>x>>y>>z;
            Vector3 edge1(x,y,z);
            cin>>x>>y>>z;
            Vector3 edge2(x,y,z);
            cin>>x>>y>>z;
            Vector3 edge3(x,y,z);
            cin>>red>>green>>blue;
            cin>>ambient>>diffuse>>specular>>reflection;
            cin>>shine;
            temp=new Triangle(edge1,edge2,edge3);
            temp->setColor(red,green,blue);
            temp->setCoEfficients(ambient,diffuse,specular,reflection);
            temp->setShine(shine);
            objects.push_back(temp);


        }
        else if(objectType=="general")
        {
            double co[10];
            for(int j=0;j<10;j++)
            {
                cin>>co[j];
            }
            cin>>x>>y>>z;
            Vector3 referencePoint(x,y,z);
            cin>>x>>y>>z;
            cin>>red>>green>>blue;
            cin>>ambient>>diffuse>>specular>>reflection;
            cin>>shine;
            temp=new GeneralQuadratic(co,referencePoint,x,y,z);
            temp->setColor(red,green,blue);
            temp->setCoEfficients(ambient,diffuse,specular,reflection);
            temp->setShine(shine);
            objects.push_back(temp);

        }
    }
    double lightNum;
    cin>>lightNum;
    for(int i=0;i<lightNum;i++)
    {
        cin>>x>>y>>z;
        Light light1(x,y,z);
        lights.push_back(light1);
    }
    temp=new Floor(1000,20,"bd.bmp");
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(0.2);
    objects.push_back(temp);

}
void loadTestData() {

    image_width = image_height = 768;


    Vector3 center(20,20,10);
    double radius = 10;

    Object *temp;
    Object *floor;

    temp = new Sphere(center, radius);
    temp->setColor(1,0,0);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    //objects.push_back(temp);
    temp = new Sphere(Vector3(20,80,20), radius);
    temp->setColor(0.93,0.51,0.93);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    //objects.push_back(temp);
    temp = new Triangle(Vector3(40,-20,20),Vector3(30,-20,0),Vector3(50,-20,0));
    temp->setColor(0,1,0);
    temp->setCoEfficients(0.4,0.2,0.1,0.3);
    temp->setShine(5);
    //objects.push_back(temp);
    floor=new Floor(1000,20,"wood.bmp");
    floor->setCoEfficients(0.4,0.2,0.2,0.2);
    floor->setShine(1);
    objects.push_back(floor);
    double coeff[] = {1, 1, 1 ,0 ,0 ,0 ,0, 0 ,0 ,-100};
    Vector3 reff(0,0,0);
    temp = new GeneralQuadratic(coeff, reff, 0, 0, 20);
    temp->setColor(1, 0, 1);
    temp->setCoEfficients(0.4, 0.2, 0.1, 0.3);
    temp->setShine(1);
    objects.push_back(temp);
    recursion_level=3;
    Light light1(-50,50,50);
    lights.push_back(light1);



}
void display() {

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-upVector camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera lookVectoring?
    //3. Which direction is the camera's upVector direction?

    //glulookVectorAt(100,100,100,	0,0,0,	0,0,1);
    //glulookVectorAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    //glulookVectorAt(0,0,200,	0,0,0,	0,1,0);
    gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    for (int k=0; k < (int)(objects.size()); k++)
    {
        objects[k]->draw();
    }
    for (int k=0; k < (int)(lights.size()); k++)
    {
        lights[k].draw();
    }

    glutSwapBuffers();
}


void animate() {
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init() {
    //codes for initialization
    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-upVector projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(fov,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance

}


int main(int argc, char **argv) {

    //freopen("out.txt", "w", stdout);
    ActualData();
    //loadActualData();
    window_height=window_width=800;
    glutInit(&argc,argv);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Ray Tracer");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);


    glutMainLoop();		//The main loop of OpenGL

    //freeMemory();

    return 0;
}
