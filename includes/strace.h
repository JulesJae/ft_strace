#ifndef STRACE_H
# define STRACE_H

# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/ptrace.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/reg.h>
# include <sys/user.h>
# include <assert.h>
# include "syscalls.h"

# define MAX_ERRNO 124

# define MAX_PATH 4096

struct s_path
{
	int			find;
	char		buf[MAX_PATH];
};

void			exit_error(char *s);
void			check_cmd(char *cmd, struct s_path *path);
char			*escape_nl(char *str);
int				wait_for_syscall(pid_t child);
void			print_syscall(pid_t child);
long			get_reg(pid_t child, int off);
void			print_result(long retval);
long			get_syscall_arg(pid_t child, int key);
const char		*syscall_name(int scn);
char			*read_string(pid_t child, unsigned long addr);
void			get_signal(pid_t child, int status);
void			empty_sig();
void			block_sig();

#endif
