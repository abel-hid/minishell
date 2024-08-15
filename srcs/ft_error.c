/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:36:37 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/05 15:36:45 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(char *str, char *str2, char *str3)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd(str2, 2);
	ft_putstr_fd(str3, 2);
}
