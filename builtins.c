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
int detect_equal(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '=')
            break;
        i++;
    }
    return (i);
}
char *_ft_substr(char *str, int start, int len)
{
    char *new;
    int i;

    i = 0;
    new = malloc(sizeof(char) * (len + 1));
    while (i < len)
    {
        new[i] = str[start];
        i++;
        start++;
    }
    new[i] = '\0';
    return (new);
}
t_env ** lst_add_back(t_env **lst, t_env *new)
{
    t_env *last_add;

    if (*lst == NULL)
    {
        *lst = new;
        return (lst);
    }
    last_add = *lst;
    while (last_add->next != NULL)
        last_add = last_add->next;
    last_add->next = new;
    new->prev = last_add;
    new->next = NULL;
    return (lst);
}
t_env *new_node(char *key, char *value)
{
    t_env *new;

    new = malloc(sizeof(t_env));
    new->key = key;
    new->value = value;
    new->next = NULL;
    return (new);
}
t_env *add_envir(t_env *s)
{
    extern char **environ;
    t_env *new =NULL;
    int i;
    i = 0;
    int j;
    j = 0;
    while(environ[i])
    {
        j = detect_equal(environ[i]);
        new = new_node(_ft_substr(environ[i], 0, j), _ft_substr(environ[i], j + 1, strlen(environ[i])));
        lst_add_back(&s, new);
        i++;
    }
    return (s);
}
 void _env(t_env **s)
{

    t_env *tmp;
    tmp = *s;
    while (tmp)
    {
        printf("%s=%s\n", tmp->key, tmp->value);
        tmp = tmp->next;
    }
    s = &tmp;
    
    
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
}
void _export(char *str, t_env **s)
{

    // int i;
    // i= 0;
    //     int j = detect_equal(&str[i]);
    //     t_env *new;
    //     new = malloc(sizeof(t_env));
    //     new = new_node(_ft_substr(&str[i], 0, j), _ft_substr(&str[i], j + 1, strlen(&str[i])));
    //     lst_add_back(s, new);

}
int _ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;

    i = 0;
    if (n <= 0)
        return (0);
    while ((s1[i] && s2[i]) && (s1[i] == s2[i]) && (i < n - 1))
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
size_t ft_strlen( const char *str)
{
    int i;

    i = 0;
    while (str[i] && str[i] != '=')
        i++;
    return (i);
}

void _unset(char *str)
{
    printf("%s\n", str);
    t_env *s = NULL;
    s = add_envir(s);
}

void _builtin_handler(char *str)
{
	char **args;
    t_env *s = NULL;
    t_env *new;
    t_env *tmp = NULL;
    s = add_envir(s);
	int i;

	i = 0;
	while(str[i])
	{
		while(str[i] == ' ')
			i++;
		if (str[i] == 'l' && str[i + 1] == 's')
		{
			_ls();
		}
		if (str[i] == 'p' && str[i + 1] == 'w' && str[i + 2] == 'd')
			_pwd();
	 if (str[i] == 'e' && str[i + 1] == 'c' && str[i + 2] == 'h' && str[i + 3] == 'o')
		{
			_echo(&str[i + 4]);
			i += 4;

		}
        if(str[i] == 'e' && str[i+1] == 'x' && str[i+2] == 'p' && str[i+3] == 'o' && str[i+4] == 'r' && str[i+5] == 't')
        {
            int j = detect_equal(&str[i + 6]);
            t_env *new;
            new = malloc(sizeof(t_env));
            new = new_node(_ft_substr(&str[i+6], 0, j), _ft_substr(&str[i+6], j + 1, strlen(&str[i+6])));
            lst_add_back(&s, new); 
                i += 6;
        }
            
         if(str[i] == 'u' && str[i+1] == 'n' && str[i+2] == 's' && str[i+3] == 'e' && str[i+4] == 't')
        {
            _unset(&str[i + 5]);
            i += 5;
        }
		 if (str[i] == 'e' && str[i + 1] == 'n' && str[i + 2] == 'v')
        {
            _env(&s);
        }
		 if(str[i] == 'c' && str[i + 1] == 'd')
		{
			_cd(&str[i + 3]);
			i += 3;
		}
		 if (str[i] == 'c' && str[i + 1] == 'l' && str[i + 2] == 'e' && str[i + 3] == 'a' && str[i + 4] == 'r')
			system("clear");
	    if (str[i] == 'e' && str[i + 1] == 'x' && str[i + 2] == 'i' && str[i + 3] == 't')
			exit(0);
		
		i++;
	}
}

void signal_handler(int sig)
{
	if(sig == SIGINT)
	{
	
		ft_putstr("\nminishell-> ");

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