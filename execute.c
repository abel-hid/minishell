
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
		exit_status = 1;
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
			exit_status = 1;
			free(p);
			return (45);
		}
		str++;
	}
	return (0);
}
void create_key_value(char *arg, char **key, char **value, t_env **g_env)
{
	int j;
	t_env *tmp;

	tmp = *g_env;
	j = 0;
	if(*arg == '=' || *arg == '+' || *arg == '-')
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return ;
	}
	while (arg[j] && (arg[j] != '=' && (arg[j] != '+')))
		j++;
	*key = ft_substr(arg, 0, j);
	if(check_key(*key,arg) == 45)
		return ;
	*value = NULL;
	if (arg[j] == '+')
	{
		*value = ft_substr(arg, j + 2, ft_strlen(arg) - j - 2);
		update_or_add_env_var(g_env, *key, *value);
	}
	else if (arg[j] == '=')
	{
		*value = ft_substr(arg, j + 1, ft_strlen(arg) - j - 1);
		add_env_var(g_env, *key, *value);
	}
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
void haha(t_env **env_list, char *str)
{
	lstadd_back(env_list,new_env(ft_strdup(str),NULL));
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




void ft_export(t_command *cmd, t_env **p_env, t_env **env_list)
{
	int i = 1;
	char *key;
	char *value;

	if(cmd->args[1] != NULL && !check_for_equal(cmd->args[1]))
	{
		if(check_key(cmd->args[1], cmd->args[1]) == 45)
			return ;
		haha(env_list, cmd->args[1]);
	}
	if(cmd->args[i] != NULL && check_for_equal(cmd->args[i]))
	{
		while (cmd->args[i] != NULL && check_for_equal(cmd->args[1]))
		{
			create_key_value(cmd->args[i], &key, &value, p_env);
			i++;
		}
	}
}
void ft_cd(t_command *cmd)
{
	const char *home;
	home = getenv("HOME");


	char *path;
	if (cmd->args[1] == NULL)
	{
		if (home == NULL)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 1);
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
			ft_putstr_fd("minishell: cd: HOME not set\n", 1);
			return ;
		}
		if(cmd->args[2] == NULL)
			path = ft_strdup(home);
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		if(!home)
		{
			ft_putstr_fd("minishell: cd: enviroment is unset \n", 1);
			return ;
		}
		path = ft_strdup(getenv("OLDPWD"));
		if (path == NULL)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 1);
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
		ft_putstr_fd("minishell: cd: ", 1);
		ft_putstr_fd(cmd->args[1], 1);
		ft_putstr_fd(": No such file or directory\n", 1);
		return ;
	}
	if(path)
		free(path);
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
		ft_putstr_fd(tmp->key, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(tmp->value, 1);
		ft_putstr_fd("\n", 1);
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
		if(ft_isdigit(cmd->args[1][0]) == 0)
		{
			printf("minishell: exit: %s: numeric argument required\n", cmd->args[1]);
			return ;
		}
		else if(ft_isdigit(cmd->args[1][0]) == 1)
		{
			if(cmd->args[2] != NULL)
			{
				printf("minishell: exit: too many arguments\n");
				return ;
			}
			else
			{
				printf("exit\n");
				exit(atoi(cmd->args[1]));
			}
		}
	}
}


void ft_unset(t_command *cmd, t_env **g_env)
{
	t_env *tmp = *g_env;
	t_env *prev = NULL;
	int i = 1;
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
char **path_splitted()
{
	extern char **environ;
	int		i;
	char	*path;
	char	**paths;

	i = 0;
	if(!*environ)
		path = ft_strdup("/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:");
	while (environ[i])
	{
		if (ft_strncmp(environ[i], "PATH=", 5) == 0)
			path = ft_strdup(environ[i] + 5);
		i++;
	}
	paths = ft_split(path, ':');
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
				printf("minishell: %s: No such file or directory\n", cmd->args[0]);
				exit(1);
			}
			else
			{
				printf("minishell : %s is a directory\n", cmd->args[0]);
				exit(1);
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
			printf("minishell: %s: Permission denied\n", cmd->args[0]);
			exit(0);
		}
		norm_help(cmd,j,existance,'/');
		norm_help(cmd,j,existance,'.');
		j++;
	}
}

