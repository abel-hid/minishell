
#include "minishell.h"

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

void ft_export(t_command *cmd, t_env **p_env)
{
	int i = 1;
	char *key;
	char *value;
	t_env *tmp = *p_env;

	while (cmd->args[i] != NULL && ft_strchr(cmd->args[i], '='))
	{
		int j = 0;
		while (cmd->args[i][j] && cmd->args[i][j] != '=')
			j++;

		key = ft_substr(cmd->args[i], 0, j);
		value = NULL;
		if (cmd->args[i][j] == '=')
			value = ft_substr(cmd->args[i], j + 1, ft_strlen(cmd->args[i]) - j - 1);
		while (tmp)
		{
			if (ft_strcmp(tmp->key, key) == 0)
			{
				if (value)
				{
					free(tmp->value);
					tmp->value = ft_strdup(value);
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

		i++;
	}
}

void ft_cd(t_command *cmd)
{
	char *path;
	// char *old = getcwd(NULL, 0);

	path = cmd->args[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 1);
		ft_putstr_fd(path, 1);
		ft_putstr_fd(": No such file or directory\n", 1);
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
		ft_putstr_fd(tmp->key, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(tmp->value, 1);
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
}

void ft_exit()
{
	exit(0);
}


void ft_unset(t_command *cmd, t_env **g_env)
{
	int i = 1;
	t_env *tmp;
	t_env *prev;
	while (cmd->args[i])
	{
		tmp = *g_env;
		prev = NULL;
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

int execute_builtins(t_command *cmd ,char **envi)
{
	char *path;
	char *lwa;
	extern char **environ;
	int i = 0;
		while(environ[i])
		{
			if(ft_strncmp(environ[i], "PATH=", 5) == 0)
				path = ft_strdup(environ[i] + 5);
			i++;
		}
		i =0;
		char **paths ;
		paths = ft_split(path, ':');
		while(paths[i] )
		{

				int file_existance = access(cmd->args[0], F_OK);
				int file_permission = access(cmd->args[0], X_OK);
					if((cmd->args[0][0] == '.' || cmd->args[0][0] == '/'))
					{
						int j = 0;
						if(file_permission == 0 && file_existance == 0)
						{
							int fd = open(cmd->args[0], O_RDONLY);
							char *str = get_next_line(fd);
							printf("%s\n", str);
						}
						while(cmd->args[0][j])
						{
							if(!file_existance && file_permission == -1)
							{
								printf("minishell: %s: Permission denied\n", cmd->args[0]);
								return (1);
								exit(0);
							}
							else if(cmd->args[0][j] == '/')
							{
								 if(file_existance == -1 )
								{
									printf("minishell: %s: No such file or directory\n", cmd->args[0]);
									return(10);
									
								}
								if(cmd->args[0][j + 1] == '/')
								{
									printf("minishell : %s is a directory\n", cmd->args[0]);
									return (1);
								}
							}
					
							else if(cmd->args[0][j] == '.')
							{
								 if(file_existance == -1)
								{
									printf("minishell: %s: No such file or directory\n", cmd->args[0]);
									return(10);
									
								}
								if(cmd->args[0][j + 1] == '/')
								{
									printf("minishell : %s is a directory\n", cmd->args[0]);
									return (1);
								}
							}
					
						j++;
						
					}
					
					
				if(execve(cmd->args[0], cmd->args,envi) == -1)
				{
					printf("minishell: %s: command not found\n", cmd->args[0]);
					// printf("cmd->args[0] = %s\n", cmd->args[0]);
					return (1);
					exit(0);
				}
				}
			char *tmp = ft_strjoin(paths[i],"/");
			tmp = ft_strjoin(tmp, cmd->args[0]);
			if (access(tmp, F_OK | X_OK) == 0)
			{
				lwa = tmp;
				break;
			}
			i++;
		}
		 if(execve(lwa, cmd->args,envi) == -1)
		{
				printf("minishell: %s: command not found\n", cmd->args[0]);
				printf("cmd->args[0] = %s\n", cmd->args[0]);
				return (1);
				exit(0);
		}
		return (0);
			
	
}
void execute_built_ins(t_command *cmd, t_env *envp)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		ft_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		ft_export(cmd, &envp);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		ft_unset(cmd, &envp);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_env(cmd, &envp);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		ft_exit();
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

void signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		//This function call is likely used to move the cursor to a new line.
		//It ensures that the next output or user input will start on a new line
		// rl_replace_line("", 0);
		//eplaces the current input line with an empty string
		rl_redisplay();
		//After modifying the input line, this function is called to refresh the display
	}

	if(sig == SIGQUIT)
	;
}


int execute_the_shOt(t_command* cmd,t_env *g_env, char **envp)
{
	int old;
	t_command *tmp =NULL;
	tmp = cmd;
	
	int fd[2] = {-1,-1};
			if(!cmd)
				return (77);
	while (cmd)
	{
		execute_built_ins(cmd, g_env);
		old = fd[0]; // -1;
		if (pipe(fd) != 0)      
		{
			perror("failed to create a pipe");
			return(2);
		}
		cmd->pid = fork();
		if (cmd->pid == 0)      
		{
			close(fd[0]);
			if (cmd->next)
			{
				dup2(fd[1],1);
				close(fd[1]);
			}
			if(cmd->fd.fd_out != 1)
			{
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
				dup2(cmd->fd.fd_in,0);
				close(cmd->fd.fd_in);	
			}
				execute_builtins(cmd, envp);
			
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
	while(tmp)
	{
		waitpid(tmp->pid,NULL,0);
		tmp = tmp->next;
	}
	return (0);               
}









