/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdel-ha <abdel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:26:10 by abdel-ha          #+#    #+#             */
/*   Updated: 2024/11/15 14:54:48 by abdel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_checksep(char *s, char c, int *index)
{
	int		start;
	int		i;
	int		end;
	char	*ptr;

	start = *index;
	while (s[start] && s[start] == c)
		start++;
	end = start;
	while (s[end] && s[end] != c)
		end++;
	*index = end + 1;
	if (start >= end)
		return (NULL);
	ptr = (char *)malloc(end - start + 1);
	if (!ptr)
		return (NULL);
	i = 0;
	while (start < end)
		ptr[i++] = s[start++];
	ptr[i] = '\0';
	return (ptr);
}

static int	ft_count_words(char *s, char c)
{
	int	count;
	int	i;
	int		inword;

	count = 0;
	i = 0;
	inword = 0;
	while (s[i])
	{
		if (!inword && s[i] != c)
		{
			inword = 1;
			count++;
		}
		if (inword && s[i] == c)
			inword = 0;
		i++;
	}
	return (count);
}

char	**ft_split(char *s, char c)
{
	int	i;
	char	**arr;
	int		index;

	if (!s)
		return (NULL);
	i = 0;
	index = 0;
	arr = (char **)malloc((ft_count_words(s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	while (i < ft_count_words(s, c))
	{
		arr[i] = ft_checksep(s, c, &index);
		if (!arr[i])
		{
			while (i > 0)
				free(arr[--i]);
			free(arr);
			return (NULL);
		}
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
#include <stdio.h>
int main()
{
	char *str = "BIBO abdelah abll3ali la7ya big la7ya";

	char **t = ft_split(str , 32);
	printf("%s %s",t[0],t[3]);
	return 0;
}
