/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdel-ha <abdel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:59:21 by abdel-ha          #+#    #+#             */
/*   Updated: 2025/03/06 17:53:31 by abdel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "piepx.h"

void	child(t_data **data, char **env, int fd[2], t_cmd *tmp)
{
	const int	pid = fork();

	if (pid == -1)
		exit_with_message(data, "fork failed");
	if (pid == 0)
	{
		// close((*data)->infile);
		check_command(data, tmp);
		dup2((*data)->old_fd, 0);
		dup2(fd[1], 1);
		close(fd[1]);
		execve(tmp->path, tmp->command, env);
	}
}

void	handle_first_cmd(t_data **data, char **env, int fd[2], t_cmd *tmp)
{
	const int	pid = fork();

	if (pid == -1)
		exit_with_message(data, "fork failed");
	if (pid == 0)
	{
		check_command(data, tmp);
		if ((*data)->here_doc)
		{
			(*data)->here_doc_fd = open("/tmp/abdo", O_RDONLY);
			if ((*data)->here_doc_fd == -1)
				exit_with_message(data, "Error opening here_doc file");
			dup2((*data)->here_doc_fd, 0);
			close((*data)->here_doc_fd);
		}
		else
		{
			(*data)->infile = open((*data)->infile_name, O_RDONLY);
			(dup2((*data)->infile, 0));
			(close((*data)->infile));
		}
			
		dup2(fd[1], 1);
		close(fd[1]);
		execve(tmp->path, tmp->command, env);
		perror(tmp->command[0]);
		exit(1);
	}
}

void	handle_last_cmd(t_data **data, char **env, int fd[2], t_cmd *tmp)
{
	const int	pid = fork();

	(void)fd;
	if (pid == -1)
		exit_with_message(data, "fork failed");
	if (pid == 0)
	{
		check_command(data, tmp);
		if ((*data)->old_fd == -1)
			exit_with_message(data, "oldfile is closed");
		dup2((*data)->old_fd, 0);
		dup2((*data)->outfile, 1);
		close((*data)->outfile);
		// close((*data)->infile);
		execve(tmp->path, tmp->command, env);
		// close((*data)->old_fd);
		perror(tmp->command[0]);
		exit(1);
	}
	else
	{
		if ((*data)->old_fd != -1)
			close((*data)->old_fd);
	}
}
