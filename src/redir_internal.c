/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_internal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/19 11:43:49 by gchainet          #+#    #+#             */
/*   Updated: 2019/01/13 16:11:43 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "shell.h"
#include "libft.h"

int	prepare_redirs(t_shell *shell, t_ast *instr, t_ast *root)
{
	t_redir	*redir;

	if (root == instr)
	{
		redir = root->redir_list;
		while (redir)
		{
			if (redir->redir_act(shell, instr, redir))
				return (1);
			redir = redir->next;
		}
	}
	else
	{
		ft_memcpy(instr->old_fds, root->old_fds, sizeof(instr->old_fds));
		ft_memcpy(instr->fds, root->fds, sizeof(instr->fds));
	}
	if (instr->left)
		prepare_redirs(shell, instr->left, root);
	if (instr->right)
		prepare_redirs(shell, instr->right, root);
	return (0);
}

int	redir_l(t_shell *shell, t_ast *instr, t_redir *redir)
{
	int	fd;

	(void)shell;
	fd = open(redir->target, O_RDONLY);
	if (fd < 0)
		return (1);
	instr->fds[redir->in] = fd;
	return (0);
}

int	redir_ll(t_shell *shell, t_ast *instr, t_redir *redir)
{
	t_heredoc	*heredoc;
	int			fd[2];

	(void)shell;
	if (!(heredoc = alloc_heredoc()))
		return (1);
	if (read_heredoc(heredoc, redir))
		return (heredoc_exit_error(heredoc));
	if (pipe(fd))
		return (heredoc_exit_error(heredoc));
	write(fd[1], heredoc->data, heredoc->len);
	close(fd[1]);
	instr->fds[redir->in] = fd[0];
	free(heredoc->data);
	free(heredoc);
	return (0);
}

int	redir_r(t_shell *shell, t_ast *instr, t_redir *redir)
{
	int	fd;

	(void)shell;
	fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC,
			S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	if (fd < 0)
		return (1);
	instr->fds[redir->in] = fd;
	return (0);
}

int	redir_rr(t_shell *shell, t_ast *instr, t_redir *redir)
{
	int	fd;

	(void)shell;
	fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND,
			S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	if (fd < 0)
		return (1);
	instr->fds[redir->in] = fd;
	return (0);
}
