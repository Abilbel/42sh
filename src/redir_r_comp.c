/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_r_comp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/23 12:35:46 by gchainet          #+#    #+#             */
/*   Updated: 2019/02/07 19:24:53 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "shell.h"

int	redir_r_comp(t_shell *shell, t_ast *instr, t_redir *redir)
{
	(void)shell;
	(void)instr;
	redir->fd = redir->out;
	return (0);
}
