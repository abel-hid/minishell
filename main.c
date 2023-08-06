/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 22:08:21 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/06 15:43:08 by heddahbi         ###   ########.fr       */
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

void	minishell(t_lexer **lexer, t_env **p_env, t_command **cmd, char **env)
{
	heredoc(lexer, p_env);
	expand(lexer, p_env);
	parse_args(lexer, cmd, p_env);
	execute_the_shot(*cmd, p_env, env);
}

int	main(int ac, char **av, char **env)
{
	char				*line;
	t_lexer				*lexer;
	struct sigaction	sa;
	t_command			*cmd;
	t_env				*p_env;

	p_env = NULL;
	lexer = NULL;
	craete_env(env, &p_env);
	initialize_data(&sa);
	cmd = NULL;
	(void)ac;
	(void)av;
	while (1)
	{
		action(&sa);
		line = readline(ANSI_COLOR_BLUE "srfak_lban" ANSI_COLOR_GREEN " $ " ANSI_COLOR_RESET);
		if (minishell_prime(line))
			continue ;
		if (lexing(&lexer, line) == 0)
			minishell(&lexer, &p_env, &cmd, env);
		free_stuff(line, &lexer, &cmd);
	}
	free_env(&p_env);
}
