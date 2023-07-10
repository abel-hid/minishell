#include "minishell.h"


int main(int ac ,char **av , char **env)
{
	char *line;

	t_lexer *lexer;
	t_env *p_env;
	t_command *cmd = NULL;
	craete_env(env,	&p_env);

	lexer = NULL;
	(void)ac;
	(void)av;
	// char *token[] = {"WORD", "PIPE_LINE", "REDIR_IN", "REDIR_OUT", "HEARDOC", "APPEND"};

	while (1)
	{
		line = readline("minishell-> ");
			if (line == NULL)
				break ;
		lexing(&lexer, line);
		expand(&lexer, &p_env);
		parse_lex(lexer,&cmd);
		// parsing(&lexer,&cmd);
		// cmd = *(srfak_lban(&lexer));

		// while(lexer != NULL)
		// {
		// 	printf("%s\n", lexer->content);
		// 	printf("%s\n", token[lexer->token]);
		// 	lexer = lexer->next;
		// }

		// while(cmd)
		// {
		// 	printf("%s\n", cmd->args[i]);
		// 	i++;
		// }
		add_history(line);

	}
}
