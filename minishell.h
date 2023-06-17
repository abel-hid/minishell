
#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

enum state
{
	IN_DQUOTE,
	IN_SQUOTE,

	GENERAL,
};

enum token
{
	WORD,
	WHITE_SPACE ,
	NEW_LINE,
	QUOTE ,
	DOUBLE_QUOTE ,
	ENV,
	PIPE_LINE ,
	REDIR_IN ,
	REDIR_OUT,
	AND ,
	HEARDOC ,
	APPEND ,
} ;

typedef struct s_list
{
    char			*content;
    enum token        token;
	enum state		state;
    struct s_list	*next;
}					t_list;

char **ft_split(char const *s, char c);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
char	*ft_strdup(const char *s1);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlen(const char *s);
t_list	*ft_lstnew(char *content, enum token type, enum state state);
void add_token(t_list **list, char *token, enum token type, enum state state);
void syntax_check(t_list **list);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
void valid_line(char *line);
# endif
