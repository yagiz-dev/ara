#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

bool ignore_case = false;
bool show_line_numbers = false;
bool invert_matches = false;
int  max_lines = 0;

void parse_arguments(int argc, char *argv[]);
void lowercase(char string[]);
void print_line(char content[], int line_number);

int main(int argc, char *argv[])
{
    parse_arguments(argc, argv);

    FILE *fptr;

    char *search = argv[optind];
    char *file = argv[optind + 1];

    fptr = fopen(file, "r");

    if (fptr == NULL) {
        printf("Unable to open file: %s", file);
        return 1;
    }

    char og_line_content[4096] = "";
    char line_content[4096] = "";

    int line_number = 0;
    int matched_line_count = 0;

    if (ignore_case == true) {
        lowercase(search);
    }    

    while (fgets(og_line_content, 4096, fptr)) {
        line_number++;

        strcpy(line_content, og_line_content); // @todo: keep a copy only when we manipulate file content

        if (ignore_case == true) {
            lowercase(line_content);
        }

        bool match = strstr(line_content, search) != NULL;
        if (invert_matches) {
            match = !match;
        }

        if (match) {
            print_line(og_line_content, line_number);

            matched_line_count++;
        }

        if (matched_line_count > 0 && matched_line_count == max_lines) {
            break;
        }
    }

    fclose(fptr);

    return 0;
}

void lowercase(char *string)
{
    for (int i = 0; string[i]; i++) {
        string[i] = tolower(string[i]);
    }
}

void parse_arguments(int argc, char *argv[])
{
    int opt;

    if (argc < 3)
    {
        printf("You must provide a search query and a source file.\nExample: ara \"error\" server.log");
        exit(1);
    }

    while ((opt = getopt(argc, argv, "invm:")) != -1)
    {
        switch (opt)
        {
            case 'i':
                ignore_case = true;
                break;

            case 'n':
                show_line_numbers = true;
                break;

            case 'v':
                invert_matches = true;
                break;

            case 'm':
                max_lines = atoi(optarg);
                break;

            default:
                printf("Unknown option: %c\n", opt);
                exit(1);
        }
    }

    if (argc - optind < 2) {
        printf("You must provide a search query and a source file. Example: ara \"error\" server.log\n");
        exit(1);
    }
}

void print_line(char *content, int line_number)
{
    if (show_line_numbers) {
        printf("%d: %s", line_number, content);
    } else {
        printf("%s", content);
    }
}