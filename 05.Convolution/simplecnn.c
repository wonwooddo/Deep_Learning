/*********************************************************/
/*            simplecnn.c                                */
/* 나선형 신경망의 기본 구조 데모                        */
/* CNN의 기본 구조(골격만)를 나타냄                      */
/* 사용 방법                                             */
/* \Users\deeplearning\ch5>simplecnn < data1.txt         */
/*********************************************************/

/* 헤더 파일 포함*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*기호 상수 정의*/
#define INPUTSIZE 11	/*입력 수*/ 
#define FILTERSIZE 3	/*필터의 크기*/
#define FILTERNO 2		/*필터 개수*/
#define POOLSIZE 3		/*풀링 크기*/
#define POOLOUTSIZE 3	/*풀링 출력 크기*/
#define MAXINPUTNO 100	/*학습 데이터 최대 개수*/ 
#define SEED 65535		/*난수 시드*/ 
#define LIMIT 0.001		/*오차 상한값*/ 
#define BIGNUM 100		/*오차 초깃값*/ 
#define HIDDENNO 3		/*중간층 셀 개수*/ 
#define ALPHA  10		/*학습 계수*/ 

/*함수 프로토타입 선언*/
void conv(double filter[FILTERSIZE][FILTERSIZE]
	,double e[][INPUTSIZE]
	,double convout[][INPUTSIZE]) ;	/*나선형 계산*/
double calcconv(double filter[][FILTERSIZE]
	,double e[][INPUTSIZE],int i,int j) ;
			/*  필터 적용  */
void pool(double convout[][INPUTSIZE]
	,double poolout[][POOLOUTSIZE]) ; 
			/*풀링 계산*/	  
double maxpooling(double convout[][INPUTSIZE]
	,int i,int j) ;	/* 최댓값 풀링 */
int getdata(double e[][INPUTSIZE][INPUTSIZE],int r[]) ;
			/*데이터 읽어들이기*/ 
void showdata(double e[][INPUTSIZE][INPUTSIZE],int t[]
	,int n_of_e) ;	/*데이터표시*/ 
void initfilter(double filter[FILTERNO][FILTERSIZE][FILTERSIZE]) ;
			/*필터 초기화*/
double drnd(void) ;	/* 난수의 생성 */
double f(double u) ;	/*전환 함수(시그모이드 함수)*/
void initwh(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]) ;
			/*중간층 가중치 초기화*/
void initwo(double wo[HIDDENNO+1]) ;	/*출력층 가중치 초기화*/
double forward(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	,double wo[HIDDENNO+1],double hi[]
	,double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]) ;
			/*순방향 계산*/
void olearn(double wo[HIDDENNO+1],double hi[]
	,double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1],double o) ;
			/*출력층 가중치 조정*/
void hlearn(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	,double wo[HIDDENNO+1],double hi[]
	,double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1],double o) ;
			/*중간층 가중치 조정*/
double f(double u) ;	/*전환 함수(시그모이드 함수)*/
void print(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	 ,double wo[HIDDENNO+1]) ;	/*결과 출력*/

/*******************/ 
/*    main() 함수  */ 
/*******************/ 
int main()
{
 double filter[FILTERNO][FILTERSIZE][FILTERSIZE] ;	 /*필터*/
 double e[MAXINPUTNO][INPUTSIZE][INPUTSIZE] ;	/*입력데이터*/
 int t[MAXINPUTNO] ;	/*교사데이터*/
 double convout[INPUTSIZE][INPUTSIZE]={0} ;	/*나선형출력*/
 double poolout[POOLOUTSIZE][POOLOUTSIZE] ;	/*출력데이터*/
 int i,j,m,n ;	/*반복 제어*/
 int n_of_e ;	/*학습 데이터 개수*/
 double err=BIGNUM ;	/*오차 평가*/
 int count=0 ;	/*반복 횟수 카운터*/
 double ef[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1] ;	/*전결합층으로의 입력 데이터*/
 double o ;	/*최종 출력*/
 double hi[HIDDENNO+1] ;	/*중간층 출력*/
 double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1] ;	/*중간층 가중치*/
 double wo[HIDDENNO+1] ;	/*출력층 가중치*/
 
 /*난수의 초기화*/
 srand(SEED) ;
 
 /*필터 초기화*/
 initfilter(filter) ;
 
 /*전결합층 가중치 초기화*/
 initwh(wh) ;	/*중간층 가중치 초기화*/
 initwo(wo) ;	/*출력층 가중치 초기화*/
 
 /*입력데이터 읽어들이기*/
 n_of_e=getdata(e,t) ;
 showdata(e,t,n_of_e) ;
 
  /*학습*/
 while(err>LIMIT){
  err=0.0 ;
  for(i=0;i<n_of_e;++i){/*학습 데이터마다 반복 */
   for(j=0;j<FILTERNO;++j){/*필터마다 반복 */
    /*나선형 계산*/
    conv(filter[j],e[i],convout) ;
    /*풀링 계산*/
    pool(convout,poolout) ;
    /*풀링 출력을 전결합층 입력으로 복사*/
    for(m=0;m<POOLOUTSIZE;++m)
     for(n=0;n<POOLOUTSIZE;++n)
      ef[j*POOLOUTSIZE*POOLOUTSIZE+POOLOUTSIZE*m+n]
	  =poolout[m][n] ;
    ef[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]=t[i] ;	/*교사 데이터*/
   }
   /*순방향 계산*/
   o=forward(wh,wo,hi,ef) ;
   /*출력층 가중치 조정*/
   olearn(wo,hi,ef,o) ;
   /*중간층 가중치 조정*/
   hlearn(wh,wo,hi,ef,o) ;
   /*오차 계산*/
   err+=(o-t[i])*(o-t[i]) ;
  }
  ++count ;
  /*오차 출력*/
  fprintf(stderr,"%d\t%lf\n",count,err) ;
 }/*학습 종료*/
 
 printf("\n***Results***\n") ;
 /*결합 하중 출력*/
 printf("Weights\n") ;
 print(wh,wo) ; 

 /*교사 데이터에 대한 출력*/
 printf("Network output\n") ;
 printf("#\tteacher\toutput\n") ;
 for(i=0;i<n_of_e;++i){
  printf("%d\t%d\t",i,t[i]) ;
  for(j=0;j<FILTERNO;++j){/*필터마다 반복 */
    /*나선형 계산*/
    conv(filter[j],e[i],convout) ;
    /*풀링 계산*/
    pool(convout,poolout) ;
    /*풀링 출력을 전결합층 입력으로 복사*/
    for(m=0;m<POOLOUTSIZE;++m)
     for(n=0;n<POOLOUTSIZE;++n)
      ef[j*POOLOUTSIZE*POOLOUTSIZE+POOLOUTSIZE*m+n]
	  =poolout[m][n] ;
    ef[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]=t[i] ;	/*교사 데이터*/
   }
   o=forward(wh,wo,hi,ef) ;
  printf("%lf\n",o) ;
 }

 return 0 ;
}

