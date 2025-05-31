/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:19:32 by gostroum          #+#    #+#             */
/*   Updated: 2025/05/27 19:20:20 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*cdst;
	const char	*csrc;
	size_t		i;

	cdst = (char *)dst;
	csrc = (const char *)src;
	i = 0;
	while (i < n)
	{
		cdst[i] = csrc[i];
		i++;
	}
}

int	has_endl(char *buf, ssize_t bytes)
{
	int	i;

	i = 0;
	while (i < bytes)
	{
		if (buf[i++] == '\n')
			return (1);
	}
	return (0);
}

int	separate_endl(t_stash *s, char **ans, ssize_t i)
{
	char	*t;

	t = malloc(s->len - i - 1);
	if (!t)
		return (0);
	*ans = malloc(i + 2);
	if (!(*ans))
	{
		free(t);
		return (0);
	}
	ft_memcpy(*ans, s->s, i + 1);
	(*ans)[i + 1] = '\0';
	ft_memcpy(t, s->s + i + 1, s->len - i - 1);
	free(s->s);
	s->s = t;
	s->len = s->len - i - 1;
	return (1);
}

int	make_line(t_stash *s, char **ans)
{
	ssize_t	i;

	i = 0;
	while (i < s->len)
	{
		if (s->s[i] == '\n')
		{
			if (!separate_endl(s, ans, i))
				return (0);
			return (1);
		}
		i++;
	}
	*ans = malloc(s->len + 1);
	if (!(*ans))
		return (0);
	ft_memcpy(*ans, s->s, s->len);
	(*ans)[s->len] = '\0';
	s->finished = 1;
	return (1);
}

int	update(t_stash *s, const char *buf, ssize_t bytes)
{
	char	*tmp;

	tmp = malloc(s->len + bytes);
	if (!tmp)
		return (0);
	ft_memcpy(tmp, s->s, s->len);
	ft_memcpy(tmp + s->len, buf, bytes);
	free(s->s);
	s->s = tmp;
	s->len = s->len + bytes;
	return (1);
}
