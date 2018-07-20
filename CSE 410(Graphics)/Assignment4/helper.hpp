#ifndef HELPER_HPP_INCLUDED
#define HELPER_HPP_INCLUDED
#define AMBIENT 0
#define DIFFUSE  1
#define SPECULAR 2
#define REFLECTION 3
#define WHITE 0,0,0
#define BLACK 1,1,1
#define EPSILON 0.0000001
#define pi (2*acos(0.0))
#include<vector>
#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>
#include <GL/glut.h>
#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>
#include<iostream>
#include<float.h>
#include "bitmap_image.hpp"
using namespace std;

int recursion_level;




class Vector3 {

public:
    double x,y,z;

    Vector3(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3() {}

    Vector3 operator+(Vector3 Value) {
        return Vector3(x+Value.x, y+Value.y, z+Value.z);
    }
//
    Vector3 operator-(Vector3 Value) {
        return Vector3(x-Value.x, y-Value.y, z-Value.z);
    }
//
    Vector3 operator*(double Value) {
        return Vector3(x*Value, y*Value, z*Value);
    }
//
    Vector3 operator/(double Value) {
        //Vector3 temp(x/Value, y/Value, z/Value);
        return Vector3(x/Value, y/Value, z/Value);
    }

    friend ostream &operator<<(ostream &output, Vector3 point) {
        output<<point.x<<","<<point.y<<","<<point.z<<endl;
        return output;
    }

    Vector3 normalize() {

        return *this / getLength();
    }
    double getLength()
    {
        return sqrt(x*x+y*y+z*z);
    }

};
class Ray {

public:

     Vector3 start;
     Vector3 dir;

    Ray( Vector3 start1, Vector3 dir1) {
        start = start1;
        dir = dir1.normalize();
    }
    friend ostream &operator<<(ostream &output, Ray r) {
        output<<r.start.x<<","<<r.start.y<<","<<r.start.z<<endl;
        return output;
    }
};
class Object {

public:

    Vector3 reference_point;

    double height, width, length, source_factor, refractorIndex ;

    double shine;

    double color[3];

    double co_efficients[4];

    Object() {
        source_factor = 1.0;
        refractorIndex = 1.5;
    }

    virtual void draw() = 0;

    virtual double getRayObjectIntersection(Ray *r) = 0;

    virtual Vector3 getNormal(Vector3 intersectionPoint) = 0;

    virtual double intersect(Ray* r, double current_color[3], int currentLevel) = 0;

    void setColor(double r, double g, double b) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    void setShine(int shine) {
        this->shine = shine;
    }

    void setCoEfficients(double ambient, double diffuse, double specular, double reflection) {
        co_efficients[AMBIENT] = ambient;
        co_efficients[DIFFUSE] = diffuse;
        co_efficients[SPECULAR] = specular;
        co_efficients[REFLECTION] = reflection;
    }

    double dotProduct(Vector3 a, Vector3 b) {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }
//
    Vector3 crossProduct(Vector3 a, Vector3 b) {
        return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
    }
//
    Vector3 getReflection(Ray* ray, Vector3 normal) {
        Vector3 reflection = ray->dir - normal * 2.0 * dotProduct(ray->dir, normal);
        return reflection.normalize();
    }
//
    Vector3 getRefraction(Ray* ray, Vector3 normal) {



        double n_dot_i = dotProduct(normal, ray->dir);
        double k = 1.0 - refractorIndex * refractorIndex * (1.0 - n_dot_i * n_dot_i);
        Vector3 out(0, 0, 0);
        if (k >=0) {
            out =  ray->dir * refractorIndex - normal * (refractorIndex * n_dot_i + sqrt(k));
            out = out.normalize();
        }

        return out;
    }

};
class Light{
public:
    Vector3 location;
    Light(Vector3 pos)
    {
        location=pos;
    }
    Light(double x, double y, double z)
    {
        location=Vector3(x,y,z);
    }
    void draw()
    {
        glPushMatrix();
        {
            glColor3f(50,0,0);
            glTranslatef(location.x, location.y, location.z);
            glutSolidSphere(1.5,100,100);
        }glPopMatrix();
    }
    Vector3 operator-(Vector3 Value) {
        return Vector3(location.x-Value.x, location.y-Value.y, location.z-Value.z);
    }
};
std::vector <Object*> objects;
std::vector <Light> lights;
class Sphere: public Object {

public:

    Sphere(Vector3 center, double radius) {
        reference_point = center;
        length = radius;
    }

