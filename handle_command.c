#include "piepx.h"

void    handle_here_doc(t_data **data, int fd[2])
{
    (void)fd;
    char *line;

    // (*data)->here_doc_fd = open("/tmp/abdo", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // if ((*data)->here_doc_fd == -1)
    // {
    //     perror("Error opening here_doc file");
    //     exit(1);
    // }
    while (1)
    {
        write(1, "heredoc> ", 9);
        line = get_next_line(0);
        if (!line)
            break;
        if (ft_strncmp(line, (*data)->limiter, ft_strlen((*data)->limiter)) == 0 && line[ft_strlen((*data)->limiter)] == '\n')
        {
            free(line);
            break;
        }
        write((*data)->here_doc_fd, line, ft_strlen(line));
        free(line);
    }
    close((*data)->here_doc_fd);
}

void    check_command(t_data **data, t_cmd *tmp)
{
    if(tmp->path == NULL)
        {
            ft_putstr_fd("command not found\n", 2);
            clean_all(data);
            exit(1);
        }
        if(tmp->command[0][0] == '\0')
        {
            ft_putstr_fd("permission denied\n", 2);
            clean_all(data);
            exit(1);
        }
}

void    child(t_data **data, char **env, int fd[2], t_cmd *tmp)
{
    int pid = fork();
    if(pid == 0)
    {
        check_command(data, tmp);
        dup2((*data)->old_fd, 0);
        close(fd[0]);
        dup2(fd[1], 1);
        execve(tmp->path, tmp->command, env);
    }
    else
    {
        // waitpid(pid, NULL, 0);
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
        check_command(data, tmp);
        if ((*data)->here_doc)
        {
            (*data)->here_doc_fd = open("/tmp/abdo", O_RDONLY);
            if ((*data)->here_doc_fd == -1)
            {
                perror("Error opening here_doc file");
                clean_all(data);
                exit(1);
            }
            dup2((*data)->here_doc_fd, 0);
            close((*data)->here_doc_fd);
        }
            
        else
        {
            close(fd[0]);
            dup2((*data)->infile, 0);
            close((*data)->infile);
        }
        dup2(fd[1], 1);
        close(fd[1]);
        execve(tmp->path, tmp->command, env);
    }
    else
    {
        close(fd[1]);
        if((*data)->old_fd != -1)
            close((*data)->old_fd);
        (*data)->old_fd = fd[0];
    }
}

void    handle_last_cmd(t_data **data, char **env, int fd[2], t_cmd *tmp)
{
    int pid = fork();
    if(pid == 0)
    {
        check_command(data, tmp);
        dup2((*data)->old_fd, 0);
        dup2((*data)->outfile, 1);
        close((*data)->old_fd);
        close((*data)->outfile);
        close(fd[0]);
        close(fd[1]);
        
        execve(tmp->path, tmp->command, env);
        // perror(tmp->command[0]);
    }
    else
    {
        close(fd[1]);
        if((*data)->old_fd != -1)
            close((*data)->old_fd);
        (*data)->old_fd = fd[0];
        // close(fd[0]);
        // waitpid(pid, NULL, 0);
    }
}