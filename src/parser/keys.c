/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvignal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/18 16:55:56 by cvignal           #+#    #+#             */
/*   Updated: 2018/12/25 10:30:51 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <term.h>
#include <stdlib.h>
#include <fcntl.h>
#include <curses.h>
#include <sys/ioctl.h>

#include "21sh.h"
#include "libft.h"
#include "fill_line.h"

void	ft_leftkey(t_shell *shell)
{
	if (shell->line.cursor > 0)
	{
		tputs(tgetstr("le", NULL), 0, ft_printchar);
		shell->line.cursor--;
	}
}

void	ft_rightkey(t_shell *shell)
{
	if (shell->line.cursor < shell->line.len)
	{
		tputs(tgetstr("nd", NULL), 0, ft_printchar);
		shell->line.cursor++;
	}
}

void	ft_backspace(t_shell *shell)
{
	if (shell->line.cursor > 0)
	{
		tputs(tgetstr("le", NULL), 0, ft_printchar);
		tputs(tgetstr("dc", NULL), 0, ft_printchar);
		if (shell->line.cursor < shell->line.len)
			ft_del_char(shell->line.data, shell->line.cursor);
		else
			--shell->line.len;
		--shell->line.cursor;
	}
}

void	ft_homekey(t_shell *shell)
{
	while (shell->line.cursor > 0)
		ft_leftkey(shell);
}

void	ft_endkey(t_shell *shell)
{
	while (shell->line.cursor < shell->line.len)
		ft_rightkey(shell);
}
