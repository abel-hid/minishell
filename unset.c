#include "minishell.h"
int parsing_unset(char *str)
{
	int i = 0;
	while(str[i])
	{
		if(ft_isalnum(str[i]) == 0 && str[i] != '_')
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(str, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			exit_st.status = 1;
			return (1);
		}
		i++;
	}
	return (0);
}
void check_for_path(t_command *cmd)
{
   if(cmd->args[1] && !ft_strcmp(cmd->args[1], "PATH")  && !exit_st.is_unset)
	{
		free(exit_st.path);
		exit_st.is_unset = 1;
	}
}
void    unset_thedummy(t_env **g_env,t_command *cmd, int i, t_env *prev)
{
    t_env *tmp ;
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
   
}


void ft_unset(t_command *cmd, t_env **g_env)
{
	t_env *prev = NULL;
	int i = 1;

   
    check_for_path(cmd);
	while (cmd->args[i])
	{
		
		if(parsing_unset(cmd->args[i]) == 1)
			return ;
        unset_thedummy(g_env,cmd,i,prev);
		i++;
	}
}
