#ifndef SYSCALLS_H
# define SYSCALLS_H

# define SYSCALL_MAXARGS 6
# define MAX_SYSCALL_NUM 329

enum argtype
{
	ARG_INT,
	ARG_PTR,
	ARG_STR
};

struct syscall_entry
{
	const char						*name;
	int								nargs;
	enum argtype					args[SYSCALL_MAXARGS];
};

extern const struct syscall_entry	syscalls[MAX_SYSCALL_NUM];

#endif
