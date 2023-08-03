
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>


int	lstsize(t_command *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
static int	check_overflow(unsigned long res, int n)
{
	if (n == 1)
		if (res >= 92233720368547758)
			return (-1);
	if (n == -1)
		if (res >= 92233720368547758)
			return (0);
	return (1);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		n;
	long	result;
	int		overflow;

	i = 0;
	n = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			n = -1;
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		overflow = check_overflow(result, n);
		if (overflow != 1)
			return (overflow);
		result = (result * 10) + str[i] - 48;
		i++;
	}
	return ((result * n));
}

void ft_echo(t_command *cmd)
{
	int i = 1;
	int n = 0;
	if (cmd->args[i] && ft_strcmp(cmd->args[i], "-n") == 0)
	{
		n = 1;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], 1);
		if (cmd->args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!n)
		ft_putstr_fd("\n", 1);
}


t_env * ft_lst_new( char *key,char *value)
{
	t_env *new;
	new = malloc(sizeof(t_env));
	if(!new)
		return(NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}
void update_or_add_env_var(t_env **p_env, char *key, char *value)
{
    t_env *tmp = *p_env;
  	 while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (value)
			{
				free(tmp->value);
				tmp->value = ft_strjoin(tmp->value, value);
				free(value);
				free(key);
			}
			break;
		}
		tmp = tmp->next;
	}
	if (tmp == NULL)
	{
		tmp = new_env(key, value);
		lstadd_back(p_env, tmp);
	}
}

