/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdel-ha <abdel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:59:33 by abdel-ha          #+#    #+#             */
/*   Updated: 2025/03/10 12:28:18 by abdel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	parent_mode(t_data **data, int fd[2], int i)
{
	if (i == (*data)->nb_cmds - 1)
	{
		if ((*data)->old_fd != -1)
			close((*data)->old_fd);
		return ;
	}
	close(fd[1]);
	if ((*data)->old_fd != -1)
		close((*data)->old_fd);
	(*data)->old_fd = fd[0];
}

void	check_command(t_data **data, t_cmd *tmp)
{
	// if (tmp->path == NULL && access(tmp->command[0], X_OK))
	// 	exit_with_message(data, "command not found\n");
	if (tmp->command[0][0] == '\0')
		exit_with_message(data, "permission denied\n");
}

void	execute_commands(t_data **data, char **env, int fd[2])
{
	t_cmd	*tmp;
	int		i;

	tmp = (*data)->cmds;
	i = 0;
	while (i < (*data)->nb_cmds)
	{
		if ((*data)->nb_cmds > 1 && i != (*data)->nb_cmds - 1)
			if (pipe(fd) == -1)
				exit_with_message(data, "pipe creation failed\n");
		if (i == 0)
			handle_first_cmd(data, env, fd, tmp);
		else if (i == (*data)->nb_cmds - 1)
			handle_last_cmd(data, env, tmp);
		else
			child(data, env, fd, tmp);
		parent_mode(data, fd, i);
		tmp = tmp->next;
		i++;
	}
}

void	fill_command(t_data **data, int ac, char **av)
{
	int		i;
	t_cmd	*current;
	t_cmd	*new_cmd;
	char	**tmp;

	i = 2 + (*data)->here_doc;
	current = NULL;
	while (i < ac - 1)
	{
		tmp = ft_split(av[i], " \t");
		new_cmd = ft_lstnew1(tmp, get_path(data, tmp[0]), av[i]);
		if (!new_cmd)
		{
			(free_2d(tmp)), (clean_all(data));
			perror("Error");
			exit(1);
		}
		if (!current)
			(*data)->cmds = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		i++;
	}
}
