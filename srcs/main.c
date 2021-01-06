#include "../includes/strace.h"

static int		do_child(int ac, char *av[], struct s_path *path)
{
	char	*args[ac + 1];

	memcpy(args, av, ac * sizeof(char*));
	args[ac] = NULL;
	// ptrace(PTRACE_TRACEME);
	kill(getpid(), SIGSTOP);
	return execve(path->buf, args, NULL);
}

static int		do_trace(pid_t child)
{
	int		status;
	long	retval;

	ptrace(PTRACE_SEIZE, child, 0, PTRACE_O_TRACESYSGOOD);
	waitpid(child, &status, 0);
	assert(WIFSTOPPED(status));
	// ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
	while (1)
	{
		if (wait_for_syscall(child) != 0)
			break;

		print_syscall(child);

		if (wait_for_syscall(child) != 0)
			break;

		retval = get_reg(child, RAX);

		print_result(retval);
	}
	return (0);
}

int		main(int ac, char *av[])
{
	pid_t			child;
	struct s_path	path;

	if (ac < 2)
	{
		fprintf(stderr, "Usage: %s prog [args]\n", av[0]);
		exit(1);
	}
	memset(path.buf, '\0', MAX_PATH);
	path.find = 0;
	check_cmd(av[1], &path);
	child = fork();	
	if (child == 0)
		return do_child(ac - 1, av + 1, &path);
	return do_trace(child);
}