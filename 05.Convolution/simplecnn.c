/*********************************************************/
/*            simplecnn.c                                */
/* ������ �Ű���� �⺻ ���� ����                        */
/* CNN�� �⺻ ����(��ݸ�)�� ��Ÿ��                      */
/* ��� ���                                             */
/* \Users\deeplearning\ch5>simplecnn < data1.txt         */
/*********************************************************/

/* ��� ���� ����*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*��ȣ ��� ����*/
#define INPUTSIZE 11	/*�Է� ��*/ 
#define FILTERSIZE 3	/*������ ũ��*/
#define FILTERNO 2		/*���� ����*/
#define POOLSIZE 3		/*Ǯ�� ũ��*/
#define POOLOUTSIZE 3	/*Ǯ�� ��� ũ��*/
#define MAXINPUTNO 100	/*�н� ������ �ִ� ����*/ 
#define SEED 65535		/*���� �õ�*/ 
#define LIMIT 0.001		/*���� ���Ѱ�*/ 
#define BIGNUM 100		/*���� �ʱ갪*/ 
#define HIDDENNO 3		/*�߰��� �� ����*/ 
#define ALPHA  10		/*�н� ���*/ 

/*�Լ� ������Ÿ�� ����*/
void conv(double filter[FILTERSIZE][FILTERSIZE]
	,double e[][INPUTSIZE]
	,double convout[][INPUTSIZE]) ;	/*������ ���*/
double calcconv(double filter[][FILTERSIZE]
	,double e[][INPUTSIZE],int i,int j) ;
			/*  ���� ����  */
void pool(double convout[][INPUTSIZE]
	,double poolout[][POOLOUTSIZE]) ; 
			/*Ǯ�� ���*/	  
double maxpooling(double convout[][INPUTSIZE]
	,int i,int j) ;	/* �ִ� Ǯ�� */
int getdata(double e[][INPUTSIZE][INPUTSIZE],int r[]) ;
			/*������ �о���̱�*/ 
void showdata(double e[][INPUTSIZE][INPUTSIZE],int t[]
	,int n_of_e) ;	/*������ǥ��*/ 
void initfilter(double filter[FILTERNO][FILTERSIZE][FILTERSIZE]) ;
			/*���� �ʱ�ȭ*/
double drnd(void) ;	/* ������ ���� */
double f(double u) ;	/*��ȯ �Լ�(�ñ׸��̵� �Լ�)*/
void initwh(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]) ;
			/*�߰��� ����ġ �ʱ�ȭ*/
void initwo(double wo[HIDDENNO+1]) ;	/*����� ����ġ �ʱ�ȭ*/
double forward(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	,double wo[HIDDENNO+1],double hi[]
	,double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]) ;
			/*������ ���*/
void olearn(double wo[HIDDENNO+1],double hi[]
	,double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1],double o) ;
			/*����� ����ġ ����*/
void hlearn(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	,double wo[HIDDENNO+1],double hi[]
	,double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1],double o) ;
			/*�߰��� ����ġ ����*/
double f(double u) ;	/*��ȯ �Լ�(�ñ׸��̵� �Լ�)*/
void print(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	 ,double wo[HIDDENNO+1]) ;	/*��� ���*/

