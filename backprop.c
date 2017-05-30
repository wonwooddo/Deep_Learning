/*********************************************************/
/*                      bp1.c                            */
/* 백프로퍼게이션을 이용한 신경망 학습                   */
/* 사용 방법                                             */
/*  \Users\deeplearning\ch4>bp1 < data.txt > result.txt  */
/* 오차의 추이나 학습결과가 되는 결합 계수 등을 출력     */
/*********************************************************/

/*Visual Studio와의 호환성 확보 */
#define _CRT_SECURE_NO_WARNINGS

/* 헤더 파일 포함*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*기호 상수 정의*/
#define INPUTNO 3  /*입력층 셀 개수*/ 
#define HIDDENNO 3  /*중간층 셀 개수*/ 
#define ALPHA  10  /*학습 계수*/ 
#define SEED 65535    /*난수 시드*/ 
#define MAXINPUTNO 100    /*학습 데이터의 최대 개수*/ 
#define BIGNUM 100    /*오차의 초깃값*/ 
#define LIMIT 0.001    /*오차의 초깃값*/ 

/*함수 프로토타입 선언*/
double f(double u); /*전환 함수(시그모이드 함수)*/
void initwh(double wh[HIDDENNO][INPUTNO + 1]);
/*중간층 가중치의초기화*/
void initwo(double wo[HIDDENNO + 1]);/*출력층 가중치의 초기화*/
double drnd(void);/* 난수 생성     */
void print(double wh[HIDDENNO][INPUTNO + 1], double wo[HIDDENNO + 1]); /*결과 출력*/
double forward(double wh[HIDDENNO][INPUTNO + 1]
	, double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1]); /*순방향 계산*/
void olearn(double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1], double o); /*출력층 가중치의 조정*/
int getdata(double e[][INPUTNO + 1]); /*학습 데이터 읽어들이기*/
void hlearn(double wh[HIDDENNO][INPUTNO + 1], double wo[HIDDENNO + 1], double hi[]
	, double e[INPUTNO + 1], double o); /*중간층 가중치의 조정*/

/*******************/
/*    main() 함수  */
/*******************/
int main()
{
	double wh[HIDDENNO][INPUTNO + 1];/*중간층 가중치*/
	double wo[HIDDENNO + 1];/*출력층 가중치*/
	double e[MAXINPUTNO][INPUTNO + 1];/*학습 데이터 세트*/
	double hi[HIDDENNO + 1];/*중간층 출력*/
	double o;/*출력*/
	double err = BIGNUM;/*오차 평가*/
	int i, j;/*반복 제어*/
	int n_of_e;/*학습 데이터 개수*/
	int count = 0;/*반복 횟수 카운터*/

				  /*난수 초기화*/
	srand(SEED);

	/*가중치 초기화*/
	initwh(wh);/*중간층 가중치의 초기화*/
	initwo(wo);/*출력층 가중치의 초기화*/
	print(wh, wo); /*결과 출력*/

				   /*학습 데이터 읽어들이기*/
	n_of_e = getdata(e);
	printf("학습 데이터 개수:%d\n", n_of_e);

	/*학습*/
	while (err>LIMIT) {
		err = 0.0;
		for (j = 0; j<n_of_e; ++j) {
			/*순방향 계산*/
			o = forward(wh, wo, hi, e[j]);
			/*출력층 가중치의 조정*/
			olearn(wo, hi, e[j], o);
			/*중간층 가중치의 조정*/
			hlearn(wh, wo, hi, e[j], o);
			/*오차 계산*/
			err += (o - e[j][INPUTNO])*(o - e[j][INPUTNO]);
		}
		++count;
		/*오차 출력*/
		fprintf(stderr, "%d\t%lf\n", count, err);
	}/*학습 종료*/

	 /*결합 하중 출력*/
	print(wh, wo);

	/*학습 데이터에 대한 출력*/
	for (i = 0; i<n_of_e; ++i) {
		printf("%d ", i);
		for (j = 0; j<INPUTNO + 1; ++j)
			printf("%lf ", e[i][j]);
		o = forward(wh, wo, hi, e[i]);
		printf("%lf\n", o);
	}

	return 0;
}

/**************************/
/*  hlearn() 함수         */
/*  중간층 가중치의 학습  */
/**************************/
void hlearn(double wh[HIDDENNO][INPUTNO + 1],
	double wo[HIDDENNO + 1],
	double hi[], double e[INPUTNO + 1], double o)
{
	int i, j;/*반복 제어*/
	double dj;/*중간층 가중치 계산에 이용*/

	for (j = 0; j < HIDDENNO; ++j) {/*중간층 각 셀 j를 대상으로*/
		dj = hi[j] * (1 - hi[j])*wo[j] * (e[INPUTNO] - o)*o*(1 - o);
		for (i = 0; i < INPUTNO; ++i)/*i번째 가중치 처리*/
			wh[j][i] += ALPHA*e[i] * dj;
		wh[j][i] += ALPHA*(-1.0)*dj;/*역치 학습*/
	}
}

