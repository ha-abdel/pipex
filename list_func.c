/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdel-ha <abdel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:50:40 by abdel-ha          #+#    #+#             */
/*   Updated: 2025/03/07 15:22:51 by abdel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_cmd	*ft_lstnew1(char **command, char *path, char *av)
{
	t_cmd	*l;

	l = (t_cmd *)malloc(sizeof(t_cmd));
	if (!l)
		return (NULL);
	l->command = command;
	if (!(l->command[0]))
	{
		l->command = calloc(2, sizeof(char *));
		if (!l->command)
			return (NULL);
		l->command[0] = ft_strdup(av);
		l->command[1] = NULL;
	}
	l->path = path;
	l->next = NULL;
	return (l);
}

void	ft_lstadd_back1(t_cmd **lst, t_cmd *new)
{
	t_cmd	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast1(*lst);
	last->next = new;
}

t_cmd	*ft_lstlast1(t_cmd *lst)
{
	t_cmd	*last;

	if (!lst)
		return (NULL);
	last = lst;
	while (last->next)
	{
		last = last->next;
	}
	return (last);
}
