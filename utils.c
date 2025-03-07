/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdel-ha <abdel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:51:29 by abdel-ha          #+#    #+#             */
/*   Updated: 2025/03/07 15:23:07 by abdel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	set_path_line(t_data **data, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			(*data)->path_line = ft_strdup(env[i] + 5);
			return ;
		}
		i++;
	}
}

char	*get_path(t_data **data, char *command)
{
	int		i;
	char	**paths;
	char	*full_path;

	paths = ft_split((*data)->path_line, ":");
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_calloc(ft_strlen(paths[i]) + ft_strlen(command) + 2, 1);
		if (!full_path)
			return (free_2d(paths), NULL);
		ft_strcpy(full_path, paths[i]);
		ft_strlcat(full_path, "/", ft_strlen(paths[i]) + ft_strlen(command)
			+ 2);
		ft_strlcat(full_path, command, ft_strlen(paths[i]) + ft_strlen(command)
			+ 2);
		if (access(full_path, X_OK) == 0)
			return (free_2d(paths), full_path);
		free(full_path);
		i++;
	}
	return (free_2d(paths), NULL);
}

void	free_command(t_cmd **cmd)
{
	int	i;

	i = 0;
	while ((*cmd)->command[i])
	{
		free((*cmd)->command[i]);
		i++;
	}
	free((*cmd)->command);
}

void	free_2d(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	clean_all(t_data **data)
{
	t_cmd	*tmp;
	t_cmd	*next;

	tmp = (*data)->cmds;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->path)
			free(tmp->path);
		free_command(&tmp);
		free(tmp);
		tmp = next;
	}
	if ((*data)->path_line)
		free((*data)->path_line);
	if ((*data)->here_doc)
		close((*data)->here_doc_fd);
	free(*data);
}
