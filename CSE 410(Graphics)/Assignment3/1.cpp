#include "bitmap_image.hpp"
#include <vector>
#include<sstream>
#include<stdlib.h>
#include<float.h>

using namespace std;

class Point{
public:
    double x;
    double y;
    double z;
    Point(double x1,double y1,double z1)
    {
        x=x1;
        y=y1;
        z=z1;
    }
    Point()
    {

    }
};


class Triangle{

public:
    Point pointlist[3];
    int color[3];
    Triangle(Point p1,Point p2, Point p3,int r, int g ,int b)
    {
        pointlist[0]=p1;
        pointlist[1]=p2;
        pointlist[2]=p3;
        color[0]=r;
        color[1]=g;
        color[2]=b;
    }
    Triangle(Point p1,Point p2, Point p3)
    {
        pointlist[0]=p1;
        pointlist[1]=p2;
        pointlist[2]=p3;
        color[0]=rand()% 256 ;
        color[1]=rand()% 256;
        color[2]=rand()% 256 ;
    }

    void print(void)
    {
        cout<<pointlist[0].x<<" "<<pointlist[0].y<<" "<<pointlist[0].z<<endl;
        cout<<pointlist[1].x<<" "<<pointlist[1].y<<" "<<pointlist[1].z<<endl;
        cout<<pointlist[2].x<<" "<<pointlist[2].y<<" "<<pointlist[2].z<<endl;
        cout<<"color:"<<endl;
        cout<<color[0]<<" "<<color[1]<<" "<<color[2]<<endl;
    }
    double getMinY()
    {
        return min(pointlist[0].y,min(pointlist[1].y,pointlist[2].y));
    }
    double getMaxY()
    {
         return max(pointlist[0].y,max(pointlist[1].y,pointlist[2].y));
    }

