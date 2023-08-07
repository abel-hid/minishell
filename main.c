/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 22:08:21 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/07 11:50:26 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_data(struct sigaction *sa)
{
	g_exit_st.status = 0;
	g_exit_st.path = NULL;
	g_exit_st.is_unset = 0;
	rl_catch_signals = 0;
	sa->sa_handler = signal_handler;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
}

void	minishell(t_lexer *lexer, t_env *p_env)
{
	heredoc(&lexer, &p_env);
	expand(&lexer, &p_env);
}

void	minishell_exec(char **env, t_lexer *lexer, char *str)
{
	char				*line;
	struct sigaction	sa;
	t_command			*cmd;
	t_env				*p_env;

	p_env = NULL;
	craete_env(env, &p_env);
	initialize_data(&sa);
	cmd = NULL;
	while (1)
	{
		action(&sa);
		line = readline(str);
		if (minishell_prime(line))
			continue ;
		if (lexing(&lexer, line) == 0)
		{
			minishell(lexer, p_env);
			parse_args(&lexer, &cmd, &p_env);
			execute_the_shot(cmd, &p_env, env);
		}
		free_stuff(line, &lexer, &cmd);
	}
	free_env(&p_env);
}

int	main(int ac, char **av, char **env)
{
	t_lexer				*lexer;
	char				*str;

	str = ft_strdup("\x1b[31msrfak_lban \x1b[34m$ \x1b[0m");
	lexer = NULL;
	(void)ac;
	(void)av;
	minishell_exec(env, lexer, str);
}