/*******************/ 
/*    main() �Լ�  */ 
/*******************/ 
int main()
{
 double filter[FILTERNO][FILTERSIZE][FILTERSIZE] ;	 /*����*/
 double e[MAXINPUTNO][INPUTSIZE][INPUTSIZE] ;	/*�Էµ�����*/
 int t[MAXINPUTNO] ;	/*���絥����*/
 double convout[INPUTSIZE][INPUTSIZE]={0} ;	/*���������*/
 double poolout[POOLOUTSIZE][POOLOUTSIZE] ;	/*��µ�����*/
 int i,j,m,n ;	/*�ݺ� ����*/
 int n_of_e ;	/*�н� ������ ����*/
 double err=BIGNUM ;	/*���� ��*/
 int count=0 ;	/*�ݺ� Ƚ�� ī����*/
 double ef[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1] ;	/*�������������� �Է� ������*/
 double o ;	/*���� ���*/
 double hi[HIDDENNO+1] ;	/*�߰��� ���*/
 double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1] ;	/*�߰��� ����ġ*/
 double wo[HIDDENNO+1] ;	/*����� ����ġ*/
 
 /*������ �ʱ�ȭ*/
 srand(SEED) ;
 
 /*���� �ʱ�ȭ*/
 initfilter(filter) ;
 
 /*�������� ����ġ �ʱ�ȭ*/
 initwh(wh) ;	/*�߰��� ����ġ �ʱ�ȭ*/
 initwo(wo) ;	/*����� ����ġ �ʱ�ȭ*/
 
 /*�Էµ����� �о���̱�*/
 n_of_e=getdata(e,t) ;
 showdata(e,t,n_of_e) ;
 
  /*�н�*/
 while(err>LIMIT){
  err=0.0 ;
  for(i=0;i<n_of_e;++i){/*�н� �����͸��� �ݺ� */
   for(j=0;j<FILTERNO;++j){/*���͸��� �ݺ� */
    /*������ ���*/
    conv(filter[j],e[i],convout) ;
    /*Ǯ�� ���*/
    pool(convout,poolout) ;
    /*Ǯ�� ����� �������� �Է����� ����*/
    for(m=0;m<POOLOUTSIZE;++m)
     for(n=0;n<POOLOUTSIZE;++n)
      ef[j*POOLOUTSIZE*POOLOUTSIZE+POOLOUTSIZE*m+n]
	  =poolout[m][n] ;
    ef[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]=t[i] ;	/*���� ������*/
   }
   /*������ ���*/
   o=forward(wh,wo,hi,ef) ;
   /*����� ����ġ ����*/
   olearn(wo,hi,ef,o) ;
   /*�߰��� ����ġ ����*/
   hlearn(wh,wo,hi,ef,o) ;
   /*���� ���*/
   err+=(o-t[i])*(o-t[i]) ;
  }
  ++count ;
  /*���� ���*/
  fprintf(stderr,"%d\t%lf\n",count,err) ;
 }/*�н� ����*/
 
 printf("\n***Results***\n") ;
 /*���� ���� ���*/
 printf("Weights\n") ;
 print(wh,wo) ; 

 /*���� �����Ϳ� ���� ���*/
 printf("Network output\n") ;
 printf("#\tteacher\toutput\n") ;
 for(i=0;i<n_of_e;++i){
  printf("%d\t%d\t",i,t[i]) ;
  for(j=0;j<FILTERNO;++j){/*���͸��� �ݺ� */
    /*������ ���*/
    conv(filter[j],e[i],convout) ;
    /*Ǯ�� ���*/
    pool(convout,poolout) ;
    /*Ǯ�� ����� �������� �Է����� ����*/
    for(m=0;m<POOLOUTSIZE;++m)
     for(n=0;n<POOLOUTSIZE;++n)
      ef[j*POOLOUTSIZE*POOLOUTSIZE+POOLOUTSIZE*m+n]
	  =poolout[m][n] ;
    ef[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]=t[i] ;	/*���� ������*/
   }
   o=forward(wh,wo,hi,ef) ;
  printf("%lf\n",o) ;
 }

 return 0 ;
}

/**********************/
/*   print() �Լ�     */
/*   ��� ���        */
/**********************/
void print(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	 ,double wo[HIDDENNO+1])
{
 int i,j ;	/*�ݺ� ����*/

 for(i=0;i<HIDDENNO;++i)
  for(j=0;j<POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1;++j)
   printf("%lf ",wh[i][j]) ;
 printf("\n") ;
 for(i=0;i<HIDDENNO+1;++i)
  printf("%lf ",wo[i]) ;
 printf("\n") ;
} 

/**********************/
/*  hlearn() �Լ�     */
/*  �߰��� ����ġ �н�*/
/**********************/
void hlearn(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	,double wo[HIDDENNO+1]
	,double hi[],double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	,double o)
{
 int i,j ;	/*�ݺ� ����*/
 double dj ;	/*�߰��� ����ġ ��꿡 �̿�*/

 for(j=0;j<HIDDENNO;++j){/*�߰��� �� �� j�� �������*/
  dj=hi[j]*(1-hi[j])*wo[j]*(e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]-o)*o*(1-o) ;
  for(i=0;i<POOLOUTSIZE*POOLOUTSIZE*FILTERNO;++i)/*i��° ����ġ�� ó��*/
   wh[j][i]+=ALPHA*e[i]*dj ;
  wh[j][i]+=ALPHA*(-1.0)*dj ;	/*��ġ �н�*/
 }
}

/***********************/
/*  olearn() �Լ�      */
/*  ����� ����ġ �н� */
/***********************/
void olearn(double wo[HIDDENNO+1]
    ,double hi[],double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1],double o)
{
 int i ;	/*�ݺ� ����*/
 double d ;	/*����ġ ��꿡 �̿�*/

 d=(e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]-o)*o*(1-o) ;	/*���� ���*/
 for(i=0;i<HIDDENNO;++i){
  wo[i]+=ALPHA*hi[i]*d ;	/*����ġ �н�*/
 }
 wo[i]+=ALPHA*(-1.0)*d ;	/*��ġ �н�*/
 
} 

/**********************/
/*  forward() �Լ�     */
/*  ������ ���      */
/**********************/
double forward(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
	,double wo[HIDDENNO+1],double hi[]
	,double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1])
{
 int i,j ;	/*�ݺ� ����*/
 double u ;	/*����ġ ���� �� ���*/
 double o ;	/*��� ���*/

 /*hi ���*/
 for(i=0;i<HIDDENNO;++i){
  u=0 ;	/*����ġ ���� �� ���ϱ�*/
  for(j=0;j<POOLOUTSIZE*POOLOUTSIZE*FILTERNO;++j)
   u+=e[j]*wh[i][j] ; 
  u-=wh[i][j] ;	/*��ġ ó��*/
  hi[i]=f(u) ;
 }
 /*��� o ���*/
 o=0 ;
 for(i=0;i<HIDDENNO;++i)
  o+=hi[i]*wo[i] ;
 o-=wo[i] ;	/*��ġ ó��*/
 
 return f(o) ;
} 