/*************************/
/*  getdata() 함수       */
/* 학습 데이터 읽어들이기*/
/*************************/

int getdata(double e[][INPUTNO + 1])
{
	FILE* fp = NULL;
	fp = fopen("majority.txt", "r");
	int n_of_e = 0;/*데이터 세트 개수*/
	int j = 0;/*반복 제어용*/

			  /*데이터 입력*/
	while (fscanf(fp, "%lf", &e[n_of_e][j]) != EOF) {
		++j;
		if (j > INPUTNO) {/*다음 데이터*/
			j = 0;
			++n_of_e;
		}
	}
	fclose(fp);
	return n_of_e;
}
/*************************/
/* olearn() 함수         */
/* 출력층의 가중치 학습  */
/*************************/
void olearn(double wo[HIDDENNO + 1]
	, double hi[], double e[INPUTNO + 1], double o)
{
	int i;/*반복 제어*/
	double d;/*가중치 계산에 이용*/

	d = (e[INPUTNO] - o)*o*(1 - o);/*오차 계산*/
	for (i = 0; i < HIDDENNO; ++i) {
		wo[i] += ALPHA*hi[i] * d;/*가중치 학습*/
	}
	wo[i] += ALPHA*(-1.0)*d;/*역치 학습*/
}

/**********************/
/*  forward() 함수    */
/*  순방향 계산       */
/**********************/
double forward(double wh[HIDDENNO][INPUTNO + 1],double wo[HIDDENNO + 1], double hi[],double e[INPUTNO + 1])
{
	int i, j;/*반복 제어*/
	double u;/*가중치를 적용한 합 계산*/
	double o;/*출력 계산*/

	/*hi 계산*/
	for (i = 0; i < HIDDENNO; ++i) {
		u = 0;/*가중치를 적용한 합 구하기*/
		for (j = 0; j < INPUTNO; ++j)
			u += e[j] * wh[i][j];
		u -= wh[i][j];/*역치 처리*/
		hi[i] = f(u);
	}
	/*출력 o 계산*/
	o = 0;
	for (i = 0; i < HIDDENNO; ++i)
		o += hi[i] * wo[i];
	o -= wo[i];/*역치 처리*/

	return f(o);
}

/**********************/
/*   print() 함수     */
/*   결과 출력        */
/**********************/
void print(double wh[HIDDENNO][INPUTNO + 1], double wo[HIDDENNO + 1])
{
	int i, j;/*반복 제어*/

	for (i = 0; i < HIDDENNO; ++i)
	{
		printf("HiddenNode [%d] =  weights: ", i);
		for (j = 0; j < INPUTNO; ++j)
		{
			printf(" %.2lf,", wh[i][j]);
		}
		printf(" bias : %.2lf", wh[i][j]);
		printf("\n");
	}
	printf("\n");

	printf("OutputNode =  weights: ");
	for (i = 0; i < HIDDENNO ; ++i)
	{
		printf(" %.2lf,", wo[i]);
	}
	printf(" bias : %.2lf", wo[i]);

	printf("\n");
}

/************************/
/*    initwh() 함수     */
/*중간층 가중치의 초기화*/
/************************/
void initwh(double wh[HIDDENNO][INPUTNO + 1])
{
	int i, j;/*반복 제어*/

	/*난수를 이용한 가중치 결정*/
	for (i = 0; i < HIDDENNO; ++i)
		for (j = 0; j < INPUTNO + 1; ++j)
			wh[i][j] = 0;
}

/************************/
/*    initwo() 함수     */
/*출력층 가중치의 초기화*/
/************************/
void initwo(double wo[HIDDENNO + 1])
{
	int i;/*반복 제어*/

	/*난수를 이용한 가중치 결정*/
	for (i = 0; i < HIDDENNO + 1; ++i)
		wo[i] = 0;
}

/*******************/
/* drnd() 함수     */
/* 난수 생성       */
/*******************/
double drnd(void)
{
	double rndno;/*생성한 난수*/

	while ((rndno = (double)rand() / RAND_MAX) == 1.0);
	rndno = rndno * 2 - 1;/*-1부터 1 사이의 난수 생성*/
	return rndno;
}

/*******************/
/* f() 함수        */
/* 전환 함수       */
/*(시그모이드 함수)*/
/*******************/
double f(double u)
{
	return 1.0 / (1.0 + exp(-u));
}
