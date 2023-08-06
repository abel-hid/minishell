/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirction.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 21:25:55 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/06 18:41:22 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handel_redirout(char *str_next, int fd, t_env **g_env)
{
	char	*str;

	str = ft_strdup(str_next);
	str = ft_expand(str, g_env);
	if (ft_strcmp(str, ""))
	{
		if (ambiguous_redirect(str, g_env, str_next) == 1)
			return (-1);
		str = del_quote(str, '\'', '\"');
		fd = open(str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
		{
			error(str);
			return (free(str), -1);
		}
	}
	else
	{
		error_ambiguous(str_next);
		return (-1);
	}
	free(str);
	return (fd);
}

int	handel_redirin(char *str_next, int fd, t_env **g_env)
{
	char	*str;

	str = ft_strdup(str_next);
	str = ft_expand(str, g_env);
	if (ft_strcmp(str, ""))
	{
		if (ambiguous_redirect(str, g_env, str_next) == 1)
			return (-1);
		str = del_quote(str, '\'', '\"');
		fd = open(str, O_RDONLY, 0644);
		if (fd == -1)
		{
			error(str);
			return (free(str), -1);
		}
	}
	else
	{
		error_ambiguous(str_next);
		return (-1);
	}
	free(str);
	return (fd);
}

int	parse_redir_out(int type, char *str_next, int fd, t_env **g_env)
{
	if (type == REDIR_OUT)
	{
		fd = handel_redirout(str_next, fd, g_env);
		if (fd == -1)
			return (-1);
	}
	else if (type == APPEND)
	{
		fd = handel_append(str_next, fd, g_env);
		if (fd == -1)
			return (-1);
	}
	return (fd);
}

int	parse_redir_in(int type, char *str_next, int fd, t_env **g_env)
{
	if (type == REDIR_IN)
	{
		fd = handel_redirin(str_next, fd, g_env);
		if (fd == -1)
			return (-1);
	}
	if (type == HEARDOC)
	{
		fd = open("/tmp/heredoc", O_RDONLY, 0644);
	}
	return (fd);
}

t_fd	parse_redir(int type, char *str_next, t_fd fd, t_env **g_env)
{
	fd.fd_out = parse_redir_out(type, str_next, fd.fd_out, g_env);
	fd.fd_in = parse_redir_in(type, str_next, fd.fd_in, g_env);
	return (fd);
}
