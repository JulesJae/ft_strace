#include "../includes/strace.h"

static char		*join(char *s1, char *s2);

void		exit_error(char *s)
{
	fprintf(stderr, "%s\n", s);
	exit (0);
}

long		get_reg(pid_t child, int off)
{
	long	val;

	val = ptrace(PTRACE_PEEKUSER, child, off * 8, NULL);
	assert(errno == 0);
	return val;
}

char		*escape_nl(char *str)
{
	char	*nl;
	char	*end;
	char	*ret;

	if ((nl = strchr(str, '\n')))
	{
		end = nl + 1;
		*nl = '\0';
		ret = join(str, "\\n");
		ret = join(ret, end);
		free(str);
		return ret;
	}
	return str;
}

char		*read_string(pid_t child, unsigned long addr)
{
	char			*val;
	unsigned int	allocated = 4096;
	int				read = 0;
	unsigned long	tmp;

	if ((val = malloc(allocated)) == NULL)
		exit_error("Malloc error");
	memset(val, '\0', allocated);
	while (1)
	{
		if (read + sizeof(tmp) > allocated)
		{
			allocated *= 2;
			if ((val = realloc(val, allocated)) == NULL)
				exit_error("Realloc error");
		}
		tmp = ptrace(PTRACE_PEEKDATA, child, addr + read);
		if (errno != 0)
		{
			val[read] = '\0';
			break;
		}
		memcpy(val + read, &tmp, sizeof(tmp));
		if (memchr(&tmp, '\0', sizeof(tmp)) != NULL)
			break;
		read += sizeof(tmp);
	}
	return val;
}

static char		*join(char *s1, char *s2)
{
	char	*str;
	int		size;

	size = strlen(s1) + strlen(s2) + 1;
	if ((str = malloc(size)) == NULL)
		exit_error("Malloc error");
	memset(str, '\0', size);
	strcat(str, s1);
	strcat(str, s2);
	return str;
}