#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 64
#define MAX_CHARCOUNT_PER_LINE 128
#define MAKESTR(V) #V
#define TOSTR(V) MAKESTR(V)
#define PTR_LINE(fp) fprintf(fp,"%s\n\n","---")
// #define PTR_LINE(fp) 

int generate_next_prev(const char *ou, const char *in);

int main(int argc, char const *argv[])
{
    char in[128] = "f-outline.txt";
    char ous[128] = "next_prev.txt";
    int ret;

    if (argc == 5)
    {
        if (0 == strncmp("-i", argv[1], 2) && 0 == strncmp("-o", argv[3], 2))
        {
            strncpy(in, argv[2], sizeof(in));
            strncpy(ous, argv[4], sizeof(ous));
        }
        else
        {
            perror("invalid params");
            exit(EXIT_FAILURE);
        }
    }

    ret = generate_next_prev(ous, in);
    if (ret > 0)
        printf("success,list count=%d\n", ret);
    else
        puts("fail.\n");

    return 0;
}

#define CHECK_FILE_OPEN(fp, fname)                           \
    do                                                       \
    {                                                        \
        if (NULL == fp)                                      \
        {                                                    \
            fprintf(stderr, "open file: %s fail.\n", fname); \
            exit(EXIT_FAILURE);                              \
        }                                                    \
    } while (0)

int generate_next_prev(const char *ou, const char *in)
{
    FILE *fpi = fopen(in, "r");
    CHECK_FILE_OPEN(fpi, in);

    char buf[MAX_LINE][MAX_CHARCOUNT_PER_LINE];
    for (int i = 0; i < MAX_LINE; i++)
        memset(buf[i], 0, MAX_CHARCOUNT_PER_LINE);

    int n = -1;
    char *ret = NULL;

    do
    {
        ++n;
        ret = fgets(buf[n], MAX_CHARCOUNT_PER_LINE, fpi);
    } while (NULL != ret);

#ifdef DEBUG
    puts("DEBUG MODE:");
    for (int i = 0; i < n; i++)
        fputs(buf[i], stdout);
#endif // DEBUG

    fclose(fpi);
    clearerr(fpi);

    FILE *fpo = stdout;
    int ep = n - 1;

    if (n < 2)
        return n;

    PTR_LINE(fpo);
    // 第一项只有 next
    fprintf(fpo, "%s%s\n", "- [下一章：2.", buf[1] + 1);

    // 其他项有 prev 和 next
    for (int i = 1; i < ep; i++) // r: [1,ep-1] => [2,ep]
    {
        PTR_LINE(fpo);
        fprintf(fpo, "%s%d.%s", "- [上一章：",i, buf[i-1] + 1);
        fprintf(fpo, "%s%d.%s\n", "- [下一章：",i+2,buf[i+1] + 1);
    }

    PTR_LINE(fpo);
    // 最后一项只有 prev
    fprintf(fpo, "\n%s%d.%s\n", "- [上一章：", ep,buf[ep - 1] + 1);

    if (fpo != stdout)
        fclose(fpo);

    clearerr(fpo);

    return n;
}
