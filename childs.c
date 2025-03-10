/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdel-ha <abdel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:59:21 by abdel-ha          #+#    #+#             */
/*   Updated: 2025/03/10 14:56:01 by abdel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	handle_file_with_cmd(t_data **data, t_cmd *tmp, char **env)
{
	char	**command;
	int		len;
	int		i;

	if (errno != ENOEXEC)
		return ;
	i = 1;
	len = get_array_length(tmp);
	command = ft_calloc(len + 2, sizeof(char *));
	if (!command)
		exit_with_message(data, "allocation failed\n");
	command[0] = "bash";
	while (tmp->command[i - 1])
	{
		command[i] = tmp->command[i - 1];
		i++;
	}
	execve("/usr/bin/bash", command, env);
}

void	handle_input(t_data **data)
{
	if ((*data)->here_doc)
	{
		(*data)->here_doc_fd = open("/tmp/abdo", O_RDONLY);
		if ((*data)->here_doc_fd == -1)
			exit_with_message(data, "Error opening here_doc file\n");
		dup2((*data)->here_doc_fd, 0);
		close((*data)->here_doc_fd);
	}
	else
	{
		(*data)->infile = open((*data)->infile_name, O_RDONLY);
		if ((*data)->infile == -1)
			exit_with_message(data, "Error opening infile\n");
		(dup2((*data)->infile, 0));
		(close((*data)->infile));
	}
}

void	child(t_data **data, char **env, int fd[2], t_cmd *tmp)
{
	const pid_t	pid = fork();

	if (pid == -1)
		exit_with_message(data, "fork failed\n");
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		check_command(data, tmp);
		dup2((*data)->old_fd, 0);
		close((*data)->old_fd);
		execve(tmp->path, tmp->command, env);
		handle_file_with_cmd(data, tmp, env);
		perror(tmp->command[0]);
		clean_all(data);
		exit(1);
	}
}

void	handle_first_cmd(t_data **data, char **env, int fd[2], t_cmd *tmp)
{
	const pid_t	pid = fork();

	if (pid == -1)
		exit_with_message(data, "fork failed\n");
	if (pid == 0)
	{
		handle_input(data);
		check_command(data, tmp);
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		execve(tmp->path, tmp->command, env);
		handle_file_with_cmd(data, tmp, env);
		perror(tmp->command[0]);
		clean_all(data);
		exit(1);
	}
}

void	handle_last_cmd(t_data **data, char **env, t_cmd *tmp)
{
	const pid_t	pid = fork();

	if (pid == -1)
		exit_with_message(data, "fork failed\n");
	if (pid == 0)
	{
		check_command(data, tmp);
		if ((*data)->here_doc)
			(*data)->outfile = open((*data)->outfile_name,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			(*data)->outfile = open((*data)->outfile_name,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if ((*data)->outfile == -1)
			exit_with_message(data, "failed to open outfile\n");
		dup2((*data)->outfile, 1);
		close((*data)->outfile);
		dup2((*data)->old_fd, 0);
		close((*data)->old_fd);
		execve(tmp->path, tmp->command, env);
		handle_file_with_cmd(data, tmp, env);
		perror(tmp->command[0]);
		clean_all(data);
		exit(1);
	}
}
