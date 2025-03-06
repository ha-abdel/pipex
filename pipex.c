#include "piepx.h"

void	exit_with_message(t_data **data, char *msg)
{
	perror(msg);
	clean_all(data);
	exit(1);
}



int main(int ac, char **av, char **env)
{
    t_data *data;
    int fd[2];

    if ((strncmp(av[1], "here_doc", 9) == 0 && ac < 6) || ac < 5)
        return (ft_putstr_fd("arguments are not valid", 2) , 1);
    data = malloc(sizeof(t_data));
    if (!data)
        return (1);
    init_vars(&data, av, ac, fd);
    set_path_line(&data, env);
    fill_command(&data, ac, av);
    execute_commands(&data, env, fd);
    close(fd[0]);
    if(wait(NULL) == -1)
        exit_with_message(&data, "wait failed\n");
    clean_all(&data);
    return (0);
}


