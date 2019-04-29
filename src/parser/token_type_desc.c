/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_desc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 13:03:51 by gchainet          #+#    #+#             */
/*   Updated: 2019/04/13 00:55:08 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ccmp(char a, char b)
{
	return (a == b);
}

int	ccmp_digit(char a, char b)
{
	(void)b;
	return (ft_isdigit(a));
}

int	ccmp_all(char a, char b)
{
	return (a && b);
}