    double getMinX()
    {
        return min(pointlist[0].x,min(pointlist[1].x,pointlist[2].x));
    }
    double getMaxX()
    {
         return max(pointlist[0].x,max(pointlist[1].x,pointlist[2].x));
    }
};
class Config{
public:
    int screenWidth;
    int screenHeight;
    double leftLimit;
    double rightLimit;
    double bottomLimit;
    double topLimit;
    double nearDistance;
    double farDistance;
    Config()
    {

    }
    void print(void)
    {
        cout<<"Screen Width: "<<screenWidth<<" Screen Height: "<<screenHeight<<endl;
        cout<<"Left Limit: "<<leftLimit<<" Right Limit: "<<rightLimit<<endl;
        cout<<"Bottom Limit: "<<bottomLimit<<"Top Limit: "<<topLimit<<endl;
        cout<<"Near Distance: "<<nearDistance<<" Far Distance: "<<farDistance<<endl;
    }
};
std::vector<double> tokenize(string line)
{
    std::vector <double> tokens;
    stringstream check1(line);
    string intermediate;
    while(getline(check1, intermediate, ' '))
    {
        tokens.push_back(atof(intermediate.c_str()));
    }
    return tokens;
}
vector<Triangle> getStageInformation(string fileName)
{
    ifstream myfile (fileName.c_str());
    string line;
    vector <Triangle>triangleList;
    vector <Point>pointList;
    vector<double> tokens;
    while(getline(myfile,line))
    {
        if(line.length() > 1)
        {
            tokens=tokenize(line);
            pointList.push_back(Point(tokens[0],tokens[1],tokens[2]));
        }
        else if(line.length()==1)
        {
            triangleList.push_back(Triangle(pointList[0],pointList[1],pointList[2]));
            pointList.clear();
        }

    }
    triangleList.push_back(Triangle(pointList[0],pointList[1],pointList[2]));
    pointList.clear();
    return triangleList;
}
Config getConfigInfo(string fileName)
{
    ifstream myfile (fileName.c_str());
    string line;
    vector<double> tokens;
    Config configInfo=Config();

    getline(myfile,line);
    tokens=tokenize(line);
    configInfo.screenWidth=tokens[0];
    configInfo.screenHeight=tokens[1];

    getline(myfile,line);
    tokens=tokenize(line);
    configInfo.leftLimit=tokens[0];
    configInfo.rightLimit=-tokens[0];

    getline(myfile,line);
    tokens=tokenize(line);
    configInfo.bottomLimit=tokens[0];
    configInfo.topLimit=-tokens[0];

    getline(myfile,line);
    tokens=tokenize(line);
    configInfo.nearDistance=tokens[0];
    configInfo.farDistance=tokens[1];

    return configInfo;

}
double ** initialize(Config configInfo,bitmap_image image)
{

    double **zBuffer = (double**)malloc(configInfo.screenWidth *sizeof(double*));
    for(int i = 0; i < configInfo.screenWidth; i++) zBuffer[i] = (double *)malloc(configInfo.screenHeight* sizeof(double));
    for(int i=0;i<configInfo.screenHeight;i++)
     {

         for(int j=0;j<configInfo.screenWidth;j++)
         {

             zBuffer[i][j]=abs(configInfo.farDistance-configInfo.nearDistance);
         }
     }
     for(int i=0;i<configInfo.screenWidth;i++){
        for(int j=0;j<configInfo.screenHeight;j++){
            image.set_pixel(i,j,0,0,0);

        }
    }
    return zBuffer;
}
 void dump(string fileName,double ** buffer,Config configInfo)
 {

     FILE *fp;
     fp = fopen(fileName.c_str(), "w");
     for(int i=0;i<configInfo.screenWidth;i++)
     {
         for(int j=0;j<configInfo.screenHeight;j++)
         {
             if(buffer[i][j]<configInfo.farDistance)
             {
                 fprintf(fp,"%0.6lf\t",buffer[i][j]);
             }

         }
         fprintf(fp,"\n");
     }
     fclose(fp);
 }
 vector<Point> get_intersection_point(Triangle triangle,double row,double minX,double maxX,double minY,double maxY)
 {
     vector<Point> pointList;

     Point p1;
     p1.y=row;
     double m1=(p1.y-triangle.pointlist[0].y)/(triangle.pointlist[1].y-triangle.pointlist[0].y);
     p1.x = triangle.pointlist[0].x + m1 * (triangle.pointlist[1].x - triangle.pointlist[0].x);
     p1.z = triangle.pointlist[0].z + m1 * (triangle.pointlist[1].z - triangle.pointlist[0].z);



     Point p2;
     p2.y=row;
     double m2=(p2.y-triangle.pointlist[1].y)/(triangle.pointlist[2].y-triangle.pointlist[1].y);
     p2.x = triangle.pointlist[1].x + m2* (triangle.pointlist[2].x - triangle.pointlist[1].x);
     p2.z = triangle.pointlist[1].z + m2 * (triangle.pointlist[2].z - triangle.pointlist[1].z);



     Point p3;
     p3.y=row;
     double m3=(p3.y-triangle.pointlist[0].y)/(triangle.pointlist[2].y-triangle.pointlist[0].y);
     p3.x = triangle.pointlist[0].x + m3* (triangle.pointlist[2].x - triangle.pointlist[0].x);
     p3.z = triangle.pointlist[0].z + m3 * (triangle.pointlist[2].z - triangle.pointlist[0].z);


     Point left_intersection_point,right_intersection_point;

    //cout<<"x "<<p1.x<<"y "<<p1.y<<"z "<<p1.z<<endl;
    //cout<<"x "<<p2.x<<"y "<<p2.y<<"z "<<p2.z<<endl;
    //cout<<"x "<<p3.x<<"y "<<p3.y<<"z "<<p3.z<<endl;

     if((p1.x>=minX && p1.x<=maxX) && (p2.x>=minX && p2.x<=maxX))
     {
         left_intersection_point=p1;
         right_intersection_point=p2;
     }
     else if((p2.x>=minX && p2.x<=maxX) && (p3.x>=minX && p3.x<=maxX))
     {
         left_intersection_point=p2;
         right_intersection_point=p3;
     }
     else if((p3.x>=minX && p3.x<=maxX) && (p1.x>=minX && p1.x<=maxX))
     {
         left_intersection_point=p3;
         right_intersection_point=p1;
     }
     if(left_intersection_point.x>right_intersection_point.x)
     {
         pointList.push_back(right_intersection_point);
         pointList.push_back(left_intersection_point);
     }
     else
     {
         pointList.push_back(left_intersection_point);
         pointList.push_back(right_intersection_point);
     }
     return pointList;

 }
