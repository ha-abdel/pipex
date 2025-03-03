#include "piepx.h"



void   open_files(t_data **data, char **av, int ac)
{
    if (ft_strncmp(av[1], "here_doc", 8) == 0)
    {
        (*data)->here_doc = 1;
        (*data)->limiter = av[2];
        (*data)->nb_cmds = ac - 4;
        (*data)->here_doc_fd = open("/tmp/abdo", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        (*data)->outfile = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
    }
    else
    {
        (*data)->here_doc = 0;
        (*data)->limiter = NULL;
        (*data)->nb_cmds = ac - 3;
        (*data)->infile = open(av[1], O_RDONLY);
        (*data)->outfile = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
}
void    init_vars(t_data **data, char **av, int ac)
{
    (*data)->infile = 0;
    (*data)->here_doc_fd = 0;
    (*data)->cmds = NULL;
    (*data)->path_line = NULL;
    // (*data)->pid = NULL;
    // (*data)->pid_index = 0;
    open_files(data, av, ac);
    if((*data)->infile == -1 || (*data)->outfile == -1 || (*data)->here_doc_fd == -1)
    {
        clean_all(data);
        perror("Error");
        exit(1);
    }
    (*data)->cmds = NULL;
    (*data)->old_fd = -1;
    // (*data)->pid = malloc(sizeof(int) * (*data)->nb_cmds);
    // (*data)->pid_index = 0;
    

}


void    fill_command(t_data **data, int ac, char **av)
{
    int i;
    t_cmd *current;
    t_cmd *new_cmd;
    char **tmp;

    i = 2 + (*data)->here_doc;
    // if ((*data)->here_doc)
    //     i = 3;
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

void    print_commands(t_data *data)
{
    t_cmd *tmp;

    tmp = data->cmds;
    while (tmp)
    {
        printf("%s %s", tmp->path, tmp->command[0]);
        printf("\n");
        tmp = tmp->next;
    }
}



int main(int ac, char **av, char **env)
{
    t_data *data;
    int i;
    int fd[2];
    t_cmd *tmp;

    if (ac < 4)
        return (1);
    i = 0;
    data = malloc(sizeof(t_data));
    if (!data)
        return (1);
    init_vars(&data, av, ac);
    set_path_line(&data, env);
    fill_command(&data, ac, av);
    if(data->here_doc)
        handle_here_doc(&data, fd);
    // print_commands(data);
    tmp = data->cmds;
    while (i < data->nb_cmds)
    {
        if(data->nb_cmds > 1 && i != data->nb_cmds - 1)
            pipe(fd);
        if(i == 0)
            handle_first_cmd(&data, env, fd, tmp);
        else if(i == data->nb_cmds - 1)
            handle_last_cmd(&data, env, fd, tmp);
        else
            child(&data, env, fd, tmp);
        tmp = tmp->next;
        i++;
    }
    // wait(NULL);
    clean_all(&data);
    return (0);
}
