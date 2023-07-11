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

// // int parsing(t_lexer **list, t_command **command)
// // {
// // 	t_lexer *tmp;
// // 	t_lexer *tmp2;
// // 	t_fd fd;
// // 	t_redir_list *redir_list;
// // 	char **content;
// // 	int i;

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

void _parsing(t_lexer **list, t_command *cmd)
{
    t_lexer *tmp = *list;

    if (!tmp)
        return;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return;
    cmd->command_name = tmp->content;
    printf(" command :%s\n", cmd->command_name);
    tmp = tmp->next;
    while(tmp)
    {
        if(tmp->token == PIPE_LINE)
        {
            tmp = tmp->next;
            cmd->command_name = tmp->content;
            printf(" command :%s\n", cmd->command_name);
        }
        if(tmp->token == WORD && tmp->prev->token != PIPE_LINE )
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
