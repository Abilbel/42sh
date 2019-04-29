/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_queue.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/16 18:22:45 by gchainet          #+#    #+#             */
/*   Updated: 2019/01/10 06:49:41 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"

void	free_input_queue(t_token *input)
{
	t_token	*tmp;

	while (input)
	{
		if (input->type == TT_STATEMENT || input->type == TT_OP)
			((t_ast *)input->data)->del(input->data);
		else if (input->data)
			free(input->data);
		tmp = input;
		input = input->next;
		free(tmp);
	}
}
