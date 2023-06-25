#include "minishell.h"

t_command *ft_new(char **content, t_fd fd, t_redir_list *redir_list)
{
	t_command *list;

	list = malloc(sizeof(t_command));
	if (!list)
		return (NULL);
	list->args = content;
	list->fd = fd;
	list->redir_list = redir_list;
	return (list);
}

int parsing(t_lexer **list, t_command **command)
{
	t_lexer *tmp;
	t_lexer *tmp2;
	t_fd fd;
	t_redir_list *redir_list;
	char **content;
	int i;

	tmp = *list;
	while(tmp)
	{
		if(tmp->token == REDIR_IN)
		{
			fd.fd_in = open(tmp->next->content, O_RDONLY);
			if(fd.fd_in == -1)
			{
				printf("Error\n");
				return (0);
			}
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			free(tmp2);
		}
		else if(tmp->token == REDIR_OUT)
		{
			fd.fd_out = open(tmp->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(fd.fd_out == -1)
			{
				printf("Error");
				return (0);
			}
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			free(tmp2);
		}
		else if(tmp->token == APPEND)
		{
			fd.fd_out = open(tmp->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if(fd.fd_out == -1)
			{
				printf("Error\n");
				return (0);
			}
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			free(tmp2);
		}
		else if(tmp->token == HEARDOC)
		{
			fd.fd_in = open(tmp->next->content, O_RDONLY);
			if(fd.fd_in == -1)
			{
				printf("Error");
				return (0);
			}
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			free(tmp2);
		}
		else if(tmp->token == PIPE_LINE)
		{
			content = malloc(sizeof(char *) * (tmp->i + 1));
			if(!content)
				return (0);
			i = 0;
			while(i < tmp->i)
			{
				content[i] = tmp->content;
				tmp = tmp->next;
				i++;
			}
			content[i] = NULL;
		}
		tmp = tmp->next;
	}
	return (1);
}

