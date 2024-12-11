#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "my_string.h"
#define MAX_ARGS 10
#define MAX_LINE 100

void handle_invalid_commands(char *command) {
  printf("%s: not found\n", command);
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
void echo(char** args) {
  int i = 1;
  while (args[i] != 0) {
    printf("%s", args[i]);
    i++;
    if (args[i] != 0) {
      printf(" ");
    }
  }
  printf("\n");
}

char *find_file_in_path(const char *command) {
  char *path = getenv("PATH");
  char *token;
  char filepath[1024];

  if (path == 0) {
    return 0;
  }
  token = str_tok(path, ":");
  while (token != 0) {
    snprintf(filepath, sizeof(filepath), "%s/%s", token, command);
    if (access(filepath, F_OK) == 0) {
      return str_dup(filepath);
    }
    token = str_tok(0, ":");
  }
  return 0;
}


void type(char ** args) {
  char *command = args[1];
  char *fullpath = find_file_in_path(command);
  if (fullpath != 0) {
    printf("%s is %s\n", command, fullpath);
  } else {
    handle_invalid_commands(command);
  }
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
    if (str_cmp(input, "exit 0") == 0) {
      return 0;
    }
    num_args = split_command(input, args);
    char* command = args[0];
    char* arg1 = args[1];
    if (str_cmp(args[0], "echo") == 0) {
      echo(args);
    } else if (str_cmp(args[0], "type") == 0) {
      type(args);
    } else {
      handle_invalid_commands(command);
    }
    
  }
  for (int i = 0; i < num_args; i++) {
    free(args[i]);
  }
  free(args);
  return 0;
}
