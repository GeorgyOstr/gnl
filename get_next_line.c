/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 01:47:44 by gostroum          #+#    #+#             */
/*   Updated: 2025/05/29 21:59:35 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <string.h>

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
	memcpy(*ans, s->s, i + 1);
	(*ans)[i + 1] = '\0';
	memcpy(t, s->s + i + 1, s->len - i - 1);
	free(s->s);
	s->s = t;
	s->len = s->len - i - 1;
	if (!has_endl(s->s, s->len))
		s->endl = 0;
	return (1);
}

int	make_line(t_stash *s, char **ans, int *ended)
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
	if (!ans)
		return (0);
	memcpy(*ans, s->s, s->len);
	(*ans)[s->len] = '\0';
	*ended = 1;
	return (1);
}

int	update(t_stash *s, const char *buf, ssize_t bytes)
{
	char	*tmp;

	tmp = malloc(s->len + bytes);
	if (!tmp)
		return (0);
	memcpy(tmp, s->s, s->len);
	memcpy(tmp + s->len, buf, bytes);
	free(s->s);
	s->s = tmp;
	s->len = s->len + bytes;
	return (1);
}

int	read_until_nl(t_stash *s, int fd)
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
		{
			free(buf);
			return (0);
		}
		if (bytes < BUFFER_SIZE)
		{
			s->eof = 1;
			break ;
		}
		if (has_endl(buf, BUFFER_SIZE))
		{
			s->endl = 1;
			break ;
		}
		bytes = read(fd, buf, BUFFER_SIZE);
	}
	free(buf);
	if (bytes < 0)
		return (0);
	return (1);
}

char	*set_stash(t_stash *s, char *str, int fd, ssize_t len)
{
	free(s->s);
	s->eof = 0;
	s->endl = 0;
	s->finished = 0;
	s->s = str;
	s->fd = fd;
	s->len = len;
	return (s->s);
}

char	*get_next_line(int fd)
{
	static t_stash	s;
	static char		*ans;
	int				ended;

	ans = NULL;
	ended = 0;
	if (s.finished || BUFFER_SIZE <= 0 || fd < 0 || (s.s != NULL && s.fd != fd))
		return (set_stash(&s, NULL, 0, 0));
	s.fd = fd;
	if (!s.eof && !s.endl)
	{
		if (!read_until_nl(&s, fd))
			return (set_stash(&s, NULL, 0, 0));
	}
	if (!make_line(&s, &ans, &ended))
		return (set_stash(&s, NULL, 0, 0));
	if (ended)
	{
		set_stash(&s, NULL, 0, 0);
		s.finished = 1;
	}
	return (ans);
}

//#include <stdio.h>
//#include <fcntl.h>
//
//int	main(int argc, char **argv)
//{	
//	char	*tmp;
//	int		fd;
//
//	if (argc == 2)
//	{
//		fd = open(argv[1], O_RDONLY);
//		while ((tmp = get_next_line(fd)))
//		{
//			printf("%s", tmp);
//			free(tmp);
//		}	
//		close(fd);	
//	}
//	else
//	{
//		fd = open("f", O_RDONLY);
//		while ((tmp = get_next_line(fd)))
//		{
//			printf("%s", tmp);
//			free(tmp);
//		}
//		close(fd);		
//	}
//	return (0);
//}
