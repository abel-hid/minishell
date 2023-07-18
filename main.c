#include "minishell.h"

void free_lexer_list(t_lexer **lst)
{
	t_lexer *tmp;

	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
void free_cmd_list(t_command **lst)
{
	t_command *tmp;

	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void free_env(t_env **lst)
{
	t_env *tmp;

	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		free((*lst)->key);
		free((*lst)->value);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}



int main(int ac ,char **av , char **env)
{
	char *line;
	t_lexer *lexer;
	t_env *p_env;
	p_env = NULL;
	lexer = NULL;


	craete_env(env,	&p_env);
	t_command *cmd;

	cmd = NULL;
	(void)ac;
	(void)av;
	// char *token[] = {"WORD", "PIPE_LINE", "REDIR_IN", "REDIR_OUT", "HEARDOC", "APPEND"};

	while (1)
	{
		signal(SIGINT, signal_handler);

		line = readline("minishell-> ");
			if (line == NULL)
				break ;

		lexing(&lexer, line);

		if(lexer != NULL)
		{
			heredoc(&lexer, &p_env);
			expand(&lexer, &p_env);
			parsing(&lexer,&cmd);
			execute(cmd, p_env);
		}


		// while(lexer != NULL)
		// {
		// 	printf("%s\n", lexer->content);
		// 	// printf("%s\n", token[lexer->token]);
		// 	lexer = lexer->next;
		// }

		// while(cmd != NULL)
		// {
		// 	int i = 0;
		// 	printf(" ----------cmd--------------\n");
		// 	while(cmd && cmd->args[i] != NULL)
		// 	{
		// 		printf("%d %s\n", i, cmd->args[i]);
		// 		i++;
		// 	}
		// 	// printf("fd_in %d\n", cmd->fd.fd_in);
		// 	// printf("fd_out %d\n", cmd->fd.fd_out);
		// 	cmd = cmd->next;
		// }


		lexer = NULL;
		add_history(line);
		free_lexer_list(&lexer);
		free_cmd_list(&cmd);

	}
	free_env(&p_env);
}


