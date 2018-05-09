#include<stdio.h>
#include<math.h>
#define DEG2RAD 3.141592654/180.0
struct point
{
	double x,y,z;
};
typedef struct point point;

double matRow=4;
double matColumn=4;

void freemat(double **a,int m,int n)
{

    for( int i = 0 ; i < m ; i++ )
    {
        delete[] a[i]; // delete array within matrix
    }
// delete actual matrix
    delete[] a;
}
double** getMat(int m,int q)
{
    double **mat = (double**)malloc(m *sizeof(double*));
    for(int i = 0; i < matRow; i++) mat[i] = (double *)malloc(q* sizeof(double));
    for(int i=0;i<m;i++)
     {

         for(int j=0;j<q;j++)
         {

             mat[i][j]=0;
         }
     }
    return mat;
}
double** matmult(double **a, double **b,int m,int n,int p,int q)
{
    double sum;
    double **mat = getMat(m,q);
     for(int i=0;i<m;i++)
     {

         for(int j=0;j<q;j++)
         {
             sum=0;
             for (int k=0;k<p;k++)
             {
                 sum+=a[i][k]*b[k][j];
             }
             mat[i][j]=sum;
         }
     }

     return mat;
}
double** point2vector(point b)
{
    double **mat = getMat(4,1);
    mat[0][0]=b.x;
    mat[1][0]=b.y;
    mat[2][0]=b.z;
    mat[3][0]=1;
    return mat;
}
double** matmultvector(double **a, point p,int m,int n)
{
     double sum;

     double **b=point2vector(p);
     double **mat =matmult(a,b,m,n,4,1);
     freemat(b,4,1);
     return mat;
}
point matmultvector(point a, point b)
{
     point mat;
     mat.x=(a.y*b.z)-(a.z-b.y);
     mat.y=(a.z*b.x)-(a.x*b.z);
     mat.z=(a.x*b.y)-(a.y*b.x);
     return mat;
}
double** matcopy(double **a,int m,int n)
{

    double **mat = getMat(m,n);
     for(int i=0;i<m;i++)
     {

         for(int j=0;j<n;j++)
         {

             mat[i][j]=a[i][j];
         }
     }

     return mat;
}

double** matscalermult(double **a, double b,int m,int n)
{
    double sum;
    double **mat = matcopy(a,m,n);
     for(int i=0;i<m;i++)
     {

         for(int j=0;j<n;j++)
         {

             mat[i][j]=a[i][j]*b;
         }
     }

     return mat;
}
double** matreplacerow(double **a ,point point,int row,int m,int n)
{
    double **mat =matcopy(a,m,n);
    for(int i=0;i<m;i++)
     {
         if(i==row)
         {
             mat[i][0]=point.x;
             mat[i][1]=point.y;
             mat[i][2]=point.z;
         }

     }
     return mat;
}
double ** matreplacecolumn(double **a ,point point,int column,int m,int n)
{
    double **mat = matcopy(a,m,n);
    for(int j=0;j<n;j++)
     {
         if(j==column)
         {
             mat[0][j]=point.x;
             mat[1][j]=point.y;
             mat[2][j]=point.z;
         }

     }
     return mat;
}


double** matidentity(int m,int n)
{
    double sum;
    double **mat = getMat(m,n);
     for(int i=0;i<m;i++)
     {

         for(int j=0;j<n;j++)
         {
            if(i==j)
            {
                mat[i][j]=1;
            }

         }
     }

     return mat;
}
double** matadd(double **a, double **b,int m,int n)
{
    double sum;
    double **mat = getMat(m,n);
     for(int i=0;i<m;i++)
     {

         for(int j=0;j<n;j++)
         {

             mat[i][j]=a[i][j]+b[i][j];
         }
     }

     return mat;
}
double** matsub(double **a, double **b,int m,int n)
{
    double sum;
    double **mat = getMat(m,n);
     for(int i=0;i<m;i++)
     {

         for(int j=0;j<n;j++)
         {

             mat[i][j]=a[i][j]-b[i][j];
         }
     }

     return mat;
}
 void printmat(double **a,int m,int n)
 {
     for(int i=0;i<m;i++)
     {

         for(int j=0;j<n;j++)
         {
             if(a[i][j]==0)
             {
                a[i][j]=0;
             }
             printf("%lf ",a[i][j]);
         }
         printf("\n");
     }
 }

   void printvector(point b)
 {
     double **a=point2vector(b);
     for(int i=0;i<4;i++)
     {

         for(int j=0;j<1;j++)
         {
            if(a[i][j]==0)
            {
                a[i][j]=0;
            }
            printf("%lf ",a[i][j]);
         }
         printf("\n");
     }
 }
 double** getTranslatemat(double x,double y,double z)
{
    double **mat=matidentity(4,4);
    mat[0][3]=x;
    mat[1][3]=y;
    mat[2][3]=z;
    mat[3][3]=1;
    return mat;

}
 double** getScalemat(double x,double y,double z)
{
    double **mat=matidentity(4,4);
    mat[0][0]=x;
    mat[1][1]=y;
    mat[2][2]=z;
    mat[3][3]=1;
    return mat;

}

