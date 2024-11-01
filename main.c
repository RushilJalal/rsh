#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define READ_LINE_BUFFER_SIZE 1024
#define RSH_TOKEN_BUFFER_SIZE 64
#define RSH_TOKEN_DELIMITER " \t\r\n\a"

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
        printf("> ");
        line = rsh_read_line();
        args = split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    } while (status);
}

char *rsh_read_line(void)
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
            buffer[position] == '\0'; // replacing EOF with null
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
            bufsize = realloc(buffer, bufsize);

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
            realloc(tokens, bufsize * sizeof(char *));

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

int rsh_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();

    if(pid == 0){
        //child
        
    }
}