    void draw() {

        glPushMatrix();
        {
            glColor3f(color[0],color[1],color[2]);
            glTranslatef(reference_point.x,reference_point.y,reference_point.z);
            glutSolidSphere(length,100,100);
        }glPopMatrix();

    }

    double getRayObjectIntersection(Ray* ray) {

        Vector3 start = ray->start - reference_point;

        double a = dotProduct(ray->dir, ray->dir);
        double b = 2 * dotProduct(ray->dir, start);
        double c = dotProduct(start, start) - length*length;

        double determinent = b*b - 4*a*c;


        if (determinent < 0) {
            return -1;
        }

        double t1 = (- b + sqrt(determinent)) / (2.0*a);
        double t2 = (- b - sqrt(determinent)) / (2.0*a);
        return min(t1,t2);
    }

    Vector3 getNormal(Vector3 intersectionPoint) {
        Vector3 normal = intersectionPoint - reference_point;
        return normal.normalize();
    }

     double intersect(Ray* ray, double current_color[3], int currentLevel){

         //cout<<1<<endl;

        double t = getRayObjectIntersection(ray);
        if(t<=0)
        {
            return-1;
        }
        if(currentLevel==0)
        {
            return t;
        }
        for(int j=0;j<3;j++)
        {
            current_color[j]=color[j]*co_efficients[AMBIENT];
        }
        Vector3 intersectionPoint = ray->start + ray->dir * t;
        Vector3 normal = getNormal(intersectionPoint);
        Vector3 reflection = getReflection(ray, normal);
        Vector3 refraction = getRefraction(ray, normal);
        for (int i=0; i<(int)(lights.size()); i++) {
            Vector3 direction = lights[i] - intersectionPoint;
            direction = direction.normalize();
            Vector3 start = intersectionPoint + direction;//Change it
            Ray L(start, direction);


                double lambert = dotProduct(L.dir, normal);
                double phong = pow(dotProduct(reflection, ray->dir), shine);
                lambert=max(0.0,lambert);
                phong=max(0.0,phong);
                for (int k=0; k<3; k++) {
                    current_color[k] += source_factor * lambert * co_efficients[DIFFUSE] * color[k];
                    current_color[k] += source_factor * phong * co_efficients[SPECULAR] * color[k];
                }

            //cout<<4<<endl;
            if (currentLevel < recursion_level) {

                start = intersectionPoint + reflection;

                Ray reflectionRay(start, reflection);

                int nearestObjectID=-10;
                double minIntersectionPoint = DBL_MAX;
                double reflectedColor[3];

                for (int k=0; k < (int)(objects.size()); k++) {

                    double reflectionIntersection = objects[k]->getRayObjectIntersection(&reflectionRay);

                    if(reflectionIntersection <= 0) {
                        continue;
                    } else if (reflectionIntersection < minIntersectionPoint) {
                        minIntersectionPoint = reflectionIntersection;
                        nearestObjectID = k;
                    }

                }

                if(nearestObjectID!=-10) {

                    objects[nearestObjectID]->intersect(&reflectionRay, reflectedColor, currentLevel+1);

                    for (int k=0; k<3; k++) {
                        current_color[k] += reflectedColor[k] * co_efficients[REFLECTION];
                    }
                }
                //cout<<5<<endl;

                start = intersectionPoint + refraction;

                Ray refractionRay(start, refraction);

                nearestObjectID=-10;
                minIntersectionPoint = DBL_MAX;
                double refractedColor[3];

                for (int k=0; k < (int)(objects.size()); k++) {

                    double refractionIntersection = objects[k]->getRayObjectIntersection(&refractionRay);

                    if(refractionIntersection <= 0) {
                        continue;
                    } else if (refractionIntersection < minIntersectionPoint) {
                        minIntersectionPoint= refractionIntersection;
                        nearestObjectID = k;
                    }

                    //cout<<tk<<endl;
                }

                if(nearestObjectID!=-10) {

                    objects[nearestObjectID]->intersect(&refractionRay, refractedColor, currentLevel+1);

                    for (int k=0; k<3; k++) {
                        current_color[k] += refractedColor[k] * refractorIndex;
                    }
                }
            }

            for (int k=0; k<3; k++) {
                current_color[k]=min(current_color[k],1.0);
                current_color[k]=max(current_color[k],0.0);
            }


        }

        return t;
    }


};

class Floor: public Object {

public:
    bitmap_image texture;
    Floor(double floorWidth, double tileWidth,string image_name) {
        reference_point = Vector3(-floorWidth/2, -floorWidth/2, 0);
        length = tileWidth;
        texture=bitmap_image(image_name);

    }

