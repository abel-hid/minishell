/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 21:52:30 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/11 03:11:42 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_squote(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

char	**is_word(char *str, char **args, t_env **g_env, int *i)
{
	char	**p;
	char	*s;
	int		k;

	k = 0;
	str = ft_delete(str);
	s = ft_expand(str, g_env);
	if (check_space(str) && !is_dquote(str) && !is_squote(str))
	{
		p = get_p(s);
		k = 0;
		while (p[k])
		{
			args[*i] = ft_strdup(p[k]);
			(*i)++;
			free(p[k++]);
		}
		free(p);
	}
	else
	{
		args[*i] = add_args(str);
		(*i)++;
	}
	return (args);
}

int	create(char **args, t_command **cmd, t_fd fd, int i)
{
	args[i] = NULL;
	my_lstadd_back(cmd, ft_new(args, fd));
	return (0);
}

int	is_redir(t_lexer *tmp)
{
	if (tmp->token == REDIR_OUT || tmp->token == APPEND
		|| tmp->token == REDIR_IN || tmp->token == HEARDOC)
		return (1);
	return (0);
}

void	parsing1(t_lexer *tmp, char **args, t_env **g_env, t_command **cmd)
{
	int		i;
	t_fd	fd;

	i = 0;
	fd = ft_fd(0, 1);
	while (tmp != NULL)
	{
		if (tmp->token == WORD && ft_strcmp(tmp->content, ""))
			args = is_word(tmp->content, args, g_env, &i);
		if (is_redir(tmp))
			if (fd.fd_in != -1 && fd.fd_out != -1)
				fd = parse_redir(tmp->token, tmp->next->content, fd, g_env);
		if (is_redir(tmp))
			tmp = tmp->next;
		if (!tmp->next || tmp->token == PIPE_LINE)
		{
			i = create(args, cmd, fd, i);
			if (tmp->next && tmp->token == PIPE_LINE)
			{
				args = realloc_args(args, calculate_args(tmp->next) + 1);
				fd = ft_fd(0, 1);
			}
		}
		tmp = tmp->next;
	}
}

void	parse_args(t_lexer **list, t_command **cmd, t_env **g_env)
{
	t_lexer	*tmp;
	char	**args;
	int		count;

	tmp = *list;
	count = calculate_args(tmp);
	tmp = *list;
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return ;
	parsing1(tmp, args, g_env, cmd);
}
