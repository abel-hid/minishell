/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 22:08:21 by abel-hid          #+#    #+#             */
/*   Updated: 2024/08/15 01:52:31 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_data(struct sigaction *sa)
{
	g_exit_st.status = 0;
	g_exit_st.path = NULL;
	g_exit_st.is_unset = 0;
	// rl_catch_signals = 0;
	sa->sa_handler = signal_handler;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
}

int	minishell(t_lexer *lexer, t_env *p_env)
{
	heredoc(&lexer, &p_env);
	if(g_exit_st.done == 1)
		return (1);
	expand(&lexer, &p_env);
	return (0);
}

int	haha(char *line)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (is_space(line[i]))
			j++;
		i++;
	}
	if (j == i)
		return(free(line),1);
	return (0);
}
void parsing_all(t_lexer **lexer, t_command **cmd, t_env **p_env, char **env)
{
	parse_args(lexer, cmd, p_env);
	g_exit_st.in_cmd = 1;
	execute_the_shot(*cmd, p_env, env);
	g_exit_st.in_cmd = 0;

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
		g_exit_st.done = 0;
		line_managment(line);
		if (minishell_prime(line) || haha(line))
			continue ;
		if (lexing(&lexer, line) == 0)
		{
			if(minishell(lexer, p_env) == 0)
				parsing_all(&lexer, &cmd, &p_env, env);
		}
		free_stuff(line, &lexer, &cmd);
	}
	free_env(&p_env);
}

int	main(int ac, char **av, char **env)
{
	t_lexer				*lexer;
	char				*str;

	str = ft_strdup("~minishell~$ ");
	lexer = NULL;
	(void)ac;
	(void)av;
	minishell_exec(env, lexer, str);
}
