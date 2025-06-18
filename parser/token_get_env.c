#include "tokenizer.h"

static int	chk_envchr(int c)
{
	if (c == '_' || ft_isalpha(c) || ft_isdigit(c))
		return (1);
	else
		return (0);
}

static int	token_get_envname(t_env *data, char *s)
{
	data->j = 0;
	data->ex_free = 0;
	if (s[0] == '?')
	{
		data->expand = ft_itoa(exit_status(0, false));
		if (!data->expand)
			return (-1);
		data->ex_free = 1;
		data->j = 1;
	}
	else
	{
		while (chk_envchr(s[data->j]))
		{
			if (ft_isdigit(s[0]))
			{
				data->j = 1;
				break ;
			}
			data->j ++;
		}
	}
/*
	if (data->j)
	{
		data->var = (char *)ft_calloc(data->j + 1, sizeof(char));
		if (!data->var)
			return (-1);
		ft_memcpy(data->var, s, data->j);
	}
*/
	return (0);
}

void	token_get_env(char **str, int *i, t_env *data, char **env)
{
//	data->var = NULL;
	data->m_err = token_get_envname(data, (*str) + (*i) + 1);
//
	if (data->m_err || !data->j)
		return ;
	else if (data->j)
	{
		data->var = (char *)ft_calloc(data->j + 1, sizeof(char));
		if (!data->var)
		{
			data->m_err = -1;
			return ;
		}
		ft_memcpy(data->var, (*str) + (*i) + 1, data->j);
	}
//
//change function later on
	if (!data->ex_free)
		data->expand = ft_getenv(env, data->var);
	free(data->var);
	data->tmp = (*str) + (*i) + data->j + 1;
	data->len_tmp = ft_strlen(data->tmp);
//
	if (!data->expand || !data->expand[0])
	{
printf("from token_get_nev *i:%d data->len_tmp:%d\n", *i, data->len_tmp);
		ft_memcpy((*str) + (*i), data->tmp, data->len_tmp + 1);
	}	
//
}
