#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int is_in(char word[])
{

}

int find_idents()
{
    int res = 0;
    int com = 0; // komentarz
    int text = 0; // text

    char line[128];
    char word[64];

    while(fgets (line, 128, stdin))
    {
        for(int i=1; i<128; i++)
        {
            if( (int)line[i] == 10 )
                break;

            if( line[i - 1] == '/' && line[i] == '*' && text == 0)
                com = 1;
            if( line[i - 1] == '*' && line[i] == '/' && text == 0)
                com = 0;

            // dostajemy komentarz liniowy wiêc nie mamy co wczytywaæ
            if( line[i - 1] == '/' && line[i] == '/' && com == 0 && text == 0)
                break;

            // zaczynamy stringa
            if( (line[i] == '\"' || line[i] == '\'') && com == 0 && text == 0)
                text = 1;
            // " ' w stringu
            if( (line[i - 1] == '\\' || line[i] == '\"') && com == 0 && text == 1)
                continue;
            else if( (line[i - 1] == '\\' || line[i] == '\'') && com == 0 && text == 1)
                continue;

            // konczymy stringa (  nie ma w nim \" ani \'  )
            else if( (line[i] == '\"' || line[i] == '\'') && com == 0 && text == 1)
                text = 0;
        }
    }
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(tab[a], tab[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

