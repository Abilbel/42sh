/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvignal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 17:11:37 by cvignal           #+#    #+#             */
/*   Updated: 2019/07/09 17:32:51 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "libft.h"
#include "shell.h"

void	free_fc(t_fc *cmd)
{
	if (cmd->editor)
		free(cmd->editor);
	if (cmd->first)
		free(cmd->first);
	if (cmd->last)
		free(cmd->last);
	if (cmd->old_p)
	{
		free(cmd->old_p);
		free(cmd->new_p);
	}
}

int		fc_exec_cmd(t_fc *cmd, t_shell *shell)
{
	size_t	len;
	char	*cpy;
	int		ret;

	shell->fc_cmd = 1;
	if (!shell->history || !shell->history->length || shell->fc_rec)
		return (0);
	len = ft_strlen(shell->history->data[cmd->i_first]);
	if (ft_strlen(cmd->old_p) && ft_strlen(cmd->new_p))
		len += ft_imax(0, ft_strlen(cmd->new_p) - ft_strlen(cmd->old_p));
	else if (!ft_strlen(cmd->old_p) && ft_strlen(cmd->new_p))
		len *= ft_strlen(cmd->new_p);
	if (!(cpy = ft_strnew(len)))
		return (1);
	if (cmd->old_p)
		ft_replace_str(cpy, shell->history->data[cmd->i_first],
				cmd->old_p, cmd->new_p);
	else
		ft_strcpy(cpy, shell->history->data[cmd->i_first]);
	ret = fc_exec_line(cpy, shell, 0);
	return (ret);
}

int		fc_display(t_fc *cmd, t_shell *shell)
{
	int		i;

	i = cmd->i_first;
	if (!shell->history || !shell->history->length)
		return (0);
	if (ft_strchr(cmd->flags, 'r'))
		return (fc_display_reverse(cmd, shell));
	while (i < shell->history->length && i <= cmd->i_last)
	{
		if (!ft_strchr(cmd->flags, 'n'))
			ft_printf("%d", i);
		if (ft_strchr(shell->history->data[i], '\n'))
		{
			if (fc_display_multi(shell->history->data[i]))
				return (1);
		}
		else
			ft_printf("\t%s\n", shell->history->data[i]);
		i++;
	}
	return (0);
}

int		fc_edit(t_fc *cmd, t_shell *shell)
{
	t_tmpfile	file;
	int			ret;

	shell->fc_cmd = 1;
	if (!shell->history || !shell->history->length || shell->fc_rec)
		return (0);
	if (fc_open_file(cmd, shell, &file))
		return (1);
	if (!(ret = fc_open_editor(cmd, &file, shell)) && cmd->editor)
		ret = fc_exec_file(file.name, shell);
	unlink(file.name);
	free(file.name);
	return (ret);
}

int		builtin_fc(t_shell *shell, char **args)
{
	t_fc	cmd;
	int		ret;

	ft_bzero(&cmd, sizeof(cmd));
	ret = 0;
	if (fc_init_args(&cmd, args, shell))
	{
		free_fc(&cmd);
		return (1);
	}
	if (cmd.flags[0] == 's')
		ret = fc_exec_cmd(&cmd, shell);
	else if (ft_strchr(cmd.flags, 'l'))
		ret = fc_display(&cmd, shell);
	else
		ret = fc_edit(&cmd, shell);
	free_fc(&cmd);
	return (ret);
}
