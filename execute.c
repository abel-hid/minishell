/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:20:27 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 05:44:35 by abel-hid         ###   ########.fr       */
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
	if(exit_st.in_here_doc == 1 && signal == SIGINT)
	{
		close(0);
		return ;
	}
	else if(signal == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		if(exit_st.in_cmd == 1)
		{
			exit_st.status = 130;
			exit_st.in_cmd = 0;
		}
		else if(exit_st.in_cmd == 0)
		{
			exit_st.status = 1;
			exit_st.in_cmd = 0;
			rl_redisplay();

		}

	}


	else if (signal == SIGQUIT && exit_st.in_cmd == 1)
	{
			ft_putstr_fd("Quit: 3\n", 1);
	}
}
