/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 01:47:44 by gostroum          #+#    #+#             */
/*   Updated: 2025/05/31 20:48:42 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	buf_free(char *p, int i)
{
	free(p);
	return (i);
}

static int	read_until_nl(t_stash *s, int fd)
{
	char	*buf;
	ssize_t	bytes;

	buf = malloc(BUFFER_SIZE);
	if (!buf)
		return (0);
	bytes = read(fd, buf, BUFFER_SIZE);
	while (bytes > 0)
	{
		if (!update(s, buf, bytes))
			return (buf_free(buf, 0));
		if (bytes == 0)
		{
			s->eof = 1;
			return (buf_free(buf, 1));
		}
		if (has_endl(buf, bytes))
			return (buf_free(buf, 1));
		bytes = read(fd, buf, BUFFER_SIZE);
	}
	free(buf);
	return (bytes >= 0);
}

static char	*reset_stash(t_stash *s)
{
	free(s->s);
	s->eof = 0;
	s->finished = 0;
	s->s = 0;
	s->fd = 0;
	s->len = 0;
	return (NULL);
}

char	*get_next_line(int fd)
{
	static t_stash	s;
	char			*ans;

	ans = NULL;
	if (s.finished || BUFFER_SIZE <= 0 || fd < 0 || (s.s != NULL && s.fd != fd))
		return (reset_stash(&s));
	s.fd = fd;
	if (!s.eof && !has_endl(s.s, s.len) && !read_until_nl(&s, fd))
		return (reset_stash(&s));
	if (!s.len)
		return (reset_stash(&s));
	if (!make_line(&s, &ans))
		return (reset_stash(&s));
	return (ans);
}