    void draw() {

        int tileCount = abs(reference_point.x*2/length);
        //int seq = 0;

        for (int i=0; i<tileCount; i++)
        {
            for (int j=0; j<tileCount; j++)
                {
                    ((i+j)&1)?glColor3f(WHITE):glColor3f(BLACK);
                    glBegin(GL_QUADS);
                    {
                        glVertex3f(reference_point.x+length*i, reference_point.y+length*j, reference_point.z);
                        glVertex3f(reference_point.x+length*(i+1), reference_point.y+length*j, reference_point.z);
                        glVertex3f(reference_point.x+length*(i+1), reference_point.y+length*(j+1), reference_point.z);
                        glVertex3f(reference_point.x+length*i, reference_point.y+length*(j+1), reference_point.z);
                    }
                    glEnd();
                }
        }
    }




    Vector3 getNormal(Vector3 intersection) {
        //Change it
        return Vector3 (0,0,1);
    }
//
    double getRayObjectIntersection(Ray* ray) {

        Vector3 normal = getNormal(reference_point);
        return -dotProduct(normal, ray->start)  / dotProduct(normal, ray->dir);;
    }
//
//
    double intersect(Ray* ray, double current_color[3], int currentLevel) {

        double t = getRayObjectIntersection(ray);
        Vector3 intersectionPoint = ray->start + ray->dir * t;


        if ((reference_point.x > intersectionPoint.x) || (intersectionPoint.x > -reference_point.x) ||
                (reference_point.y> intersectionPoint.y) || (intersectionPoint.y > -reference_point.y)) {
            return -1;
        }
        if(currentLevel==0)
        {
                return t;
        }
        int x = ((intersectionPoint.x -reference_point.x)/length);
        int y = ((intersectionPoint.y -reference_point.y)/length) ;
        int textureX=(texture.height()/abs(reference_point.x*2))*(intersectionPoint.x +abs(reference_point.x));
        int textureY=(texture.width()/abs(reference_point.y*2))*(intersectionPoint.y +abs(reference_point.y));
        unsigned char red,green,blue;
        texture.get_pixel(textureY,textureX,red,green,blue);
        double textureColor[]={(double)(red),(double)(green),(double)(blue)};
        if ((x+y)&1) {
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
        } else {
            color[0] = 1;
            color[1] = 1;
            color[2] = 1;
        }

        for (int i=0; i<3; i++) {
            current_color[i] = color[i] *co_efficients[AMBIENT] *textureColor[i]/255.0;
        }


        Vector3 normal = getNormal(intersectionPoint);
        Vector3 reflection = getReflection(ray, normal);
        Vector3 refraction = getRefraction(ray, normal);

        for (int i=0; i<(int)(lights.size()); i++) {

            Vector3 direction = lights[i] - intersectionPoint;
            direction = direction.normalize();

            Vector3 start = intersectionPoint + direction;//change it
            Ray L(start, direction);

                double lambert = dotProduct(L.dir, normal);
                double phong = pow(dotProduct(reflection, ray->dir), shine);
                lambert=max(0.0,lambert);
                phong=max(0.0,phong);
                for (int k=0; k<3; k++) {
                    current_color[k] += source_factor * lambert * co_efficients[DIFFUSE] * color[k];
                    current_color[k] += source_factor * phong * co_efficients[SPECULAR] * color[k];
                }


            if (currentLevel < recursion_level) {

                start = intersectionPoint + reflection ;

                Ray reflectionRay(start, reflection);

                int nearestObjectID=-10;
                double minIntersectionPoint = DBL_MAX;
                double reflectedColor[3];

                for (int k=0; k < (int)(objects.size()); k++) {

                    double reflectionIntersection = objects[k]->getRayObjectIntersection(&reflectionRay);

                    if(reflectionIntersection <= 0) {
                        continue;
                    } else if (reflectionIntersection < minIntersectionPoint) {
                        minIntersectionPoint = reflectionIntersection;
                        nearestObjectID = k;
                    }

                }

                if(nearestObjectID!=-10) {

                    objects[nearestObjectID]->intersect(&reflectionRay, reflectedColor, currentLevel+1);

                    for (int k=0; k<3; k++) {
                        current_color[k] += reflectedColor[k] * co_efficients[REFLECTION];
                    }
                }
                start = intersectionPoint + refraction;

                Ray refractionRay(start, refraction);

                nearestObjectID=-10;
                minIntersectionPoint = DBL_MAX;
                double refractedColor[3];

                for (int k=0; k < (int)(objects.size()); k++) {

                    double refractionIntersection = objects[k]->getRayObjectIntersection(&refractionRay);

                    if(refractionIntersection <= 0) {
                        continue;
                    } else if (refractionIntersection < minIntersectionPoint) {
                        minIntersectionPoint= refractionIntersection;
                        nearestObjectID = k;
                    }

                    //cout<<tk<<endl;
                }

                if(nearestObjectID!=-10) {

                    objects[nearestObjectID]->intersect(&refractionRay, refractedColor, currentLevel+1);

                    for (int k=0; k<3; k++) {
                        current_color[k] += refractedColor[k] * refractorIndex;
                    }
                }
            }

            for (int k=0; k<3; k++) {
                current_color[k]=min(current_color[k],1.0);
                current_color[k]=max(current_color[k],0.0);
            }


        }

        return t;
   }
};

class Triangle: public Object {

public:

