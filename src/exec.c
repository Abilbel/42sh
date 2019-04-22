/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 09:03:28 by gchainet          #+#    #+#             */
/*   Updated: 2019/04/22 19:39:29 by agrouard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "shell.h"
#include "fill_line.h"
#include "libft.h"
#include "expand.h"

int		fail(char *proc, char *err, char *message, int ret)
{
	ft_dprintf(2, "%s: %s: %s\n", proc, err, message);
	return (ret);
}

static int	do_error_handling(char *name)
{
	struct stat info;

	if (access(name, F_OK) == 0)
	{
		if (stat(name, &info) != -1 && S_ISDIR(info.st_mode))
			return (fail(EXEC_NAME, name, "Is a directory", 126));
		else if (access(name, X_OK) == -1)
			return (fail(EXEC_NAME, name, "Permission denied", 126));
		else
			return (fail(EXEC_NAME, name, "Unexpected error", 127));
	}
	if (ft_strchr(name, '/') != NULL)
		return (fail(EXEC_NAME, name, "No such file or directory", 127));
	return (fail(EXEC_NAME, name, "Command not found", 127));
}

static void	exec_internal(t_shell *shell, t_ast *instr, const char *bin_path)
{
	set_pipeline(shell, instr);
	if (apply_redirs(shell, instr))
	{
		free_shell(shell);
		exit(1);
	}
	enable_signal();
	execve(bin_path, ((t_command *)instr->data)->args_value, shell->env);
	exit(1);
}

pid_t		do_exec(t_shell *shell, t_ast *instr)
{
}

pid_t		exec(t_shell *shell, t_ast *instr)
{
	pid_t		pid;
	char		*bin_path;
	t_builtin	builtin;

	if ((builtin = is_builtin(((t_command *)instr->data)->args_value[0])))
		return (exec_builtin(shell, builtin, instr));
	bin_path = hbt_command(shell, ((t_command *)instr->data)->args_value[0]);
	if (!bin_path)
	{
		bin_path = ((t_command *)instr->data)->args_value[0];
		instr->ret = do_error_handling(bin_path);
		return (-1);
	}
	if (!(pid  = fork()))
		exec_internal(shell, instr, bin_path);
	instr->pid = pid;
	return (0);
}
