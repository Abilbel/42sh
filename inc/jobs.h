/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrouard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 09:52:57 by agrouard          #+#    #+#             */
/*   Updated: 2018/12/12 09:55:28 by agrouard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOBS_H
# define JOBS_H

# include "libft.h"
# include <sys/types.h>

# define JOB_BG 0
# define JOB_FG 1

typedef struct	s_proc
{
	pid_t			pid;
	char			*command;
	int				ret;
	int				done;
	int				stopped;
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
	enum e_state	state;
	struct s_job	*next;
}				t_job;

struct s_ast;
struct s_shell;

t_job			*new_job(void);
void			free_job(struct s_shell *shell, t_job *job);

t_job			*find_job(struct s_shell *shell, int index);

int				register_proc(struct s_ast *ast);
int				register_job(struct s_shell *shell, t_job *job, int foreground);

int				job_fg(struct s_shell *shell, t_job *job, int cont);
void			job_bg(t_job *job, int cont);

int				job_is_stopped(t_job *j);
int				job_is_done(t_job *j);

void			update_proc(struct s_shell *shell, pid_t pid, int status);
void			update_jobs(struct s_shell *shell);
int				wait_job(struct s_shell *shell, t_job *job);
t_job			*report_job(struct s_shell *shell, t_job *job, int opts);
void			print_job_command(t_job *job);

int				parse_number(char *str);

#endif