void add_env_var(t_env **p_env, char *key, char *value)
{
	t_env *tmp = *p_env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (value)
			{
				free(tmp->value);
				tmp->value = ft_strdup(value);
				free(value);
				free(key);
			}
			break;
		}
		tmp = tmp->next;
	}
	if (tmp == NULL)
	{
		tmp = new_env(key, value);
		lstadd_back(p_env, tmp);
	}
}
int ft_isaplpha(char c)
{
	if((c >= 'a' && c <= 'z' )|| (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}
char *ft_strcpy(char *dest, char *src)
{
	int i = 0;
	while(src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int check_key(char *str, char *arg)
{
	char *p;
	if(*str >= '0' && *str <= '9')
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		exit_st.status = 1;
		return(45);
	}
	p = ft_strdup(str);
	while(*str)
	{
		if(!ft_isalnum(*str) && *str != '_')
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(arg, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			exit_st.status = 1;
			free(p);
			return (45);
		}
		str++;
	}
	return (0);
}
int _check(char *key,t_env **g_env)
{
	t_env *tmp = *g_env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
void create_key_value(char *arg, t_env **g_env)
{
	int j;
	t_env *tmp;
	char *key;
	char *value;

	tmp = *g_env;
	j = 0;
	if(*arg == '=' || *arg == '+' || *arg == '-')
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		exit_st.status = 1;
		ft_putendl_fd("': not a valid identifier", 2);
		return ;
	}
	while (arg[j] && (arg[j] != '=' && (arg[j] != '+')))
		j++;
	key = ft_substr(arg, 0, j);
	if(check_key(key,arg) == 45)
	{
		exit_st.status = 1;
		return ;
	}

	value = NULL;
	if (arg[j] == '+')
	{
		value = ft_substr(arg, j + 2, ft_strlen(arg) - j - 2);
		update_or_add_env_var(g_env, key, value);
	}
	else if (arg[j] == '=')
	{
		value = ft_substr(arg, j + 1, ft_strlen(arg) - j - 1);
		add_env_var(g_env, key, value);
	}
	if(value == NULL && _check(key,g_env) == 0)

		lstadd_back(g_env, new_env(key, NULL));


}

int check_for_equal(char *str)
{
	int i = 0;
	while(str[i])
	{
		if(str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void print_export(t_env **env_list)
{

	t_env *tmp = *env_list;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(tmp->key, 1);
		if(tmp->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(tmp->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
}

void ft_export(t_command *cmd, t_env **p_env)
{
	int i = 1;
	if(cmd->args[1] != NULL && !check_for_equal(cmd->args[1]))
	{
		if(check_key(cmd->args[1], cmd->args[1]) == 45)
		{
			exit_st.status = 1;
			return ;
		}
	}
	if(cmd->args[i] != NULL)
	{
		while (cmd->args[i] != NULL)
		{
			create_key_value(cmd->args[i], p_env);
			i++;
		}
	}
	if(cmd->args[1] == NULL)
		print_export(p_env);
}
void ft_cd(t_command *cmd , t_env **g_env)
{
	const char *home = getenv("HOME");
	char *path = getenv("PWD");
	t_env *tmp;
	tmp = *g_env;

	if (cmd->args[1] == NULL)
	{
		if (home == NULL)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return ;
		}
		path = ft_strdup(home);
	}
	else if (ft_strcmp(cmd->args[1], "~") == 0)
	{
		if(cmd->args[2])
			path = ft_strdup(cmd->args[2]);

		if (home == NULL)
		{
			home =  ft_strdup("");
			return ;
		}
		if(cmd->args[2] == NULL)
			path = ft_strdup(home);
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		if(!home)
		{
			ft_putstr_fd("minishell: cd: enviroment is unset \n", 2);
			return ;
		}
		if (path == NULL)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return ;
		}
		ft_putstr_fd(path, 1);
		ft_putstr_fd("\n", 1);
	}
	else
	{
		path = ft_strdup(cmd->args[1]);
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit_st.status = 1;
		return ;
	}
		tmp = *g_env;
	if (path)
	{
		while (tmp)
		{
			if(ft_strcmp(tmp->key, "PWD") == 0)
			{
				free(tmp->value);
				tmp->value = ft_strdup(path);
			}
			tmp = tmp->next;
		}
	}

}

void ft_pwd()
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd() error");
}

void ft_env(t_command *cmd, t_env **g_env)
{
	(void)cmd;
	t_env *tmp = *g_env;
	while (tmp)
	{
		if(tmp->value != NULL)
		{
			ft_putstr_fd(tmp->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(tmp->value, 1);
			ft_putstr_fd("\n", 1);
		}
		tmp = tmp->next;
	}
}
int ft_isdigit(int c)
{
	if(c >= '0' && c <= '9')
		return (1);
	return (0);
}

void ft_exit(t_command *cmd)
{
	if(cmd->args[1] == NULL)
		exit (0);
	else if(cmd->args[1] != NULL)
	{
		if(ft_isdigit(cmd->args[1][0]) == 0  && (cmd->args[1][0] != '-' && cmd->args[1][0] != '+') )
		{
				ft_putstr_fd("exit\n", 1);
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(cmd->args[1], 2);
				exit_st.status = 255;
				ft_putstr_fd(": numeric argument required\n", 2);
				exit(255);
		}
		else if(ft_isdigit(cmd->args[1][0]) == 1 || (cmd->args[1][0] == '-' || cmd->args[1][0] == '+'))
		{
			if(cmd->args[2] != NULL)
			{
				ft_putstr_fd("exit\n", 1);
				exit_st.status = 1;
				ft_putstr_fd("minishell: exit: too many arguments\n", 2);
				exit(1);

			}
			else
			{
				ft_putstr_fd("exit\n", 1);
				exit_st.status = ft_atoi(cmd->args[1]);
				exit(ft_atoi(cmd->args[1]));
			}
		}
	}
}


void ft_unset(t_command *cmd, t_env **g_env)
{
	t_env *tmp = *g_env;
	extern char **environ;
	t_env *prev = NULL;
	int i = 1;

	if(cmd->args[1] && !ft_strcmp(cmd->args[1], "PATH"))
	{
		exit_st.path = ft_strdup("");
		exit_st.is_unset = 1;
	}
	while (cmd->args[i])
	{
		tmp = *g_env;
		while (tmp)
		{
			if (ft_strcmp(tmp->key, cmd->args[i]) == 0)
			{
				if (prev == NULL)
					*g_env = tmp->next;
				else
					prev->next = tmp->next;
				free(tmp->key);
				free(tmp->value);
				free(tmp);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
}
char **path_splitted(t_env **g_env)
{
	extern char **environ;
	int		i;
	char	**paths;
	t_env	*tmp;
	tmp = *g_env;

	i = 0;
	if(!*environ && !exit_st.is_unset)
		exit_st.path =ft_strdup("/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	while(tmp)
	{
		if(ft_strcmp(tmp->key, "PATH") == 0)
			exit_st.path = ft_strdup(tmp->value);
		tmp = tmp->next;
	}
	if(!exit_st.path)
		exit_st.path = ft_strdup(" ");
	// printf("%s\n", exit_st.path);
	paths = ft_split(exit_st.path, ':');
	return(paths);

}

void norm_help(t_command *cmd , int j ,int existance ,char c)
{
	if(cmd->args[0][j] == c)
	{
		if(cmd->args[0][j + 1] == '/' || cmd->args[0][j + 1] == '.')
		{
			if(existance == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd->args[0], 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				exit(127);
			}
		}

	}
}
void _manipulate_files(int permission, int existance , t_command *cmd)
{
	int j;

	j = 0;
	while(cmd->args[0][j])
	{
		if(!existance && permission == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
		norm_help(cmd,j,existance,'/');
		norm_help(cmd,j,existance,'.');
		j++;
	}
}

int check_filepermission_ndexistance(t_command *cmd)
{
	int	file_existance;
	int	file_permission;

	file_existance = access(cmd->args[0], F_OK);
	file_permission = access(cmd->args[0], X_OK);

	if((cmd->args[0][0] == '.' || cmd->args[0][0] == '/'))
	{
		_manipulate_files(file_permission,file_existance,cmd);
		return (1);
	}
	return (0);
}

void execute_bin(t_command *cmd ,char **envi, t_env **g_env)
{
	char	*lwa = ft_strdup("");
	extern char **environ;
	int		i;
	char **paths;
	i = 0;

	paths = path_splitted(g_env);
		while(paths[i])
		{
			// printf("number %d of %s\n", i, paths[i]);
			char *tmp = ft_strjoin(paths[i],"/");
			tmp = ft_strjoin(tmp, cmd->args[0]);
			if (access(tmp, F_OK| X_OK) == 0)
				lwa = ft_strdup(tmp);
			if(check_filepermission_ndexistance(cmd))
				lwa = ft_strdup(cmd->args[0]);
			i++;
		}

		if(execve(lwa, cmd->args,envi) == -1)
		{
			exit_st.status = 127;
			if(!*environ || !ft_strcmp(exit_st.path, " "))
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd->args[0], 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				exit(127);
			}
			if(check_filepermission_ndexistance(cmd) && access(cmd->args[0], F_OK | X_OK) == 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd->args[0], 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				exit(126);
			}
			lwa = cmd->args[0];
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(lwa, 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(exit_st.status);
		}
}
int checkfor_builtins(t_command *cmd)
{
	if(ft_strcmp(cmd->args[0], "echo") == 0)
		return(1);
	else if(ft_strcmp(cmd->args[0], "cd") == 0)
		return(1);
	else if(ft_strcmp(cmd->args[0], "export") == 0)
		return(1);
	else if(ft_strcmp(cmd->args[0], "pwd") == 0)
		return(1);
	else if(ft_strcmp(cmd->args[0], "env") == 0)
		return(1);
	else if(ft_strcmp(cmd->args[0], "unset") == 0)
		return(1);
	else if(ft_strcmp(cmd->args[0], "exit") == 0)
		return(1);
	return(0);

}
int execute_built_ins(t_command *cmd, t_env **envp)
{
	int in, out;
		if(cmd->fd.fd_in != 0)
		{
			if(cmd->fd.fd_in == -1)
			{
				exit_st.status = 1;
				return (1);
			}
			in = dup(0);
			dup2(cmd->fd.fd_in, 0);
			close(cmd->fd.fd_in);
		}
		if(cmd->fd.fd_out != 1)
		{
			if(cmd->fd.fd_in == -1)
			{
				exit_st.status = 1;
				return (1);
			}
			out = dup(1);
			dup2(cmd->fd.fd_out, 1);
			close(cmd->fd.fd_out);
		}
	if(ft_strcmp(cmd->args[0], "echo") == 0)
		ft_echo(cmd);
	else if(ft_strcmp(cmd->args[0], "cd") == 0 && lstsize(cmd) == 1)
		ft_cd(cmd, envp);
	else if(ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_pwd();
	else if(ft_strcmp(cmd->args[0], "export") == 0)
			ft_export(cmd, envp);
	else if(ft_strcmp(cmd->args[0], "env") == 0)
		ft_env(cmd, envp);
	else if(ft_strcmp(cmd->args[0], "unset") == 0)
		ft_unset(cmd, envp);
	else if(ft_strcmp(cmd->args[0], "exit") == 0)
		ft_exit(cmd);
	if (cmd->fd.fd_in != 0)
		{
			dup2(in, 0);
			close(in);
		}
		if (cmd->fd.fd_out != 1)
		{
			dup2(out, 1);
			close(out);
		}
	return (1);
}

int ft_lst_size(t_command *cmd)
{
	int i =0;
	while(cmd)
	{
		i++;
		cmd = cmd->next;
	}
	return i;
}

void signal_handler(int signal) {
    if(signal == SIGINT)
	{

	}
	if(signal == SIGQUIT)
	;
}

void handle_child_process(t_command *cmd, int *fd ,int old)
{
		close(fd[0]);
		if (cmd->next)
		{
			dup2(fd[1],1);
			close(fd[1]);
		}
		if(cmd->fd.fd_out != 1)
		{
			if(cmd->fd.fd_out == -1)
			{
				exit_st.status = 1;
				exit(1);
			}
			dup2(cmd->fd.fd_out,1);
			close(cmd->fd.fd_out);
		}
		if (old != -1)
		{
			dup2(old, 0);
			close(old);
		}
		if(cmd->fd.fd_in != 0)
		{
			if(cmd->fd.fd_in == -1)
			{
				exit_st.status = 1;
				exit(1);
			}
			dup2(cmd->fd.fd_in,0);
			close(cmd->fd.fd_in);
		}


}
void protection(int *fd)
{
	if (pipe(fd) != 0)
	{
		perror("failed to create a pipe");
		return;
	}
}


int execute_the_shOt(t_command* cmd,t_env **g_env, char **envp)
{
	int old;
	int a =0;
	t_command *tmp = NULL;
	tmp = cmd;
	int fd[2] = {-1,-1};
	if(!cmd->args[0])
	{
		if(!cmd->next)
			return (0);
	}
	if (!cmd->next)
	{
		a = checkfor_builtins(cmd);
		if(a == 1 )
			execute_built_ins(cmd,g_env);
		if(a == 0)
		{
			cmd->pid = fork();
			if(cmd->pid == 0)
			{
				if(cmd->fd.fd_out != 1)
				{
					if(cmd->fd.fd_out == -1)
					{
						exit_st.status = 1;
						exit(1);
					}
					dup2(cmd->fd.fd_out,1);
					close(cmd->fd.fd_out);
				}
				if(cmd->fd.fd_in != 0)
				{
					if(cmd->fd.fd_in == -1)
					{
						exit_st.status = 1;
						exit(1);
					}
					dup2(cmd->fd.fd_in,0);
					close(cmd->fd.fd_in);
				}
				execute_bin(cmd, envp, g_env);
			}
			if(cmd->pid > 0)
			{
				int a = 0;
				waitpid(cmd->pid, &a, 0);
				if(a != 0)
					exit_st.status = a / 256;
				else
					exit_st.status = 0;
			}
			else
			{
				perror("fork() failed");
				return (1);
			}
			}
		return (0);
	}
	while (cmd)
	{
		if(cmd->args[0])
			a = checkfor_builtins(cmd);
		old = fd[0]; // -1;
		protection(fd);
		cmd->pid = fork();
		if (cmd->pid == 0)
		{
				handle_child_process(cmd,fd,old);

			if(a == 1)
				execute_built_ins(cmd,g_env);
			if(a == 0 )
				execute_bin(cmd, envp, g_env);
			exit(exit_st.status);
		}
		else if (cmd->pid > 0)
		{
			close(fd[1]);
			if (old != -1)
				close(old);
		}
		else
		{
			perror("fork() failed");
			return (1);
		}

		cmd = cmd->next;
	}
	close(fd[1]);
	int s = 0;
	while(tmp)
	{
		waitpid(tmp->pid, &s, 0);
		tmp = tmp->next;
	}
	if(s != 0)
		exit_st.status =s / 256;
	else
		exit_st.status = 0;
	return (0);
}







