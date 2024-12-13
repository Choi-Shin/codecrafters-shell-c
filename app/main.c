#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
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
    int flag = 0;
    if (line[i] == '\'') {
      i++;
      while (line[i] != '\'') {
        if (line[i] == '\"') {
          flag = !flag;
        }
        if (!flag && line[i] == '\\') {
          i++;
        }
        args[j][k++] = line[i++];
      }
      i++;
    } else if (line[i] == '\"') {
      i++;
      while (line[i] != '\"') {
        if (line[i] == '\'') {
          flag = !flag;
        }
        if (!flag && line[i] == '\\') {
          i++;
        }
        args[j][k++] = line[i++];
      }
      i++;
    } else {
      while (line[i] != ' ' && line[i] != '\t' && line[i] != '\0') {
        if (line[i] == '\\') {
          i++;
        }
        args[j][k++] = line[i++];
      }
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
  }
  printf("\n");
}

char *find_file_in_path(const char *command) {
  char *path = getenv("PATH");
  char *token;
  static char filepath[1024];

  if (path == 0) {
    return 0;
  }
  char *path_copy = str_dup(path);
  token = str_tok(path_copy, ":");
  while (token != 0) {
    snprintf(filepath, sizeof(filepath), "%s/%s", token, command);
    if (access(filepath, X_OK) == 0) {
      return str_dup(filepath);
    }
    token = str_tok(0, ":");
  }
  return 0;
}


void type(char ** args) {
  char *builtins[] = {"echo", "exit", "type", "pwd", 0};
  char *command = args[1];
  for (int i=0; builtins[i] != 0; i++) {
    if (str_cmp(command, builtins[i]) == 0) {
      printf("%s is a shell builtin\n", command);
      return;
    }
  }
  char *fullpath = find_file_in_path(command);
  if (fullpath != 0) {
    printf("%s is %s\n", command, fullpath);
  } else {
    handle_invalid_commands(command);
  }
}

int execute_command(char **args) {
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(1);
  } else if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("execvp");
    } 
  } else {
    waitpid(pid, 0, 0);
  }
  return 0;
}

char *pwd() {
  char * current_path = getcwd(NULL, 0);
  printf("%s\n", current_path);
  return current_path;
}

// char *home() {
  
// }

char *get_homedir() {
  struct passwd *pw = getpwuid(getuid());
  return pw->pw_dir;
}

void cd(char *dest) {
  char *mem = dest;
  char path[100];
  int idx = 0;
  if (*dest == '~') {
    char *home = getenv("HOME");
    dest++;
    while (*home) {
      path[idx++] = *home++;
    }
  }
  while (*dest) {
    path[idx++] = *dest++;
  }
  path[idx] = '\0';
  if (chdir(path) != 0) {
    printf("cd: %s: No such file or directory\n", mem);
  }
}

int main() {
  char input[MAX_LINE];

  // Uncomment this block to pass the first stage
  while(1) {
    char **args = (char**) malloc(sizeof(char*) * MAX_ARGS);
    int num_args = 0;
    for (int i=0; i<MAX_ARGS; i++) {
      args[i] = (char*) malloc(sizeof(char) * MAX_LINE);
    }
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
    if (str_cmp(command, "echo") == 0) {
      echo(args);
    } else if (str_cmp(command, "type") == 0) {
      type(args);
    } else if (str_cmp(command, "pwd") == 0) {
      pwd();
    } else if (str_cmp(command, "cd") == 0) {
      cd(arg1);
    } else {
      char *path = find_file_in_path(command);
      if (access(path, X_OK) == 0) {
        int result = execute_command(args);
        if (result < 0) {
          perror("failed");
        }
      } else {
        handle_invalid_commands(command);
      }
    }
    for (int i = 0; i < num_args; i++) {
      free(args[i]);
    }
    free(args);
  }

  return 0;
}
