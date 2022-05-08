#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

double get(int cols, int row, int col, const double *A);

void set(int cols, int row, int col, double *A, double value);

void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB);

void read_mat(int rows, int cols, double *t);

void print_mat(int rows, int cols, double *t);

int read_char_lines(char *tab[]);

void write_char_line(char *tab[], int n);

void delete_lines(char *tab[], int line_count);

int read_dbl_lines_v1(char *tab[]);

void write_dbl_line_v1(char *tab[], int n);

int main(void) {
	int to_do;

	scanf ("%d", &to_do);

	double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
	int n, lines_counter, rowsA,colsA,rowsB,colsB;
	char *char_lines_table[TAB_SIZE];
	double series_table[TAB_SIZE];
	double *ptr_table[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d",&rowsA,&colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d",&rowsB,&colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA,colsA,colsB,A,B,C);
			print_mat(rowsA,colsB,C);
			break;
		case 2:
			scanf("%d",&n);
			ptr_table[0] = series_table;
			lines_counter = read_dbl_lines_v1(char_lines_table);
			write_dbl_line_v1(char_lines_table,n);
			delete_lines(char_lines_table,lines_counter);
			break;
		case 3:
			scanf("%d", &n);
			lines_counter = read_char_lines(char_lines_table);
			write_char_line(char_lines_table,n);
			delete_lines(char_lines_table,lines_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}

double get(int cols, int row, int col, const double *A)
{
	return A[ (cols*row) + col];
}

void set(int cols, int row, int col, double *A, double value)
{
	A[ (cols*row) + col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB)
{
	double a;
	for (int i = 0; i < rowsA; i++) {
		for (int j = 0; j < colsB; j++) {
			for (int k = 0; k < colsA; k++) {
				a = get(colsA, i, k, A) * get(colsB, k, j, B);
				set(colsB, i, j, AB, get(colsB, i, j, AB) + a); 
			}
		}
	}
}

void read_mat(int rows, int cols, double *t)
{
	int n = rows * cols;
	for (int i = 0; i < n; i++) {
		scanf("%lf", &t[i]);
	}
}

void print_mat(int rows, int cols, double *t)
{
	int n = cols * rows;
	printf("%.2f ", t[0]);
	for (int i = 1; i < n; i++) {
		if ( i % cols == 0) {
			printf("\n");
		}
		printf("%.2f ", t[i]);
	}
}

int read_char_lines(char *tab[])
{
	int n = 0;
	char buf[TAB_SIZE];
	while ( fgets(buf, BUF_SIZE, stdin) ) {
		for (int i = 0; i < BUF_SIZE; i++) {
			if (buf[i] == '\n') {
				tab[n] = (char*)malloc( sizeof(char) * (i+1) );
				
				for (int j = 0; j < i + 1; j++) {
					tab[n][j] = buf[j];
				}
				tab[n][i + 1] = '\0';
				break;
			}
		}
		n++;
	}
	return n;
}

void write_char_line(char *tab[], int n)
{
	int i = 0;
	while ( tab[n][i] != '\0' ) {
		printf("%c", tab[n][i]);
		i++;
	}
}

void delete_lines(char *tab[], int line_count)
{
	for (int i = 0; i < line_count; i++) {
		free(tab[i]);
	}
}

int read_dbl_lines_v1(char *tab[])
{
	int n = 0;
	char buf[TAB_SIZE];
	while ( fgets(buf, BUF_SIZE, stdin) ) {
		for (int i = 0; i < BUF_SIZE; i++) {
			if (buf[i] == '\n') {
				tab[n] = (char*)malloc( sizeof(char) * (i+1) );
				
				for (int j = 0; j < i + 1; j++) {
					tab[n][j] = buf[j];
				}
				tab[n][i + 1] = '\0';
				break;
			}
		}
		n++;
	}
	return n;

//-------------------------------------------------
	/*
	int n = 0, i = 0;
	char buf[TAB_SIZE];
	char *pEnd, *pStart;
	double number;
	while ( fgets(buf, BUF_SIZE, stdin) ) {
		number = strtod(buf, &pEnd);
		ptr_tab[0][i] = number;
		printf("%lf ", number);

		while( ) {
			i++;
			pStart = pEnd;
			number = strtod(pStart, &pEnd);
			ptr_tab[0][i] = number;
			printf("%lf ", number);
		}
		i++;
		n++;
	}
	return n;
	*/
}

void write_dbl_line_v1(char *tab[], int n)
{
	int i = 0, id = 0;
	char number[999];

	for (int i = 0; i < 999; i++) {
		number[i] = '#';
	}
	while ( tab[n][i] != '\0' ) {
		if (tab[n][i] >= '0' && tab[n][i] <= '9' ) {
			number[id] = tab[n][i];
			id++;
		}
		else if (tab[n][i] == 'e') {
			number[id] = 'e';
			id++;
		}
		else if (tab[n][i] == '-') {
			number[id] = '-';
			id++;
		}
		else if (tab[n][i] == '.') {
			number[id] = '.';
			id++;
		}
		else if (tab[n][i] == ',') {
			number[id] = ',';
			id++;
		}
		else {
			double res;
			res = atof(number);
			printf("%.2f ", res);
			id = 0;
			for (int i = 0; i < 999; i++) {
				number[i] = '#';
			}
		}
		i++;
	}
}