void check_filepermission_ndexistance(t_command *cmd)
{
	int	file_existance;
	int	file_permission;

	file_existance = access(cmd->args[0], F_OK);
	file_permission = access(cmd->args[0], X_OK);

	if((cmd->args[0][0] == '.' || cmd->args[0][0] == '/'))
		_manipulate_files(file_permission,file_existance,cmd);
}

void execute_builtins(t_command *cmd ,char **envi)
{
	char	*lwa;
	int		i;
	char **paths;
	i = 0;

	paths = path_splitted();
		while(paths[i] )
		{
			check_filepermission_ndexistance(cmd);
			char *tmp = ft_strjoin(paths[i],"/");
			tmp = ft_strjoin(tmp, cmd->args[0]);

			if (access(tmp, F_OK ) == 0)
				lwa = tmp;
			i++;
		}
		if(execve(lwa, cmd->args,envi) == -1)
		{
			lwa = cmd->args[0];
			exit_status = 127;
			printf("minishell: %s: command not found\n",lwa);
			exit(exit_status);
		}
		// else
		// {
		// 	exit_status = 0;
		// 	exit(0);
		// }
}
int checkfor_builtins(t_command *cmd)
{
	if(ft_strcmp(cmd->args[0], "echo") == 0)
		return(44);
	else if(ft_strcmp(cmd->args[0], "cd") == 0)
		return(77);
	else if(ft_strcmp(cmd->args[0], "export") == 0)
		return(44);
	else if(ft_strcmp(cmd->args[0], "pwd") == 0)
		return(44);
	else if(ft_strcmp(cmd->args[0], "env") == 0)
		return(44);
	else if(ft_strcmp(cmd->args[0], "unset") == 0)
		return(44);
	else if(ft_strcmp(cmd->args[0], "exit") == 0)
		return(44);	return(0);

}
int execute_built_ins(t_command *cmd, t_env **envp ,t_env **env_list)
{

	if(ft_strcmp(cmd->args[0], "echo") == 0)
		ft_echo(cmd);
	else if(ft_strcmp(cmd->args[0], "cd") == 0)
		ft_cd(cmd);
	else if(ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_pwd();
	else if(ft_strcmp(cmd->args[0], "export") == 0)
	{
		if(cmd->args[1] != NULL)
			ft_export(cmd, envp, env_list);
		else
			print_export(env_list);
	}
	else if(ft_strcmp(cmd->args[0], "env") == 0)
		ft_env(cmd, envp);
	else if(ft_strcmp(cmd->args[0], "unset") == 0)
		ft_unset(cmd, envp);
	else if(ft_strcmp(cmd->args[0], "exit") == 0)
		ft_exit(cmd);
	return (0);


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
}

void handle_child_process(t_command *cmd, int *fd ,int old)
{
		close(fd[0]);
			if (cmd->next)
			{
				dup2(fd[1],1);
				close(fd[1]);
			}

			if(cmd->fd.fd_out != 1 )
			{
				if(cmd->fd.fd_out == -1)
				{
					exit_status = 1;
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
					exit_status = 1;
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


int execute_the_shOt(t_command* cmd,t_env **g_env, char **envp, t_env **env_list)
{
	int old;
	t_command *tmp = NULL;
	tmp = cmd;
	int fd[2] = {-1,-1};

			if(!cmd)
				return (77);
	while (cmd && cmd->args[0])
	{
		int a = checkfor_builtins(cmd);
		old = fd[0]; // -1;
		protection(fd);
		if(lstsize(cmd) == 1)
			execute_built_ins(cmd,g_env,env_list);
		cmd->pid = fork();
		if (cmd->pid == 0)
		{
			handle_child_process(cmd,fd,old);
			if(lstsize(cmd)> 1)
				execute_built_ins(cmd,g_env,env_list);
			if(a == 0 && a != 77)
				execute_builtins(cmd, envp);
				exit(0);
		}
		else if (cmd->pid > 0)
		{
			int a;
			waitpid(cmd->pid, &a, 0);
			if(a != 0)
				exit_status = a / 256;
			else
				exit_status = 0;
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
	return (0);
}








