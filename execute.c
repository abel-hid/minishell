/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:20:27 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 01:04:38 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_lst_size(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}


void	signal_handler(int signal)
{
	if(exit_st.in_here_doc == 1)
	{
		close(0);
		return ;
	}
	if (signal == SIGINT && ft_strcmp(rl_line_buffer,"") == 0)
	{
			ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
	}
	if(signal == SIGINT && exit_st.in_cmd == 1)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	if (signal == SIGQUIT)
	{
		if(exit_st.in_cmd == 1)
			ft_putstr_fd("Quit: 3\n", 1);
	}
}
