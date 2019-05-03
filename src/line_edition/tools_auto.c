/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_auto.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvignal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/19 18:30:31 by cvignal           #+#    #+#             */
/*   Updated: 2019/04/30 16:46:51 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <term.h>
#include <curses.h>

#include "shell.h"
#include "libft.h"
#include "fill_line.h"

static char	*g_builtins[] =\
{
	"unsetenv",
	"setenv",
	"exit"
};

void		ft_add_builtins(char *word, t_list **list)
{
	int		i;
	t_list	*new;

	i = 0;
	while (i < 3)
	{
		if (ft_strnequ(word, g_builtins[i], ft_strlen(word)))
		{
			new = ft_lstnew(g_builtins[i], ft_strlen(g_builtins[i]) + 1);
			ft_lstadd(list, new);
		}
		i++;
	}
}

int			ft_comp(char *word, char *name)
{
	char	*rtn;

	if ((rtn = ft_strrchr(word, '/')))
	{
		if ((ft_strequ(".", name) || ft_strequ("..", name))
				&& *(rtn + 1) != '.')
			return (0);
		if (!*(rtn + 1) && !ft_strchr(word, '.'))
			return (1);
		return (ft_strnequ(rtn + 1, name, ft_strlen(rtn + 1)));
	}
	else
	{
		if ((ft_strequ(".", name) || ft_strequ("..", name))
				&& *word != '.')
			return (0);
		return (ft_strnequ(word, name, ft_strlen(word)));
	}
}

char		*ft_escape(char *name)
{
	char	*ret;
	size_t	len;
	size_t	i;

	i = -1;
	len = 0;
	while (name[++i])
		if (name[i] == ' ' || name[i] == '"' || name[i] == '\'')
			len++;
	len += i;
	if (!(ret = ft_strnew(len)))
		return (NULL);
	i = -1;
	len = 0;
	while (name[++i])
	{
		if (name[i] == ' ' || name[i] == '"' || name[i] == '\'')
		{
			ret[len++] = '\\';
			ret[len++] = name[i];
		}
		else
			ret[len++] = name[i];
	}
	return (ret);
}
