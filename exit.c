#include "minishell.h"

void set_exit_status(t_exit *exit_status, int status)
{
    exit_status->status = status;
    exit_status->is_exit = 1;
}
