#include "piepx.h"

void   open_files(t_data **data, char **av, int ac)
{
    if (ft_strncmp(av[1], "here_doc", 9) == 0)
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
        // (*data)->infile = open(av[1], O_RDONLY);
        (*data)->outfile = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
}
void    init_vars(t_data **data, char **av, int ac, int fd[2])
{
    (*data)->infile = 0;
    (*data)->infile_name = av[1];
    (*data)->here_doc_fd = 0;
    (*data)->cmds = NULL;
    (*data)->path_line = NULL;
    open_files(data, av, ac);
    if((*data)->infile == -1 || (*data)->outfile == -1 || (*data)->here_doc_fd == -1)
    {
        clean_all(data);
        perror("Error");
        exit(1);
    }
    (*data)->cmds = NULL;
    (*data)->old_fd = -1;
    if((*data)->here_doc)
    {
        handle_here_doc(data, fd);
        close((*data)->here_doc_fd);
    }

}