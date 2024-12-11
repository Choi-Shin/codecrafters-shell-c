#include <stdio.h>
#include <stdlib.h>
#include "my_string.h"
#define MAX_ARGS 10
#define MAX_LINE 100

void handle_invalid_commands(char *command) {
  printf("%s: command not found\n", command);
}

int split_command(const char *line, char **args) {
  int i = 0, j = 0, k = 0;
  while (line[i] != '\0' && i < MAX_LINE) {
    while (line[i] == ' ' || line[i] == '\t') {
      i++;
    }
    while (line[i] != ' ' && line[i] != '\t' && line[i] != '\0') {
      args[j][k++] = line[i++];
    }
    args[j][k] = '\0';
    j++;
    k = 0;
  }
  args[j] = 0;
  return j;
}

int main() {
  char input[MAX_LINE];
  char **args = (char**) malloc(sizeof(char*) * MAX_ARGS);
  int num_args = 0;
  for (int i=0; i<MAX_ARGS; i++) {
    args[i] = (char*) malloc(sizeof(char*) * MAX_LINE);
  }
  // Uncomment this block to pass the first stage
  while(1) {
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    fgets(input, MAX_LINE, stdin);
    remove_newline(input);
    num_args = split_command(input, args);
    char* command = args[0];
    handle_invalid_commands(command);
  }

  for (int i = 0; i < num_args; i++) {
    free(args[i]);
  }
  free(args);
  return 0;
}
