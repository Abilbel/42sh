/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 09:03:28 by gchainet          #+#    #+#             */
/*   Updated: 2019/05/01 21:15:55 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#include "jobs.h"
#include "shell.h"
#include "libft.h"
#include "expand.h"
#include "fill_line.h"

static void	exec_internal(t_shell *shell, t_ast *instr,
	const char *path, t_builtin builtin)
{
	char **args;

	instr->pid = getpid();
	if (!instr->job->pgid)
		instr->job->pgid = instr->pid;
	setpgid(instr->pid, instr->job->pgid);
	if (!instr->job->async)
		tcsetpgrp(0, instr->job->pgid);
	set_pipeline(shell, instr);
	if (apply_redirs(shell, instr))
	{
		free_shell(shell);
		exit(127);
	}
	enable_signal();
	args = ((t_command *)instr->data)->args_value;
	if (builtin)
		exit(builtin(shell, args));
	exit(execve(path, args, build_env(shell->exec_vars)));
}

pid_t		do_exec(t_shell *shell, char **argv)
{
	int			status;
	pid_t		pid;
	char		*bin_path;

	if (!(bin_path = find_command(shell->vars, argv[0])))
		return (do_error_handling(argv[0]));
	if ((pid = fork()) == -1)
		return (-1);
	if (pid == 0)
		exit(execve(bin_path, argv, build_env(shell->exec_vars)));
	free(bin_path);
	wait(&status);
	if (WIFEXITED(status) || WIFSIGNALED(status))
		return (0);
	return (WEXITSTATUS(status));
}

int			exec(t_shell *shell, t_ast *instr)
{
	char		*prgm;
	char		*bin_path;
	t_builtin	builtin;

	prgm = ((t_command *)instr->data)->args_value[0];
	if (!(builtin = is_builtin(prgm))
		&& !(bin_path = hbt_command(shell, prgm)))
		return (instr->ret = do_error_handling(prgm), 0);
	if (!builtin || instr->flags & CMD_FORK)
	{
		if ((instr->pid = fork()) == -1)
			return (-1);
	}
	else
		return (instr->ret = exec_builtin(shell, builtin, instr), 0);
	if (instr->pid == 0)
		exec_internal(shell, instr, bin_path, builtin);
	register_proc(instr);
	return (0);
}

/*
** This functions executes a node and adds its processes to the job.
** If job is NULL, it gets created and registered.
*/

int			exec_job(t_shell *shell, t_ast *node, t_job *job)
{
	node->job = job;
	if (!job && !(node->job = new_job()))
		return (-1);
	if (node->exec(shell, node))
		return (-1);
	if (!job && node->job->proc)
		node->ret = register_job(shell, node->job);
	else if (!job)
		free(node->job);
	return (0);
}
