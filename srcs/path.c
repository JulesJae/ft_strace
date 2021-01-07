#include "../includes/strace.h"

#define ENV_END -1

static int	check_path(struct s_path *path)
{
	if (access(path->buf, X_OK) == -1)
		return (0);
	path->find = 1;
	return (1);
}

static int	try_env_path(char *cmd, struct s_path *path)
{
	static int		i = 0;
	char			*env_path;
	char			*next;

	if (i == ENV_END || (env_path = getenv("PATH")) == NULL)
		return (0);
	memset(path->buf, '\0', MAX_PATH);
	env_path += i;
	if ((next = strchr(env_path, ':')))
	{
		*next = '\0';
		i += next - env_path + 1;
	} else
		i = ENV_END;
	strcat(path->buf, env_path);
	strcat(path->buf, "/");
	strcat(path->buf, cmd);
	return (1);
}

void		check_cmd(char *cmd, struct s_path *path)
{
	if ((cmd[0] == '.' && cmd[1] == '/') || cmd[0] == '/')
	{
		if (cmd[0] == '.')
			strcpy(path->buf, &cmd[2]);
		else
			strcpy(path->buf, cmd);
		check_path(path);
	}
	else 
	{
		while ((try_env_path(cmd, path)))
		{
			if (check_path(path))
				break;
		}
	}
	if (!path->find)
	{
		fprintf(stderr, "ft_strace: Can't access \'%s\'\n", cmd);
		exit(0);
	}
}