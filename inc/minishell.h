/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 07:30:06 by gchainet          #+#    #+#             */
/*   Updated: 2018/12/20 20:26:48 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "parser.h"
# include "ast.h"

# define ARGS_ALLOC_SIZE 8

# define CHAR_TILDE	'~'
# define CHAR_VAR '$'

# define MAX_PATH	1024

typedef struct		s_shell
{
	t_parser		parser;
	t_lexer			lexer;
	char			**env;
	char			*line;
}					t_shell;

struct s_command;
struct s_redir;
typedef int			(*t_redir_act)(struct s_redir *, int);

typedef struct		s_redir
{
	t_ttype			type;
	char			*target;
	t_redir_act		redir_act;
	int				fd_copy[2];
	struct s_redir	*next;
}				t_redir;

typedef struct		s_command
{
	char			**args;
	size_t			alloc_size;
	size_t			args_len;
	pid_t			pid;
	t_redir			*redir_list;
}					t_command;

typedef struct			s_pipeline
{
	t_command		*command;
	int			in_fd[2];
	int			out_fd[2];
	int			running;
	struct s_pipeline	*next;
}				t_pipeline;

typedef int			(*t_builtin)(t_shell *, char **);

typedef struct		s_builtin_desc
{
	char			*desc;
	t_builtin		builtin;
}					t_builtin_desc;

/*
** command.c
*/
t_command			*alloc_command(void);
void				free_command(t_command *command);
int					add_to_command(t_command *command, char *word);

/*
** exec.c
*/
int					exec(t_shell *shell, t_pipeline *pipeline, t_pipeline *current);
int					exec_from_char(t_shell *shell, char **arg, t_shell *tmp_shell);
int					wait_loop(t_pipeline *pipeline);

/*
** path.c
*/
char				*find_command(t_shell *shell, char *command);

/*
** shell.c
*/
void				free_shell(t_shell *shell);
int					init_shell(t_shell *shell, char **environ);

/*
** env.c
*/
int					set_env_var(t_shell *shell, const char *var,
		const char *value);
void				remove_env(t_shell *shell);

/*
** env_utils.c
*/
char				**copy_env(char **env);
char				**split_env_arg(char *arg);
char				*get_env_value(t_shell *shell, char *name);
int					remove_env_value(t_shell *shell, char *name);

/*
** buiiltins/
*/
t_builtin			is_builtin(char *cmd);
int					builtin_cd(t_shell *shell, char **args);
int					builtin_env(t_shell *shell, char **args);
int					builtin_env_get_opts(char **args, t_shell *tmp_shell);
int					builtin_setenv(t_shell *shell, char **args);
int					builtin_unsetenv(t_shell *shell, char **args);
int					builtin_echo(t_shell *shell, char **args);
int					builtin_exit(t_shell *shell, char **args);

/*
** replace.c
*/
int					replace_vars(t_shell *shell, t_token *token);

/*
** signal.c
*/
void				signal_sigint(int sig);

/*
** exec*
*/
struct s_ast;
int					exec_cmd(t_shell *shell, struct s_ast *ast);
void				free_cmd(struct s_ast *ast);
int					exec_pipeline(t_shell *shell, struct s_ast *ast);
void				free_pipeline(struct s_ast *ast);
int					exec_end(t_shell *shell, struct s_ast *ast);
void				free_end(struct s_ast *ast);
int					exec_cmd(t_shell *shell, struct s_ast *ast);
void				free_cmd(struct s_ast *ast);
int					exec_end(t_shell *shell, struct s_ast *ast);
void				free_end(struct s_ast *ast);
int					exec_or(t_shell *shell, struct s_ast *ast);
void				free_or(struct s_ast *ast);
int					exec_and(t_shell *shell, struct s_ast *ast);
void				free_and(struct s_ast *ast);

/*
** redir.c
*/
int					add_redir(t_command *command, t_ttype type, char *arg,
		t_redir_act act);

/*
** redir_internal.c
*/
int					apply_redirs(t_command *command, int is_builtin);
int					redir_l(t_redir *redir, int is_builtin);
int					redir_ll(t_redir *redir, int is_builtin);
int					redir_r(t_redir *redir, int is_builtin);
int					redir_rr(t_redir *redir, int is_builtin);

/*
** redir_reset.c
*/
int					reset_redir(t_command *command);

/*
** pipeline.c
*/
int					add_to_pipeline(t_pipeline *first, t_command *last);
t_pipeline				*create_pipeline(t_command *command);
void					delete_pipeline(t_pipeline *pipeline);
int					prepare_pipeline(t_pipeline *pipeline);
void					open_close_pipe(t_pipeline *pipeline, t_pipeline *current);

#endif