    Vector3 a, b, c;

    Triangle(Vector3 a1, Vector3 b1, Vector3 c1) {
        a = a1;
        b = b1;
        c = c1;
    }

    void draw() {
        glColor3f(color[0],color[1],color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
    }

    Vector3 getNormal(Vector3 intersection)
    {
        Vector3 normal = crossProduct((b-a), (c-a));
        return normal.normalize();
    }

    double getRayObjectIntersection(Ray* ray) {


        Vector3 edge1 = b - a;
        Vector3 edge2 = c - a;

        Vector3 h = crossProduct(ray->dir, edge2);
        double algo_a = dotProduct(edge1, h);

        if(algo_a > -EPSILON && algo_a < EPSILON) {
            return -1;
        }

        double f = 1.0 / algo_a;

        Vector3 s = ray->start - a;

        double u = dotProduct(s, h) * f;

        if(u < 0 || u > 1) {
            return -1;
        }

        Vector3 q = crossProduct(s, edge1);

        double v = dotProduct(ray->dir, q) * f;

        if(v < 0 || u + v  > 1.0) {
            return -1;
        }

        double t = dotProduct(edge2, q) * f;

        if(t > EPSILON) {
            return t;
        }

        return -1;
    }


    double intersect(Ray* ray, double current_color[3], int currentLevel) {

        double t = getRayObjectIntersection(ray);
        Vector3 intersectionPoint = ray->start + ray->dir * t;
        if(t<=0)
        {
            return-1;
        }
        if(currentLevel==0)
        {
            return t;
        }
        for(int j=0;j<3;j++)
        {
            current_color[j]=color[j]*co_efficients[AMBIENT];
        }


        Vector3 normal = getNormal(intersectionPoint);
        Vector3 reflection = getReflection(ray, normal);
        //Vector3 refraction = getRefraction(ray, normal);

        for (int i=0; i<(int)(lights.size()); i++) {

            Vector3 direction = lights[i] - intersectionPoint;
            direction = direction.normalize();
            if (dotProduct(direction, normal) > 0) {
                normal = normal * (-1);
            }

            Vector3 start = intersectionPoint + direction;
            Ray L(start, direction);

                double lambert = dotProduct(L.dir, normal);
                double phong = pow(dotProduct(reflection, ray->dir), shine);
                lambert=max(0.0,lambert);
                phong=max(0.0,phong);
                for (int k=0; k<3; k++) {
                    current_color[k] += source_factor * lambert * co_efficients[DIFFUSE] * color[k];
                    current_color[k] += source_factor * phong * co_efficients[SPECULAR] * color[k];
                }

            if (currentLevel < recursion_level) {

                start = intersectionPoint + reflection;

                Ray reflectionRay(start, reflection);

                int nearestObjectID=-10;
                double minIntersectionPoint = DBL_MAX;
                double reflectedColor[3];

                for (int k=0; k < (int)(objects.size()); k++) {

                    double reflectionIntersection = objects[k]->getRayObjectIntersection(&reflectionRay);

                    if(reflectionIntersection <= 0) {
                        continue;
                    } else if (reflectionIntersection < minIntersectionPoint) {
                        minIntersectionPoint = reflectionIntersection;
                        nearestObjectID = k;
                    }

                }

                if(nearestObjectID!=-10) {

                    objects[nearestObjectID]->intersect(&reflectionRay, reflectedColor, currentLevel+1);

                    for (int k=0; k<3; k++) {
                        current_color[k] += reflectedColor[k] * co_efficients[REFLECTION];
                    }
                }
//                start = intersectionPoint + refraction;
//
//                Ray refractionRay(start, refraction);
//
//                nearestObjectID=-10;
//                minIntersectionPoint = DBL_MAX;
//                double refractedColor[3];
//
//                for (int k=0; k < (int)(objects.size()); k++) {
//
//                    double refractionIntersection = objects[k]->getRayObjectIntersection(&refractionRay);
//
//                    if(refractionIntersection <= 0) {
//                        continue;
//                    } else if (refractionIntersection < minIntersectionPoint) {
//                        minIntersectionPoint= refractionIntersection;
//                        nearestObjectID = k;
//                    }
//
//                    //cout<<tk<<endl;
//                }
//
//                if(nearestObjectID!=-10) {
//
//                    objects[nearestObjectID]->intersect(&refractionRay, refractedColor, currentLevel+1);
//
//                    for (int k=0; k<3; k++) {
//                        current_color[k] += refractedColor[k] * refractorIndex;
//                    }
//                }
            }

            for (int k=0; k<3; k++) {
                current_color[k]=min(current_color[k],1.0);
                current_color[k]=max(current_color[k],0.0);

            }


        }

        return t;
   }




};

class GeneralQuadratic: public Object {

public:

