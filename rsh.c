#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_LINE_BUFFER_SIZE 1024
#define RSH_TOKEN_BUFFER_SIZE 64
#define RSH_TOKEN_DELIMITER " \t\r\n\a"

int rsh_cd(char **args);
int rsh_help(char **args);
int rsh_exit(char **args);

void rsh_loop(void);
char *rsh_read_line(void);
char **rsh_split_line(char *line);
int rsh_execute(char **args);
int rsh_launch(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
};

int (*builtin_func[])(char **) = {
    &rsh_cd,
    &rsh_help,
    &rsh_exit,
};

int rsh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

int main(int argc, char const *argv[])
{
    rsh_loop();
    return 0;
}

void rsh_loop(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        char *cwd = getcwd(NULL, 0);
        if (cwd)
        {
            printf("rsh:%s> ", cwd);
            free(cwd);
        }
        else
        {
            printf("rsh> ");
        }
        line = rsh_read_line();
        args = rsh_split_line(line);
        status = rsh_execute(args);

        free(line);
        free(args);
    } while (status);
}

char *rsh_read_line()
{
    int bufsize = READ_LINE_BUFFER_SIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer)
    {
        fprintf(stderr, "rsh: Buffer allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        c = getchar();

        // if c is EOF, null terminate and return
        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0'; // replacing EOF with null
            return buffer;
        }

        else
        {
            buffer[position] = c;
        }
        position++;

        // reallocate memory on exceeding buffer size
        if (position >= bufsize)
        {
            bufsize += READ_LINE_BUFFER_SIZE;
            buffer = realloc(buffer, bufsize);

            if (!buffer)
            {
                fprintf(stderr, "rsh: Buffer allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

// a shorter way to read a line using getline()
// char *lsh_read_line(void)
// {
//   char *line = NULL;
//   ssize_t bufsize = 0; // have getline allocate a buffer for us

//   if (getline(&line, &bufsize, stdin) == -1){
//     if (feof(stdin)) {
//       exit(EXIT_SUCCESS);  // We recieved an EOF
//     } else  {
//       perror("readline");
//       exit(EXIT_FAILURE);
//     }
//   }

//   return line;
// }

// splits the line into tokens
char **rsh_split_line(char *line)
{
    int bufsize = RSH_TOKEN_BUFFER_SIZE;
    int position = 0;
    char **tokens = malloc(sizeof(char *) * bufsize);
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "rsh: Buffer allocation error\n");
        exit(EXIT_FAILURE);
    }

    // searches for a delimiter, replaces it with '\0'
    // returns pointer to the first token
    token = strtok(line, RSH_TOKEN_DELIMITER);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += RSH_TOKEN_BUFFER_SIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));

            if (!tokens)
            {
                fprintf(stderr, "rsh: Buffer allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        // searches for the next delimiter
        token = strtok(NULL, RSH_TOKEN_DELIMITER);
    }
    tokens[position] = NULL;
    return tokens;
}

int rsh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    // clones the calling process
    pid = fork();

    if (pid == 0)
    {
        // child
        if (execvp(args[0], args) == -1)
        {
            perror("rsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // error in forking
        perror("rsh");
    }
    else
    {
        // parent process
        do
        {
            // wait for the child process to terminate using waitpid
            // loop continues until child process exits or is killed using a signal
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int rsh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "rsh: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0) // error
        {
            perror("rsh");
        }
    }
    return 1;
}

int rsh_help(char **args)
{
    printf("Rushil's RSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (int i = 0; i < rsh_num_builtins(); i++)
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int rsh_exit(char **args)
{
    return 0;
}

int rsh_execute(char **args)
{
    if (args[0] == NULL)
    {
        // empty command
        return 1;
    }

    for (int i = 0; i < rsh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }
    return rsh_launch(args);
}
