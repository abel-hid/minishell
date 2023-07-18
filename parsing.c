#include "minishell.h"


t_command	*lstlast(t_command *lst)
{
	printf("[%p]\n", lst);
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
	int i;
	i = 0;
	while (tmp != NULL)
	{
		if (tmp->token == WORD)
			i++;
		if(tmp->token == PIPE_LINE)
			return (i);
		tmp = tmp->next;
	}
	return (i);
}




void parsing(t_lexer **list, t_command **cmd)
{
	t_lexer *tmp = *list;
	int i = 0;
	t_fd fd;

	*cmd = NULL;
	fd.fd_in = 0;
	fd.fd_out = 1;
	char **args;
	int count = calculate_args(tmp);

	args = malloc(sizeof(char *) * (count + 1));
	if(!args)
		return ;

	while (tmp != NULL)
	{
		if (tmp->token == WORD)
		{
			args[i] = ft_strdup(tmp->content);
			i++;
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

void heredoc(t_lexer **lexer, t_env **g_env)
{
	char *line;
	int fd;
	int check;
	t_lexer *tmp = *lexer;


	while(tmp != NULL)
	{
		if (tmp->token == HEARDOC)
		{
			unlink("/tmp/srfak");
			tmp = tmp->next;
			fd = open("/tmp/srfak", O_RDWR | O_CREAT, 0644);
			check = valid(tmp->content);
			tmp->content = del_quote(tmp->content, '\'', '\"');
			while(1)
			{
				line = readline("heredoc> ");

				if (ft_strncmp(line, tmp->content,ft_strlen(line)) == 0)
					break ;

				if (!check)
				line = ft_expand(line, g_env);

				ft_putendl_fd(line, fd);

			}
			close(fd);
		}
		tmp = tmp->next;
	}
}
