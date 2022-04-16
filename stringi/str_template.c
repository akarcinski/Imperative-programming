        #include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_DIGRAMS] = { 0 };

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int comp (const void * elem1, const void * elem2)
{
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return -1;
    if (f < s) return  1;
    return 0;
}
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb; // sort alphabetically if counts equal
	return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stdin

void wc(int *nl, int *nw, int *nc)
{
    char line[128];
    int l = 0, w = 0, c = 0;
    char last;
    for(int i = 0; i<128; i++)
            line[i] = 0;

    while(fgets (line, 128, stdin))
    {
// line
        l++;
// word
        for(int i=0; i<128; i++)
        {
            if(line[i] > 32)
            {
                last = line[i];

                for(int j = i + 1; j < 128; j++)
                {
                    if((last > 32) && (line[j] <= 32)) // zmiana ze znaku na odstep
                    {
                        //printf("%c %c ", last, line[j]);
                        w++;
                    }

                    last = line[j];
                }

                break;
            }
        }
        //printf("%d ", w);

// character
        for(int i=0; i<128; i++)
        {
            if(line[i] >= 32)
                c++;
            if(line[i] == 9)
                c += 1;
        }
        for(int i = 0; i<128; i++)
            line[i] = 0;
    }

    *nl = l;
    *nc = c+l;
    *nw = w;
    //printf("%d ", l);
    //for(int i=0; i<10; i++)
    //    printf("%s", line[i]);
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stdin. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt)
{
    int arr[128];
    int arr_copy[95];
    for(int i=0; i<128; i++)
        arr[i] = 0;

    char line[128];
    while(fgets (line, 128, stdin))
    {
        for(int i=0; i<128; i++)
        {
            if((int)line[i] == 10)
                break;
            arr[ (int)line[i] ]++;
        }
        for(int i=0; i<128; i++)
            line[i] = 0;
    }
    for(int i=0, j=33; j < 128; i++, j++)
    {
        arr_copy[i] = arr[j];
    }

    qsort(arr_copy, 95, sizeof(*arr_copy), comp);

    int biggest = arr_copy[char_no - 1];
    for(int i=33; i< 128; i++)
    {
        if(biggest == arr[i])
        {
            *n_char = (char)i;
            *cnt = biggest;
            break;
        }

    }
    /*
    for(int i = 0; i<95; i++)
    {
        printf("%d ", arr_copy[i]);
    }
    printf("\n\n");
    for(int i = 0; i<128; i++)
    {
        printf("%d %d\n", i, arr[i]);
    }
    */
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stdin. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[])
{
    char line[128];
    int arr[16512];
    int arr_copy[11998];

    for(int i = 0; i<128; i++)
            line[i] = 0;
    for(int i=0; i<16255; i++) arr[i] = 0;
    for(int i=0; i<11998; i++) arr_copy[i] = 0;
    int res;
    while(fgets (line, 128, stdin))
    {
        for(int i=1; i<128; i++)
        {
            if((int)line[i] == 10)
                break;
            res = (int)line[i - 1] * 128 + (int)line[i];
            //printf("%d %d %d\n", line[i], res, arr[res]);
            arr[ res ]++;
            //printf("%d %d %d\n", line[i], res, arr[res]);
        }

        for(int i = 0; i<128; i++)
            line[i] = 0;
    }
    // !! = 4257
    // ~~ = 16254
    for(int i = 4257, j=0; i<16255; i++, j++)
    {
        arr_copy[j] = arr[i];
        //printf("%d %d %d\n", i, j, arr[i]);
    }
    qsort(arr_copy, 11998, sizeof(*arr_copy), comp);

    int biggest = arr_copy[digram_no - 1];
    //printf("%d", biggest);

    //for(int i=0; i<20; i++)
    //    printf("%d\n", arr_copy[i]);

    for(int i=4256; i< 16255; i++)
    {
        if(biggest == arr[i])
        {
            digram[0] = i / 128;
            digram[1] = i % 128;
            digram[2] = biggest;
            break;
        }
    }
}

// Count block and line comments in the text read from stdin. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter)
{
    char line[128];
    char last;
    int flag = 0, b=0, l=0;
    for(int i = 0; i<128; i++)
            line[i] = 0;

    while(fgets (line, 128, stdin))
    {
        last = line[0];
        for(int i=1; i<128; i++)
        {
            //printf("flag = %d ", flag);
            if((last == '/' && line[i] == '*') && (flag == 0))
            {
                i++;
                flag = 1;
                b++;
                //printf("b\n");
            }
            else if(last == '*' && line[i] == '/')
            {
                flag = 0;
            }
            else if((last == '/' && line[i] == '/') && (flag == 0))
            {
                l++;
                //printf("l\n");
                break;
            }
            last = line[i];
        }
        for(int i = 0; i<128; i++)
            line[i] = 0;
    }
    *line_comment_counter  = l;
    *block_comment_counter = b;
}

#define MAX_LINE 128

int read_line() {
	char line[MAX_LINE];
	int n;

	fgets (line, MAX_LINE, stdin); // to get the whole line
	sscanf (line, "%d", &n);
	return n;
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int digram[3];

	to_do = read_line();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_line();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // digram_count()
			char_no = read_line();
			digram_count(char_no, digram);
			printf("%c%c %d\n", digram[0], digram[1], digram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
/*
12 | int main() {
22|   printf ("Hello\n");
12|   return 0;
1|}

int main() {
    printf ("Hello\n");
    return 0;
}

    // // //
    // /*
    /* // // /* */
