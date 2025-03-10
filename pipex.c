/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdel-ha <abdel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:51:09 by abdel-ha          #+#    #+#             */
/*   Updated: 2025/03/10 12:55:42 by abdel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exit_with_message(t_data **data, char *msg)
{
	ft_putstr_fd(msg, 2);
	clean_all(data);
	exit(1);
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;
	int		fd[2];
	int		i;

	if (ac < 5 || (ac < 6 && strncmp(av[1], "here_doc", 9) == 0))
		return (ft_putstr_fd("arguments are not valid\n", 2), 1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	data->outfile_name = NULL;
	init_vars(&data, av, ac, fd);
	set_path_line(&data, env);
	fill_command(&data, ac, av);
	execute_commands(&data, env, fd);
	close(fd[0]);
	i = 0;
	while (i < data->nb_cmds)
	{
		if (wait(NULL) == -1)
			exit_with_message(&data, "wait for child failed\n");
		i++;
	}
	clean_all(&data);
	return (0);
}
