#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <readline/readline.h>
#include "minishell.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>

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
	printf("\n");
	closedir(dir);

}
<<<<<<< HEAD

// int ft_strcmp(const char *s1, const char *s2)
// {
//     int i;

//     i = 0;
//     while (s1[i] && (s1[i] == s2[i]))
//         i++;
//     return ((unsigned char)s1[i] - (unsigned char)s2[i]);
// }

// char **ft_split(char *str, char c)
// {
//     char **tab;
//     int i;
//     int j;
//     int k;

//     i = 0;
//     j = 0;
//     tab = malloc(sizeof(char *) * 100);
//     while (str[i])
//     {
//         k = 0;
//         tab[j] = malloc(sizeof(char) * 100);
//         while (str[i] && str[i] != c)
//         {
//             tab[j][k] = str[i];
//             i++;
//             k++;
//         }
//         tab[j][k] = '\0';
//         j++;
//         if (str[i])
//             i++;
//     }
//     tab[j] = NULL;
//     return (tab);
// }
=======
char *ft_putstr(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	return (str);
}
int ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	while (s1[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
char **_ft_split(char *str, char c)
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
>>>>>>> refs/remotes/origin/master

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
	printf("\n");
}

void _pwd()
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd() error");

	printf("\n");
}
void _cd(char *args)
{
    int i;

    i = 0;
    while (args[i] == ' ')
        i++;
    if (chdir(&args[i]) != 0)
        perror("chdir");
    else
        printf("Directory changed to: %s\n", &args[i]);

    printf("\n");
}
int _isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			return (1);
	return (0);
}

int ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
void _echo (char *args)
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
void _export(char *str)
{
	extern char **environ;
	char **tab;
	int i;
	int j;
	int k;

	i = 0;
	j = 0;
	k = 0;
	tab = malloc(sizeof(char *) * 100);
	while (environ[i])
	{
		tab[j] = malloc(sizeof(char) * 100);
		while (environ[i][k])
		{
			tab[j][k] = environ[i][k];
			k++;
		}
		tab[j][k] = '\0';
		j++;
		i++;
	}
	tab[j] = malloc(sizeof(char) * 100);
	k = 0;
	while (str[k])
	{
		tab[j][k] = str[k];
		k++;
	}
	tab[j][k] = '\0';
	tab[j + 1] = NULL;
	environ = tab;
}
void _unset(char *str)
{
	extern char **environ;
	char **tab;
	int i;
	int j;
	int k;

	i = 0;
	j = 0;
	k = 0;
	tab = malloc(sizeof(char *) * 100);
	while (environ[i])
	{
		if (ft_strcmp(environ[i], str) != 0)
		{
			tab[j] = malloc(sizeof(char) * 100);
			while (environ[i][k])
			{
				tab[j][k] = environ[i][k];
				k++;
			}
			tab[j][k] = '\0';
			j++;
		}
		i++;
	}
	tab[j] = NULL;
	environ = tab;
}

void _builtin_handler(char *str)
{
<<<<<<< HEAD
    // char **args;
    int i;
=======
	char **args;
	int i;
>>>>>>> refs/remotes/origin/master

	i = 0;
	while(str[i])
	{
		while(str[i] == ' ')
			i++;
		if (str[i] == 'l' && str[i + 1] == 's' && (str[i + 2] == '\n'|| str[i + 2] == ' '))
		{
			if(str[i] - 1 != ' ' || _isalpha(str[i] - 1))
			_ls();
		}
		else if (str[i] == 'p' && str[i + 1] == 'w' && str[i + 2] == 'd')
			_pwd();
		else if (str[i] == 'e' && str[i + 1] == 'c' && str[i + 2] == 'h' && str[i + 3] == 'o')
		{
			_echo(&str[i + 4]);
			i += 4;

		}
		else if (str[i] == 'e' && str[i + 1] == 'n' && str[i + 2] == 'v')
			_env();
		else if(str[i] == 'c' && str[i + 1] == 'd')
		{
			_cd(&str[i + 3]);
			i += 3;
		}
		else if (str[i] == 'c' && str[i + 1] == 'l' && str[i + 2] == 'e' && str[i + 3] == 'a' && str[i + 4] == 'r')
			system("clear");
		else if (str[i] == 'e' && str[i + 1] == 'x' && str[i + 2] == 'i' && str[i + 3] == 't')
			exit(0);
		else if(str[i] == 'u' && str[i + 1] == 'n' && str[i + 2] == 's' && str[i + 3] == 'e' && str[i + 4] == 't')
		{
			_unset(&str[i + 6]);
			i += 6;
		}
		i++;
	}
}

void signal_handler(int sig)
{
	if(sig == SIGINT)
	{
	
		ft_putstr("\nminishell-> ");

<<<<<<< HEAD
// int main(int ac , char **av)
// {
//     char *line;
//     // char **args;
//     int i;

//     i = 1;
//     (void)ac;
//     (void)av;
//     while (1)
//     {
//         printf("\nminishell-> ");
//         getline(&line, (size_t*)&i, stdin);
//         _builtin_handler(line);
//     }
//     return (0);
// }
=======
	}
}

int main(int ac , char **av)
{
	char *line = NULL;
	char **args;
	int i;

	while(1)
	{
		line = readline("minishell-> ");
		if(!line)
			exit(0);
		signal(SIGINT, signal_handler);
		_builtin_handler(line);
		add_history(line);
	}
}
>>>>>>> refs/remotes/origin/master
