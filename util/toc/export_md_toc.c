#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define MAX_LINE 64
#define MAX_CHARCOUNT_PER_LINE 1024
#define MAKESTR(V) #V
#define TOSTR(V) MAKESTR(V)
#define FILE_PATH_MAXLEN 256
#define MAX_HEAD_COUNT_PER_FILE 64
#define MAX_MDFILE_COUNT 64

#ifdef DEBUG
#define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

#define INIT_CHAR_ARRAY(array) memset(array, 0, sizeof(array))
#define PRINT_CHAR_ARRAY(array) printf(#array ": %s\n", array)
#define DEBUG_PRINT_CHAR_ARRAY(array) DEBUG_PRINT(#array ": %s\n", array)

#define CHECK_FILE_OPEN(fp, fname)                           \
    do                                                       \
    {                                                        \
        if (NULL == fp)                                      \
        {                                                    \
            fprintf(stderr, "open file: %s fail.\n", fname); \
            exit(EXIT_FAILURE);                              \
        }                                                    \
    } while (0)

int export_md_toc(const char *fname, FILE *ous_fp);
int md_toc(const char mdlist[][FILE_PATH_MAXLEN], int n, const char *mdf_dir, const char *outf);

int main(int argc, char const *argv[])
{

    // default value
    char mdsrc_f_dir[FILE_PATH_MAXLEN] = "G:/itstudy-doc/gnumake-zh-docs/docs";
    const char mdsrc_fname_list[FILE_PATH_MAXLEN] = "md_flist.txt";
    char out_file[FILE_PATH_MAXLEN] = "md_toc.md";

    char md_flist[MAX_MDFILE_COUNT][FILE_PATH_MAXLEN];
    int md_len = 0;
    int param_check_flag = 0;

    INIT_CHAR_ARRAY(md_flist);

    printf("enter program: %s\n", argv[0]);

#ifdef DEBUG
    printf("param count:%d\n", argc);
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]=%s\n", i, argv[i]);
    }
#endif

    // parse command params
    if (argc == 7)
    {
        if (0 == strncmp("-b", argv[1], 2) && 0 == strncmp("-i", argv[3], 2) 
            && 0 == strncmp("-o", argv[5], 2))
        {
            param_check_flag = 1;
            strncpy(mdsrc_f_dir, argv[2], sizeof(mdsrc_f_dir) - 1);
            strncpy(mdsrc_fname_list, argv[4], sizeof(mdsrc_fname_list) - 1);
            strncpy(out_file, argv[6], sizeof(out_file) - 1);

#ifdef DEBUG
            PRINT_CHAR_ARRAY(mdsrc_f_dir);
            PRINT_CHAR_ARRAY(mdsrc_fname_list);
            PRINT_CHAR_ARRAY(out_file);
#endif
        }
        else
        {
            perror("invalid params");
            exit(EXIT_FAILURE);
        }
    }

    if (!param_check_flag)
    {
        puts("use default value");
        PRINT_CHAR_ARRAY(mdsrc_f_dir);
        PRINT_CHAR_ARRAY(mdsrc_fname_list);
        PRINT_CHAR_ARRAY(out_file);
        puts("-----------\n\n");
    }

    FILE *fpi = fopen(mdsrc_fname_list, "r");
    CHECK_FILE_OPEN(fpi, mdsrc_fname_list);

    // read all .md file name into md_flist
    char *ret = NULL;
    int len;

    do
    {
        ret = fgets(md_flist[md_len], MAX_CHARCOUNT_PER_LINE, fpi);

        if (NULL == ret)
            break;

        if (md_flist[md_len][0] != 0 && md_flist[md_len][0] != '\n')
        {
            len = strnlen(md_flist[md_len], FILE_PATH_MAXLEN);

            if (md_flist[md_len][len - 1] == '\n')
                md_flist[md_len][len - 1] = 0;

            ++md_len;
        }
    } while (1);

    fclose(fpi);
    clearerr(fpi);

#ifdef DEBUG
    printf("read file:%s finish.\n", fpi);
    printf("real count:%d\n", md_len);
    for (int i = 0; i < md_len; i++)
        puts(md_flist[i]);
#endif

    int n = 0;
    n = md_toc(md_flist, md_len, mdsrc_f_dir, out_file);
    printf("export finish, total md file count: %d,real done with count: %d, left count:%d\n",md_len, n,md_len - n);

#ifdef DEBUG
    getchar();
#endif

    return 0;
}

int md_toc(const char mdlist[][FILE_PATH_MAXLEN], int n, const char *mdf_dir, const char *outf)
{
    int rn = 0;
    char f_all_name[FILE_PATH_MAXLEN];
    INIT_CHAR_ARRAY(f_all_name);

    FILE *ous_fp = fopen(outf, "a+");
    CHECK_FILE_OPEN(ous_fp, outf);

    fputs("# add h1\n\n",ous_fp);

    for (int i = 0; i < n; i++)
    {
        sprintf(f_all_name, "%s/%s", mdf_dir, mdlist[i]);
        rn += export_md_toc(f_all_name, ous_fp);
    }

    fputc('\n',ous_fp);

    fflush(ous_fp);
    fclose(ous_fp);
    clearerr(ous_fp);

    return rn;
}

int export_md_toc(const char *fname, FILE *ous_fp)
{
    char buf[MAX_CHARCOUNT_PER_LINE];
    INIT_CHAR_ARRAY(buf);

    FILE *in_fp = fopen(fname, "r");

    if (NULL == in_fp)
    {
        fprintf(stderr, "function:%s errmsg: open file: %s fail.\n", __FUNCTION__, fname);
        return 0;
    }

    char *ret = NULL;
    int not_codeblock_flag = 1;
    int len;

    do
    {
        ret = fgets(buf, sizeof(buf), in_fp);
        if (NULL == ret)
            break;
        len = strnlen(buf,sizeof(buf));
        if(len >= 3 && NULL != strstr(buf,"```"))
            not_codeblock_flag = (not_codeblock_flag + 1) % 2;
         
        // head title line
        if (buf[0] == '#' && not_codeblock_flag)
        {
            fputc('#',ous_fp);
            fputs(buf,ous_fp);
            fputc('\n',ous_fp);
        }

    } while (1);

    fflush(ous_fp);
    fclose(in_fp);
    clearerr(in_fp);

    return 1;
}