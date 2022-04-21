#include "../../includes/minishell.h"

static int	len_envp_var(char *str)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != '=')
		i++;
	return (i);
}

static char	*get_value_variable(char *src, int len, char **envp)
{
	char	*var;
	int		i;

	var = (char *)malloc(sizeof(char) * (len + 1));
	if (!var)
		exit (1); // manage_error
	ft_strlcpy(var, src, len + 1);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(var, envp[i], max(len, len_envp_var(envp[i]))))
		{
			free (var);
			var = ft_strdup(envp[i] + len_envp_var(envp[i]) + 1);
			return (var);
		}
		i++;
	}
	if (!ft_strncmp(var, "?", 1))
	{
		free (var);
		return (ft_itoa(g_data.status));
	}
	free(var);
	return (NULL);
}

static void	replace_variable(t_token *token, char *variable, \
int *i, int len_name_var)
{
	char	*old;
	char	*new;
	int		start;
	int		len_value_var;

	old = token->content;
	start = *i - len_name_var - 1;
	len_value_var = ft_strlen(variable);
	new = (char *)malloc(sizeof(char) * (ft_strlen(old) + len_value_var));
	if (!new)
	{
		free (variable);
		exit (1); // error manager
	}
	ft_strlcpy(new, old, start + 1);
	if (variable)
		ft_strlcpy(new + start, variable, len_value_var + 1);
	ft_strlcpy(new + start + len_value_var, old + start + len_name_var + 1, \
	ft_strlen(old + start + len_name_var) + 1);
	free (token->content);
	token->content = ft_strdup(new);
	if (!token->content)
		exit (1); // error manager не скопировал данные для токена, ошибка malloc
	(*i) = start;
	free (new);
}

static void	insert_variable(t_token *token, char *content, int *i, char **envp)
{
	char	*variable;
	int		len;

	len = 0;
	(*i)++;
	if (content[*i] == '?' || ft_isdigit(content[*i]))
	{
		len++;
		(*i)++;
	}
	else
	{
		while (content[*i] && (ft_isalnum(content[*i]) || content[*i] == '_'))
		{
			len++;
			(*i)++;
		}
	}
	variable = get_value_variable(content + *i - len, len, envp);
	replace_variable(token, variable, i, len);
	free (variable);
}

void	open_variable(t_token *token)
{
	char	*content;
	int		i;
	int		dquotes;
	int		quotes;

	content = token->content;
	i = 0;
	dquotes = 0;
	quotes = 0;
	while (content && content[i])
	{
		if (content[i] == '"' && quotes % 2 == 0)
			dquotes++;
		if (content[i] == '\'')
			quotes++;
		if ((quotes % 2 == 0 || dquotes % 2 == 1) && content[i] == '$')
		{
			insert_variable(token, content, &i, g_data.envp);
			content = token->content;
		}
		else
			i++;
	}
}
