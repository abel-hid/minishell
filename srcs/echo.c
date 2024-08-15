/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:53:31 by heddahbi          #+#    #+#             */
/*   Updated: 2023/09/16 12:02:48 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_command *cmd)
{
	int	i;
	int	n;

	i = 1;
	n = 0;

	size_t j ;


	while (cmd->args[i] && cmd->args[i][0] == '-' && cmd->args[i][1] == 'n')
	{
		j = 1;
		while(cmd->args[i][j] && cmd->args[i][j] == 'n')
			j++;
		if (cmd->args[i][j] == '\0')
			n = 1;
		else
			break;
		i++;

	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], 1);
		if (cmd->args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!n)
		ft_putstr_fd("\n", 1);
}
