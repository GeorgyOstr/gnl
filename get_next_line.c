/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 01:47:44 by gostroum          #+#    #+#             */
/*   Updated: 2025/05/29 22:27:32 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	memcpy(*ans, s->s, s->len);
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
	memcpy(tmp, s->s, s->len);
	memcpy(tmp + s->len, buf, bytes);
	free(s->s);
	s->s = tmp;
	s->len = s->len + bytes;
	return (1);
}

int	buf_free(char *p, int i)
{
	free(p);
	return (i);
}

int	read_until_nl(t_stash *s, int fd)
{
	char	*buf;
	ssize_t	bytes;

	buf = malloc(BUFFER_SIZE);
	if (!buf)
		return (0);
	bytes = read(fd, buf, BUFFER_SIZE);
	if (bytes < 0)
		return (buf_free(buf, 0));
	if (bytes == 0)
		return (buf_free(buf, 1));
	while (1)
	{
		if (!update(s, buf, bytes))
			return (buf_free(buf, 0));
		if (bytes < BUFFER_SIZE)
		{
			s->eof = 1;
			return (buf_free(buf, 1));
		}
		if (has_endl(buf, bytes))
			return (buf_free(buf, 1));
		bytes = read(fd, buf, BUFFER_SIZE);
	}
	free(buf);
	if (bytes < 0)
		return (0);
	return (1);
}

char	*reset_stash(t_stash *s)
{
	free(s->s);
	s->eof = 0;
	s->endl = 0;
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
	if (!s.eof && !read_until_nl(&s, fd))
		return (reset_stash(&s));
	if (!s.len)
		return (reset_stash(&s));
	if (!make_line(&s, &ans))
		return (reset_stash(&s));
	return (ans);
}

// #include <stdio.h>
// #include <fcntl.h>

// int	main(int argc, char **argv)
// {	
// 	char	*tmp;
// 	int		fd;

// 	if (argc == 2)
// 	{
// 		fd = open(argv[1], O_RDONLY);
// 		while ((tmp = get_next_line(fd)))
// 		{
// 			printf("%s", tmp);
// 			free(tmp);
// 		}	
// 		close(fd);	
// 	}
// 	else
// 	{
// 		int i = 0;
// 		fd = open("f", O_RDONLY);
// 		while (i < 3)
// 		{
// 			tmp = get_next_line(fd);
// 			printf("Line: '%s'\n", tmp);
// 			free(tmp);
// 			i++;
// 		}
// 		close(fd);		
// 	}
// 	return (0);
// }
