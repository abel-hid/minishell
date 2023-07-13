#include "minishell.h"

// t_command *ft_new(char **content, int  fd, t_redir_list *redir_list)
// {
// 	t_command *list;

// 	list = malloc(sizeof(t_command));
// 	if (!list)
// 		return (NULL);
// 	list->args = content;
// 	// list->redir_list = redir_list;
// 	return (list);
// }

// t_command **srfak_lban(t_lexer **list)
// {
// 	t_lexer *tmp;
// 	t_fd fd;


// 	tmp = *list;
// 	t_command **command = malloc(sizeof(t_command ));
// 	if (!command)
// 		return (NULL);

// 	while(tmp)
// 	{
// 		if(tmp->token == WORD)
// 		{
// 			*command = ft_new(&tmp->content, fd.fd_in, NULL);
// 		}
// 		if(tmp->token == REDIR_IN)
// 		{
// 			tmp= tmp->next;
// 			fd.fd_in = open(tmp->content,O_CREAT | O_RDONLY);
// 			if(fd.fd_in == -1)
// 			{
// 				printf("error\n");
// 				return (NULL);
// 			}
// 		}
// 		if(tmp->token == REDIR_OUT)
// 		{
// 			tmp= tmp->next;
// 			fd.fd_out = open(tmp->content,O_CREAT | O_WRONLY);
// 			if(fd.fd_out == -1)
// 			{
// 				printf("error\n");
// 				return (NULL);
// 			}
// 		}

// 		if(tmp->token == PIPE_LINE)
// 		{
// 			tmp = tmp->next;
// 			*command = ft_new(&tmp->content, fd.fd_in, NULL);
// 		}
// 		if(tmp->token == HEARDOC)
// 		{
// 			tmp = tmp->next;
// 			*command = ft_new(&tmp->content, fd.fd_in, NULL);
// 		}
// 		tmp = tmp->next;
// 	}
// 	list = &tmp;
// 	return (command);
// }

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


// // 	tmp = *list;
// // }
// void itis_acmd(char *str, t_command *cmd)
// {
//     int i ; 
//     i =0;
//     // int j = 0;
//     while(str[i])
//     {
//         while(str[i] && str[i] != ' '  && str[i] != '|' && str[i] != '<' && str[i] != '>' && str[i] != ';')
//             i++;
//         cmd->command_name = malloc(sizeof(char) * (i + 1));
//         if(!cmd->command_name)
//             return ;
//         printf("%s\n", cmd->command_name);
        
//     }
// }

// void _parsing(t_lexer **list, t_command *cmd)
// {
//     t_lexer *tmp = *list;

//     if (!tmp)
//         return;

//     cmd = malloc(sizeof(t_command));
//     if (!cmd)
//         return;

//     cmd->command_name = tmp->content;
//     printf("%s\n", cmd->command_name);
//     if(tmp->token == WORD)
//     {
//         cmd->args = malloc(sizeof(char *) * 2);
//         if(!cmd->args)
//             return ;
//         cmd->args[0] = tmp->content;
//         cmd->args[1] = NULL;
//         printf("%s\n", cmd->args[0]);
//     }
//     list = &tmp;
// }
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


void _parsing(t_lexer **list, t_command *cmd)
{
    t_lexer *tmp = *list;
	t_fd fd;
	char *line;

    if (!tmp)
        return;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return;
		while(tmp->token ==  HEARDOC)
		{
			tmp = tmp->next;
			fd.fd_in = open("heredoc", O_CREAT | O_WRONLY | O_APPEND);
			while(1)
			{
				line = readline("heredoc> ");
				if(!ft_strncmp(line, tmp->content, ft_strlen(tmp->content)))
					break;
				ft_putendl_fd(line, fd.fd_in);
			}
		}
		 if(tmp->token == WORD && tmp->token != PIPE_LINE && tmp->token != REDIR_IN && tmp->token != REDIR_OUT && tmp->token != HEARDOC)
    		cmd->command_name = tmp->content;
    printf(" command :%s\n", cmd->command_name);
    tmp = tmp->next;
    while(tmp)
    {
		while(tmp->token == HEARDOC)
		{
			
				tmp = tmp->next;
				fd.fd_in = open("heredoc", O_CREAT | O_WRONLY | O_APPEND);
				while(1)
				{
					line = readline("heredoc> ");
					if(!ft_strncmp(line, tmp->content, ft_strlen(tmp->content)))
						break;
					ft_putendl_fd(line, fd.fd_in);
				}
			
		}
		if(tmp->token == REDIR_IN)
			{
				tmp= tmp->next;
				fd.fd_in = open(tmp->content,O_CREAT | O_RDONLY);
				if(fd.fd_in == -1)
				{
					printf("error\n");
					return ;
				}
			}
			if (tmp->token == REDIR_OUT)
			{
				tmp= tmp->next;
				fd.fd_out = open(tmp->content,O_CREAT | O_WRONLY);
				if(fd.fd_out == -1)
				{
					printf("error\n");
					return ;
				}
			}
			if(tmp->token == APPEND)
			{
				tmp= tmp->next;
				fd.fd_out = open(tmp->content,O_CREAT | O_WRONLY | O_APPEND);
				if(fd.fd_out == -1)
				{
					printf("error\n");
					return ;
				}
			}
        if(tmp->token == PIPE_LINE)
        {
            tmp = tmp->next;
            cmd->command_name = tmp->content;
            printf(" command :%s\n", cmd->command_name);
		}
        if(tmp->token == WORD && tmp->prev->token != PIPE_LINE && tmp->prev->token != REDIR_IN && tmp->prev->token != REDIR_OUT && tmp->prev->token != HEARDOC && tmp->prev->token != APPEND)
        {
            cmd->args = malloc(sizeof(char *) * 2);
            if(!cmd->args)
                return ;
            cmd->args[0] = tmp->content;
            cmd->args[1] = NULL;
            printf(" args : %s\n", cmd->args[0]);
        }
        tmp = tmp->next;
    }

    tmp = NULL;
    *list = tmp;
}