    double A, B, C, D, E, F, G, H, I, J;

    GeneralQuadratic(double coeff[10], Vector3 reff, double length1, double width1, double height1) {
        A = coeff[0];
        B = coeff[1];
        C = coeff[2];
        D = coeff[3];
        E = coeff[4];
        F = coeff[5];
        G = coeff[6];
        H = coeff[7];
        I = coeff[8];
        J = coeff[9];
        reference_point = reff;
        height = height1;
        width = width1;
        length = length1;
    }

    void draw() {}

    Vector3 getNormal(Vector3 intersectionPoint) {

        double df_dx = 2 * A * intersectionPoint.x + D * intersectionPoint.y + F * intersectionPoint.z  + G;
        double df_dy = 2 * B * intersectionPoint.y + D * intersectionPoint.x + E * intersectionPoint.z  + H;
        double df_dz = 2 * C * intersectionPoint.z + E * intersectionPoint.y + F * intersectionPoint.x  + I;

        Vector3 normal(df_dx, df_dy, df_dz);

        return normal.normalize();
    }

    double getRayObjectIntersection(Ray* ray) {

        double a = A * pow(ray->dir.x,2) +
                   B * pow(ray->dir.y,2) +
                   C * pow(ray->dir.z,2) +
                   (D*ray->dir.x * ray->dir.y) +
                   (E*ray->dir.y * ray->dir.z) +
                   (F*ray->dir.z * ray->dir.x);
        double b = 2 * (A * ray->start.x * ray->dir.x + B * ray->start.y * ray->dir.y + C * ray->start.z * ray->dir.z)+
                        D * (ray->start.x * ray->dir.y + ray->dir.x * ray->start.y)+
                        E * (ray->start.y * ray->dir.z + ray->dir.y * ray->start.z)+
                        F * (ray->start.z * ray->dir.x + ray->dir.z * ray->start.x)+
                        G * ray->dir.x +
                        H * ray->dir.y +
                        I * ray->dir.z;
        double c = A *pow(ray->start.x,2)+
                   B *pow(ray->start.y,2)+
                   C *pow(ray->start.z,2)+
                   (D * ray->start.x * ray->start.y) +
                   (E * ray->start.y * ray->start.z) +
                   (F * ray->start.z * ray->start.x) +
                   (G * ray->start.x) +
                   (H * ray->start.y) +
                   (I * ray->start.z) +
                    J;

        double d = b*b - 4*a*c;

        if (d < 0) {
            return -1;
        }

        double t1 = (- b + sqrt(d)) / (2.0*a);
        double t2 = (- b - sqrt(d)) / (2.0*a);


        Vector3 intersection_point1 = ray->start + ray->dir * t1;
        Vector3 intersection_point2 = ray->start + ray->dir * t2;

        double x_min = reference_point.x;
        double x_max = reference_point.x + length;

        double y_min = reference_point.y;
        double y_max = reference_point.y + width;

        double z_min = reference_point.z;
        double z_max = reference_point.z + height;
        bool point1_within_volume = (((length<=0)||(x_min <= intersection_point1.x && intersection_point1.x <= x_max)) &&
                       ((width<=0)||(y_min <= intersection_point1.y && intersection_point1.y <= y_max)) &&
                       ((height<=0)||(z_min <= intersection_point1.z && intersection_point1.z <= z_max)));

        bool point2_within_volume = (((length<=0)||(x_min <= intersection_point2.x && intersection_point2.x <= x_max)) &&
                       ((width<=0)||(y_min <= intersection_point2.y && intersection_point2.y <= y_max)) &&
                       ((height<=0)||(z_min <= intersection_point2.z && intersection_point2.z <= z_max)));


        if(point1_within_volume && point2_within_volume)
        {
            return min(t1,t2);
        }
        else if (point1_within_volume)
        {


            return t1;
        }
        else if(point2_within_volume)
        {


            return t2;
        }
        else
        {
            return -1;
        }
    }



