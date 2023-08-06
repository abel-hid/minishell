/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 14:41:50 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 20:30:15 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	line_managment(char *line)
{
	if (line == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	add_history(line);
}

void	free_stuff(char *line, t_lexer **lexer, t_command **cmd)
{
	free(line);
	free_lexer_list(lexer);
	free_cmd_list(cmd);
}

void	action(struct sigaction *sa)
{
	sigaction(SIGINT, sa, NULL);
	sigaction(SIGQUIT, sa, NULL);
}

int	minishell_prime(char *line)
{
	line_managment(line);
	if (ft_strlen(line) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}
