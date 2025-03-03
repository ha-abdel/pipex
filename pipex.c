#include "piepx.h"
#include "libft/get_next_line.h"
#include "libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void   free_2d(char **arr)
{
    int i = 0;

    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void   open_files(t_data **data, char **av, int ac)
{
    if (ft_strncmp(av[1], "here_doc", 8) == 0)
    {
        (*data)->here_doc = 1;
        (*data)->limiter = av[2];
        (*data)->nb_cmds = ac - 4;
        (*data)->here_doc_fd = open("/tmp/abdo", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    else
    {
        (*data)->here_doc = 0;
        (*data)->limiter = NULL;
        (*data)->nb_cmds = ac - 3;
        (*data)->infile = open(av[1], O_RDONLY);
    }
    (*data)->outfile = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
}
void    init_vars(t_data **data, char **av, int ac)
{
    (*data)->infile = 0;
    (*data)->here_doc_fd = 0;
    open_files(data, av, ac);
    if((*data)->infile == -1 || (*data)->outfile == -1 || (*data)->here_doc_fd == -1)
        {
            perror("Error");
            exit(1);
        }
    (*data)->cmds = NULL;
    (*data)->old_fd = -1;
    // (*data)->pid = malloc(sizeof(int) * (*data)->nb_cmds);
    // (*data)->pid_index = 0;
    

}

void    set_path_line(t_data **data, char **env)
{
    int i = 0;

    while (env[i])
    {
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
        {
            (*data)->path_line = ft_strdup(env[i] + 5);
            return;
        }
        i++;
    }
}

void    free_command(t_cmd **cmd)
{
    int i = 0;

    while ((*cmd)->command[i])
    {
        free((*cmd)->command[i]);
        i++;
    }
    free((*cmd)->command);
}

void    clean_all(t_data **data)
{
    t_cmd *tmp;
    t_cmd *next;

    tmp = (*data)->cmds;
    while (tmp)
    {
        next = tmp->next;
        if(tmp->path)
            free(tmp->path);
        free_command(&tmp);
        free(tmp);
        tmp = next;
    }
    if((*data)->path_line)
        free((*data)->path_line);
    if((*data)->here_doc)
        close((*data)->here_doc_fd);
    close((*data)->infile);
    close((*data)->outfile);
    free(*data);
}

char    *get_path(t_data **data, char *command)
{
    int i;
    char **paths;
    char *full_path;

    paths = ft_split((*data)->path_line, ":");
    if(!paths)
        return (NULL);
    i = 0;
    while (paths[i])
    {
        full_path = ft_calloc(ft_strlen(paths[i]) + ft_strlen(command) + 2, 1);
        if (!full_path)
            return (NULL);
        ft_strcpy(full_path, paths[i]);
        ft_strlcat(full_path, "/", ft_strlen(paths[i]) + 1);
        ft_strlcat(full_path, command, ft_strlen(paths[i]) + ft_strlen(command) + 1);
        if (access(full_path, X_OK) == 0)
        {
            free_2d(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    free_2d(paths);
    return (NULL);
}


void    fill_command(t_data **data, int ac, char **av)
{
    int i;
    t_cmd *current;
    t_cmd *new_cmd;

    i = 2;
    if ((*data)->here_doc)
        i = 3;
    current = NULL;
    while (i < ac - 1)
    {
        new_cmd = ft_lstnew1(ft_split(av[i], " \t"), get_path(data, ft_split(av[i], " \t")[0]), av[i]);
        if (!new_cmd)
        {
            clean_all(data);
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
    wait(NULL);
    return (0);
}
