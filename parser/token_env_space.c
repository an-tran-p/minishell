#include "tokenizer.h"

static void	token_env_space_1(t_token **cur, int *i, t_env *env)
{
	env->var = env->expand;
	if (*i)
	{
		env->j = 0;
		while (env->var[env->j] && !ft_isspace(env->var[env->j]))
			env->j ++;
		env->s = (char *)ft_calloc(*i + env->j + 1, sizeof(char));
		if (!env->s)
		{
			env->m_err = -1;
			return ;
		}
		ft_memcpy(env->s, (*cur)->s, *i);
		ft_memcpy(env->s + *i, env->var, env->j);
		env->m_err = tk_appendfront(cur, env->s);
		if (env->m_err)
			return ;
		env->var += env->j;
	}
	while (ft_isspace(*env->var))
	{
		env->var ++;
		env->count --;
	}
}

static void	token_env_space_2(t_token **cur, t_env *env)
{
	while (env->count)
	{
		env->j = 0;
		while (env->var[env->j] && !ft_isspace(env->var[env->j]))
			env->j ++;
		env->s = (char *)ft_calloc(env->j + 1, sizeof(char));
		if (!env->s)
		{
			env->m_err = -1;
			return ;
		}
		ft_memcpy(env->s, env->var, env->j);
		env->var += env->j;
		while (ft_isspace(*env->var))
		{
			env->var ++;
			env->count --;
		}
		env->m_err = tk_appendfront(cur, env->s);
		if (env->m_err)
			return ;
/*
		if (env->var[0])
			env->var += env->j - 1;
*/
	}
}

static void	token_env_space_3(t_token **cur, int *i, t_env *env, char *tmp)
{
	env->j = 0;
	while (env->var[env->j] && !ft_isspace(env->var[env->j]))
		env->j ++;
	if (env->j || env->len_tmp)
	{
		env->s = (char *)ft_calloc(env->j + env->len_tmp + 1,
				sizeof(char));
		if (!env->s)
		{
			env->m_err = -1;
			return ;
		}
		ft_memcpy(env->s, env->var, env->j);
		ft_memcpy(env->s + env->j, env->tmp, env->len_tmp);
		*i = env->j - 1;
		(*cur)->s = env->s;
		free(tmp);
	}
	else if (!env->j)
	{
		(*cur)->s = tmp;
		(*cur)->type = DEL;
	}
}

void	token_env_space(t_token **cur, int *i, t_env *env)
{
	char	*tmp;

	tmp = (*cur)->s;
	token_env_space_1(cur, i, env);
	if (env->m_err)
		return ;
	token_env_space_2(cur, env);
	if (env->m_err)
		return ;
	token_env_space_3(cur, i, env, tmp);
}
