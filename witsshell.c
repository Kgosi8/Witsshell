#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>




#define MAX_INPUT_SIZE 1024

void execute_command(char *command) {
    // Tokenize the command
    char *args[MAX_INPUT_SIZE];
    int arg_count = 0;

    char *token = strtok(command, " \n");
    while (token != NULL) {
        args[arg_count] = token;
        arg_count++;
        token = strtok(NULL, " \n");
    }
    args[arg_count] = NULL;

    if (arg_count > 0) {
        // Fork a child process
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            execvp(args[0], args);
            perror("witsshell");
            exit(1);
        } else if (pid < 0) {
            perror("witsshell");
        } else {
            // Parent process
            wait(NULL);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 1 && argc != 2) {
        fprintf(stderr, "Usage: %s [batch_file]\n", argv[0]);
        return 1;
    }

    FILE *input_file = stdin;
    int interactive_mode = 1;

    if (argc == 2) {
        input_file = fopen(argv[1], "r");
        if (input_file == NULL) {
            perror("witsshell");
            return 1;
        }
        interactive_mode = 0;
    }

    while (1) {
        if (interactive_mode) {
            printf("witsshell> ");
            fflush(stdout);
        }

        char input[MAX_INPUT_SIZE];
        if (fgets(input, sizeof(input), input_file) == NULL || strcmp(input, "exit\n") == 0) {
            break;
        }

        execute_command(input);
    }

    if (!interactive_mode) {
        fclose(input_file);
    }

    return 0;
}

