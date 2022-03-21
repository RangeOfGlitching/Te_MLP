// Te_MLP.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <string.h>
/**********************************************************/
#include <fstream>
#include <vector>
#include <algorithm>
/**********************************************************/
using namespace std;

#define   Input_file   "in.dat"
#define   Desired_output_file  "out.dat"
/**********************************************************/
#define   Desired_output_file_normolization "out_normolization.dat"
/**********************************************************/
#define   Hid_Out_weight "w.dat"
#define   In_Hid_weight "v.dat"
#define   Test_output_file   "test_y.dat"


#define a(f)   1 / (1 + exp(-f))
#define _in_varl   4
#define _out_varl  1
#define _node   10
#define  _dat_num    500


void READ_W(FILE *ft);
void READ_V(FILE *ft);
void READ_input_data(FILE *fx);
void READ_output_Normolization_data(FILE *fy);
void TEST_SAVE(void);
/**********************************************************/
void read_output_data();
void Renormalization_acty();
float ReverseNormalization(float NormolizationNumber);
/**********************************************************/

float _x[_dat_num+1][_in_varl+1],_d[_dat_num+1][_out_varl+1]  ;
float _y[_out_varl+1] ;
float _v[_node+1][_in_varl+1], _w[_out_varl+1][_node+1] ; 
/**********************************************************/
float _outMax;
float _outMin;
float _difference;
/**********************************************************/

int _tmain(int argc, _TCHAR* argv[])
{
    /**********************************************************/
     read_output_data();
    /**********************************************************/
	 FILE *stream ;
 
      if ( fopen_s(&stream, In_Hid_weight,"r") !=0) 
		  { printf("open error 1 ");
	       exit(1); }
      READ_V(stream);
      fclose(stream) ;

     if ( fopen_s(&stream,Hid_Out_weight,"r") !=0) 
		  { printf("open error 2 ");
	       exit(1); }
      READ_W(stream);
      fclose(stream) ;

      if ( fopen_s(&stream,Input_file,"r") !=0) 
	       { printf("open error 3 ");
	       exit(1); }
	  READ_input_data(stream);
      fclose(stream) ;

	 if ( fopen_s(&stream, Desired_output_file_normolization,"r") !=0)
	       { printf("open error 4 ");
	       exit(1); }
     READ_output_Normolization_data(stream);
      fclose(stream) ; 
	  	  
	  TEST_SAVE();
      Renormalization_acty();

	  _getch();    
      return 0;
}

/**********************************************************/
void read_output_data(){
    std::ifstream fileRead_OutPut_Data(Desired_output_file);
    std::ofstream fileWrite_OutPut_Normolization_Data(Desired_output_file_normolization);
    float number, normolizationNumber;
    std::vector<float> numbers;
    if (!fileRead_OutPut_Data.is_open()) {
        std::cout << "file open failed" << endl;
        system("pause < 0");
    }
    while (fileRead_OutPut_Data >> number) {
        numbers.push_back(number);
    }
    fileRead_OutPut_Data.close();

    auto pr = std::minmax_element(numbers.begin(), numbers.end());
    _outMin = *pr.first;
    _outMax = *pr.second;
    _difference = _outMax - _outMin;
}
/**********************************************************/

void READ_W(FILE *ft)
{  int  j,q ;
  
for(j=0;j< _out_varl;j++)
{  for(q=0;q<= _node;q++)
    fscanf_s(ft,"%f ",&_w[j][q]) ;
}   
}

void READ_V(FILE *ft)
{ int  i, q ;

for(q=0;q<_node;q++)
{ 	for(i=0;i<= _in_varl;i++)
	  fscanf_s(ft,"%f ",&_v[q][i]);	 
}
}

void READ_input_data(FILE *fx)
{ int i,j;

 for(i=0; i< _dat_num; i++)
  {   for(j=0;j< _in_varl ;j++)
       fscanf_s(fx,"%f",&_x[i][j]);  	
	_x[i][_in_varl]= -1 ; // for bias
 }
}


void READ_output_Normolization_data(FILE *fy)
{
   int i,j;

   for (i=0;i<_dat_num;i++)
    for(j=0;j< _out_varl;j++)
        fscanf_s(fy,"%f ",&_d[i][j]);  
 
}


void TEST_SAVE(void)

{  int i,j,q,num ;
   FILE *stream ;
   float h[_node+1],Y,H,error[_out_varl+1],difference[_out_varl+1]; 
   FILE  *fout  ;

   printf("\n   testing : ********************************** \n");
       
  if( fopen_s(&fout, Test_output_file,"w") != 0 ) exit(1) ;

   for(j=0;j< _out_varl ;j++)
	   error [j]=0 ;

  for(num=0;num< _dat_num ;num++)
   {
    for(q=0;q< _node ;q++)
    { H = 0; 
      for(i=0;i<= _in_varl ;i++)
	   H = H + _v[q][i]*_x[num][i] ;
      h[q] = a(H) ;
    }
	h[_node]=-1 ;  // for bias  

   for(j=0;j< _out_varl ;j++)
    {  Y = 0 ;
       for(q=0;q<= _node ;q++)
	   Y = Y  + _w[j][q]*h[q] ;
       _y[j] = a(Y) ;

       fprintf(fout,"%f  ",_y[j] );

	    difference [j] = _d[num][j] - _y[j]  ;
       error [j] = error [j] + difference [j]*difference [j]  ;
    }

      fprintf(fout," \n ");

   }/* end for num */

   printf("\n\n ++++ Final result: ");
	   for(j=0;j< _out_varl ;j++)
	    printf(" output %d is --- %f ", j+1, sqrt(error[j]/_dat_num));

   fclose(fout) ;

}
void Renormalization_acty() {
    float number;
    std::ifstream file_Read_Actual_output_file(Test_output_file);
    std::vector<float> renormolization;
    if (!file_Read_Actual_output_file.is_open()) {
        std::cout << "fileOpenfiled" << endl;
        system("pause<0");
    }
    while (file_Read_Actual_output_file >> number) {
        renormolization.push_back(number);
    }
    file_Read_Actual_output_file.close();

    std::ofstream file_wite_Actual_output_file(Test_output_file);
    for (float NormolizationNumber : renormolization) {
        number = ReverseNormalization(NormolizationNumber);
        file_wite_Actual_output_file << "    " << number << std::endl;
    }
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
float ReverseNormalization(float NormolizationNumber) {
    NormolizationNumber = NormolizationNumber * _difference + _outMin;
    return NormolizationNumber;
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


