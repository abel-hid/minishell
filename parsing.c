#include "minishell.h"


t_command	*lstlast(t_command *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

void my_lstadd_back(t_command **lst, t_command *new)
{
	t_command	*last_add;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last_add = lstlast(*lst);
	last_add->next = new;
}


t_command *ft_new(char **args, t_fd fd)
{
	t_command *list;

	list = malloc(sizeof(t_command));
	if (!list)
		return (NULL);
	list->args = args;
	list->fd = fd;
	list->next = NULL;
	return (list);
}

int ft_lstsize(t_lexer *lst)
{
	int i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
int calculate_args(t_lexer *tmp)
{
	int i = 0;

	while(tmp != NULL && tmp->token != PIPE_LINE)
	{
		if(tmp->token == WORD)
			i++;
		tmp = tmp->next;
	}
	return (i);
}

int count_agrs_after_pipe(t_lexer **list, int pos)
{
	t_lexer *tmp = *list;
	int i = 0;
	int j = -1;
	printf("pos %d\n", pos);
	while (tmp)
	{
		printf("tmp->token %d\n", tmp->token);
		if(tmp->token == PIPE_LINE)
			j++;
		if(pos == j && tmp->token != PIPE_LINE)
		{
				i++;
		}
		tmp = tmp->next;
	}
	return (i);

}


int check_space(char *line)
{
	int i = 0;

	while(line[i])
	{
		if(line[i] == ' ' || line[i] == '\t')
			return (1);
		i++;
	}
	return (0);
}
int is_quote1(char *str)
{
	int i = 0;

	while(str[i])
	{
		if(str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}


void parsing(t_lexer **list, t_command **cmd, t_env **g_env)
{
	t_lexer *tmp = *list;
	int i = 0;
	t_fd fd;
	int j;
	int k;

	(void)g_env;
	t_env *env ;
	(void)env;

	*cmd = NULL;
	fd.fd_in = 0;
	fd.fd_out = 1;
	char **args;
	char **p;
	int count = calculate_args(tmp);

	args = malloc(sizeof(char *) * (count + 1));
	if(!args)
		return ;


	while (tmp != NULL)
	{
		if (tmp->token == WORD)
		{
			j = 0;
			env = *g_env;
			while(env)
			{
				env->value = del_quote(env->value, '\'', '\"');
				if(ft_strcmp(env->value, tmp->content) == 0)
					j = 1;
				env = env->next;
			}

			if(check_space(tmp->content) && j)
			{
				tmp->content = del_quote(tmp->content, '\'', '\"');
				p = ft_split1(tmp->content, "' ' '\t' '\n' '\v' '\f' '\r'");
				k = 0;
				while(p[k])
				{
					args[i++] = ft_strdup(p[k]);
					free(p[k]);
					k++;
				}
				free(p);
			}
			else
			{
				tmp->content = del_quote(tmp->content, '\'', '\"');
				args[i] = ft_strdup(tmp->content);
				i++;
			}

		}
		else if (tmp->token == REDIR_IN)
		{
			tmp = tmp->next;
			fd.fd_in = open(tmp->content, O_RDONLY, 0644);
			if (fd.fd_in == -1)
			{
				printf("minishell: %s: No such file or directory\n", tmp->content);
				return;
			}
		}
		else if (tmp->token == REDIR_OUT)
		{
			tmp = tmp->next;
			fd.fd_out = open(tmp->content, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd.fd_out == -1)
			{
				printf("minishell: $s: ambiguous redirect\n");
				return;
			}
		}
		else if(tmp->token == HEARDOC)
		{
			tmp = tmp->next;
			fd.fd_in = open("/tmp/srfak", O_RDONLY, 0644);
		}
		else if (tmp->token == APPEND)
		{
			tmp = tmp->next;
			fd.fd_out = open(tmp->content, O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (fd.fd_out == -1)
			{
				printf("minishell: $s: ambiguous redirect\n");
				return;
			}
		}

		if (!tmp->next || tmp->next->token == PIPE_LINE)
		{
			args[i] = NULL;
			my_lstadd_back(cmd, ft_new(args, fd));
			if (tmp->next && tmp->next->token == PIPE_LINE)
			{
				tmp->next = tmp->next->next;
				args = malloc(sizeof(char *) * (calculate_args(tmp)));
				if(!args)
					return ;
			}
			i = 0;
		}
		tmp = tmp->next;
	}

}
int valid(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

char *expand_heredoc(char *str, t_env **g_env)
{
	static char   *new;
	int i;

	i = 0;
	while (str[i])
	{
		if(str[i] == '$' && is_digit(str[i + 1]))
		{
				i += 2;
				new = ft_substr(str, i, ft_strlen(str));
				free(str);
				str = new;
				new = NULL;
		}
		if(str[i] == '$' && !is_digit(str[i + 1]) && str[i + 1] != '\'' && str[i + 1] != '$' && str[i + 1] != '\0' && !is_spaces(str[i + 1]) && str[i + 1] != '\"' )
		{
			if(str[i + 1] == '\"' && str[i + 2] == '\0')
				break;
			else
				str = handler(str, &i, g_env);
		}
		i++;
	}
	free(new);
	return (str);
}

int here_doc(char *str, char *line, t_env **g_env)
{
	int fd;
	int check;

	unlink("/tmp/srfak");
	fd = open("/tmp/srfak", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	check = valid(str);
	str = del_quote(str, '\'', '\"');
	while(1)
	{
		line = readline("heredoc>");
		if(ft_strcmp(line, str) == 0)
		{
			free(line);
			return (1);
			break ;
		}
		if (!check)
		line = expand_heredoc(line, g_env);
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	return (0);
}

void heredoc(t_lexer **lexer, t_env **g_env)
{

	char *line;
	line = NULL;

	t_lexer *tmp = *lexer;

	while(tmp != NULL)
	{
		if (tmp->token == HEARDOC)
		{
			tmp = tmp->next;
			if (here_doc(tmp->content, line, g_env))
			{
				break;
			}
		}
		tmp = tmp->next;
	}
}
