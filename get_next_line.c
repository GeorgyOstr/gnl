/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 01:47:44 by gostroum          #+#    #+#             */
/*   Updated: 2025/05/27 21:04:26 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoinf(t_stash *s1, char const *s2)
{
	ssize_t	len1;
	ssize_t	len2;
	char	*buf;

	if (!s1->s || !s2)
		return (NULL);
	len1 = 0;
	while (s1->s[len1])
		len1++;
	len2 = 0;
	while (s2[len2])
		len2++;
	buf = malloc(len1 + len2 + 1);
	if (!buf)
		return (NULL);
	buf[len1 + len2] = '\0';
	while (--len2 >= 0)
		buf[len1 + len2] = s2[len2];
	while (--len1 >= 0)
		buf[len1] = s1->s[len1];
	free(s1->s);
	return (buf);
}

//Contains EOF or ENDL -> 1 else 0
int	has_line(t_stash *s)
{
	ssize_t	i;

	i = 0;
	while (i < s->len)
	{
		if (!s->s[i] || s->s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

int	read_until_nl(t_stash *s)
{
	char	*buffer;
	int		read_res;

	read_res = 1;
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (0);
	while (read_res > 0 && !(has_line(s)))
	{
		read_res = read(s->fd, buffer, BUFFER_SIZE);
		if (read_res < 0)
			return (0);
		s->s = ft_strjoinf(s, buffer);
		if (!(s->s))
		{
			free(buffer);
			return (0);
		}
	}
	free(buffer);
	return (1);
}

char	*stash_set(t_stash *s_in, char *s, int fd, int len)
{
	t_stash	stash;

	free(s_in->s);
	stash.s = s;
	stash.fd = fd;
	stash.len = len;
	*s_in = stash;
	return (stash.s);
}

//fail = 0 endl = 1 eof = 2
int	make_line(t_stash *s, char **out)
{
	ssize_t	i;
	ssize_t	len;
	char	*tmp;

	i = 0;
	if (!s->s)
		return (-1);
	while (s->s[i] && s->s[i] != '\n')
		i++;
	len = i;
	*out = malloc(len + 1);
	if (!out)
		return (0);
	i = 0;
	while (s->s[i] && s->s[i] != '\n')
	{
		(*out)[i] = s->s[i];
		i++;
	}
	tmp = malloc(s->len - i);
	if (!tmp)
	{
		free(out);
		return (0);
	}
	while (i < s->len)
	{
		tmp[i - len] = s->s[i];
		i++;
	}
	stash_set(s, tmp, -1, 0);
	if (s->s[len] == '\0')
		return (2);
	return (1);
}

char	*get_next_line(int fd)
{
	static t_stash	s;
	int				result_ok;
	char			*tmp;

	if (BUFFER_SIZE <= 0 || fd < 0 || (s.s != NULL && s.fd != fd))
		return (stash_set(&s, NULL, -1, 0));
	s.fd = fd;
	result_ok = read_until_nl(&s);
	if (!result_ok)
		return (stash_set(&s, NULL, -1, 0));
	result_ok = make_line(&s, &tmp);
	if (!result_ok)
		return (stash_set(&s, NULL, -1, 0));
	else if (result_ok == 2)
		stash_set(&s, NULL, -1, 0);
	return (tmp);
}