/**********************/
/*   print() 함수     */
/*   결과 출력        */
/**********************/
void print(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	 ,double wo[HIDDENNO+1])
{
 int i,j ;	/*반복 제어*/

 for(i=0;i<HIDDENNO;++i)
  for(j=0;j<POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1;++j)
   printf("%lf ",wh[i][j]) ;
 printf("\n") ;
 for(i=0;i<HIDDENNO+1;++i)
  printf("%lf ",wo[i]) ;
 printf("\n") ;
} 

/**********************/
/*  hlearn() 함수     */
/*  중간층 가중치 학습*/
/**********************/
void hlearn(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	,double wo[HIDDENNO+1]
	,double hi[],double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	,double o)
{
 int i,j ;	/*반복 제어*/
 double dj ;	/*중간층 가중치 계산에 이용*/

 for(j=0;j<HIDDENNO;++j){/*중간층 각 셀 j를 대상으로*/
  dj=hi[j]*(1-hi[j])*wo[j]*(e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]-o)*o*(1-o) ;
  for(i=0;i<POOLOUTSIZE*POOLOUTSIZE*FILTERNO;++i)/*i번째 가중치를 처리*/
   wh[j][i]+=ALPHA*e[i]*dj ;
  wh[j][i]+=ALPHA*(-1.0)*dj ;	/*역치 학습*/
 }
}

/***********************/
/*  olearn() 함수      */
/*  출력층 가중치 학습 */
/***********************/
void olearn(double wo[HIDDENNO+1]
    ,double hi[],double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1],double o)
{
 int i ;	/*반복 제어*/
 double d ;	/*가중치 계산에 이용*/

 d=(e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]-o)*o*(1-o) ;	/*오차 계산*/
 for(i=0;i<HIDDENNO;++i){
  wo[i]+=ALPHA*hi[i]*d ;	/*가중치 학습*/
 }
 wo[i]+=ALPHA*(-1.0)*d ;	/*역치 학습*/
 
} 

/**********************/
/*  forward() 함수     */
/*  순방향 계산      */
/**********************/
double forward(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	,double wo[HIDDENNO+1],double hi[]
	,double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1])
{
 int i,j ;	/*반복 제어*/
 double u ;	/*가중치 적용 합 계산*/
 double o ;	/*출력 계산*/

 /*hi 계산*/
 for(i=0;i<HIDDENNO;++i){
  u=0 ;	/*가중치 적용 합 구하기*/
  for(j=0;j<POOLOUTSIZE*POOLOUTSIZE*FILTERNO;++j)
   u+=e[j]*wh[i][j] ; 
  u-=wh[i][j] ;	/*역치 처리*/
  hi[i]=f(u) ;
 }
 /*출력 o 계산*/
 o=0 ;
 for(i=0;i<HIDDENNO;++i)
  o+=hi[i]*wo[i] ;
 o-=wo[i] ;	/*역치 처리*/
 
 return f(o) ;
} 

