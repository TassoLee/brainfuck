#include <stdio.h>

static void jumpforward(FILE *fp)
{
    char c;
    int count = 1;
    
    while ((c=fgetc(fp)) != EOF)
    {
        if (c == '[') ++count;
        if ((c == ']') && (--count == 0)) break;
    }
}

static void jumpbackward(FILE *fp)
{
    char c;
    int count = 1;
    
    while ((fseek(fp, -2, SEEK_CUR) == 0) && ((c=fgetc(fp)) != EOF))
    {
        if (c == ']') ++count;
        if ((c == '[') && (--count == 0)) break;
    }
}

int main(int argc, char *argv[])
{
    enum { CELL_SIZE = 30000 };

    FILE *fp;
    char c;
    char cell[CELL_SIZE] = {0};
    char *p = cell;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        return (-1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Can't open file [%s]\n", argv[1]);
        return (-1);
    }

    while ((c=fgetc(fp)) != EOF)
    {
        switch(c)
        {
            case '>':
                if (++p-cell == CELL_SIZE) p -= CELL_SIZE;
                break;
            case '<':
                if (--p < cell) p += CELL_SIZE;
                break;
            case '+':
                ++(*p);
                break;
            case '-':
                --(*p);
                break;
            case '.':
                putchar(*p);
                break;
            case ',':
                *p = getchar();
                break;
            case '[':
                if (*p == 0) jumpforward(fp);
                break;
            case ']':
                if (*p != 0) jumpbackward(fp);
                break;
            default:
                continue;
        }
    }

    return fclose(fp);
}
