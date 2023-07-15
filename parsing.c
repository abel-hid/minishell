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

void ft_putendl_fd(char *s, int fd)
{
	int i;

	i = 0;
	if (!s)
		return ;
	while(s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
	write(fd, "\n", 1);
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
	while (tmp != NULL && tmp->token != PIPE_LINE)
	{
		if (tmp->token == WORD)
			i++;
		tmp = tmp->next;
	}
	return (i);
}

void _parsing(t_lexer **list, t_command **cmd)
{
	t_lexer *tmp = *list;
	int i = 0;
	t_fd fd;

	*cmd = NULL;
	fd.fd_in = 0;
	fd.fd_out = 1;
	char **args;
	int args_size = calculate_args(tmp);
	args = malloc(sizeof(char *) * (args_size + 1));
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
				args = malloc(sizeof(char *) * (args_size + 1));
				if(!args)
					return ;
			}
			i = 0;
		}
		tmp = tmp->next;
	}

}
