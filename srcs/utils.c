#include "../includes/minishell.h"

int	max(int a, int b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

char	**malloc_two_array_char(int len)
{
	char	**array;
	int		i;

	array = (char **)malloc(sizeof(char *) * (len + 1));
	i = 0;
	while (i <= len)
		array[i++] = NULL;
	return (array);
}

void	free_two_array_char(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
		free (array[i++]);
	free (array);
	array = NULL;
}

void	add_back_elem_two_array_char(char **array, char *elem, int len)
{
	int	i;

	i = 0;
	while (array && i < len && array[i])
		i++;
	if (i < len)
		array[i] = ft_strdup(elem);
	if (!array[i])
		exit (1); // manager error
}

char	*find_str(char **array, char *str)
{
	int	i;

	i = 0;
	while (array && array[i])
	{
		if (!ft_strncmp(array[i], str, ft_strlen(str)))
			return (array[i]);
		i++;
	}
	return (NULL);
}

int	size_two_array_char(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
		i++;
	return (i);
}
