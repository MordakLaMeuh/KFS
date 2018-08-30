/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/10 14:15:30 by bmickael          #+#    #+#             */
/*   Updated: 2017/04/26 01:31:48 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	char *s;

	c = (unsigned char)c;
	s = (char *)b;

	/*
	for (int i = 0; i < len; i++)
	{
		s[i++] = c;
	}
	*/


	while (len--)
		*s++ = c;

	return (b);
}
