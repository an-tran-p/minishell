#include "minishell.h"

typedef struct s_env
{
        int             j;
        int             count;
        int             m_err;
        int             len_expand;
        int             len_tmp;
        char    *var;
        char    *expand;
        char    *s;
        char    *tmp;
}       t_env;

void	token_get_env(char **str, int *i, t_env *env);

int     token_expand_str(char **str, int *i, t_env *env)
{
        env->len_expand = ft_strlen(env->expand);
        env->s = (char *)ft_calloc((*i) + env->len_expand + env->len_tmp + 1,
                        sizeof(char));
        if (!env->s)
                return (-1);
        ft_memcpy(env->s, *str, *i);
        ft_memcpy(env->s + *i, env->expand, env->len_expand);
        ft_memcpy(env->s + *i + env->len_expand, env->tmp, env->len_tmp + 1);
        free(*str);
        env->tmp = NULL;
        *i = *i + env->len_expand - 1;
        *str = env->s;
        return (0);
}

static int      token_quote_env(char **str, int *i)
{
        t_env   env;

        token_get_env(str, i, &env);
        if (env.m_err || !env.j || !env.expand || !env.expand[0])
                return (env.m_err);
        env.m_err = token_expand_str(str, i, &env);
        return (env.m_err);
}

int	heredoc_expand(char **s)
{
	int	i;
	int	m_err;

	m_err = 0;
	i = 0;
	while ((*s)[i])
	{
		if ((*s)[i] == '$')
		{
			m_err = token_quote_env(s, &i);
			if(m_err)
				return(m_err);
		}
		i ++;
	}
	return (m_err);
}

int	main(void)
{
	int		len;
	char	str[] = "$USER heldl";
	char	*line;

	len = ft_strlen(str);
	line = ft_calloc(len + 1, 1);
	ft_memcpy(line, str, len);
	printf("%s\n", line);
	heredoc_expand(&line);
	printf("%s\n", line);
	if (line)
		free(line);
}
