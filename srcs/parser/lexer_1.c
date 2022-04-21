#include "../../includes/minishell.h"

static char	*word_token(int *index)
{
	char	*content;
	char	*tmp;
	int		i;

	content = g_data.input + *index;
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(content) + 1));
	if (!tmp)
		exit (1); // manage_error (error malloc token)
	i = 0;
	while (!ft_strchr(" \t<>|\"\'", content[i]))
	{
		tmp[i] = content[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

static char	*quotes_token(char quote, int *index)
{
	char	*content;
	char	*tmp;
	int		i;

	content = g_data.input + *index;
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(content) + 1));
	if (!tmp)
		exit (1); // manage_error (error malloc token)
	i = 0;
	tmp[i++] = quote;
	while (content[i] != quote)
	{
		tmp[i] = content[i];
		i++;
	}
	tmp[i++] = quote;
	tmp[i] = '\0';
	return (tmp);
}

static void	token_word_quotes_dquotes(int *i)
{
	if (*(g_data.input + *i))
	{
		if (g_data.input[*i] == '"')
			add_token(&g_data.tokens, new_token(quotes_token('"', i), \
			DQUOTES, i));
		else if (g_data.input[*i] == '\'')
			add_token(&g_data.tokens, new_token(quotes_token('\'', i), \
			QUOTES, i));
		else
			add_token(&g_data.tokens, new_token(word_token(i), WORD, i));
	}
}

static void	token_separator(int *i)
{
	add_token(&g_data.tokens, new_token(ft_strdup(" "), SEPARATOR, i));
	while (*(g_data.input + *i) && ft_strchr(" \t", *(g_data.input + *i)))
		(*i)++;
}

void	lexer(void)
{
	int		i;

	i = 0;
	while (*(g_data.input + i) && i < (int)ft_strlen(g_data.input))
	{
		if (ft_strchr(" \t", *(g_data.input + i)))
			token_separator(&i);
		if (!ft_strncmp(g_data.input + i, "<<", 2))
			add_token(&g_data.tokens, new_token(ft_strdup("<<"), RDR_SRC, &i));
		else if (!ft_strncmp(g_data.input + i, ">>", 2))
			add_token(&g_data.tokens, new_token(ft_strdup(">>"), RDR_APD, &i));
		else if (!ft_strncmp(g_data.input + i, "<", 1))
			add_token(&g_data.tokens, new_token(ft_strdup("<"), RDR_IN, &i));
		else if (!ft_strncmp(g_data.input + i, ">", 1))
			add_token(&g_data.tokens, new_token(ft_strdup(">"), RDR_OUT, &i));
		else if (!ft_strncmp(g_data.input + i, "|", 1))
			add_token(&g_data.tokens, new_token(ft_strdup("|"), PIPE, &i));
		else
			token_word_quotes_dquotes(&i);
		i++;
	}
	lexer_update_tokens();
}
