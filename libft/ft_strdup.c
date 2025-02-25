/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdel-ha <abdel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:27:18 by abdel-ha          #+#    #+#             */
/*   Updated: 2024/11/05 11:49:01 by abdel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(char *s1)
{
	int	len;
	char	*result;

	len = ft_strlen(s1) + 1;
	result = (char *)malloc(len);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, len);
	return (result);
}
