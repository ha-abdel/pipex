#include "piepx.h"
#include "libft/get_next_line.h"
#include "libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
// #include <p>

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
    free(*data);
}

char    *get_path(t_data **data, char *command)
{
    int i = 0;
    char **paths;
    char *full_path;

    paths = ft_split((*data)->path_line, ":");
    if(!paths)
        return NULL;
    i = 0;
    while (paths[i])
    {
        full_path = malloc(ft_strlen(paths[i]) + ft_strlen(command) + 2);
        if (!full_path)
            return (NULL);
        ft_strcpy(full_path, paths[i]);
        strcat(full_path, "/");
        strcat(full_path, command);
        if (access(full_path, X_OK) == 0)
        {
            // free_split(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    // free_split(paths);
    return (NULL);
}


void    fill_command(t_data **data, int ac, char **av)
{
    int i;
    t_cmd *current;
    t_cmd *new_cmd;
    char *path;

    i = 2;
    if ((*data)->here_doc)
        i = 3;
    current = NULL;
    while (i < ac - 1)
    {
        // new_cmd = malloc(sizeof(t_cmd));
        // if (!new_cmd)
        //     return;
        // new_cmd->command = ft_split(av[i], " \t");
        // if (!new_cmd->command[0])
        // {
        //     new_cmd->command = calloc(2 , sizeof(char *));
        //     if (!new_cmd->command)
        //         return;
        //     new_cmd->command[0] = ft_strdup(av[i]);
        //     new_cmd->command[1] = NULL;
        // }
        // new_cmd->path = get_path(data, new_cmd->command[0]);
        // new_cmd->next = NULL;
        // if (!current)
        //     (*data)->cmds = new_cmd;
        // else
        //     current->next = new_cmd;
        // current = new_cmd;
        new_cmd = ft_lstnew1(ft_split(av[i], " \t"), get_path(data, ft_split(av[i], " \t")[0]), av[i]);
        i++;
    }
}

void    print_commands(t_data *data)
{
    t_cmd *tmp;

    tmp = data->cmds;
    while (tmp)
    {
        // int i = 0;
        printf("%s %s", tmp->path, tmp->command[0]);
        // while (tmp->command[i])
        // {
        //     printf("%s ", tmp->command[i]);
        //     i++;
        // }
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
