
#include "minishell.h"

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

void execute_builtins(t_command *cmd, t_env **g_env)
{
	char *path;
	path = NULL;

	if (ft_strcmp(cmd->args[0], "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
	{
		if (cmd->args[1] != NULL)
			ft_cd(cmd);
	}
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->args[0], "export") == 0)
	{
		if (cmd->args[1] != NULL)
			ft_export(cmd, g_env);
	}
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
	{
		exit(0);
		if (cmd->args[1] != NULL)
			ft_unset(cmd, g_env);
	}
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_env(cmd, g_env);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
	{
		exit(0);
		ft_exit();
	}
	else
	{
		path = "/bin/";
		path = ft_strjoin(path, cmd->args[0]);
		if(execve(path, cmd->args, NULL) == -1)
		{
			ft_putstr_fd("minishell: ", 1);
			ft_putstr_fd(cmd->args[0], 1);
			ft_putstr_fd(": command not found\n", 1);
			exit(1);
		}
	}
	free(path);
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
	{


	}
}


int execute(t_command* cmd, t_env* g_env)
{
	int pipe_fd[2];
	pid_t pid;

	while (cmd != NULL && cmd->args[0] != NULL)
	{
		if (cmd->next != NULL)
		{
			if (pipe(pipe_fd) < 0)
			{
				perror("pipe");
				exit(1);
			}
			pipe_fd[1] = 1;
			pipe_fd[0] = 0;
		}

		pid = fork();
		if (pid == 0)
		{
			if (cmd->fd.fd_in != 0)
			{
				dup2(cmd->fd.fd_in, 0);
				close(cmd->fd.fd_in);
			}
			if (cmd->fd.fd_out != 1)
			{
				dup2(cmd->fd.fd_out, 1);
				close(cmd->fd.fd_out);
			}
			if (cmd->next != NULL)
			{
				dup2(pipe_fd[1], 1);
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			execute_builtins(cmd, &g_env);
			exit(0);
		}
		else if (pid > 0)
		{
			waitpid(pid, NULL, 0);
			if (cmd->fd.fd_in != 0)
			{
				close(cmd->fd.fd_in);
			}
			if (cmd->fd.fd_out != 1)
			{
				close(cmd->fd.fd_out);
			}

			if(ft_strcmp(cmd->args[0], "exit") == 0)
				exit(0);
			if(ft_strcmp(cmd->args[0], "cd") == 0)
				ft_cd(cmd);
			if(ft_strcmp(cmd->args[0], "export") == 0)
				ft_export(cmd, &g_env);
			if(ft_strcmp(cmd->args[0], "unset") == 0)
				ft_unset(cmd, &g_env);

		}
		else
		{
			perror("fork");
			exit(1);
		}

		cmd = cmd->next;
	}

	return (0);
}








