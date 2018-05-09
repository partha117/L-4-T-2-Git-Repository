#include <iostream>
#include <fstream>
#include <string>
#include<stdlib.h>
#include "matrix.h"
#include<stack>
#include<vector>
#include<sstream>
using namespace std;

stack<double**> matStack;
stack<int> stateStack;

void fileWrite(string name,double **a,double **b,double **c)
{
  FILE *fp;
  fp = fopen(name.c_str(), "a");

  for(int i=0;i<matRow-1;i++)
  {

    for(int j=0;j<1;j++)
    {
        if(a[i][j]==0.0)
        {
            a[i][j]=0.0;
        }

        fprintf(fp,"%0.7lf",a[i][j]);

    }
    fprintf(fp," ");
  }
  fprintf(fp,"\n");
  for(int i=0;i<matRow-1;i++)
  {

    for(int j=0;j<1;j++)
    {
        if(a[i][j]==0.0)
        {
            a[i][j]=0.0;
        }

        fprintf(fp,"%0.7lf",b[i][j]);
    }
    fprintf(fp," ");
  }
  fprintf(fp,"\n");
  for(int i=0;i<matRow-1;i++)
  {

    for(int j=0;j<1;j++)
    {
        if(a[i][j]==0.0)
        {
            a[i][j]=0.0;
        }

        fprintf(fp,"%0.7lf",c[i][j]);
    }
    fprintf(fp," ");
  }
  fprintf(fp,"\n\n");
  fclose(fp);
}
std::vector<double> tokenize(string line)
{
    std::vector <double> tokens;

    // stringstream class check1
    stringstream check1(line);

    string intermediate;

    // Tokenizing w.r.t. space ' '
    while(getline(check1, intermediate, ' '))
    {
        tokens.push_back(atof(intermediate.c_str()));
    }

    // Printing the token vector
    return tokens;
}
void createStage1(string filename)
{

    string line;
    ifstream myfile (filename.c_str());
    vector<double> tokens;
    if (myfile.is_open())
    {
        matStack.push(matidentity(4,4));
        getline(myfile,line);
        tokens=tokenize(line);

        double** tmat=geteyemat(tokens[0],tokens[1],tokens[2]);


        getline(myfile,line);
        tokens=tokenize(line);
        point look=(point){.x=tokens[0],.y=tokens[1],.z=tokens[2]};

        getline(myfile,line);
        tokens=tokenize(line);
        point up=(point){.x=tokens[0],.y=tokens[1],.z=tokens[2]};

        double** rmat=getallignmat(look,up,tmat);


        double **view=matmult(rmat,tmat,4,4,4,4);


        freemat(rmat,4,4);
        freemat(tmat,4,4);


        getline(myfile,line);
        tokens=tokenize(line);

        double **pmat=getPmat(tokens[0],tokens[1],tokens[2],tokens[3]);

        while ( getline (myfile,line) )
        {


            if(line.compare("push")==0)
            {
                stateStack.push(matStack.size());

            }
            else if(line.compare("pop")==0)
            {
                int pastSize=stateStack.top();
                stateStack.pop();
                for(;pastSize!=matStack.size();)
                {
                    double **temp=matStack.top();
                    matStack.pop();
                    freemat(temp,4,4);
                }

            }
            else if(line.compare("translate")==0)
            {

                  getline(myfile,line);
                  tokens=tokenize(line);
                  double **temp=getTranslatemat(tokens[0],tokens[1],tokens[2]);
                  double **temp1=matmult(matStack.top(),temp,4,4,4,4);
                  freemat(temp,4,4);
                  matStack.push(temp1);
                  //printf("---------------------------translate-----------------\n");
                  //printmat(temp1);

            }
            else if(line.compare("scale")==0)
            {

                  getline(myfile,line);
                  tokens=tokenize(line);
                  double **temp=getScalemat(tokens[0],tokens[1],tokens[2]);
                  //printmat(matStack.top());
                  //printmat(temp);
                  double **temp1=matmult(matStack.top(),temp,4,4,4,4);
                  freemat(temp,4,4);
                  matStack.push(temp1);
                  //printf("---------------------------Scale-----------------\n");
                  //printmat(temp1);

            }
            else if(line.compare("rotate")==0)
            {

                  getline(myfile,line);
                  tokens=tokenize(line);
                  double **temp=getRotationalmat(tokens[0],(point){.x=tokens[1],.y=tokens[2],.z=tokens[3]});
                  double **temp1=matmult(matStack.top(),temp,4,4,4,4);
                  freemat(temp,4,4);
                  matStack.push(temp1);
                  //printf("---------------------------Rotate-----------------\n");
                  //printmat(temp1);
            }
            else if(line.compare("triangle")==0)
            {

                  getline(myfile,line);
                  tokens=tokenize(line);
                  point a=(point){.x=tokens[0],.y=tokens[1],.z=tokens[2]};
                  double **tempa1=matmultvector(matStack.top(),a,4,4);
                  double **tempa2=matmult(view,tempa1,4,4,4,1);
                  double **tempa3=matmult(pmat,tempa2,4,4,4,1);
                  //printmat(tempa3,4,1);
                  double **tempa4=matscalermult(tempa3,1/tempa3[3][0],4,1);

                  getline(myfile,line);
                  tokens=tokenize(line);
                  point b=(point){.x=tokens[0],.y=tokens[1],.z=tokens[2]};
                  double **tempb1=matmultvector(matStack.top(),b,4,4);
                  double **tempb2=matmult(view,tempb1,4,4,4,1);
                  double **tempb3=matmult(pmat,tempb2,4,4,4,1);
                  //printmat(tempb3,4,1);
                  double **tempb4=matscalermult(tempb3,1/tempb3[3][0],4,1);

                  getline(myfile,line);
                  tokens=tokenize(line);
                  point c=(point){.x=tokens[0],.y=tokens[1],.z=tokens[2]};
                  double **tempc1=matmultvector(matStack.top(),c,4,4);
                  double **tempc2=matmult(view,tempc1,4,4,4,1);
                  double **tempc3=matmult(pmat,tempc2,4,4,4,1);
                  //printmat(tempc3,4,1);
                  double **tempc4=matscalermult(tempc3,1/tempc3[3][0],4,1);
                  //printmat(temp1,3,1);
                  //printf("--------------------------\n");
                  fileWrite("stage1.txt",tempa1,tempb1,tempc1);
                  fileWrite("stage2.txt",tempa2,tempb2,tempc2);
                  fileWrite("stage3.txt",tempa4,tempb4,tempc4);

                  freemat(tempa1,4,1);
                  freemat(tempb1,4,1);
                  freemat(tempc1,4,1);
                  freemat(tempa2,4,1);
                  freemat(tempb2,4,1);
                  freemat(tempc2,4,1);
                  freemat(tempa3,4,1);
                  freemat(tempb3,4,1);
                  freemat(tempc3,4,1);
                  freemat(tempa4,4,1);
                  freemat(tempb4,4,1);
                  freemat(tempc4,4,1);
            }


        }
        myfile.close();
    }
}


int main()
{
    createStage1("scene.txt");
}

