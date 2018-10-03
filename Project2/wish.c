#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>

/**
  Builtin commands function declarations:
 */
int wish_cd(char **args);
int wish_help();
int wish_exit(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (builtin_func[]) (char *) = {
  &wish_cd,
  &wish_help,
  &wish_exit
};
int wish_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}


char *wish_read_line(void)
{
  char *line = NULL;
  ssize_t bufsize = 64; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}


#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **wish_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char *tokens = malloc(bufsize * sizeof(char));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int wish_help()
{
    int i;
    printf("*****\n");
    printf("Wish is working \n");
    printf("This version of Wish is a very simplified one and can only do a few stuff. Below are all available commands on it\n");
    printf("\t-cd\n \t-ls\n  \t-help\n \t-exit\n\n");
    printf("If you are not used to running these commands, type the one you need with arguments if any, and hit enter.\n\n");
    printf("Use the man command for information on other programs.\n");
    printf("*****\n\n");
    return 1;
}

 
int wish_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "wish: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("wish");
        }
    }
    return 1;
}


int wish_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("wish");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("wish");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}


int wish_execute_arguments(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < wish_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return wish_launch(args);
}


void wish_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("wish> ");
    line = wish_read_line();
    args = wish_split_line(line);
    status = wish_execute_arguments(args);

    free(line);
    free(args);
  } while (status);
}


int main(int argc, char **argv)
{
    wish_loop();
	return 0;
}