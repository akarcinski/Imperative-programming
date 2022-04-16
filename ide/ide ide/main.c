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

int is_letter(char c)
{
    int nc = (int)c;
    if(nc >= 48 && nc <= 57) // number
        return 1;
    if(nc >= 65 && nc <= 90) // big letter
        return 1;
    if(nc >= 97 && nc <= 122) // small letter
        return 1;
    if(nc == 95) // _
        return 1;

    return 0;
}

int find_idents()
{
    char arr[300][130];
    char line[128];
    int i,j,indeks = 0;
    int st = 0,len;
    int flag = 0,flag1;

    while(fgets(line, 128, stdin))
    {
        i = 0;
        while(line[i] != '\n')
        {
            flag = 0;
            if(st == 1)
            {
                if(line[i-1]=='*' && line[i] == '/')
                {
                    st = 0;
                }
            }
            if(st == 2)
            {
                if(line[i] == '\"'){
                    if(line[i-1] == '\\')
                    {
                        if(line[i-2] == '\\')
                        {
                            st = 0;
                            flag = 1;
                        }
                    }

                else
                {
                    st = 0;
                    flag = 1;
                }}
            }
            if(st == 3)
            {
                if(line[i] == '\'')
                {
                    if(line[i-1]=='\\') {
                        if (line[i - 2] == '\\') {
                            st = 0;
                            flag = 1;
                        }  }

                    else{
                        st = 0;
                        flag = 1;}
                }
            }
            if(st == 0)
            {
                if(line[i] =='/' && line[i+1] =='/')
                {
                    line[i+1]='\n';
                }
                if(line[i] =='/' && line[i+1] == '*')
                {
                    st = 1;
                    if(line[i+1] != '\n')i++;
                }
                if(line[i] == '"' && flag == 0)
                {
                    st = 2;
                }
                if(line[i]== '\'' && flag ==0)
                {
                    st = 3;
                }
                //ZACZYNAMY ZABAWE
                if(is_letter(line[i+1])==1){
                    if(is_letter(line[i])==1)
                    {}
                    else
                    {
                        j = i + 2;
                        len = 1;
                        while(((((int)line[j] >= 65 && (int)line[j] <= 90) || ((int)line[j] >= 97 && (int)line[j] <= 122)) || ((int)line[j] >= 48 && (int)line[j] <= 57)) || (int)line[j]==95)
                        {
                            len++;
                            j++;
                        }
                        j = i + 1;
                        //SPRAWDZAMY CZY JES KEYWORDEM
                        for(int k = 0; k <= 31; k++)
                        {
                            flag1 = 0;
                            for(int l = 0; l < len; l++)
                            {
                                if(keywords[k][l] != line[j+l])flag1=1;

                            }
                            if(flag1 == 0)
                            {
                                if(is_letter(keywords[k][len])==1)
                                {flag1 = 1;
                                }
                                else break;
                            }

                        }
                        if(flag1 == 1)
                        {
                            for(int k = 0; k < indeks; k++)
                            {
                                flag1 = 0;
                                for(int l = 0; l < len;l++)
                                {
                                    if(arr[k][l] != line[j+l])flag1=1;
                                }
                                if(flag1 == 0)
                                {
                                    if(is_letter(arr[k][len])==1)
                                    {flag1 = 1;
                                    }
                                    else break;
                                }

                            }
                        }
                        if(flag1 == 1)
                        {
                            for(int k = 0; k < len; k++)
                                arr[indeks][k] = line[j+k];
                            indeks ++;
                        }

                    }}
                //NA POCZATKU LINII
                if(i == 0 && (is_letter(line[i])==1))
                {
                    j = i + 1;
                    len = 1;
                    while(is_letter(line[j])==1)
                    {
                        len++;
                        j++;
                    }
                    j = i;
                    //SPRAWDZAMY CZY JES KEYWORDEM
                    for(int k = 0; k <= 31; k++)
                    {
                        flag1 = 0;
                        for(int l = 0; l < len;l++)
                        {
                            if(keywords[k][l] != line[j+l])flag1=1;

                        }
                        if(flag1 == 0)
                        {
                            if(is_letter(keywords[k][len])==1)
                            {
                                flag1 = 1;
                            }
                            else break;
                        }
                    }
                    if(flag1 == 1)
                    {
                        for(int k = 0; k < indeks; k++)
                        {
                            flag1 = 0;
                            for(int l = 0; l < len;l++)
                            {
                                if(arr[k][l] != line[j+l])flag1=1;
                            }
                            if(flag1 == 0)
                            {
                                if(is_letter(arr[k][len])==1)
                                {flag1 = 1;
                                }
                                else break;
                            }

                        }
                    }
                    if(flag1 == 1)
                    {
                        for(int k = 0; k < len; k++)
                            arr[indeks][k] = line[j+k];
                        indeks ++;
                    }
                }

            }
            i++;
        }
    }
    return indeks;

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
