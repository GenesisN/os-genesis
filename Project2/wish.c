#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>

// Builtin commands function declarations:
int wish_cd(char **args);
int wish_help();
int wish_batch(char *filename);
int wish_exit(char **args);
int wish_path(char **args);
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "path"
};
int (*builtin_func[]) (char **) = {
  &wish_cd,
  &wish_help,
  &wish_exit,
  &wish_path
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
  char **tokens = malloc(bufsize * sizeof(char));
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
    printf("\t-cd\n \t-help\n \t-exit\n \t-batchmode\n \t-path\n\n");
    printf("If you are not used to running these commands, type the one you need with arguments if any, and hit enter.\n\n");
    printf("Use the man command for information on other programs.\n");
    printf("*****\n\n");
    return 1;
}
int wish_exit(char **args)
{
    exit(0);
    return 0;
}
int status = 1;
int wish_path(char **args)
{
    if (args[1] == NULL){
        status =0;
    }
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
  if (status ==1){
      return wish_launch(args);
  }
  return 1;
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

int wish_batch(char *filename)
{
    FILE* file_pointer = fopen(filename, "r");
    FILE* output = fopen("output.txt", "w");

    if(!file_pointer)
        {
            printf("\033[1;31m");
            perror("cannot open file\n");
            printf("\033[0m");
            return EXIT_FAILURE;
        }

    char *line;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file_pointer)) != -1)
    {
        char **args;
        int status;
       
        line[strcspn(line, "\n")] = 0;
        args = wish_split_line(line);
        status = wish_execute_arguments(args);
    }
    fclose(file_pointer);
    fclose(output);
    return 0;
}
int main(int argc, char **argv)
{
  if (argc == 1){
    wish_loop();
  }
  if (argc == 2){
    wish_batch(argv[1]);
  }
    return 0;
}

