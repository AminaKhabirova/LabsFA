#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int GetFilenames(int argc, char** argv, char* files[]) {
    if (argc != 3) {
        return 1;
    }
    char input_path[PATH_MAX];
    char output_path[PATH_MAX];
    int not_same = 0;
    realpath(argv[1], input_path);
    realpath(argv[2], output_path);
    for (int i = 0; input_path[i] && output_path[i]; i++) {
        if (input_path[i] != output_path[i]) not_same = 1;
    }
    if (!not_same) return 1;
    files[0] = argv[1];
    files[1] = argv[2];

    return 0;
}

void PrintIndent(FILE *output, int level) {
    for (int i = 0; i < level; ++i) {
        fprintf(output, "|");
    }
}

void ParseExpression(FILE *input, FILE *output, int level) {
    int in_expression = 0;
    int ch;

    while ((ch = fgetc(input)) != EOF) {
    	if (ch == '\r') continue;
    	if (ch == '\n') {
    		fprintf(output, "\n\n");
    	} else if (ch == '(') {
            if (in_expression) {
                fprintf(output, "\n");
            }
            in_expression = 1;
            level++;
            ParseExpression(input, output, level);
        	level--;
        } else if (ch == ')') {
            return;
        } else if (ch == ',') {
            fprintf(output, "\n");
            PrintIndent(output, level);
        } else if (ch != ' ') {
            if (!in_expression) {
                PrintIndent(output, level);
                in_expression = 1;
            }
            fprintf(output, "%c", ch);
        }
    }
}

int main(int argc, char **argv) {
    char* files[2];

    if (GetFilenames(argc, argv, files)) {
        printf("Incorrect option\n");
        return 1;
    }

    FILE *input;
    if ((input = fopen(files[0], "r")) == NULL) {
        printf("File opening error\n");
        return 2;
    }

    FILE *output;
    if ((output = fopen(files[1], "w")) == NULL) {
        printf("File opening error\n");
        fclose(input);
        return 2;
    }

    ParseExpression(input, output, 0);

    fclose(input);
    fclose(output);

    return 0;
}