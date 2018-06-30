#include "bitmap_image.hpp"
#include <vector>
#include<sstream>
#include<stdlib.h>
#include<float.h>
#include <algorithm>

using namespace std;

int global_start;

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
    bool in;
    double a;
    double b;
    double c;
    double d;


    Triangle(Point p1,Point p2, Point p3)
    {
        pointlist[0]=p1;
        pointlist[1]=p2;
        pointlist[2]=p3;
        color[0]=rand()% 256 ;
        color[1]=rand()% 256;
        color[2]=rand()% 256 ;
        in =false;
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
        cout<<"Bottom Limit: "<<bottomLimit<<" Top Limit: "<<topLimit<<endl;
        cout<<"Near Distance: "<<nearDistance<<" Far Distance: "<<farDistance<<endl;
    }
};
class Edge{
public:
    double x_at_yMIN;
    double del_x;
    double polygonID;
    double yMIN;
    double yMAX;

    Edge(double x_at_ymin,double increment_x,double polyID,double ymin,double ymax)
    {
        x_at_yMIN=x_at_ymin;
        del_x=increment_x;
        polygonID=polyID;
        yMIN=ymin;
        yMAX=ymax;
    }
    Edge copyEdge()
    {
        return Edge(x_at_yMIN,del_x,polygonID,yMIN,yMAX);
    }
};
class ActiveEdge
{
public:
    int index;
    int val;
    double fullVal;
    ActiveEdge(int index1, int val1,double fullval1)
    {
        index=index1;
        val=val1;
        fullVal=fullval1;
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
vector<Triangle> triangleList;
vector<Edge> edgeList;
vector<int>*edgeTable ;
vector<ActiveEdge> activeEdgeTable;
vector<int>activePolygonTable ;
double toPixel(double val,double leftx,double topy, double dx, double dy, bool convert)
{

    if (convert)
    {
        return round((topy-val)/dy);
    }
    else
    {
        return floor((val-leftx)/dx);
    }
}
bool sortonXMIN(int i, int j) { return edgeList[i].x_at_yMIN < edgeList[j].x_at_yMIN; }
bool sortonXA(ActiveEdge i, ActiveEdge j) { return i.val < j.val; }
void processEdgeTable(double dx, double dy, double left_x, double top_y,vector<Triangle> triangleList,int polygonID,int edge1,int edge2,Config configinfo)
{
    double edge1_y=triangleList[polygonID].pointlist[edge1].y;
    double edge1_x=triangleList[polygonID].pointlist[edge1].x;
    double edge2_y=triangleList[polygonID].pointlist[edge2].y;
    double edge2_x=triangleList[polygonID].pointlist[edge2].x;
    //cout<<endl<<"x1 "<<edge1_x<<" y1 "<<edge1_y<<" x2 "<<edge2_x<<" y2 "<<edge2_y<<endl;
    int x1=toPixel(edge1_x,left_x,top_y,dx,dy,false);
    int x2=toPixel(edge2_x,left_x,top_y,dx,dy,false);
    int y1=toPixel(edge1_y,left_x,top_y,dx,dy,true);
    int y2=toPixel(edge2_y,left_x,top_y,dx,dy,true);
    //cout<<"x1 "<<x1<<" y1 "<<y1<<" x2 "<<x2<<" y2 "<<y2<<endl;
    double x_at_yMIN,yMIN,yMAX;
    double bottom_y=configinfo.bottomLimit+(dy/2);
    if (y1!=y2)
    {
        //cout<<" in loop"<<endl;
        if(edge1_y>edge2_y)
        {
            yMIN=(edge1_y>top_y)?top_y:edge1_y;
            x_at_yMIN=(edge1_y>top_y)?edge1_x + ((yMIN -edge1_y)/(edge2_y - edge1_y))*(edge2_x- edge1_x):edge1_x;
            yMAX=(edge2_y<bottom_y)? bottom_y:edge2_y;
        }
        else
        {
            yMIN=(edge2_y>top_y)?top_y:edge2_y;
            x_at_yMIN=(edge2_y>top_y)?edge1_x + ((yMIN -edge1_y)/(edge2_y - edge1_y))*(edge2_x- edge1_x):edge2_x;
            yMAX=(edge1_y<bottom_y)? bottom_y:edge1_y;

        }

        x_at_yMIN=toPixel(x_at_yMIN,left_x,top_y,dx,dy,false);
        yMIN=toPixel(yMIN,left_x,top_y,dx,dy,true);
        yMAX=toPixel(yMAX,left_x,top_y,dx,dy,true);

        double temp=(double)(x2-x1)/(y2-y1);
        Edge e=Edge(x_at_yMIN,temp,polygonID,yMIN,yMAX);
        edgeList.push_back(e);

    }
}
void createEdgeANDPolygonTable(double dx, double dy, double left_x, double top_y,Config configinfo)
{
    //cout<<"t size "<<triangleList.size()<<endl;
    for(int i=0;i<triangleList.size();i++)
    {

        triangleList[i].a = (triangleList[i].pointlist[1].y - triangleList[i].pointlist[0].y)*(triangleList[i].pointlist[2].z - triangleList[i].pointlist[0].z) - (triangleList[i].pointlist[2].y - triangleList[i].pointlist[0].y)*(triangleList[i].pointlist[1].z - triangleList[i].pointlist[0].z);
        triangleList[i].b = (triangleList[i].pointlist[1].z - triangleList[i].pointlist[0].z)*(triangleList[i].pointlist[2].x - triangleList[i].pointlist[0].x) - (triangleList[i].pointlist[1].x - triangleList[i].pointlist[0].x)*(triangleList[i].pointlist[2].z - triangleList[i].pointlist[0].z);
        triangleList[i].c = (triangleList[i].pointlist[1].x - triangleList[i].pointlist[0].x)*(triangleList[i].pointlist[2].y - triangleList[i].pointlist[0].y) - (triangleList[i].pointlist[1].y - triangleList[i].pointlist[0].y)*(triangleList[i].pointlist[2].x - triangleList[i].pointlist[0].x);
        triangleList[i].d = -(triangleList[i].a)*(triangleList[i].pointlist[0].x) -(triangleList[i].b)*(triangleList[i].pointlist[0].y) -(triangleList[i].c)*(triangleList[i].pointlist[0].z);


        processEdgeTable(dx,dy,left_x,top_y,triangleList,i,0,1,configinfo);
        processEdgeTable(dx,dy,left_x,top_y,triangleList,i,1,2,configinfo);
        processEdgeTable(dx,dy,left_x,top_y,triangleList,i,0,2,configinfo);

    }


    edgeTable = new vector<int>[configinfo.screenHeight];
    global_start=256000;
    for(int i=0;i<edgeList.size();i++)
    {
        int index = edgeList[i].yMIN;
        if(index<global_start)
        {
            global_start=index;
        }
        edgeTable[index].push_back(i);
    }
    for(int i=0;i<sizeof(edgeTable);i++)
    {
        for(int j=0;j<edgeTable[i].size();j++)
        {
            sort( edgeTable[i].begin( ), edgeTable[i].end( ),sortonXMIN);
        }
    }
    return;
}
int findZ(int xPoint, int scan,double dx,double dy, double left_x, double top_y,Config configinfo)
{
    double x_co=xPoint*dx+left_x;
    double y_co=top_y-scan*dy;
    int current_polygon_index=activePolygonTable[0];
    double current_polygon_z=-(triangleList[current_polygon_index].d+triangleList[current_polygon_index].a*x_co+triangleList[current_polygon_index].b*y_co)/(triangleList[current_polygon_index].c);
    double temp;
    double running_polygon_index;
    for(int i=0;i<activePolygonTable.size();i++)
    {
        int running_polygon_index=activePolygonTable[i];
        temp=-(triangleList[running_polygon_index].d+triangleList[running_polygon_index].a*x_co+triangleList[running_polygon_index].b*y_co)/(triangleList[running_polygon_index].c);
        if (temp<current_polygon_z)
        {
            current_polygon_z=temp;
            current_polygon_index=running_polygon_index;
        }

    }
    if(current_polygon_z>=configinfo.farDistance)
    {
        return -1;
    }
    return current_polygon_index;

}
void prepareforNextIter(int i)
{
    int before_delete_size=activeEdgeTable.size();
    for(int j=0;j<before_delete_size;j++)
    {
        for(int k=0;k<activeEdgeTable.size();k++)
        {
            int index = activeEdgeTable[k].index;
            if(edgeList[index].yMAX <= i+1)
            {
                activeEdgeTable.erase(activeEdgeTable.begin()+k);
                break;
            }
        }
        if(activeEdgeTable.size()==0)
            break;
    }
}
void updateActiveEdge(Config configInfo)
{
    for(int j=0;j<activeEdgeTable.size();j++)
    {
        int index= activeEdgeTable[j].index;
        activeEdgeTable[j].fullVal = activeEdgeTable[j].fullVal + edgeList[index].del_x;
        activeEdgeTable[j].fullVal=(activeEdgeTable[j].fullVal > configInfo.screenWidth - 1)?configInfo.screenWidth - 1:activeEdgeTable[j].fullVal;
        activeEdgeTable[j].fullVal=(activeEdgeTable[j].fullVal < 0)?0:activeEdgeTable[j].fullVal;
        activeEdgeTable[j].val = round(activeEdgeTable[j].fullVal);
    }
}
int main()
{
    Config configInfo=getConfigInfo("config.txt");
    configInfo.print();

    triangleList=getStageInformation("stage3.txt");

    bitmap_image image(configInfo.screenWidth,configInfo.screenHeight);

    double dx=(configInfo.rightLimit-configInfo.leftLimit)/((double)configInfo.screenWidth);
    double dy=(configInfo.topLimit-configInfo.bottomLimit)/((double)configInfo.screenHeight);
    double top_y=configInfo.topLimit-(dy/2);
    double left_x=configInfo.leftLimit+(dx/2);
    createEdgeANDPolygonTable(dx,dy,left_x,top_y,configInfo);
    //WHY
    for(int i=global_start;i<configInfo.screenHeight-1;i++)
    {
        for(int j=0;j<edgeTable[i].size();j++)
        {
            int index=edgeTable[i][j];
            ActiveEdge e=ActiveEdge(index,edgeList[index].x_at_yMIN,edgeList[index].x_at_yMIN);
            //cout<<"x "<<edgeList[index].x_at_yMIN<<" i "<<i<<endl;
            activeEdgeTable.push_back(e);
        }
        sort( activeEdgeTable.begin( ), activeEdgeTable.end( ),sortonXA);

        for(int j=0;j<activeEdgeTable.size();j++)
        {
            int edgeIndex=activeEdgeTable[j].index;
            int polygonIndex=edgeList[edgeIndex].polygonID;
            triangleList[polygonIndex].in=!triangleList[polygonIndex].in;

            if(triangleList[polygonIndex].in)
            {
                activePolygonTable.push_back(polygonIndex);
                int min_Z_polygon_index=findZ(activeEdgeTable[j].val,i,dx,dy,left_x,top_y,configInfo);
                if(min_Z_polygon_index!=-1)
                {
                    for(int k=activeEdgeTable[j].val;k<activeEdgeTable[j+1].val;k++)
                    {
                        image.set_pixel(k,i,triangleList[min_Z_polygon_index].color[0],triangleList[min_Z_polygon_index].color[1],triangleList[min_Z_polygon_index].color[2]);
                    }
                }
            }
            else
            {

                activePolygonTable.erase(std::remove(activePolygonTable.begin(), activePolygonTable.end(), polygonIndex), activePolygonTable.end());

                if(activePolygonTable.size()!=0)
                {

                    int min_Z_polygon_index=findZ(activeEdgeTable[j].val,i,dx,dy,left_x,top_y,configInfo);
                    if(min_Z_polygon_index!=-1)
                    {
                        int lastEdge = activeEdgeTable.size()-1;
                        int last_x;
                        if(activeEdgeTable.size()-1==j)
                            last_x = configInfo.screenWidth-1;
                        else
                            last_x = activeEdgeTable[j+1].val;
                        for(int k=activeEdgeTable[j].val ; k<=last_x ; k++)
                        {
                            //cout<<"from 2 k "<<k<<" i "<<i<<" j "<<j<<endl;
                            image.set_pixel(k,i,triangleList[min_Z_polygon_index].color[0],triangleList[min_Z_polygon_index].color[1],triangleList[min_Z_polygon_index].color[2]);
                        }

                    }
                }
            }

        }

        prepareforNextIter(i);
        updateActiveEdge(configInfo);
    }
    image.save_image("test1.bmp");
    return 0;
}
