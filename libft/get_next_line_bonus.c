/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdel-ha <abdel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:04:59 by abdel-ha          #+#    #+#             */
/*   Updated: 2024/12/02 16:09:24 by abdel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include "libft.h"

// int	ft_strlen(char *s)
// {
// 	int	i;

// 	i = 0;
// 	if (!s)
// 		return (0);
// 	while (s[i])
// 	{
// 		i++;
// 	}
// 	return (i);
// }

void	*ft_calloc(int count, int size)
{
	int		len;
	void	*ptr;
	int		i;

	len = count * size;
	i = 0;
	if (size != 0 && count > (int)-1 / size)
		return (NULL);
	ptr = (void *)malloc(len);
	if (!ptr)
		return (NULL);
	while (i < len)
	{
		((char *)ptr)[i] = '\0';
		i++;
	}
	return (ptr);
}

char	*fill_line(char **leftover)
{
	char	*line;
	char	*new_leftover;
	int	i;

	if (!*leftover || **leftover == '\0')
		return (NULL);
	i = 0;
	while ((*leftover)[i] && (*leftover)[i] != '\n')
		i++;
	if ((*leftover)[i] == '\n')
		line = ft_substr(*leftover, 0, i + 1);
	else
		line = ft_substr(*leftover, 0, i);
	if (!line)
		return (free(*leftover), *leftover = NULL, NULL);
	if ((*leftover)[i] == '\n')
		new_leftover = ft_strdup(*leftover + i + 1);
	else
		new_leftover = NULL;
	if ((*leftover)[i] == '\n' && !new_leftover)
		return (free(line), free(*leftover), *leftover = NULL, NULL);
	free(*leftover);
	*leftover = new_leftover;
	return (line);
}

char	*read_data_from_buffer(char **leftover, int fd)
{
	char	*buffer;
	int	bytes_read;
	char	*temp;

	buffer = ft_calloc(BUFFER_SIZE + 1, 1);
	if (!buffer)
		return (free(*leftover), *leftover = NULL, NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(*leftover, buffer);
		if (!temp)
			return (free(buffer), free(*leftover), *leftover = NULL, NULL);
		free(*leftover);
		*leftover = temp;
		if (ft_strchr(buffer, '\n'))
			return (free(buffer), fill_line(leftover));
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	if (bytes_read < 0 || (!*leftover))
		return (free(buffer), free(*leftover), *leftover = NULL, NULL);
	if (bytes_read < 0 || **leftover == '\0')
		return (free(buffer), free(*leftover), *leftover = NULL, NULL);
	return (free(buffer), fill_line(leftover));
}

char	*get_next_line(int fd)
{
	static char	*leftover[1024];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (leftover[fd] && ft_strchr(leftover[fd], '\n'))
		return (fill_line(&(leftover[fd])));
	if (!leftover[fd])
		leftover[fd] = ft_strdup("");
	if (!leftover[fd])
		return (NULL);
	line = read_data_from_buffer(&(leftover[fd]), fd);
	if (!line && leftover[fd] && leftover[fd][0] != '\0')
	{
		line = ft_strdup(leftover[fd]);
		free(leftover[fd]);
		leftover[fd] = NULL;
	}
	if (!line && (!leftover[fd] || *leftover[fd] == '\0'))
		return (NULL);
	return (line);
}
