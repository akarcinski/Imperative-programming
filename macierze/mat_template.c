#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) { 
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

void print_mat_ind(double A[][SIZE], int m, int n, const int indices[]);

// 5.1
// Calculate matrix product, AB = A X B
// A[m][p], B[p][n]                                                         r1 c1 r2     c2
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n)
{
    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
        {
            for(int k=0; k<p; k++)
            {
                 AB[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


// Calculate matrix - vector product
//void mat_vec_product(double A[][SIZE], const double b[], double Ab[], int m, int n);

//void backward_substit(double A[][SIZE], double x[], int n);

//void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n);

// 5.2
// Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.


double gauss_simplified(double A[][SIZE], int n)
{
    int row_perm[SIZE]; // zmiana
    int sign = 1;

    for(int i = 0; i < n; i++)
        row_perm[i] = i;

    for(int i = 0; i < n; i++) // POTENCJALNIE n-1
    {
        //if(A[i][i] == 0)
        if(A[ row_perm[i] ][i] == 0)
        {
            int j = 0;
            for(j = i + 1; j < n; j++)
            {
                //if(A[j][i] != 0)
                if(A[ row_perm[j] ][i] != 0)
                {
                    //swap( row_perm[i], row_perm[j] );
                    int tmp = row_perm[i];
                    row_perm[i] = row_perm[j];
                    row_perm[j] = tmp;

                    //swap_row(A, i, j, n);

                    sign = (-1)*sign;
                    break;
                }
            }
            if(j == n)
                return NAN;
        }

        for(int j = i + 1; j < n; j++)
        {
            double rat = A[ row_perm[j] ][i] / A[ row_perm[i] ][i];
            //double rat = A[j][i] / A[i][i];
            for(int k = 0; k < n; k++)
            {
                A[ row_perm[j] ][k] -= rat * A[ row_perm[i] ][k];
                //A[j][k] -= rat * A[i][k];
            }
        }
    }
    
    double sol = 1;
    for(int i = 0; i < n; i++)
        sol *= A[ row_perm[i] ][i];
        //sol *= A[i][i];

    return sign * sol;
}

// 5.3
// Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double AB(double a)
{
    if( a < 0)
        return (-1) * a;
    return a;
}
double MX(double a, double b)
{
    if( a > b )
        return a;
    return b;
}

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps)
{
    double mat[n][n+1];
    for (int i = 0; i < n;  i++) {
        for (int j = 0; j < n; j++)
            mat[i][j] = A[i][j];
    }
    for (int i = 0; i < n; i++) 
        mat[i][n] = b[i];

    int row_perm[SIZE];
    for(int i = 0; i < n; i++)
        row_perm[i] = i;

    double det = gauss_simplified(A, n);
    if (det == NAN){
        return 0;
    }
    for(int i = 0; i <= n; i++) // POTENCJALNIE n-1
    {
        //if(A[i][i] == 0)
        if(mat[ row_perm[i] ][i] == 0)
        {
            int j = 0;
            for(j = i + 1; j < n; j++)
            {
                //if(A[j][i] != 0)
                if(mat[ row_perm[j] ][i] != 0)
                {
                    //swap( row_perm[i], row_perm[j] );
                    int tmp = row_perm[i];
                    row_perm[i] = row_perm[j];
                    row_perm[j] = tmp;

                    //swap_row(A, i, j, n);
                    break;
                }
            }
            if(j == n)
                return 0;
        }

        for(int j = i + 1; j < n; j++)
        {
            double rat = mat[ row_perm[j] ][i] / mat[ row_perm[i] ][i];
            //double rat = A[j][i] / A[i][i];
            for(int k = 0; k <= n; k++)
            {
                mat[ row_perm[j] ][k] -= rat * mat[ row_perm[i] ][k];
                //A[j][k] -= rat * A[i][k];
            }
        }
    }



    for (int i = n-1; i >= 0; i--) {
        //double s = mat[ row_perm[i] ][n] / mat[ row_perm[i] ][i];
        //x[i] = s;
        //mat[ row_perm[i] ][n] /= s; 
        //mat[ row_perm[i] ][i] /= s;
        //mat[ row_perm[i] ][i] = 1;
        x[ row_perm[i] ] = mat[ row_perm[i] ][n];
        for (int j = i+1; j < n; j++) {
            //mat[ row_perm[j] ][n] -= x[i] * (mat[ row_perm[j] ][i] / mat[ row_perm[i] ][i]);
            //mat[ row_perm[j] ][n] -= mat[ row_perm[j] ][i] / mat[ row_perm[i] ][i];
            //mat[ row_perm[j] ][i] = 0; 
            //printf("%d %lf\n",i, x[i]);
            x[ row_perm[i] ] -= mat[ row_perm[i] ][j]*x[ row_perm[j] ];
        }
        x[ row_perm[i] ] = x[ row_perm[i] ]/mat[ row_perm[i] ][i];
    }

    /*for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            printf("%lf ", mat[ row_perm[i] ][j]);
        }
        printf("\n");
    }*/
    int cos;
    double COS;
    for(int j = 0; j < n - 1; j++)
        for(int i = 0; i < n - 1; i++)
            if(row_perm[i] > row_perm[i + 1]) {
                //swap(x[ row_perm[i] ], x[ row_perm[i+1] ]);
                //swap(row_perm[i], row_perm[i + 1]);
                COS = x[ row_perm[i] ];
                x[ row_perm[i] ] = x[ row_perm[i]+1 ];
                x[ row_perm[i+1] ] = COS;

                cos = row_perm[i];
                row_perm[i] = row_perm[i+1];
                row_perm[i+1] = cos;

            }

    for (int i = 0; i < n; i++)
        //printf("%d %d %lf \n", i, row_perm[i], x[ row_perm[i] ]);
    return det;
}
  
// function to calculate the values of the unknowns

// 5.4
// Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.



double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps)
{
    int permutation[n];

    int m = 2 * n;
    double augment_Matrix[n][m];

    for(int i = 0; i < n; i++) {
        permutation[i] = i;
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i == j) {
                augment_Matrix[i][j+n] = 1;
            }
            augment_Matrix[i][j] = A[i][j];
        }
    }

    double det = gauss_simplified(A, n);
    if(det == NAN) {
        return 0;
    }
    double M = 0;
    for(int i = 0; i < n; i++) {
        M = MX(M, A[i][i]);
    }
    if( M < eps ) {
        return 0;
    }
    int t;
    for(int i = n - 1; i > 0; i--)
    {
        if(augment_Matrix[ permutation[i-1] ][0]<augment_Matrix[ permutation[i] ][0]) {
            t = permutation[i-1];
            permutation[i-1] = permutation[i];
            permutation[i] = t;
        }
    }

    double tmp;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i != j) {
                tmp = augment_Matrix[   permutation[j] ][i] / augment_Matrix[ permutation[i] ][i];
                for(int k = 0; k < m; k++){
                    augment_Matrix[ permutation[j] ][k] -= augment_Matrix[ permutation[i] ][k] * tmp;
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        tmp = augment_Matrix[ permutation[i] ][i];
        for (int j = 0; j < m; j++) {
            augment_Matrix[ permutation[i] ][j] = augment_Matrix[ permutation[i] ][j] / tmp;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B[i][j] = augment_Matrix[ permutation[i] ][j+n];
        }
    }
    return det;
    /*
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%lf ", augment_Matrix[ permutation[i] ][j]);
        }
        printf("\n");
    }*/
}

int main(void) 
{

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if(det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A,n,n);
			printf("%.4f\n",matrix_inv(A, B, n, eps));
			print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}