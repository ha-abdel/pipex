#include "piepx.h"
#include "libft/get_next_line.h"
#include "libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
// #include <p>

void    init_vars(t_data **data, char **av, int ac)
{
    (*data)->limiter = NULL;
    (*data)->infile = open(av[1], O_RDONLY);
    (*data)->outfile = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    (*data)->cmds = NULL;
    (*data)->old_fd = -1;
    if((*data)->infile == -1 || (*data)->outfile == -1)
    {
        perror("Error");
        exit(1);
    }
    if (ft_strncmp(av[1], "here_doc", 8) == 0)
    {
        (*data)->here_doc = 1;
        (*data)->limiter = av[2];
        (*data)->nb_cmds = ac - 4;
    }
    else
    {
        (*data)->here_doc = 0;
        (*data)->nb_cmds = ac - 3;
    }
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
    // free(path_env);
    i = 0;
    while (paths[i])
    {
        full_path = malloc(strlen(paths[i]) + strlen(command) + 2);
        if (!full_path)
            return (NULL);
        strcpy(full_path, paths[i]);
        strcat(full_path, "/");
        strcat(full_path, command);
        if (access(full_path, X_OK) == 0)
        {
            // ft_free_split(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    // ft_free_split(paths);
    return (NULL);
}

void    fill_command(t_data **data, int ac, char **av)
{
    int i = 2;
    t_cmd *current;
    t_cmd *new_cmd;

    if ((*data)->here_doc)
        i = 3;
    current = NULL;
    while (i < ac - 1)
    {
        new_cmd = malloc(sizeof(t_cmd));
        if (!new_cmd)
            return;
        new_cmd->command = ft_split(av[i], "    ");
        new_cmd->path = get_path(data, new_cmd->command[0]);
        new_cmd->next = NULL;
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

void    child(t_data **data, char **env, int fd[2], t_cmd *tmp)
{
    int pid = fork();
    if(pid == 0)
    {
        dup2((*data)->old_fd, 0);
        dup2(fd[1], 1);
        if(tmp->path == NULL)
        {
            printf("command not found\n");
            clean_all(data);
            exit(1);
        }
        execve(tmp->path, tmp->command, env);
    }
    else
    {
        waitpid(pid, NULL, 0);
        close(fd[1]);
        close((*data)->old_fd);
        (*data)->old_fd = fd[0];
    }
}

void    handle_first_cmd(t_data **data, char **env, int fd[2], t_cmd *tmp)
{
    int pid = fork();
    if(pid == 0)
    {
        close(fd[0]);
        dup2((*data)->infile, 0);
        close((*data)->infile);
        if((*data)->nb_cmds == 1)
            dup2((*data)->outfile, 1);
        else
        {
            dup2(fd[1], 1);
            close(fd[1]);
        }
        if(tmp->path == NULL)
        {
            printf("command not found\n");
            exit(1);
        }
        execve(tmp->path, tmp->command, env);
    }
    else
    {
        close(fd[1]);
        if((*data)->old_fd != -1)
            close((*data)->old_fd);
        (*data)->old_fd = fd[0];
        // waitpid(pid, NULL, 0);
        // close(fd[0]);
    }
}

void    handle_last_cmd(t_data **data, char **env, int fd[2], t_cmd *tmp)
{
    int pid = fork();
    if(pid == 0)
    {
        dup2((*data)->old_fd, 0);
        dup2((*data)->outfile, 1);
        close((*data)->old_fd);
        close((*data)->outfile);
        if(tmp->path == NULL)
        {
            ft_putstr_fd("command not found\n", 2);
            exit(1);
        }
        execve(tmp->path, tmp->command, env);
        perror(tmp->command[0]);
    }
    else
    {
        close(fd[1]);
        if((*data)->old_fd != -1)
            close((*data)->old_fd);
        (*data)->old_fd = fd[0];
        // waitpid(pid, NULL, 0);
        // close(fd[0]);
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
