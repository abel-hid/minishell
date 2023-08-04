/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 16:47:00 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/04 21:37:19 by abel-hid         ###   ########.fr       */
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

 typedef struct s_exit
{
	int		status;
	char *path;
	int is_unset;
	int ambigous;
}	t_exit;

t_exit  exit_st;
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
int	get_quote_length(char *line, int i, char quote_type);
int	is_quote_end(char c, char quote_type);
int	is_quote(char c);
int	skip_spaces(char *line, int i);
int	is_token(char c);
char *expand_variables(t_lexer **list, t_env **p_env);
void expand(t_lexer **list, t_env **g_env);
t_command **srfak_lban(t_lexer **list);
char	*my_strjoin(char *s1, char *s2);
void	add_token(t_lexer **list, char *line, t_tokens type);
int	check_dollar(char *str);
int	is_space(char c);
int	help_token(t_lexer **list);
int	pipe_line_syntax(t_lexer **list);
int	redir_syntax(t_lexer **list);
int	quote_syntax(t_lexer **list);
void parsing(t_lexer **list, t_command **cmd, t_env **g_env);
int	check_digit(t_lexer *list);
char	*delete_quote(char *str, char c);
int	skip_squote(char *str, int i);
void	delete_dollar(char *str);
int	detect_dollar(char *str);
char	*ft_strdup(const char *s1);
int skip_spaces(char *line, int i);
char *ft_strjoin(char *s1, char *s2);
void heredoc(t_lexer **lexer, t_env **g_env);
int ft_strcmp(const char *s1, const char *s2);
int	quote_error(char *str);
int	is_qoute(char *line, int i, char c);
void	create_lexer(char *line, t_lexer **list);
int	get_token(char *line, int i, t_lexer **list);
int	add_redir_out(char *line, int i, t_lexer **list);
int is_spaces(char c);
char *handler(char *str, int *i, t_env **g_env);
char *handler_value(char *str, int *i,int j, char *value);
int is_digit(char c);
char *ft_expand(char *str, t_env **g_env);
char *get_env_value(t_env *env, const char *key);
char *del_quote(char *str, char c, char c2);
int      quote_check(char *line);
int	get_word(char *line, int i, t_lexer **list);
void free_lexer_list(t_lexer **lst);
void	my_lstadd_back(t_command **lst, t_command *new);
t_command	*ft_new(char **args, t_fd fd);
int	norm(char *str, int i);
int	more_than_one(char *str);
char	*ft_delete(char *str);
t_fd	parse_redir(int type, char *str_next, t_fd fd, t_env **g_env);
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
void	error(char *str);
void	error_ambiguous(char *str);
int	ambiguous_redirect(char *str, t_env **g_env, char *str_next);
int	ft_isalnum(int c);
int	is_env(t_env **g_env, char *str);
char	*add_args(char *str);
char	**get_p(char *str);
int	calculate_args(t_lexer *tmp);
#endif
