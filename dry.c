#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "RLE.h"

#define MODE_PACK 0
#define MODE_UNPACK 1
#define ALGORITHM_RLE 0
#define ALGORITHM_PB 2

#define NO_OPTION_ARGUMENT { printf("No argument for option {%s}!\n", argv[argi]); return 0; }
#define BAD_OPTION_ARGUMENT { printf("Bad argument for option {%s}!\n", argv[argi - 1]); return 0; }

int show_usage(const char *name)
{
    printf("usage: %s <source> [<options>]\n", name);
    printf("options:\n");
    printf("  -p            : pack source file to output file\n");
    printf("  -u            : unpack source file to output file\n");
    printf("  -RLE          : use RLE algorithm\n");
    printf("  -PB           : use PackBits algorithm\n");
    printf("  -s <size>     : set size of i/o buffers\n");
    printf("  -o <filename> : set name of output file\n");
    printf("default: %s %%source%% -p -PB -s 1024 -o %%source%%.dried\n");
}

typedef int (*compress_function_t)(byte_buffer_t *src, byte_buffer_t *dst);

int try_to_parse_long(const char *str, long *result)
{
    *result = 0;
    while(*str)
    {
        int d = *str - '0';
        if(isdigit(*str) && (*result < (LONG_MAX - d) / 10))
            *result = 10 * (*result) + d;
        else
            return 1;
        str++;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        show_usage(argv[0]);
        return 0;
    }

    char *in_file_name = argv[1];
    char *out_file_name = 0;
    byte_t mode = 0;
    byte_t algorithm = 2;
    long buffers_size = 1024;
    compress_function_t functions[] = {
        RLE_encode,
        RLE_decode,
        RLE_pack_bits,
        RLE_unpack_bits
    };
    
    int argi = 2;
    while(argi < argc)
    {
        if(!strcmp(argv[argi], "-p"))
        {
            mode = MODE_PACK;
        }
        else if (!strcmp(argv[argi], "-u"))
        {
            mode = MODE_UNPACK;
        }
        else if (!strcmp(argv[argi], "-RLE"))
        {
            algorithm = ALGORITHM_RLE;
        }
        else if (!strcmp(argv[argi], "-PB"))
        {
            algorithm = ALGORITHM_PB;
        }
        else if (!strcmp(argv[argi], "-s"))
        {
            argi++;
            if(argi < argc)
            {
                if(try_to_parse_long(argv[argi], &buffers_size))
                    BAD_OPTION_ARGUMENT
            }
            else NO_OPTION_ARGUMENT
        }
        else if (!strcmp(argv[argi], "-o"))
        {
            argi++;
            if(argi < argc)
            {
                out_file_name = strdup(argv[argi]);
            }
            else NO_OPTION_ARGUMENT
        }
        else
        {
            printf("Unexpected option {%s}!\n", argv[argi]);
            return 0;
        }
        argi++;
    }

    if(!out_file_name)
    {
        out_file_name = (char *)calloc(strlen(in_file_name) + 7, sizeof(char));
        strcat(out_file_name, in_file_name);
        strcat(out_file_name, ".dried");
    }

    byte_buffer_t input = {};
    byte_buffer_t output = {};
    if(byte_buffer_init(&input, in_file_name, buffers_size, "rb"))
    {
        printf("Incorrect input file!");
        return 1;
    }
    if(byte_buffer_init(&output, out_file_name, buffers_size, "wb"))
    {
        printf("Incorrect output file!");
        return 1;
    }

    int error = functions[algorithm + mode](&input, &output);

    free(out_file_name);
    byte_buffer_release(&input);
    byte_buffer_release(&output);
    if(error)
        remove(out_file_name);
    return error;
}