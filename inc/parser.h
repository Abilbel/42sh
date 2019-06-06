/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvignal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:03:52 by cvignal           #+#    #+#             */
/*   Updated: 2019/06/04 14:47:11 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>
# include <limits.h>

# define META_CHARS ";|&<>()"

# define TOKEN_ALLOC_SIZE	128

# define PS_NONE (1 << 0)
# define PS_IFNOCD (1 << 1)
# define PS_IFCD (1 << 2)
# define PS_ELSE (1 << 3)
# define PS_WHILENOCD (1 << 4)
# define PS_WHILECD (1 << 5)
# define PS_EXPR (1 << 6)
# define PS_CMD (1 << 7)
# define PS_ARI (1 << 8)
# define PS_ALL (~0)

struct s_shell;
struct s_ast;

typedef struct			s_token
{
	void				*data;
	size_t				alloc_size;
	size_t				len;
	unsigned int		type;
	struct s_token		*next;
}						t_token;

typedef struct			s_token_desc
{
	char				*str;
	int					type;
}						t_token_desc;

typedef int				(*t_char_cmp)(char, char);

typedef struct			s_match_desc
{
	char				desc;
	t_char_cmp			func;
}						t_match_desc;

typedef struct			s_precedence
{
	int					type;
	int					prec;
}						t_precedence;

typedef enum			e_lstate
{
	LSTATE_NONE,
	LSTATE_WORD,
	LSTATE_META,
	LSTATE_SQUOTE,
	LSTATE_DQUOTE,
	LSTATE_ARI_NONE,
	LSTATE_ARI_ID,
	LSTATE_ARI_OP,
	LSTATE_PAREN,
	LSTATE_ESCAPED,
	NUMBER_LSTATE
}						t_lstate;

typedef enum			e_lexer_ret
{
	LEXER_RET_CUT,
	LEXER_RET_CREATE,
	LEXER_RET_CONT,
	LEXER_RET_ERROR,
	LEXER_RET_OVER,
	LEXER_RET_MORE_INPUT
}						t_lexer_ret;

enum
{
	MATCH_NO,
	MATCH_PARTIAL,
	MATCH_COMPLETE
};

enum
{
	PARSER_MORE_INPUT,
	PARSER_EMPTY,
	PARSER_COMPLETE,
	PARSER_ERROR
};

typedef struct			s_lss
{
	t_lstate			state;
	int					escaped;
	struct s_lss		*next;
}						t_lss;

typedef struct			s_pss
{
	int					state;
	int					status;
	t_token				*output_queue;
	t_token				*op_stack;
	t_token				*stack;
	struct s_ast		*ret;
	struct s_pss		*next;
}						t_pss;

typedef int				(*t_lexer_act)(struct s_shell *, t_token *, char);

typedef struct			s_lexer
{
	t_lexer_act			lexer_actions[NUMBER_LSTATE][CHAR_MAX + 1];
	t_lss				*lss;
}						t_lexer;

typedef struct			s_parser
{
	t_pss				*pss;
	int					ret_status;
	struct s_ast		*ret;
	t_token				*input_queue;
}						t_parser;

/*
** parser/lss.c
*/
int						lss_push(t_lexer *lexer, t_lstate state);
t_lstate				lss_pop(t_lexer *lexer);

/*
** parser/pss.c
*/
int						pss_push(t_parser *parser, int state);
struct s_ast			*pss_pop(t_parser *parser);

/*
** parser/parser.c
*/
int						parse(struct s_shell *shell, t_token *tokens);
int						init_parser(t_parser *parser);

/*
** parser/token.c
*/
t_token					*alloc_token(void);
int						add_to_token(t_token *token, char c);
int						insert_into_token(t_token *token, char *data, int pos);
void					add_to_token_list(t_token **list, t_token *new_token);
void					free_token_list(t_token *list);

/*
** parser/token_type.c
*/
int						get_token_type(t_token *token, int state);

/*
** parser/lexer_act.c
*/
int						lexer_add(struct s_shell *shell,
		t_token *token, char c);
int						lexer_cut(struct s_shell *shell,
		t_token *token, char c);
int						lexer_create(struct s_shell *shell,
		t_token *token, char c);
int						lexer_pass(struct s_shell *shell,
		t_token *token, char c);
int						lexer_pop_pass(struct s_shell *shell,
		t_token *token, char c);

/*
** parser/lexer_act_meta.c
*/
int						lexer_create_meta(struct s_shell *shell,
		t_token *token, char c);
int						lexer_add_meta(struct s_shell *shell,
		t_token *token, char c);
int						lexer_try_meta(struct s_shell *shell,
		t_token *token, char c);
int						lexer_push_paren(struct s_shell *shell,
		t_token *token, char c);
int						lexer_pop_paren(struct s_shell *shell,
		t_token *token, char c);

/*
** parser/lexer_act_quote.c
*/
int						lexer_push_dquote(struct s_shell *shell,
		t_token *token, char c);
int						lexer_push_squote(struct s_shell *shell,
		t_token *token, char c);
int						lexer_pop_quote(struct s_shell *shell,
		t_token *token, char c);

/*
** parser/lexer_act_var.c
*/
int						lexer_push_var(struct s_shell *shell, t_token *token,
		char c);
int						lexer_pop_var(struct s_shell *shell, t_token *token,
		char c);

/*
** parser/lexer_act_over.c
*/
int						lexer_over(struct s_shell *shell, t_token *token,
		char c);
int						lexer_more_input(struct s_shell *shell, t_token *token,
		char c);
int						lexer_more_input_nl(struct s_shell *shell
		, t_token *token, char c);
int						lexer_more_input_escaped(struct s_shell *shell
		, t_token *token, char c);

/*
** parser/lexer_act_escaped.c
*/
int						lexer_push_escaped(struct s_shell *shell,
		t_token *token, char c);
int						lexer_add_pop_escaped(struct s_shell *shell,
		t_token *token, char c);

/*
** parser/lexer.c
*/
t_token					*lex(struct s_shell *shell, const char *input);
int						init_lexer(t_lexer *lexer);
int						clean_exit_lexer(t_lexer *lexer, t_token **list
		, t_token **current, const char *msg);

/*
** parser/token_type_desc.c
*/
int						ccmp_digit(char a, char b);
int						ccmp_all(char a, char b);
int						ccmp(char a, char b);

/*
** parser/keywords.c
*/
int						keyword_type(const char *s);
int						is_a_keyword(const char *s);

/*
**	parser/assignement.c
*/
int						token_is_assignement(const char *value);

/*
** parser/tools_shunting_yard.c
*/
int						set_leaves(struct s_ast *node, struct s_token **stack);

#endif
