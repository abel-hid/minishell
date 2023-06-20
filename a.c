#include "minishell.h"


void shell_syntax(char *line , t_list **list)
{
	int i;
	int j;
	char *p;

	i = 0;
	// printf("line = %s\n", line);

	while(line[i])
	{
		if(line[i] == '\'')
		{
			j = i;
			while(line[i] != '\'')
				i++;
			p = ftft(line, j, i - j);
			add_token(list, p, WORD);
		}
		else if(line[i] == ' ')
		{
			while(line[i] == ' ')
				i++;
		}
		else if(line[i] == '|')
		{
			add_token(list, "|", PIPE_LINE);
		}
		if(line[i] == '>')
		{
			if(line[i + 1] == '>')
			{
				add_token(list, ">>", APPEND);
				i++;
			}
			else
				add_token(list, ">", REDIR_OUT);
		}

		else if(line[i] == '<')
		{
			if(line[i + 1] == '<')
			{
				add_token(list, "<<", HEARDOC);
				i++;
			}
			else
				add_token(list, "<", REDIR_IN);
		}

		else if(line[i] == '\"')
		{
			j = i;
			i++;
			while(line[i] != '\"')
				i++;
			p = ftft(line, j, i - j);
			add_token(list, p, WORD);
		}
		else if(line[i] == '\'')
		{
			j = i;
			i++;
			while(line[i] != '\'')
				i++;
			p = ftft(line, j, i - j);
			add_token(list, p, WORD);
		}
		i++;
	}
}


