/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 21:33:04 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/11 02:06:32 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valid(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

char	*expand_heredoc(char *str, t_env **g_env)
{
	static char	*new;
	int			i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && !is_digit(str[i + 1]) && str[i + 1] != '\''
			&& str[i + 1] != '$' && str[i + 1] != '\0'
			&& !is_spaces(str[i + 1]) && str[i + 1] != '\"' )
		{
			if (str[i + 1] == '\"' && str[i + 2] == '\0')
				break ;
			else
				str = handler(str, &i, g_env);
		}
		i++;
	}
	free(new);
	return (str);
}

int	ft_create_fd(int fd)
{
	unlink("/tmp/heredoc");
	fd = open("/tmp/heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	g_exit_st.in_here_doc = 1;
	return (fd);
}

int	here_doc(char *str, char *line, t_env **g_env)
{
	int	fd;
	int	check;

	fd = 0;
	fd = ft_create_fd(fd);
	check = valid(str);
	str = del_quote(str, '\'', '\"');
	while (1)
	{
		line = readline(">");
		if (close_here_doc() == 1 || !line)
			break ;
		if (ft_strcmp(line, str) == 0)
		{
			free(line);
			break ;
		}
		if (!check)
			line = expand_heredoc(line, g_env);
		ft_putendl_fd(line, fd);
		free(line);
	}
	g_exit_st.in_here_doc = 0;
	close(fd);
	return (0);
}

void	heredoc(t_lexer **lexer, t_env **g_env)
{
	t_lexer	*tmp;
	char	*line;
	int		i;

	line = NULL;
	tmp = *lexer;
	i = 0;
	while (tmp != NULL)
	{
		if (tmp->token == HEARDOC)
		{
			tmp = tmp->next;
			g_exit_st.in_here_doc = 0;
			here_doc(tmp->content, line, g_env);
			i++;
		}
		tmp = tmp->next;
	}
}