/**********************/
/*    initwo() �Լ�   */
/*�߰��� ����ġ �ʱ�ȭ*/
/**********************/
void initwh(double wh[][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1])
{
 int i,j ;	/*�ݺ� ����*/

 /*������ �̿��� ����ġ ����*/ 
 for(i=0;i<HIDDENNO;++i)
  for(j=0;j<POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1;++j)
   wh[i][j]=drnd() ;
} 

/**********************/
/*    initwo() �Լ�   */
/*����� ����ġ �ʱ�ȭ*/
/**********************/
void initwo(double wo[])
{
 int i ;	/*�ݺ� ����*/

 /*������ �̿��� ����ġ ����*/
 for(i=0;i<HIDDENNO+1;++i)
   wo[i]=drnd() ;
} 

/**********************/
/*  initfilter() �Լ� */
/*  ���� �ʱ�ȭ       */
/**********************/
void initfilter(double filter[FILTERNO][FILTERSIZE][FILTERSIZE])
{
 int i,j,k ;	/*�ݺ� ����*/
 
 for(i=0;i<FILTERNO;++i)
  for(j=0;j<FILTERSIZE;++j)
   for(k=0;k<FILTERSIZE;++k)
    filter[i][j][k]=drnd() ;
} 

/*******************/
/* drnd() �Լ�     */
/* ������ ����     */
/*******************/
double drnd(void)
{
 double rndno ;	/*������ ����*/

 while((rndno=(double)rand()/RAND_MAX)==1.0) ;
 rndno=rndno*2-1 ;	/*-1���� 1 ������ ���� ����*/
 return rndno; 
}

/**********************/
/*  pool() �Լ�       */
/* Ǯ�� ���	 */
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
 int halfpool=POOLSIZE/2 ;	/*Ǯ��  ũ�� 1/2*/
 
 max
 =convout[i*POOLOUTSIZE+1+halfpool][j*POOLOUTSIZE+1+halfpool];
 for(m=POOLOUTSIZE*i+1;m<=POOLOUTSIZE*i+1+(POOLSIZE-halfpool);++m)
  for(n=POOLOUTSIZE*j+1;n<=POOLOUTSIZE*j+1+(POOLSIZE-halfpool);++n)
   if(max<convout[m][n]) max=convout[m][n] ;

 return max ;
}

/**********************/
/* showdata() �Լ�    */
/*�Է� ������ ǥ��    */
/**********************/
void showdata(double e[][INPUTSIZE][INPUTSIZE],int t[],int n_of_e)
{
 int i=0,j=0,k=0 ;	/*�ݺ� �����*/

 /*������ ǥ��*/
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
/*  getdata() �Լ�      */
/*�Է� ������ �о���̱�*/
/************************/
int getdata(double e[][INPUTSIZE][INPUTSIZE],int t[])
{
 FILE *fp=fopen("data/data1.txt","r");
  if(fp==NULL)
  {
    printf("file open error!\n");
    exit(1);    
  }
 int i=0,j=0,k=0 ;	/*�ݺ� �����*/

 /*������ �Է�*/
 while(fscanf(fp,"%d",&t[i])!=EOF){/*���� ������ �о���̱�*/
  /*�̹��� ������ �о���̱�*/
  while(fscanf(fp,"%lf",&e[i][j][k])!=EOF){
   ++ k ;
   if(k>=INPUTSIZE){/*���� ������*/
    k=0 ;
    ++j ;
    if(j>=INPUTSIZE) break ;	/*�Է� ����*/
   }
  }
  j=0; k=0 ;
  ++i ; 
 }
 fclose(fp);
 return i ;
}

/**********************/
/*  conv() �Լ�       */
/*  ������ ���       */
/**********************/
void conv(double filter[][FILTERSIZE]
	,double e[][INPUTSIZE],double convout[][INPUTSIZE])
{
 int i=0,j=0 ;	/*�ݺ� �����*/
 int startpoint=FILTERSIZE/2 ;	/*������ ���� ����*/

 for(i=startpoint;i<INPUTSIZE-startpoint;++i)
  for(j=startpoint;j<INPUTSIZE-startpoint;++j)
   convout[i][j]=calcconv(filter,e,i,j) ;
}

/**********************/
/*  calcconv() �Լ�   */
/*  ���� ����	*/
/**********************/
double calcconv(double filter[][FILTERSIZE]
	    ,double e[][INPUTSIZE],int i,int j)
{
 int m,n ;	/*�ݺ� �����*/
 double sum=0 ;	/*�� ��*/
 
 for(m=0;m<FILTERSIZE;++m)
  for(n=0;n<FILTERSIZE;++n)
   sum+=e[i-FILTERSIZE/2+m][j-FILTERSIZE/2+n]*filter[m][n];
   
 return sum ;
}

/********************/
/* f() �Լ�	*/
/* ��ȯ �Լ�        */
/*(�ñ׸��̵� �Լ�) */
/********************/
double f(double u)
{
 return 1.0/(1.0+exp(-u)) ;
}
