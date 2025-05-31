/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 01:43:44 by gostroum          #+#    #+#             */
/*   Updated: 2025/05/31 22:57:57 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE	42
# endif

# include <stdlib.h>
# include <unistd.h>

typedef struct s_stash
{
	char	*s;
	int		fd;
	int		finished;
	ssize_t	len;
}	t_stash;

void	ft_memcpy(void *dst, const void *src, size_t n);
int		has_endl(char *buf, ssize_t bytes);
int		update(t_stash *s, const char *buf, ssize_t bytes);
int		separate_endl(t_stash *s, char **ans, ssize_t i);
int		make_line(t_stash *s, char **ans);

char	*get_next_line(int fd);

#endif
