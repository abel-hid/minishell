#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

void _ls()
{
     DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s ", entry->d_name);
    }
    closedir(dir);

}
int ft_strcmp(const char *s1, const char *s2)
{
    int i;

    i = 0;
    while (s1[i] && (s1[i] == s2[i]))
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
char **ft_split(char *str, char c)
{
    char **tab;
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    tab = malloc(sizeof(char *) * 100);
    while (str[i])
    {
        k = 0;
        tab[j] = malloc(sizeof(char) * 100);
        while (str[i] && str[i] != c)
        {
            tab[j][k] = str[i];
            i++;
            k++;
        }
        tab[j][k] = '\0';
        j++;
        if (str[i])
            i++;
    }
    tab[j] = NULL;
    return (tab);
}

void _env()
{
    extern char **environ;
    int i;

    i = 0;
    while (environ[i])
    {
        printf("%s\n", environ[i]);
        i++;
    }
}

void _pwd()
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("getcwd() error");

}
void echo (char *args)
{
    int i;

    i = 0;
    while (args[i])
    {
        printf("%c", args[i]);
        i++;
    }
    printf("\n");
}

void _builtin_handler(char *str)
{
    char **args;
    int i;

    i = 0;
    while(str[i])
    {
        while(str[i] == ' ')
            i++;
        if (str[i] == 'l' && str[i + 1] == 's')
            _ls();
        else if (str[i] == 'p' && str[i + 1] == 'w' && str[i + 2] == 'd')
            _pwd();
        else if (str[i] == 'e' && str[i + 1] == 'c' && str[i + 2] == 'h' && str[i + 3] == 'o')
        {
            echo(&str[i + 4]);
            i += 4;
        }
        else if (str[i] == 'e' && str[i + 1] == 'n' && str[i + 2] == 'v')
            _env();
        else if (str[i] == 'e' && str[i + 1] == 'x' && str[i + 2] == 'i' && str[i + 3] == 't')
            exit(0);
        i++;
    }
}



int main(int ac , char **av)
{
    char *line;
    char **args;
    int i;

    i = 1;
    (void)ac;
    (void)av;
    while (1)
    {
        printf("\nminishell-> ");
        getline(&line, (size_t*)&i, stdin);
        _builtin_handler(line);
    }
    return (0);
}