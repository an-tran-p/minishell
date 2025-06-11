#include "tokenizer.h"

int	token_expand_str(char **str, int *i, t_env *env)
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

static void	token_env_all_space_back(t_token **cur, int *i, t_env *env)
{
	if (env->len_tmp)
	{
		if (!(*i))
		{
			ft_memcpy((*cur)->s, env->tmp, env->len_tmp + 1);
			return ;
		}
		env->s = (char *)ft_calloc(env->len_tmp + 1, sizeof(char));
		if (!env->s)
		{
			env->m_err = -1;
			return ;
		}
		ft_memcpy(env->s, env->tmp, env->len_tmp);
		*i = 0;
		(*cur)->s = env->s;
	}
}

static void	token_env_all_space(t_token **cur, int *i, t_env *env)
{
	if (!(*i) && !env->len_tmp)
		(*cur)->type = DEL;
	if (*i)
	{
		(*cur)->s[*i] = 0;
		if (!env->len_tmp)
			return ;
		env->m_err = tk_appendfront(cur, (*cur)->s);
		if (env->m_err)
			return ;
	}
	token_env_all_space_back(cur, i, env);
}

static int	chk_env_space(char *s)
{
	int	i;
	int	len;
	int	count;

	i = 0;
	len = ft_strlen(s);
	count = 0;
	while (s[i])
	{
		if (ft_isspace(s[i]))
			count ++;
		i ++;
	}
	if (count == len)
		return (-1);
	return (count);
}

int	token_env(t_token **cur, int *i, int rd)
{
	t_env	env;

	token_get_env(&(*cur)->s, i, &env);
	if (env.m_err || !env.j || !env.expand || !env.expand[0])
	{
		if (!env.expand || !env.expand[0])
		{
			if (rd && !(*i) && !env.len_tmp)
			{
				(*cur)->type = RD_ERR;
				return (1);
			}
			ft_memcpy((*cur)->s + (*i), env.tmp, env.len_tmp + 1);
			(*i)--;
		}
/*
		if ((!env.expand || !env.expand[0])
			&& (*i) == -1 && !env.len_tmp)
		{
			if (rd)
			{
				(*cur)->type = RD_ERR;
				return (1);
			}
			(*cur)->type = DEL;
		}
*/
		return (env.m_err);
	}
	env.count = chk_env_space(env.expand);
	if (rd && env.count)
	{
		(*cur)->type = RD_ERR;
		return (1);
	}
	if (!env.count)
		env.m_err = token_expand_str(&(*cur)->s, i, &env);
	else if (env.count == -1)
		token_env_all_space(cur, i, &env);
	else
		token_env_space(cur, i, &env);
	return (env.m_err);
}
