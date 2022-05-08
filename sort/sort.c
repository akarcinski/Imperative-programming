#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *p1, const void *p2);

// Read data to Person array (till EOF)
int read_person_array(Person *persons);

// Print Person array
void print_person_array(Person *persons, int n);

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (int that order)
int cmp_lines(const void *l1, const void *l2);

// Read lines with students' data (as text)
int read_lines(char lines[][MAX_STR_LEN]);

// Print sorted lines
void print_lines(char lines[][MAX_STR_LEN], int n);

// -------------------------------------------------

int read_int() {
	char buf[MAX_STR_LEN];
	int n;
	fgets(buf, MAX_STR_LEN, stdin);
	sscanf(buf, "%d", &n);
	return n;
}

int main(void) {
	int to_do = read_int();
	int n;
	Person persons[MAX_PERSONS];
	char lines[MAX_PERSONS][MAX_STR_LEN];
	switch (to_do) {
		case 1:
			n = read_person_array(persons);
			qsort(persons, (size_t)n, sizeof(Person), cmp_person);
			print_person_array(persons, n);
			break;
		case 2:
			n = read_lines(lines);
			qsort(lines, (size_t) n, MAX_STR_LEN, cmp_lines);
			print_lines(lines, n);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
}

int cmp_person(const void *p1, const void *p2)
{
	Person *person1 = (Person *) p1;
	Person *person2 = (Person *) p2;

	int a = person1 -> age;
	int b = person2 -> age;

	if( a == b ) {
		if ( strcmp(person1 -> first_name, person2 -> first_name ) != 0 ) {
			return strcmp(person1 -> first_name, person2 -> first_name );
		}
		return strcmp(person1 -> last_name, person2 -> last_name );
	}
	return (a < b);
}

int read_person_array(Person *persons)
{
	Person new;
	
	char *buff;
	int i = 0;
	char line[MAX_STR_LEN];
	//char name[MAX_STR_LEN];
	//char surename[MAX_STR_LEN];

	while ( fgets(line, MAX_STR_LEN, stdin) ) {

		if (strlen(line) < 1) {
			break;
		}

		buff = strtok(line, " ");
		new.age = atoi(buff);

		buff = strtok(NULL, " ");
		strcpy(new.first_name, buff);

		buff = strtok(NULL, " ");
		strcpy(new.last_name, buff);

		persons[i] = new;
		i++;
	}

	return i;
}

void print_person_array(Person *persons, int n)
{
	for (int i = 0; i < n; i++) {
		printf("%d %s %s", persons[i].age, persons[i].first_name, persons[i].last_name);
	}
}

int cmp_lines(const void *l1, const void *l2)
{
	char *line1 = (char *) l1;
	char *line2 = (char *) l2;
    
    int a1, a2;
    char f_name1[MAX_STR_LEN], l_name1[MAX_STR_LEN], f_name2[MAX_STR_LEN], l_name2[MAX_STR_LEN];
	char buff[MAX_STR_LEN];
	char *b;
	
	strcpy(buff, line1);

    b = strtok( buff, " " );
    a1 = atoi( b );
    b = strtok( NULL, " " );
    strcpy( f_name1, b );
    b = strtok( NULL, " " );
    strcpy( l_name1, b );


	strcpy(buff, line2);

    b = strtok( buff, " " );
    a2 = atoi( b );
    b = strtok( NULL, " " );
    strcpy( f_name2, b );
    b = strtok( NULL, " " );
    strcpy( l_name2, b );
    
	char last1, last2;
	last1 = f_name1[strlen(f_name1)-1];
	last2 = f_name2[strlen(f_name2)-1];

	if (last1 == 'a' && last2 == 'a') { // obie osoby są kobietami
		return (a1 > a2);
	}
	else if (last1 == 'a' && last2 != 'a') { 
		return 0;
	}
	else if (last1 != 'a' && last2 == 'a') {
		return 1;
	}
	else {
		//printf("%s %s\n", l_name1, l_name2);
		return ( strcmp(l_name1, l_name2) );
	}
	//printf("%c %c\n", last1, last2);
    //printf(" [ %d %d ] \n", a1, a2);
	return 0;
}

int read_lines(char lines[][MAX_STR_LEN])
{
	Person new;
	
	char *buff;
	int i = 0;
	char line[MAX_STR_LEN];
	//char name[MAX_STR_LEN];
	//char surename[MAX_STR_LEN];

	while ( fgets(line, MAX_STR_LEN, stdin) ) {

		if (strlen(line) <= 1) {
			break;
		}

		strcpy(lines[i], line);
		i++;
	}
	return i;
}

void print_lines(char lines[][MAX_STR_LEN], int n)
{
    int j = 0;
    for (int i = 0; i < n; i++) {
        j = 0;
        while ( lines[i][j] != '\0' ) {
            printf("%c", lines[i][j]);
            j++;
        }
    }
}