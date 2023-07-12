#include "minishell.h"


int help(t_lexer *list)
{
	t_lexer *tmp;
	tmp = list;

	while (tmp)
	{
		if(tmp->token == HEARDOC && ((tmp->prev->token == WORD) || (tmp->prev->token == WORD)))
		{
			return (1);
		}
		printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
	return (0);
}

void parsing(t_lexer** list, t_command* cmd)
{
	t_lexer* tmp = *list;
	char* line;
	ssize_t size;
	int fd;
	char str[1000];
	(void)cmd;


	while (tmp)
	{
		if (tmp->token == HEARDOC)
		{
			fd = open("tmp.txt", O_CREAT | O_RDWR );
			while (1)
			{
				line = readline("heredoc>");
				if (line == NULL)
					break;
				if (strcmp(line, tmp->next->content) == 0)
				{
					free(line);
					break;
				}

				write(fd, line, strlen(line));
				write(fd, "\n", 1);
				free(line);
			}

			close(fd);
			fd = open("tmp.txt", O_RDONLY);
			size = read(fd, str, sizeof(str) - 1);
			str[size] = '\0';
			printf("%s\n", str);
			close(fd);
		}
		tmp = tmp->next;
	}
}




