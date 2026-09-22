#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

struct Options {
    bool ignore_case;
    bool show_line_numbers;
    bool invert_matches;
    int max_lines;
};

void read_file(char search[], char file[], const struct Options *options);
void parse_arguments(int argc, char *argv[], struct Options *options);
void print_line(char content[], int line_number, const struct Options *options);
void lowercase(char string[]);

int main(int argc, char *argv[])
{
    struct Options options;

    parse_arguments(argc, argv, &options);

    char *search = argv[optind];
    char *file = argv[optind + 1];

    read_file(search, file, &options);

    return 0;
}

void read_file(char search[], char file[], const struct Options *options)
{
    FILE *fptr;

    fptr = fopen(file, "r");

    if (fptr == NULL) {
        printf("Unable to open file: %s\n", file);
        exit(1);
    }

    char original_line[4096];
    char modified_line[4096];
    char *line;

    int line_number = 0;
    int matched_line_count = 0;

    if (options->ignore_case == true) {
        lowercase(search);
    }    

    while (fgets(original_line, 4096, fptr)) {
        line_number++;

        line = original_line;

        if (options->ignore_case) {
            strcpy(modified_line, original_line);
            lowercase(modified_line);

            line = modified_line;
        }

        bool match = strstr(line, search) != NULL;
        if (options->invert_matches) {
            match = !match;
        }

        if (match) {
            print_line(original_line, line_number, options);

            matched_line_count++;
        }

        if (options->max_lines >= 0 &&
            matched_line_count >= options->max_lines) {
            break;
        }
    }

    fclose(fptr);
}

void parse_arguments(int argc, char *argv[], struct Options *options)
{
    int opt;

    options->ignore_case = false;
    options->invert_matches = false;
    options->show_line_numbers = false;
    options->max_lines = -1;

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
                options->ignore_case = true;
                break;

            case 'n':
                options->show_line_numbers = true;
                break;

            case 'v':
                options->invert_matches = true;
                break;

            case 'm':
                options->max_lines = atoi(optarg);
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

void print_line(char *content, int line_number, const struct Options *options)
{
    if (options->show_line_numbers) {
        printf("%d: %s", line_number, content);
    } else {
        printf("%s", content);
    }
}

void lowercase(char *string)
{
    for (int i = 0; string[i]; i++) {
        string[i] = tolower(string[i]);
    }
}