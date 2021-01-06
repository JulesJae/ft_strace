#include "../includes/strace.h"

static const char		*g_signal_names[] = {
	"","SIGHUP","SIGINT","SIGQUIT","SIGILL",
	"SIGTRAP","SIGABRT","SIGBUS","SIGFPE",
	"SIGKILL","SIGUSR1","SIGSEGV","SIGUSR2",
	"SIGPIPE","SIGALRM","SIGTERM","SIGSTKFLT",
	"SIGCHLD","SIGCONT","SIGSTOP","SIGTSTP",
	"SIGTTIN","SIGTTOU","SIGURG","SIGXCPU",
	"SIGXFSZ","SIGVTALRM","SIGPROF","SIGWINCH",
	"SIGIO","SIGPWR","SIGSYS"
};

static int				has_been_killed(int status)
{
	int		code;

	code = WSTOPSIG(status);
	if (code != SIGCHLD && code != SIGURG && code != SIGWINCH && code != SIGSTOP
		&& code != SIGTSTP && code != SIGTTIN && code != SIGTTOU && code != SIGCONT)
		return (1);
	return (0);
}

void					empty_sig()
{
	sigset_t	sigset;

	sigemptyset(&sigset);
	sigprocmask(SIG_SETMASK, &sigset, NULL);
}

void					block_sig()
{
	sigset_t	sigset;

	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	sigaddset(&sigset, SIGHUP);
	sigaddset(&sigset, SIGQUIT);
	sigaddset(&sigset, SIGPIPE);
	sigaddset(&sigset, SIGTERM);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
}

void					get_signal(pid_t child, int status)
{
	siginfo_t	siginfo;

	ptrace(PTRACE_GETSIGINFO, child, NULL, &siginfo);
	if (WSTOPSIG(status) < (sizeof(g_signal_names) / sizeof(g_signal_names[0])))
	{
		if (has_been_killed(status))
		{
			fprintf(stderr, "--- %s {si_signo=%s, si_code=%d, si_pid=%d, si_uid=%d} ---\n",
			g_signal_names[WSTOPSIG(status)],
			g_signal_names[WSTOPSIG(status)],
			siginfo.si_code,
			siginfo.si_pid,
			siginfo.si_uid
			);
			fprintf(stderr, "+++ killed by %s (%s) +++\n", g_signal_names[WSTOPSIG(status)], strsignal(WSTOPSIG(status)));
			exit (0);
		} 
		fprintf(stderr, "--- %s {si_signo=%s, si_code=%d, si_pid=%d, si_uid=%d, si_status=%d, si_utime=%ld, si_stime=%ld} ---\n",
		g_signal_names[WSTOPSIG(status)],
		g_signal_names[WSTOPSIG(status)],
		siginfo.si_code,
		siginfo.si_pid,
		siginfo.si_uid,
		siginfo.si_status,
		siginfo.si_utime,
		siginfo.si_stime
		);
	}
}