double** rodriguez(double angle, point a)
{
    double val=sqrt((a.x*a.x)+(a.y*a.y)+(a.z*a.z));
    a.x=a.x/val;
    a.y=a.y/val;
    a.z=a.z/val;
    point c1=(point){.x=0,.y=0,.z=0};
    point c2=(point){.x=0,.y=0,.z=0};
    point c3=(point){.x=0,.y=0,.z=0};

    angle=angle*DEG2RAD;
    double x1,x2,x3;
    x1=x2=x3=0;
    //*****************************C1 calculation************************/
    x1=cos(angle);          //Cos(theta)X
    double **first=matidentity(4,4);
    double **first1=matscalermult(first,x1,4,4);

    x1=1-x1;
    double **second=getMat(4,4);


    second[0][0]=a.x*a.x;
    second[0][1]=a.x*a.y;
    second[0][2]=a.x*a.z;

    second[1][0]=a.x*a.y;
    second[1][1]=a.y*a.y;
    second[1][2]=a.y*a.z;

    second[2][0]=a.x*a.z;
    second[2][1]=a.y*a.z;
    second[2][2]=a.z*a.z;

    double **second1=matscalermult(second,x1,4,4);


    double **third=getMat(4,4);

    third[0][1]=-a.z;
    third[0][2]=a.y;

    third[1][0]=a.z;
    third[1][2]=-a.x;

    third[2][0]=-a.y;
    third[2][1]=a.x;

    double **third1=matadd(first1,second1,4,4);
    double **third2=matadd(third1,third,4,4);
    freemat(first,4,4);
    freemat(second,4,4);
    freemat(third,4,4);
    freemat(third1,4,4);



    return third2;
}
double** getRotationalmat(double angle, point a)
{
    double **R=rodriguez(angle,a);
    double **c1=matmultvector(R,(point){.x=1,.y=0,.z=0},4,4);
    double **c2=matmultvector(R,(point){.x=0,.y=1,.z=0},4,4);
    double **c3=matmultvector(R,(point){.x=0,.y=0,.z=1},4,4);
    double **mat=getMat(4,4);
    mat[0][0]=c1[0][0];
    mat[0][1]=c2[0][0];
    mat[0][2]=c3[0][0];
    mat[0][3]=0;

    mat[1][0]=c1[1][0];
    mat[1][1]=c2[1][0];
    mat[1][2]=c3[1][0];
    mat[1][3]=0;

    mat[2][0]=c1[2][0];
    mat[2][1]=c2[2][0];
    mat[2][2]=c3[2][0];
    mat[2][3]=0;

    mat[3][0]=0;
    mat[3][1]=0;
    mat[3][2]=0;
    mat[3][3]=1;

    freemat(c1,4,1);
    freemat(c2,4,1);
    freemat(c3,4,1);
    return mat;
}

double** geteyemat(double eyex,double eyey,double eyez)
{
    double** mat=matidentity(4,4);
    mat[0][3]=-eyex;
    mat[1][3]=-eyey;
    mat[2][3]=-eyez;
    mat[3][3]=1;
    return mat;
}
 double** getallignmat(point look,point up, double **eye)
 {

     look.x+=eye[0][3];
     look.y+=eye[1][3];
     look.z+=eye[2][3];
     //printf("-----------------Look------------\n");
     //printvector(look);
     double val=sqrt((look.x*look.x)+(look.y*look.y)+(look.z*look.z));
     look.x/=val;
     look.y/=val;
     look.z/=val;
     //printf("-----------------Look------------\n");
     //printvector(look);
     point right=matmultvector(look,up);
     //printf("-----------------right------------\n");
     //printvector(right);
     val=sqrt((right.x*right.x)+(right.y*right.y)+(right.z*right.z));
     right.x/=val;
     right.y/=val;
     right.z/=val;
     //printf("-----------------right------------\n");
     //printvector(right);

     up=matmultvector(right,look);

     //printf("-----------------up------------\n");
     //printvector(up);
     double **temp=getMat(4,4);

     temp[0][0]=right.x;
     temp[0][1]=right.y;
     temp[0][2]=right.z;
     temp[0][3]=0;


     temp[1][0]=up.x;
     temp[1][1]=up.y;
     temp[1][2]=up.z;
     temp[1][3]=0;


     temp[2][0]=-look.x;
     temp[2][1]=-look.y;
     temp[2][2]=-look.z;
     temp[2][3]=0;


     temp[3][0]=0;
     temp[3][1]=0;
     temp[3][2]=0;
     temp[3][3]=1;

     return temp;

 }
 double** getPmat(double fovY,double aspectRatio,double near,double far)
 {
     double ** mat=getMat(4,4);
     double fovX=fovY*aspectRatio;
     double t=near*tan(fovY*0.5*DEG2RAD);
     double r=near*tan(fovX*0.5*DEG2RAD);

     mat[0][0]=near/r;
     mat[1][1]=near/t;
     mat[2][2]=(-(far+near))/(far-near);
     mat[2][3]=(-(2*far*near))/(far-near);
     mat[3][2]=-1;
     return mat;

 }


