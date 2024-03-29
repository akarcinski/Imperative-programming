#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

typedef union data_union {
	int int_data;
	char char_data;
	void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef  int (*CompareDataFp)(data_union, data_union);
typedef  int (*HashFp)(data_union, int);

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef struct hash_table {
	int size;
	int no_elements;
	ht_element **ht;
	DataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	exit(MEMORY_ALLOCATION_ERROR);
}

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
		 CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
	p_table->size = size;
	p_table->no_elements = 0;

	p_table->ht = (ht_element**)malloc( sizeof(ht_element*) * size );
	for (int i=0; i < size; i++) {
		p_table->ht[i] = NULL;
	}

	p_table->dump_data = dump_data;
	p_table->free_data = free_data;
	p_table->compare_data = compare_data;
	p_table->hash_function = hash_function;
	p_table->modify_data = modify_data;
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, int n) {
	ht_element * pointer = p_table->ht[n];
	while (pointer != NULL) {
		p_table->dump_data(pointer->data);
		pointer = pointer->next;
	}
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
	if (free_data != NULL) {
		free_data(to_delete->data);
	}
	free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
	for (int i = 0; i<p_table->size; i++) {
		ht_element * pointer = p_table->ht[i];
		while (pointer != NULL) {
			ht_element * to_delete = pointer;
			pointer = pointer->next;
			free_element(p_table->free_data, to_delete);
		}
	}
	free(p_table->ht);
}

// calculate hash function for integer k
int hash_base(int k, int size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (int)floor(size * (tmp - floor(tmp)));
}
void insert_element(hash_table *p_table, data_union *data);

void rehash(hash_table *p_table) {
	ht_element **old_ht = p_table->ht;
	p_table->ht = (ht_element**)safe_malloc(p_table->size * 2 * sizeof(ht_element *));
	for (int i = 0; i < p_table->size * 2; ++i) p_table->ht[i] = NULL;
	p_table->no_elements = 0;
	for (int i = 0; i < p_table->size; i++)
	{
		ht_element* element = old_ht[i];
		while (element)
		{
			ht_element* next_element = element->next;
			++p_table->no_elements;
			element->next = NULL;
			int index = p_table->hash_function(element->data, p_table->size * 2);
			ht_element* tmp = p_table->ht[index];
			element->next = tmp;
			p_table->ht[index] = element;
			element = next_element;
		}
	}
	p_table->size <<= 1;
	free(old_ht);
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data, int *first) { // zwraca wskaźnik do poprzednika i sprawdza czy nie jest pierwszy
	int key = p_table->hash_function(data, p_table->size);
	ht_element * pointer = p_table->ht[key];
	if ( p_table->compare_data(pointer->data, data) == 0 ) { // zwróć pierwszy element
		*first = 1;
		return pointer;
	}
	while (pointer->next != NULL) {
		if ( p_table->compare_data(pointer->next->data, data) == 0 ) {
			return pointer;
		}
		pointer = pointer->next;
	}
	return 0;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) { // zwraca wskaźnik do noda z daną wartością
	int key = p_table->hash_function(*data, p_table->size);
	ht_element * pointer = p_table->ht[key];
	while (pointer != NULL) {
		if ( p_table->compare_data(pointer->data, *data) == 0) {
			return pointer;
		}
		pointer = pointer -> next;
	}
	return pointer;
}

// insert element
void insert_element(hash_table *p_table, data_union *data) {
	int key = p_table->hash_function(*data, p_table->size);
	ht_element * pointer = p_table->ht[key];
	ht_element * new_ht = (ht_element*)malloc(sizeof(ht_element));
	//printf("nie dizala :( ");
	// sprawdzić powtórki
	ht_element * get;
	get = get_element(p_table, data);
	if (get == NULL) {
		//printf("jest nullem\n");
		new_ht->data = *data;
		new_ht->next = pointer;
		pointer = new_ht;
		p_table->ht[key] = new_ht;
	}
	else {
		//printf("NIE jest nullem\n");
		if(p_table->modify_data != NULL) {
			p_table->modify_data( &(get->data) );
		}
	}

	p_table->no_elements++;
	if ( (double)( p_table->no_elements / p_table->size ) > MAX_RATE) {
		rehash(p_table);
	}
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
	int key = p_table->hash_function(data, p_table->size);
	int first = 0;
	ht_element * prev = find_previous(p_table, data, &first);
	if (prev == NULL && first == 0) { // prowadzący jest zły i chce usuwać element który nie istnieje :((
		return;
	}
	ht_element * to_delete = prev->next;
	if (p_table->free_data != NULL) {
		p_table->free_data(to_delete->data);
	}
	if (first) {
		p_table->ht[key] = p_table->ht[key]->next;
		return;
	}
	ht_element * direction = prev->next->next;
	prev->next = direction;
	free_element(p_table->free_data, to_delete);
}

// type-specific definitions -------------------------------------------

// int element

int hash_int(data_union data, int size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
	printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
	return a.int_data - b.int_data;
}

// read int value and insert to the union
data_union create_data_int() {
	int value;
	scanf("%d", &value);
	union data_union new_union = (data_union)malloc( sizeof(int) );
	new_union.int_data = value;
	return new_union;
}

// char element --------------------------------------------------------

int hash_char(data_union data, int size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
	printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
	return (a.char_data - b.char_data);
}

// read char value and insert to the union
data_union create_data_char() {
	char value;
	scanf("%c", &value);
	if (value == ' ') {
		scanf("%c", &value);
	}
	union data_union new_union = (data_union)malloc( sizeof(char) );
	new_union.char_data = value;
	return new_union;
}

// Word element --------------------------------------------------------

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data) {
	printf("%s ", ((DataWord*)data.ptr_data)->word);
	printf("%d", ((DataWord*)data.ptr_data)->counter);
}

void free_word(data_union data) {
	free(((DataWord*)data.ptr_data)->word);
	free(data.ptr_data);
}

int cmp_word(data_union a, data_union b) {
	return strcmp ( ( (DataWord*)a.ptr_data )->word, ( (DataWord*)b.ptr_data )->word );
}

int hash_word(data_union data, int size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

// increase the counter
void modify_word(data_union *data) {
	( (DataWord*)(data->ptr_data) ) -> counter += 1;
}

// allocate DataWord structure and insert to the union
data_union create_data_word(char *value) {
	char* word = strdup(value);
	for(int i = 0; word[i]; i++) {
  				word[i] = tolower(word[i]);
	}

	DataWord* data_word = safe_malloc(sizeof(DataWord));
	data_word->word = word;
	data_word->counter = 1;

	data_union data;
	data.ptr_data = data_word;

	return data;
}

// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
	char* line = NULL;
	const char *delimiters = ".,?!:;-\t\n\b\r ";
	size_t bufs = BUFFER_SIZE;
	int read = 0;

	while ((read = getline(&line, &bufs, stream)) != -1)
	{
		char* token = NULL;
		token = strtok(line, delimiters);
		while (token != NULL)
		{
			data_union data = create_data_word(token);
			insert_element(p_table, &data);
			token = strtok(NULL, delimiters);
		}
	}
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		data = create_data();
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n, index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_int,NULL,
					cmp_int, hash_int, NULL);
			table_test(&table, n, create_data_int);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			free_table(&table);
			break;
		case 2: // test char table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_char,NULL,
			        cmp_char, hash_char, NULL);
			table_test(&table, n, create_data_char);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			free_table(&table);
			break;
		case 3: // read words from text, insert into table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, free_word,
			        cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%d\n", table.size);
			data = create_data_word(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			free_table(&table);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
