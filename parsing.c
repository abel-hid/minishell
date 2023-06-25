#include "minishell.h"

t_command *ft_new(char **content, int  fd, t_redir_list *redir_list)
{
	t_command *list;

	list = malloc(sizeof(t_command));
	if (!list)
		return (NULL);
	list->args = content;
	// list->redir_list = redir_list;
	return (list);
}

t_command **srfak_lban(t_lexer **list)
{
	t_lexer *tmp;
	t_fd fd;
	int i;


	tmp = *list;
	t_command **command = malloc(sizeof(t_command ));
	if (!command)
		return (NULL);

	while(tmp)
	{
		if(tmp->token == WORD)
		{
			*command = ft_new(&tmp->content, fd.fd_in, NULL);
		}
		if(tmp->token == REDIR_IN)
		{
			tmp= tmp->next;
			fd.fd_in = open(tmp->content,O_CREAT | O_RDONLY);
			if(fd.fd_in == -1)
			{
				printf("error\n");
				return (NULL);
			}
		}
		if(tmp->token == REDIR_OUT)
		{
			tmp= tmp->next;
			fd.fd_out = open(tmp->content,O_CREAT | O_WRONLY);
			if(fd.fd_out == -1)
			{
				printf("error\n");
				return (NULL);
			}
		}
		if(tmp->token == PIPE_LINE)
		{
			tmp = tmp->next;
			*command = ft_new(&tmp->content, fd.fd_in, NULL);
		}
		if(tmp->token == HEARDOC)
		{
			tmp = tmp->next;
			*command = ft_new(&tmp->content, fd.fd_in, NULL);
		}
		tmp = tmp->next;
	}
	list = &tmp;
	return (command);
}

// int parsing(t_lexer **list, t_command **command)
// {
// 	t_lexer *tmp;
// 	t_lexer *tmp2;
// 	t_fd fd;
// 	t_redir_list *redir_list;
// 	char **content;
// 	int i;

// 	tmp = *list;
// }

