/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_auto.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvignal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 14:26:00 by cvignal           #+#    #+#             */
/*   Updated: 2018/12/22 15:39:31 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "fill_line.h"
#include <unistd.h>
#include <term.h>
#include <curses.h>

static void	fill_table(int *table, t_list *list)
{
	int		width;
	t_list	*curr;
	size_t	max_len;
	int		nb;

	width = tgetnum("co");
	curr = list;
	max_len = 0;
	nb = 0;
	while (curr)
	{
		if (ft_strlen(curr->content) > max_len)
			max_len = ft_strlen(curr->content);
		curr = curr->next;
		nb++;
	}
	table[1] = (int)max_len;
	table[0] = width / (max_len + 1);
	table[2] = nb;
	table[3] = nb / table[0] + (nb % table[0] != 0);
}

static void	display_table(char **array, int table[4])
{
	int	i;
	int	j;

	ft_printf("\n");
	i = 0;
	while (i < table[3])
	{
		j = i;
		while (j < table[2])
		{
			ft_printf("%-*s", table[1] + 1, array[j]);
			j += table[3];
		}
		if (i + 1 < table[3])
			ft_printf("\n");
		i++;
	}
}

void		display_list(t_list *list)
{
	int		table[4];
	char	**array;
	t_curs	*cursor;
	int		nb;

	tputs(tgetstr("sc", NULL), 0, ft_printchar);
	cursor = get_cursor_pos();
	fill_table(table, list);
	if (table[3] + cursor->line > tgetnum("li"))
		nb = table[3] + cursor->line - tgetnum("li") + 1;
	else
		nb = 0;
	array = ft_listtotab(list, table[2]);
	display_table(array, table);
	tputs(tgetstr("rc", NULL), 0, ft_printchar);
	while (nb)
	{
		tputs(tgetstr("up", NULL), 0, ft_printchar);
		nb--;
	}
}