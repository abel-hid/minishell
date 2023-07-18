#include "minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	while (s1[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void ft_putendl_fd(char *s, int fd)
{
	int i;

	i = 0;
	if (!s)
		return ;
	while(s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}
void ft_putstr_fd(char *s, int fd)
{
	int i;

	i = 0;
	if (!s)
		return ;
	while(s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}
