#include "../includes/strace.h"

const char	*syscall_name(int scn)
{
	static char		buf[128];

	if (scn <= MAX_SYSCALL_NUM)
		return syscalls[scn].name;
	snprintf(buf, sizeof(buf), "sys_%d", scn);
	return buf;
}

int			wait_for_syscall(pid_t child)
{
	int		status;

	while (1)
	{
		empty_sig();
		ptrace(PTRACE_SYSCALL, child, 0, 0);
		waitpid(child, &status, 0);
		block_sig();
		if (WIFSTOPPED(status))
		{
			if (WSTOPSIG(status) & 0x80)
				return (0);
			get_signal(child, status);
		}
		if (WIFEXITED(status))
		{
			fprintf(stderr, " = ?\n+++ exited with %d +++\n", WEXITSTATUS(status));
			exit(0);
		}
	}
}

long		get_syscall_arg(pid_t child, int key)
{
	switch (key)
	{
		case 0: return get_reg(child, RDI);
		case 1: return get_reg(child, RSI);
		case 2: return get_reg(child, RDX);
		case 3: return get_reg(child, R10);
		case 4: return get_reg(child, R9);
		case 5: return get_reg(child, R8);
		default:
			return -1L;
	}
}