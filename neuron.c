/*********************************************************/
/*                   neuron.c                            */
/* �ϳ��� �ΰ� �Ű� ���                                 */
/* ������ ����ġ�� ��ġ�� ���� �ΰ� �Ű��� �䳻��        */
/*********************************************************/

/*Visual Studio���� ȣȯ�� Ȯ�� */
#define _CRT_SECURE_NO_WARNINGS

/* ��� ���� ����*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*��ȣ ��� ����*/
#define INPUTNO 2  /*�Է� ��*/ 
#define MAXINPUTNO 100    /*�������� �ִ� ����*/ 

/*�Լ� ������Ÿ�� ����*/
double f(double u); /*��ȯ �Լ�*/
void initw(double w[INPUTNO + 1]);
/*����ġ�� ��ġ�� �ʱ�ȭ*/
double forward(double w[INPUTNO + 1]
	, double e[INPUTNO]); /*������ ���*/
int getdata(double e[][INPUTNO]);/*������ �о���̱�*/

/*******************/
/*    main() �Լ�  */
/*******************/
int main()
{
	double w[INPUTNO + 1];/*����ġ�� ��ġ*/
	double e[MAXINPUTNO][INPUTNO];/*������ ��Ʈ*/
	double o;/*���*/
	int i, j;/*�ݺ� ����*/
	int n_of_e;/*������ ����*/

	/*����ġ �ʱ�ȭ*/
	initw(w);

	/*�Է� ������ �о���̱�*/
	n_of_e = getdata(e);
	printf("������ ����:%d\n", n_of_e);

	/*��� ��ü*/
	for (i = 0; i < n_of_e; ++i) {
		printf("[Test_Set %d]   Inputs = ", i+1);
		for (j = 0; j < INPUTNO; ++j)
			printf("%.0lf  ", e[i][j]);
		o = forward(w, e[i]);
		printf("  Out = %.0lf\n", o);
	}

	return 0;
}

/************************/
/*  getdata() �Լ�      */
/* �н������� �о���̱�*/
/************************/
int getdata(double e[][INPUTNO])
{
	FILE* fp = fopen("data/data1.txt","r");
	if (fp == NULL)
	{
		printf("file open error!\n");
		exit(1);
	}
	int n_of_e = 0;/*������ ��Ʈ ����*/
	int j = 0;/*�ݺ� �����*/

	/*������ �Է�*/
	while (fscanf(fp,"%lf", &e[n_of_e][j]) != EOF) {
		++j;
		if (j >= INPUTNO) {/*���� ������*/
			j = 0;
			++n_of_e;
		}
	}
	fclose(fp);
	return n_of_e;
}

/**********************/
/*  forward() �Լ�    */
/*  ������ ���       */
/**********************/
double forward(double w[INPUTNO + 1], double e[INPUTNO])
{
	int i;/*�ݺ� ����*/
	double u, o;/*�߰� ��갪 u�� ��� o*/

	/*��� ��ü*/
	u = 0;
	for (i = 0; i < INPUTNO; ++i)
		u += e[i] * w[i];
	u += w[i];/*��ġ ó��*/
	/*��°� ���*/
	o = f(u);
	return o;
}

/**********************/
/*    initw() �Լ�    */
/*  ����ġ �ʱ�ȭ     */
/**********************/
void initw(double w[INPUTNO + 1])
{
	/*����� ����ġ�� ��*/
	w[0] = 1;
	w[1] = 1;
	w[2] = -2;
	// w[2]=0.5 ;
}

/*******************/
/* f() �Լ�        */
/* ��ȯ �Լ�       */
/*******************/
double f(double u)
{
	/*��� �Լ� ���*/
	if (u >= 0) return 1.0;
	else return 0.0;

	/*�ñ׸��̵� �Լ� ���*/
   // return 1.0/(1.0+exp(-u)) ;
}
