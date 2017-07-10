/*********************************************************/
/*                   cp.c                                */
/* �������� Ǯ�� ó��                                    */
/* 2���� �����͸� �о�鿩 �������� Ǯ���� �ǽ�          */
/* ��� ���                                             */
/*  \Users\deeplearning\ch5>cp < data1.txt               */
/*********************************************************/

/* ��� ���� ����*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Hyperparameters*/
#define INPUTSIZE 11  /*�Է� ��*/ 
#define FILTERSIZE 3  /*���� ũ��*/
#define POOLSIZE 3    /*Ǯ�� ũ��*/
#define POOLOUTSIZE 3 /*Ǯ�� ��� ũ��*/
/*�Լ� ������Ÿ�� ����*/
void conv(double filter[][FILTERSIZE],double e[][INPUTSIZE],double convout[][INPUTSIZE]) ; /*������ ���*/
double calcconv(double filter[][FILTERSIZE]
               ,double e[][INPUTSIZE],int i,int j) ;
                               /*  ���� ����  */
void convres(double convout[][INPUTSIZE]) ;
              /*������ ��� ���*/
void pool(double convout[][INPUTSIZE]
         ,double poolout[][POOLOUTSIZE]) ; 
             /*Ǯ�� ���*/           
double maxpooling(double convout[][INPUTSIZE]
                 ,int i,int j) ;	/* �ִ� Ǯ�� */
void poolres(double poolout[][POOLOUTSIZE]) ;	/*��� ���*/
void getdata(double e[][INPUTSIZE]) ;	/*������ �о���̱�*/ 
         
/*******************/ 
/*   main() �Լ�   */ 
/*******************/
int main()
{
 double filter[FILTERSIZE][FILTERSIZE]
      ={{0,0,0},{1,1,1},{0,0,0}} ;	/*���� ����*/
 //       ={{0,1,0},{0,1,0},{0,1,0}} ;	/*���� ����*/
 double e[INPUTSIZE][INPUTSIZE] ;	/*�Է� ������*/
 double convout[INPUTSIZE][INPUTSIZE]={0} ;	/*������ ���*/
 double poolout[POOLOUTSIZE][POOLOUTSIZE] ;	/*��� ������*/
 
 /*�Է� ������ �о���̱�*/
 getdata(e) ;
  convres(e) ;
 /*������ ���*/
 conv(filter,e,convout) ;
 convres(convout) ;
 
 /*Ǯ�� ���*/
 pool(convout,poolout) ;
 /*��� ���*/
 poolres(poolout) ;

 return 0 ;
}

/**********************/
/*  poolres() �Լ�    */
/*  ��� ���         */
/**********************/
void poolres(double poolout[][POOLOUTSIZE]) 
{
 int i,j ;	/*�ݺ� ����*/
 
 for(i=0;i<POOLOUTSIZE;++i){
  for(j=0;j<POOLOUTSIZE;++j){
   printf("%.3lf ",poolout[i][j]) ;
  }
  printf("\n") ;
 }
 printf("\n") ;	
} 
 
/**********************/
/*  pool() �Լ�       */
/*  Ǯ�� ���         */
/**********************/
void pool(double convout[][INPUTSIZE]
         ,double poolout[][POOLOUTSIZE]) 
{
 int i,j ;	/*�ݺ� ����*/

 for(i=0;i<POOLOUTSIZE;++i)
  for(j=0;j<POOLOUTSIZE;++j)
   poolout[i][j]=maxpooling(convout,i,j) ;
}
 
/**********************/
/* maxpooling() �Լ�  */
/* �ִ� Ǯ��        */
/**********************/
double maxpooling(double convout[][INPUTSIZE]
                 ,int i,int j)
{
 int m,n ;	/*�ݺ� �����*/
 double max ;	/*�ִ�*/
 int halfpool=POOLSIZE/2 ;	/*Ǯ�� ũ���� 1/2*/
 
 max
 =convout[i*POOLOUTSIZE+1+halfpool][j*POOLOUTSIZE+1+halfpool];
 for(m=POOLOUTSIZE*i+1;m<=POOLOUTSIZE*i+1+(POOLSIZE-halfpool);++m)
  for(n=POOLOUTSIZE*j+1;n<=POOLOUTSIZE*j+1+(POOLSIZE-halfpool);++n)
   if(max<convout[m][n]) max=convout[m][n] ;

 return max ;
}

/**********************/
/*  convres() �Լ�    */
/* ������ ��� ���   */
/**********************/
void convres(double convout[][INPUTSIZE]) 
{
 int i,j ;	/*�ݺ� ����*/
 
 for(i=1;i<INPUTSIZE-1;++i){
  for(j=1;j<INPUTSIZE-1;++j){
   printf("%.3lf ",convout[i][j]) ;
  }
  printf("\n") ;
 }
 printf("\n") ;	
} 

/*************************/
/*  getdata() �Լ�       */
/*�Է� ������ �о���̱� */
/*************************/
void getdata(double e[][INPUTSIZE])
{
  FILE *fp=fopen("data/data1.txt","r");
  if(fp==NULL)
  {
    printf("file open error!\n");
    exit(1);    
  }
  int i=0,j=0 ;	/*�ݺ� �����*/

  /*������ �Է�*/
  while(fscanf(fp,"%lf",&e[i][j])!=EOF){
    ++ j;
  if(j>=INPUTSIZE){/*���� ������*/
   j=0 ;
   ++i ;
   if(i>=INPUTSIZE) break ;	/*�Է� ����*/
  }
 }
 fclose(fp);
}

/**********************/
/*  conv() �Լ�       */
/*  ������ ���       */
/**********************/
void conv(double filter[][FILTERSIZE]
         ,double e[][INPUTSIZE],double convout[][INPUTSIZE])
{
 int i=0,j=0 ;	/*�ݺ� �����*/
 int startpoint=FILTERSIZE/2 ;	/*������ ������ ����*/

 for(i=startpoint;i<INPUTSIZE-startpoint;++i)
  for(j=startpoint;j<INPUTSIZE-startpoint;++j)
   convout[i][j]=calcconv(filter,e,i,j) ;
}

/**********************/
/*  calcconv() �Լ�   */
/*  ���� ����         */
/**********************/
double calcconv(double filter[][FILTERSIZE]
             ,double e[][INPUTSIZE],int i,int j)
{
 int m,n ;	/*�ݺ� �����*/
 double sum=0 ;	/*���� ��*/
 
 for(m=0;m<FILTERSIZE;++m)
  for(n=0;n<FILTERSIZE;++n)
   sum+=e[i-FILTERSIZE/2+m][j-FILTERSIZE/2+n]*filter[m][n];
   
 return sum ;
}