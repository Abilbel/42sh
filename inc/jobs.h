/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrouard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 09:52:57 by agrouard          #+#    #+#             */
/*   Updated: 2019/07/09 11:04:02 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOBS_H
# define JOBS_H

# include "libft.h"
# include <sys/types.h>

typedef struct	s_proc
{
	char			*name;
	pid_t			pid;
	int				ret;
	int				done;
	int				stopped;
	int				status;
	struct s_proc	*next;

}				t_proc;

enum			e_state
{
	JOB_NONE = 0,
	JOB_RUNNING = 1,
	JOB_STOPPED = 2,
	JOB_DONE = 3
};

typedef struct	s_job
{
	t_proc			*proc;
	t_proc			*last;
	pid_t			pgid;
	int				index;
	int				async;
	int				notified;
	enum e_state	state;
	struct s_job	*next;
}				t_job;

struct s_ast;
struct s_shell;

void			free_job(struct s_shell *shell, t_job *job);

t_job			*new_job(void);

int				register_proc(struct s_ast *ast);
int				register_job(struct s_shell *shell, t_job *job);

int				job_fg(struct s_shell *shell, t_job *job, int cont);
void			job_bg(t_job *job, int cont);

int				job_is_stopped(t_job *j);
int				job_is_done(t_job *j);

int				wait_job(struct s_shell *shell, t_job *job);
void			update_jobs(struct s_shell *shell);

void			job_notify(struct s_shell *shell);
t_job			*report_job(struct s_shell *shell, t_job *job, int opts
		, int fd);
int				parse_number(char *str);
t_job			*parse_jobspec(struct s_shell *shell, char *jobspec);
char			*ast_to_string(struct s_ast *ast);

#endif