int main()
{


    Config configInfo=getConfigInfo("config.txt");
    configInfo.print();

    vector<Triangle> triangleList=getStageInformation("stage3.txt");

    bitmap_image image(configInfo.screenWidth,configInfo.screenHeight);

    double dx=(configInfo.rightLimit-configInfo.leftLimit)/((double)configInfo.screenWidth);
    double dy=(configInfo.topLimit-configInfo.bottomLimit)/((double)configInfo.screenHeight);
    double top_y=configInfo.topLimit-(dy/2);
    double left_x=configInfo.leftLimit+(dx/2);
    double ** zBuffer=initialize(configInfo,image);


    for(int i=0;i<triangleList.size();i++)
    {
        Triangle currentTriangle=triangleList[i];

        double minY = triangleList[i].getMinY();
        double maxY = triangleList[i].getMaxY();
        double minX = triangleList[i].getMinX();
        double maxX = triangleList[i].getMaxX();;

        double topScanline = maxY>configInfo.topLimit? configInfo.topLimit : maxY;
        double bottomScanline = minY<configInfo.bottomLimit? configInfo.bottomLimit :minY;

        double left_most_column=minX>configInfo.leftLimit? minX:configInfo.leftLimit;
        double right_most_column=maxX<configInfo.rightLimit? maxX:configInfo.rightLimit;

        int scanRowStart = round((top_y - topScanline)/dy);
        int scanRowEnd = round((top_y - bottomScanline)/dy);
        //cout<<"topy "<<top_y<<"left_X "<<left_x<<endl;
        //cout<<"row "<<scanRowStart<<","<<scanRowEnd<<endl;
        for(int row=scanRowStart;row<scanRowEnd;row++)
        {
           vector<Point> pointList=get_intersection_point(currentTriangle,top_y -row*dy,minX,maxX,minY,maxY);
           //cout<<"left "<<pointList[0].x<<","<<pointList[1].x<<endl;

            double left_Intersecting_Column  = pointList[0].x>left_most_column  ?  pointList[0].x : left_most_column;
            double right_Intersecting_Column = pointList[1].x<right_most_column ?  pointList[1].x : right_most_column;

            //cout<<left_Intersecting_Column<<", "<<right_Intersecting_Column<<endl;

            int scanColumnStart = floor((left_Intersecting_Column - left_x)/dx);
            int scanColumnEnd = floor((right_Intersecting_Column - left_x)/dx);
            //cout<<"col "<<scanColumnStart<<" "<<scanColumnEnd<<endl;
            for(int column=scanColumnStart;column<scanColumnEnd;column++)
            {

                double m = (left_x + column*dx - pointList[0].x);

                if (pointList[1].x - pointList[0].x){
                    m /= pointList[1].x - pointList[0].x;
                }

                double z = pointList[0].z + m * (pointList[1].z - pointList[0].z);

                if (z > configInfo.nearDistance && z <= zBuffer[row][column])
                {
                    zBuffer[row][column] = z;
                    image.set_pixel(column,row,triangleList[i].color[0],triangleList[i].color[1],triangleList[i].color[2]);
                }


            }

        }
        cout<<"-------------------------------------------------------------------------------------------------------------------------------"<<endl;
        cout<<"GOT it"<<endl;



    }
    image.save_image("test1.bmp");
    dump("z_buffer.txt",zBuffer,configInfo);


}