    double intersect(Ray* ray, double current_color[3], int currentLevel)
    {


        double t = getRayObjectIntersection(ray);
        if(t<=0)
        {
            return-1;
        }
        if(currentLevel==0)
        {
            return t;
        }
        for(int j=0;j<3;j++)
        {
            current_color[j]=color[j]*co_efficients[AMBIENT];
        }

        Vector3 intersectionPoint = ray->start + ray->dir * t;
        Vector3 normal = getNormal(intersectionPoint);
        Vector3 reflection = getReflection(ray, normal);
        //Vector3 refraction = getRefraction(ray, normal);


        for (int i=0; i<(int)(lights.size()); i++) {

            Vector3 direction = lights[i] - intersectionPoint;
            direction = direction.normalize();
            Vector3 start = intersectionPoint + direction;//Change it
            Ray L(start, direction);
            double phong = pow(dotProduct(reflection, ray->dir), shine);
            double lambert = dotProduct(L.dir, normal);
            lambert=max(0.0,lambert);
            phong=max(0.0,phong);
            for (int k=0; k<3; k++) {
                current_color[k] += source_factor * lambert * co_efficients[DIFFUSE] * color[k];
                current_color[k] += source_factor * phong * co_efficients[SPECULAR] * color[k];
            }


            if (currentLevel < recursion_level) {

                start = intersectionPoint + reflection * 1.0;

                Ray reflectionRay(start, reflection);

                int nearestObjectID=-10;
                double minIntersectionPoint = DBL_MAX;
                double reflectedColor[3];

                for (int k=0; k < (int)(objects.size()); k++) {

                    double reflectionIntersection = objects[k]->getRayObjectIntersection(&reflectionRay);

                    if(reflectionIntersection <= 0) {
                        continue;
                    } else if (reflectionIntersection < minIntersectionPoint) {
                        minIntersectionPoint = reflectionIntersection;
                        nearestObjectID = k;
                    }

                }

                if(nearestObjectID!=-10) {

                    objects[nearestObjectID]->intersect(&reflectionRay, reflectedColor, currentLevel+1);

                    for (int k=0; k<3; k++) {
                        current_color[k] += reflectedColor[k] * co_efficients[REFLECTION];
                    }
                }
                //cout<<5<<endl;

//                start = intersectionPoint + refraction;
//
//                Ray refractionRay(start, refraction);
//
//                nearestObjectID=-10;
//                minIntersectionPoint = DBL_MAX;
//                double refractedColor[3];
//
//                for (int k=0; k < (int)(objects.size()); k++) {
//
//                    double refractionIntersection = objects[k]->getRayObjectIntersection(&refractionRay);
//
//                    if(refractionIntersection <= 0) {
//                        continue;
//                    } else if (refractionIntersection < minIntersectionPoint) {
//                        minIntersectionPoint= refractionIntersection;
//                        nearestObjectID = k;
//                    }
//
//                    //cout<<tk<<endl;
//                }
//
//                if(nearestObjectID!=-10) {
//
//                    objects[nearestObjectID]->intersect(&refractionRay, refractedColor, currentLevel+1);
//
//                    for (int k=0; k<3; k++) {
//                        current_color[k] += refractedColor[k] * refractorIndex;
//                    }
//                }
            }

            for (int k=0; k<3; k++) {
                current_color[k]=min(current_color[k],1.0);
                current_color[k]=max(current_color[k],0.0);
            }


        }

        return t;
    }




};

#endif
