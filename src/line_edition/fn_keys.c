/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_keys.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvignal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 16:24:09 by cvignal           #+#    #+#             */
/*   Updated: 2019/07/10 15:03:49 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include <curses.h>

#include "fill_line.h"
#include "shell.h"
#include "parser.h"
#include "libft.h"

int	ft_nextword(t_shell *shell)
{
	size_t	i;

	i = shell->line.cursor;
	if (shell->line.cursor < shell->line.len)
	{
		while (shell->line.data[i] != ' ' && i < shell->line.len)
		{
			ft_rightkey(shell);
			i++;
		}
		while (shell->line.data[i] == ' ' && i < shell->line.len)
		{
			ft_rightkey(shell);
			i++;
		}
	}
	return (0);
}

int	ft_prevword(t_shell *shell)
{
	size_t	i;

	i = shell->line.cursor;
	if (shell->line.cursor > 0)
	{
		while (shell->line.data[i] != ' ' && i > 0)
		{
			ft_leftkey(shell);
			i--;
		}
		while (shell->line.data[i] == ' ' && i > 0)
		{
			ft_leftkey(shell);
			i--;
		}
		while (shell->line.data[i] != ' ' && i > 0)
		{
			ft_leftkey(shell);
			i--;
		}
		if (shell->line.cursor > 0)
			ft_rightkey(shell);
	}
	return (0);
}

int	ft_ctrld(t_shell *shell)
{
	shell->more_input = 0;
	if (!shell->ctrld && shell->line.len == 0 && !shell->output
			&& !shell->current)
	{
		ft_addchar(shell, "exit", 0);
		return (1);
	}
	else if (shell->line.len == 0)
	{
		shell->end_heredoc = shell->ctrld;
		if (!shell->ctrld)
			clean_exit_lexer(&shell->lexer, &shell->output,
					&shell->current, NULL);
		shell->prev_cmd_state = 1;
		return (1);
	}
	return (0);
}

int	ft_ctrlc(t_shell *shell)
{
	if (!shell->ctrld)
		clean_exit_lexer(&shell->lexer, &shell->output, &shell->current, NULL);
	free_line(&shell->line);
	ft_dprintf(shell->fd_op, "\n");
	set_ret(shell, NULL, 1);
	shell->more_input = 0;
	if (shell->ctrld)
		shell->end_heredoc = 2;
	else
	{
		clean_under_line(shell);
		print_prompt(shell, 0);
	}
	shell->prev_cmd_state = 1;
	return (shell->end_heredoc);
}

int	ft_ctrll(t_shell *shell)
{
	t_puts("cl");
	if (shell->line.len)
	{
		print_prompt(shell, 0);
		ft_dprintf(shell->fd_op, "%s", shell->line.data);
	}
	else
		print_prompt(shell, 0);
	return (0);
}
