/*********************************************************/
/*                   cp.c                                */
/* 나선형과 풀링 처리                                    */
/* 2차원 데이터를 읽어들여 나선형과 풀링을 실시          */
/* 사용 방법                                             */
/*  \Users\deeplearning\ch5>cp < data1.txt               */
/*********************************************************/

/* 헤더 파일 포함*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Hyperparameters*/
#define INPUTSIZE 11  /*입력 수*/ 
#define FILTERSIZE 3  /*필터 크기*/
#define POOLSIZE 3    /*풀링 크기*/
#define POOLOUTSIZE 3 /*풀링 출력 크기*/
/*함수 프로토타입 선언*/
void conv(double filter[][FILTERSIZE],double e[][INPUTSIZE],double convout[][INPUTSIZE]) ; /*나선형 계산*/
double calcconv(double filter[][FILTERSIZE]
               ,double e[][INPUTSIZE],int i,int j) ;
                               /*  필터 적용  */
void convres(double convout[][INPUTSIZE]) ;
              /*나선형 결과 출력*/
void pool(double convout[][INPUTSIZE]
         ,double poolout[][POOLOUTSIZE]) ; 
             /*풀링 계산*/           
double maxpooling(double convout[][INPUTSIZE]
                 ,int i,int j) ;	/* 최댓값 풀링 */
void poolres(double poolout[][POOLOUTSIZE]) ;	/*결과 출력*/
void getdata(double e[][INPUTSIZE]) ;	/*데이터 읽어들이기*/ 
         
/*******************/ 
/*   main() 함수   */ 
/*******************/
int main()
{
 double filter[FILTERSIZE][FILTERSIZE]
      ={{0,0,0},{1,1,1},{0,0,0}} ;	/*가로 필터*/
 //       ={{0,1,0},{0,1,0},{0,1,0}} ;	/*세로 필터*/
 double e[INPUTSIZE][INPUTSIZE] ;	/*입력 데이터*/
 double convout[INPUTSIZE][INPUTSIZE]={0} ;	/*나선형 출력*/
 double poolout[POOLOUTSIZE][POOLOUTSIZE] ;	/*출력 데이터*/
 
 /*입력 데이터 읽어들이기*/
 getdata(e) ;
  convres(e) ;
 /*나선형 계산*/
 conv(filter,e,convout) ;
 convres(convout) ;
 
 /*풀링 계산*/
 pool(convout,poolout) ;
 /*결과 출력*/
 poolres(poolout) ;

 return 0 ;
}

/**********************/
/*  poolres() 함수    */
/*  결과 출력         */
/**********************/
void poolres(double poolout[][POOLOUTSIZE]) 
{
 int i,j ;	/*반복 제어*/
 
 for(i=0;i<POOLOUTSIZE;++i){
  for(j=0;j<POOLOUTSIZE;++j){
   printf("%.3lf ",poolout[i][j]) ;
  }
  printf("\n") ;
 }
 printf("\n") ;	
} 
 
/**********************/
/*  pool() 함수       */
/*  풀링 계산         */
/**********************/
void pool(double convout[][INPUTSIZE]
         ,double poolout[][POOLOUTSIZE]) 
{
 int i,j ;	/*반복 제어*/

 for(i=0;i<POOLOUTSIZE;++i)
  for(j=0;j<POOLOUTSIZE;++j)
   poolout[i][j]=maxpooling(convout,i,j) ;
}
 
/**********************/
/* maxpooling() 함수  */
/* 최댓값 풀링        */
/**********************/
double maxpooling(double convout[][INPUTSIZE]
                 ,int i,int j)
{
 int m,n ;	/*반복 제어용*/
 double max ;	/*최댓값*/
 int halfpool=POOLSIZE/2 ;	/*풀링 크기의 1/2*/
 
 max
 =convout[i*POOLOUTSIZE+1+halfpool][j*POOLOUTSIZE+1+halfpool];
 for(m=POOLOUTSIZE*i+1;m<=POOLOUTSIZE*i+1+(POOLSIZE-halfpool);++m)
  for(n=POOLOUTSIZE*j+1;n<=POOLOUTSIZE*j+1+(POOLSIZE-halfpool);++n)
   if(max<convout[m][n]) max=convout[m][n] ;

 return max ;
}

/**********************/
/*  convres() 함수    */
/* 나선형 결과 출력   */
/**********************/
void convres(double convout[][INPUTSIZE]) 
{
 int i,j ;	/*반복 제어*/
 
 for(i=1;i<INPUTSIZE-1;++i){
  for(j=1;j<INPUTSIZE-1;++j){
   printf("%.3lf ",convout[i][j]) ;
  }
  printf("\n") ;
 }
 printf("\n") ;	
} 

/*************************/
/*  getdata() 함수       */
/*입력 데이터 읽어들이기 */
/*************************/
void getdata(double e[][INPUTSIZE])
{
  FILE *fp=fopen("data/data1.txt","r");
  if(fp==NULL)
  {
    printf("file open error!\n");
    exit(1);    
  }
  int i=0,j=0 ;	/*반복 제어용*/

  /*데이터 입력*/
  while(fscanf(fp,"%lf",&e[i][j])!=EOF){
    ++ j;
  if(j>=INPUTSIZE){/*다음 데이터*/
   j=0 ;
   ++i ;
   if(i>=INPUTSIZE) break ;	/*입력 종료*/
  }
 }
 fclose(fp);
}

/**********************/
/*  conv() 함수       */
/*  나선형 계산       */
/**********************/
void conv(double filter[][FILTERSIZE]
         ,double e[][INPUTSIZE],double convout[][INPUTSIZE])
{
 int i=0,j=0 ;	/*반복 제어용*/
 int startpoint=FILTERSIZE/2 ;	/*나선형 범위의 하한*/

 for(i=startpoint;i<INPUTSIZE-startpoint;++i)
  for(j=startpoint;j<INPUTSIZE-startpoint;++j)
   convout[i][j]=calcconv(filter,e,i,j) ;
}

/**********************/
/*  calcconv() 함수   */
/*  필터 적용         */
/**********************/
double calcconv(double filter[][FILTERSIZE]
             ,double e[][INPUTSIZE],int i,int j)
{
 int m,n ;	/*반복 제어용*/
 double sum=0 ;	/*합의 값*/
 
 for(m=0;m<FILTERSIZE;++m)
  for(n=0;n<FILTERSIZE;++n)
   sum+=e[i-FILTERSIZE/2+m][j-FILTERSIZE/2+n]*filter[m][n];
   
 return sum ;
}