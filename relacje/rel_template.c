#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair* relation, int n, int a, int b);
// Read number of pairs, n, and then n pairs of ints
int read_relation(pair* relation);

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* relation, int size);
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* relation, int size);
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* relation, int size);
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* relation, int size);
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair* relation, int size);
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* relation, int size);

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair*, int);
// A total order relation is a partial order relation that is connected
int is_total_order(pair*, int);
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair*, int);
int find_max_elements(pair*, int, int*);
int find_min_elements(pair*, int, int*);
int get_domain(pair*, int, int*);

// Case 3:
int composition (pair*, int, pair*, int, pair*);

int cmp (pair p1, pair p2) {
	if (p1.first == p2.first) return p1.second - p2.second;
	return p1.first - p2.first;
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;
	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int max_elements[MAX_REL_SIZE];
			int min_elements[MAX_REL_SIZE];
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size,
			   relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

int add_relation (pair* relation, int n, int a, int b) {
	for (int i = 0; i < n; i++) {
		if ( relation[i].first == a && relation[i].second == b )
			return 0;
	}
	return 1;
}

int read_relation(pair* relation) {
	int n, a, b;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d" "%d", &a, &b);
		if ( add_relation(relation, i, a, b) == 1) {
			relation[i].first = a;
			relation[i].second = b;
		}
	}
	return n;
}

int is_reflexive(pair* relation, int size) {
	
	int is = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if ( relation[j].first == relation[j].second && relation[j].first == relation[i].first ) {
				is = 1;
			}
		}
		if (is == 0) {
			return 0;
		}
		is = 0;
	}

	return 1;
}

int is_irreflexive(pair* relation, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if ( relation[j].first == relation[j].second && relation[j].first == relation[i].first ) {
				return 0;
			}
		}
	}
	return 1;
}

int is_symmetric(pair* relation, int size) {
	int is = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (relation[j].first == relation[i].second && relation[j].second == relation[i].first) {
				is = 1;
			}
		}
		if ( is == 0 ) {
			return 0;
		}
		is = 0;
	}
	return 1;
}

int is_antisymmetric(pair* relation, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if ( relation[j].first == relation[i].second && relation[j].second == relation[i].first ) {
				if ( relation[j].first != relation[j].second ) {
					return 0;
				}
			}
		}
	}
	return 1;
}

int is_asymmetric(pair* relation, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if ( relation[i].first == relation[j].second && relation[i].second == relation[j].first ) {
				return 0;
			}
		}
	}
	return 1;
}

int is_transitive(pair* relation, int size) {
	int is = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (relation[i].second == relation[j].first) {
				is = 0;
				//printf("pierwsza %d R %d   druga %d R %d\n", relation[i].first, relation[i].second, relation[j].first, relation[j].second);
				for (int k = 0; k < size; k++) {
					if (relation[i].first == relation[k].first && relation[j].second == relation[k].second) {
						//printf("I %d R %d\n", relation[k].first, relation[k].second);
						is = 1;
					}
				}
				if (is == 0) {
					return 0;
				}

			}
		}
	}
	return 1;
}

int is_partial_order(pair* relation, int size) {
	if( (is_reflexive(relation, size) && is_antisymmetric(relation, size)) && is_transitive(relation, size) ) {
		return 1;
	}
	return 0;
}


int is_total_order(pair* relation, int size) {
	if (is_partial_order(relation, size) && is_connected(relation, size)) {
		return 1;
	}
	return 0;
}	

int is_connected(pair* relation, int size) {
	int d[MAX_REL_SIZE];
	int n = get_domain(relation, size, d);
	int is = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j) {
				is = 0;
				for (int k = 0; k < size; k++) {
					if ( ( relation[k].first == d[i] && relation[k].second==d[j] ) || ( relation[k].first == d[j] && relation[k].second==d[i] ) ) {
						is = 1;
						break;
					}
				}

				if (is == 0) {
					return 0;
				}

			}
		}
	}

	return 1;
}

int find_max_elements(pair* relation, int size, int* max_elements) {
	int index = 0, is = 1;

	for (int i = 0; i < size; i++) {
		is = 1;
		for (int j = 0; j < size; j++) {
			if ( relation[i].second == relation[j].first ) {
				if( relation[i].second < relation[j].second ) {
					is = 0;
					break;
				}
			}
		}

		if(is == 1) {
			for(int j = 0; j < index; j++) {
				if (relation[i].second == max_elements[j]) {
					is = 0;
					break;
				}
			}
			if(is == 1) {
				max_elements[index] = relation[i].second;
				index++;
			}
		}
	}
	qsort(max_elements, index, sizeof(int), cmpfunc);
	return index;
}

int find_min_elements(pair* relation, int size, int* min_elements) {
	int index = 0, is = 1;
// 3 4
// 5 3
	for (int i = 0; i < size; i++) {
		is = 1;
		for (int j = 0; j < size; j++) {
			if ( relation[i].first == relation[j].second ) {
				if( relation[i].first > relation[j].first ) {
					is = 0;
					break;
				}
			}
		}

		if(is == 1) {
			for(int j = 0; j < index; j++) {
				if (relation[i].first == min_elements[j]) {
					is = 0;
					break;
				}
			}
			if(is == 1) {
				min_elements[index] = relation[i].first;
				index++;
			}
		}
	}
	qsort(min_elements, index, sizeof(int), cmpfunc);
	return index;
}

int get_domain(pair* relation, int size, int* domain) {
	int index = 0, flag = 0;

	for (int i = 0; i < size; i++) {
		flag = 0;
		for (int j = 0; j < index; j++) {
			if( domain[j] == relation[i].first ) {
				flag = 1;
				break;
			}
		}
		if (!flag) {
			domain[index] = relation[i].first;
			index++;
		}

		flag = 0;

		for (int j = 0; j < index; j++) {
			if( domain[j] == relation[i].second ) {
				flag = 1;
				break;
			}
		}
		if (!flag) {
			domain[index] = relation[i].second;
			index++;
		}

	}
	qsort(domain, index, sizeof(int), cmpfunc);
	return index;
}

int composition (pair* A, int a, pair* B, int b, pair* C) {
	int index = 0, is = 0;
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b; j++) {
			if (A[i].second == B[j].first) { //  x A y and y B z => x C z
				is = 0;
				for (int k = 0; k < index; k++) {
					if( C[k].first == A[i].first && C[k].second == B[j].second ) { // relation exist
						is = 1;
						break;
					} 
				}
				if (is == 0) {
					C[index].first = A[i].first;
					C[index].second = B[j].second;
					index++;
				}
			}
		}
	}
	return index;
}

/*\
R
7
1 2
2 3
3 4
3 2
2 5
1 5
2 4

S
6
2 4
1 3
5 4
3 5
3 1
1 2
*/

/*
1 4
2 5
2 1
3 4
2 4
-1 4-
*/