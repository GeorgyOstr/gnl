/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 01:47:44 by gostroum          #+#    #+#             */
/*   Updated: 2025/05/27 17:18:18 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoinf(char *s1, char const *s2)
{
	ssize_t	len1;
	ssize_t	len2;
	char	*buf;

	if (!s1 || !s2)
		return (NULL);
	len1 = 0;
	while (s1[len1])
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
		buf[len1] = s1[len1];
	free(s1);
	return (buf);
}

//Contains EOF or ENDL -> 1 else 0
int	has_line(char *s, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (!s[i] || s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*get_line(char **s, int fd, size_t len)
{
	char	*buffer;
	int		read_res;

	read_res = 1;
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	while (read_res > 0 && !(has_line(*s, len)))
	{
		read_res = read(fd, buffer, BUFFER_SIZE);
		if (read_res <= 0)
		{
			free(buffer);
			return (NULL);
		}
		*s = ft_strjoinf(*s, buffer);
		if (!(*s))
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (*s);
}

int	make_line(char **s, char **out, ssize_t curr_len)
{
	ssize_t	i;
	ssize_t	len;

	i = 0;
	if (!s || !*s)
		return (-1);
	while ((*s)[i] && (*s)[i] != '\n')
		i++;
	len = i;
	*out = malloc(len + 1);
	if (!out)
		return (-1);
	i = 0;
	while ((*s)[i] && (*s)[i] != '\n')
	{
		(*out)[i] = (*s)[i];
		i++;
	}
	if ((*s)[len] == '\0')
		return (0);
	return (1);
}

char	*get_next_line(int fd)
{
	static char		*s = NULL;
	static int		curr_fd = -1;
	static size_t	curr_len = 0;
	char			*tmp;
	int				i;

	i = 0;
	if (BUFFER_SIZE <= 0 || fd < 0 || (s != NULL && curr_fd != fd))
	{
		free(s);
		return (NULL);
	}
	curr_fd = fd;
	tmp = get_line(&s, fd, curr_len);
	if (!tmp)
	{
		free(s);
		return (NULL);
	}
	i = make_line(&s, &tmp, curr_len);
	if (i == -1)
	{
		free(s);
		return (NULL);
	}
	else if (i == 0)
	{
		free(s);
		s = NULL;
		curr_fd = -1;
		curr_len = 0;
		return (tmp);
	}
	return (tmp);
}