/**********************/
/*    initwo() 함수   */
/*중간층 가중치 초기화*/
/**********************/
void initwh(double wh[][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1])
{
 int i,j ;	/*반복 제어*/

 /*난수를 이용한 가중치 결정*/ 
 for(i=0;i<HIDDENNO;++i)
  for(j=0;j<POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1;++j)
   wh[i][j]=drnd() ;
} 

/**********************/
/*    initwo() 함수   */
/*출력층 가중치 초기화*/
/**********************/
void initwo(double wo[])
{
 int i ;	/*반복 제어*/

 /*난수를 이용한 가중치 결정*/
 for(i=0;i<HIDDENNO+1;++i)
   wo[i]=drnd() ;
} 

/**********************/
/*  initfilter() 함수 */
/*  필터 초기화       */
/**********************/
void initfilter(double filter[FILTERNO][FILTERSIZE][FILTERSIZE])
{
 int i,j,k ;	/*반복 제어*/
 
 for(i=0;i<FILTERNO;++i)
  for(j=0;j<FILTERSIZE;++j)
   for(k=0;k<FILTERSIZE;++k)
    filter[i][j][k]=drnd() ;
} 

/*******************/
/* drnd() 함수     */
/* 난수의 생성     */
/*******************/
double drnd(void)
{
 double rndno ;	/*생성한 난수*/

 while((rndno=(double)rand()/RAND_MAX)==1.0) ;
 rndno=rndno*2-1 ;	/*-1부터 1 사이의 난수 생성*/
 return rndno; 
}

/**********************/
/*  pool() 함수       */
/* 풀링 계산	 */
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
 int halfpool=POOLSIZE/2 ;	/*풀링  크기 1/2*/
 
 max
 =convout[i*POOLOUTSIZE+1+halfpool][j*POOLOUTSIZE+1+halfpool];
 for(m=POOLOUTSIZE*i+1;m<=POOLOUTSIZE*i+1+(POOLSIZE-halfpool);++m)
  for(n=POOLOUTSIZE*j+1;n<=POOLOUTSIZE*j+1+(POOLSIZE-halfpool);++n)
   if(max<convout[m][n]) max=convout[m][n] ;

 return max ;
}

/**********************/
/* showdata() 함수    */
/*입력 데이터 표시    */
/**********************/
void showdata(double e[][INPUTSIZE][INPUTSIZE],int t[],int n_of_e)
{
 int i=0,j=0,k=0 ;	/*반복 제어용*/

 /*데이터 표시*/
 for(i=0;i<n_of_e;++i){
  printf("N=%d category=%d\n",i,t[i]) ;
  for(j=0;j<INPUTSIZE;++j){
   for(k=0;k<INPUTSIZE;++k)
    printf("%.3lf ",e[i][j][k]) ;
   printf("\n") ;
  }
  printf("\n") ;
 }
}

/************************/
/*  getdata() 함수      */
/*입력 데이터 읽어들이기*/
/************************/
int getdata(double e[][INPUTSIZE][INPUTSIZE],int t[])
{
 FILE *fp=fopen("data/data1.txt","r");
  if(fp==NULL)
  {
    printf("file open error!\n");
    exit(1);    
  }
 int i=0,j=0,k=0 ;	/*반복 제어용*/

 /*데이터 입력*/
 while(fscanf(fp,"%d",&t[i])!=EOF){/*교사 데이터 읽어들이기*/
  /*이미지 데이터 읽어들이기*/
  while(fscanf(fp,"%lf",&e[i][j][k])!=EOF){
   ++ k ;
   if(k>=INPUTSIZE){/*다음 데이터*/
    k=0 ;
    ++j ;
    if(j>=INPUTSIZE) break ;	/*입력 종료*/
   }
  }
  j=0; k=0 ;
  ++i ; 
 }
 fclose(fp);
 return i ;
}

/**********************/
/*  conv() 함수       */
/*  나선형 계산       */
/**********************/
void conv(double filter[][FILTERSIZE]
	,double e[][INPUTSIZE],double convout[][INPUTSIZE])
{
 int i=0,j=0 ;	/*반복 제어용*/
 int startpoint=FILTERSIZE/2 ;	/*나선형 범위 하한*/

 for(i=startpoint;i<INPUTSIZE-startpoint;++i)
  for(j=startpoint;j<INPUTSIZE-startpoint;++j)
   convout[i][j]=calcconv(filter,e,i,j) ;
}

/**********************/
/*  calcconv() 함수   */
/*  필터 적용	*/
/**********************/
double calcconv(double filter[][FILTERSIZE]
	    ,double e[][INPUTSIZE],int i,int j)
{
 int m,n ;	/*반복 제어용*/
 double sum=0 ;	/*합 값*/
 
 for(m=0;m<FILTERSIZE;++m)
  for(n=0;n<FILTERSIZE;++n)
   sum+=e[i-FILTERSIZE/2+m][j-FILTERSIZE/2+n]*filter[m][n];
   
 return sum ;
}

/********************/
/* f() 함수	*/
/* 전환 함수        */
/*(시그모이드 함수) */
/********************/
double f(double u)
{
 return 1.0/(1.0+exp(-u)) ;
}
