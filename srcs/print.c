#include "../includes/strace.h"

#define PADDING 39

void			print_result(long retval)
{
	if (retval <= -1 && retval >= -MAX_ERRNO)
		fprintf(stderr, " = -1 %s\n", strerror(-(int)retval));
	else if (retval < -MAX_ERRNO || retval > MAX_ERRNO)
		fprintf(stderr, " = 0x%lx\n", retval);
	else
		fprintf(stderr, " = %ld\n", retval);
}

static void		print_syscall_args(pid_t child, int scn, int *printed)
{
	int						i = 0;
	char					*strval;
	struct syscall_entry	ent;
	long					arg;
	int						type;

	if (scn <= MAX_SYSCALL_NUM && syscalls[scn].name)
		ent = syscalls[scn];
	while (i < ent.nargs)
	{
		arg = get_syscall_arg(child, i);
		type = ent.args[i];
		switch (type)
		{
			case ARG_INT:
				*printed += fprintf(stderr, "%ld", arg);
				break;
			case ARG_STR:
				strval = escape_nl(read_string(child, arg));
				*printed += fprintf(stderr, "\"%s\"", strval);
				free(strval);
				break;
			default:
				*printed += fprintf(stderr, "0x%lx", arg);
		}
		if (i != ent.nargs - 1)
			*printed += fprintf(stderr, ", ");
		else
			*printed += fprintf(stderr, ")");
		i++;
	}
}

void			print_syscall(pid_t child)
{
	int		num;
	int		printed = 0;

	num = get_reg(child, ORIG_RAX);
	assert(errno == 0);

	printed += fprintf(stderr, "%s(", syscall_name(num));
	print_syscall_args(child, num, &printed);
	if (PADDING - printed > 0)
		fprintf(stderr, "%*c", PADDING - printed, ' ');
}
