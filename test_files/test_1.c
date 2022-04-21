#include <stdlib.h>
#include <stdio.h>

int	main(void)
{
	char	**arr;
	int		len = 5;
	int		i;
	int		len_str = 5;

	arr = (char **)malloc(sizeof(char *) * (len + 1));
	i = 0;
	while (i <= len)
	{
		arr[i] = NULL;
		printf("p_arr[%d] = %p\n", i, &arr[i]);
		i++;
	}

	printf("-----------------\n");

	i = 0;
	while (i < len)
	{
		arr[i] = (char *)malloc(sizeof(char) * (len_str + 1));
		printf("p_arr[%d] = %p\n", i, &arr[i]);
		i++;
	}

	// arr[0] = ""
}
