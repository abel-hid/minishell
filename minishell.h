/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 16:47:00 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/01 15:06:30 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <signal.h>

typedef enum e_tokens
{
	WORD,
	PIPE_LINE ,
	REDIR_IN ,
	REDIR_OUT,
	HEARDOC ,
	APPEND ,
} 	t_tokens;

typedef struct s_fd
{
	int		fd_in;
	int		fd_out;
}	t_fd;


typedef struct s_command
{
	char	**args;
	t_fd	fd;
	pid_t	pid;
	struct s_command	*next;
}	t_command;

typedef struct s_lexer
{
	char    	*content;
	t_tokens        token;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}	t_env;

//  typedef struct s_exit
// {
// 	int		status;
// }	t_exit;

// t_exit  *exit_st;
int   exit_status;
int lexing(t_lexer **list, char *line);
void craete_env(char **env_list, t_env **g_env);
char **ft_split(char const *s, char c);
char	*ft_strdup(const char *s1);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlen(const char *s);
int	ft_strncmp( char *s1, char *s2, size_t n);
void ft_lstadd_back(t_lexer **lst, t_lexer *new);
t_lexer *ft_lstnew(char *content, t_tokens type);
char *ft_strncpy(char *s1, char *s2, int n);

char *expand_variables(t_lexer **list, t_env **p_env);
void expand(t_lexer **list, t_env **g_env);
t_command **srfak_lban(t_lexer **list);

void parsing(t_lexer **list, t_command **cmd, t_env **g_env);
char	*ft_strdup(const char *s1);
int skip_spaces(char *line, int i);
char *ft_strjoin(char *s1, char *s2);
void heredoc(t_lexer **lexer, t_env **g_env);
int ft_strcmp(const char *s1, const char *s2);

int is_spaces(char c);
char *handler(char *str, int *i, t_env **g_env);
char *handler_value(char *str, int *i,int j, char *value);
int is_digit(char c);
char *ft_expand(char *str, t_env **g_env);
char *get_env_value(t_env *env, const char *key);
char *del_quote(char *str, char c, char c2);
int      quote_check(char *line);
void free_lexer_list(t_lexer **lst);

t_env *new_env(char *key, char *value);
void lstadd_back(t_env **lst, t_env *new);
int	ft_strchr(char *s, int c);
void craete_env(char **env_list, t_env **g_env);
char *get_env_value(t_env *env, const char *key);
void ft_putendl_fd(char *s, int fd);
void ft_putstr_fd(char *s, int fd);
void signal_handler(int sig);
int hh(char *str);
int detect_dollar(char *str);
char	**ft_split1(char *str, char *charset);
void	free_args(char **s);
int check_space(char *line);
int	count_strings(char *str, char *charset);
int	detect_dollar(char *str);
int	hh(char *str);
void free_cmd_list(t_command **lst);
char	*ft_expand(char *str, t_env **g_env);
char *expand_heredoc(char *str, t_env **g_env);
char	*ft_itoa(int n);
int is_dquote(char *str);
char *ft_delete(char *str);
void parse_args(t_lexer **list,t_command **cmd,  t_env **g_env);
int execute_the_shOt(t_command* cmd,t_env **g_env, char **envp);
int execute(t_command* cmd, t_env* g_env);
int	ft_isalnum(int c);
#endif
