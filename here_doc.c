#include "piepx.h"

void	ft_merge_strings(char **s1, char *s2)
{
	char	*res;

	res = ft_strjoin(*s1, s2);
	free(*s1);
	(*s1) = res;
}

void	check_line(t_data **data, char **line, char **total)
{
	while (1)
	{
		*line = get_next_line(0);
		ft_merge_strings(total, *line);
		if (*line && (*line)[ft_strlen(*line) - 1] == '\n')
		{
			if (ft_strncmp(*total, (*data)->limiter,
					ft_strlen((*data)->limiter)) == 0 && ft_strncmp(*total
					+ ft_strlen((*data)->limiter), "\n", 2) == 0)
			{
				(free(*line)), (free(*total));
				*line = NULL;
				break ;
			}
			else
			{
				ft_putstr_fd(*line, (*data)->here_doc_fd);
				(free(*line)), (free(*total));
				*total = NULL;
				write(1, "here_doc>", 9);
			}
		}
	}
}

void	handle_here_doc(t_data **data, int fd[2])
{
	char	*line;
	char	*total;

	total = NULL;
	(void)fd;
	write(1, "here_doc>", 9);
	check_line(data, &line, &total);
}