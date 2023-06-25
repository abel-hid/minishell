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

