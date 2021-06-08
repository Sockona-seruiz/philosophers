/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wrmalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 15:45:07 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/08 17:05:45 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/*
**	Small wrapper for malloc purpose
**	4 functions
**	-wrgetter : protect list malloc from extern files
**	-malloc : create a malloc, return ptr of the malloc.
**				fail to create malloc : destoy all malloc and return 0
**	-wrfree : free a malloc, return 1.
**				fail to find the element: return 0
**	-wrdestroy : free all malloc created by malloc
*/

#include "philo_two.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*buffer;

	if (!alst)
		return ;
	if (!(*alst))
	{
		*alst = new;
		return ;
	}
	buffer = *alst;
	while (buffer->next != 0)
		buffer = buffer->next;
	buffer->next = new;
}

static t_list	**wrgetter(void)
{
	static t_list	*wrm;

	return (&wrm);
}

void	*wrmalloc(unsigned long size)
{
	char	*buffer;
	t_list	*new_elem;

	buffer = malloc(size);
	if (buffer == 0)
	{
		wrdestroy();
		return (0);
	}
	new_elem = malloc(sizeof(t_list));
	if (new_elem == 0)
	{
		free(buffer);
		wrdestroy();
		return (0);
	}
	new_elem->content = buffer;
	new_elem->next = 0;
	ft_lstadd_back(wrgetter(), new_elem);
	return (buffer);
}

int	wrfree(void *ptr)
{
	t_list	*prev;
	t_list	*current;
	t_list	*next;
	t_list	**wrstart;

	prev = 0;
	wrstart = wrgetter();
	current = *wrstart;
	while (current)
	{
		next = current->next;
		if (current->content == ptr)
		{
			free(ptr);
			free(current);
			if (prev)
				prev->next = next;
			else
				*wrstart = next;
			return (EXIT_SUCCESS);
		}
		prev = current;
		current = current->next;
	}
	return (EXIT_FAILURE);
}

void	wrdestroy(void)
{
	t_list	*current;
	t_list	*next;
	t_list	**wrstart;

	wrstart = wrgetter();
	current = *wrstart;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	*wrstart = 0;
}
