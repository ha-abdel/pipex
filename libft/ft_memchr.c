/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdel-ha <abdel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:23:49 by abdel-ha          #+#    #+#             */
/*   Updated: 2024/11/06 10:28:40 by abdel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(void *s, int c, int n)
{
	int			i;
	unsigned char	tofind;
	unsigned char	*ptr;

	i = 0;
	tofind = (unsigned char)c;
	ptr = (unsigned char *)s;
	while (i < n)
	{
		if (ptr[i] == tofind)
			return ((void *)(s + i));
		i++;
	}
	return (NULL);
}
