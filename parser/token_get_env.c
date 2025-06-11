#include "tokenizer.h"

static int	chk_envchr(int c)
{
	if (c == '_' || ft_isalpha(c) || ft_isdigit(c))
		return (1);
	else
		return (0);
}

static int	token_get_envname(t_env *env, char *s)
{
	env->j = 0;
	while (chk_envchr(s[env->j]))
	{
		if (ft_isdigit(s[0]))
		{
			env->j = 1;
			break ;
		}
		env->j ++;
	}
	if (env->j)
	{
		env->var = (char *)ft_calloc(env->j + 1, sizeof(char));
		if (!env->var)
			return (-1);
		ft_memcpy(env->var, s, env->j);
	}
	return (0);
}

void	token_get_env(char **str, int *i, t_env *env)
{
	env->var = NULL;
	env->m_err = token_get_envname(env, (*str) + (*i) + 1);
	if (env->m_err || !env->j)
		return ;
//change function later on
	env->expand = getenv(env->var);
	free(env->var);
	env->tmp = (*str) + (*i) + env->j + 1;
	env->len_tmp = ft_strlen(env->tmp);
/*
	if (!env->expand || !env->expand[0])
	{
printf("from token_get_nev *i:%d env->len_tmp:%d\n", *i, env->len_tmp);
		if(!(*i) && !env->len_tmp)
		ft_memcpy((*str) + (*i), env->tmp, env->len_tmp + 1);
		(*i)--;
	}	
*/
}
