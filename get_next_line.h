/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 01:43:44 by gostroum          #+#    #+#             */
/*   Updated: 2025/05/27 17:19:02 by gostroum         ###   ########.fr       */
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
	int		symb;
	int		len;
}	t_stash;

char	*get_next_line(int fd);

#endif
