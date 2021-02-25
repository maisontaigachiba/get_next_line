/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchiba <tchiba@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 12:35:07 by tchiba            #+#    #+#             */
/*   Updated: 2020/11/21 15:00:56 by tchiba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int			ft_lsjoin(char **line, char **save)
{
	char	*tmp;
	char	*newline;

	if ((newline = ft_strchr(*save, '\n')))
	{
		tmp = *line;
		*line = ft_substr(*save, 0, newline - *save);
		free(tmp);
		if (!(*line))
			return (-1);
		tmp = *save;
		*save = ft_substr(newline + 1, 0, ft_strlen(newline + 1));
		free(tmp);
		if (!(*save))
			return (-1);
		return (1);
	}
	else
	{
		tmp = *line;
		*line = *save;
		*save = NULL;
		free(tmp);
		return (-2);
	}
}

int			ft_nlsplit(char **line, char **save, char *buf)
{
	char	*oldline;
	char	*newline;
	char	*tmp;

	newline = ft_strchr(buf, '\n');
	if (!(tmp = ft_substr(buf, 0, newline - buf)))
		return (-1);
	oldline = *line;
	*line = ft_strjoin(*line, tmp);
	free(oldline);
	free(tmp);
	if (!(*line))
		return (-1);
	if (!(*save = ft_substr(newline + 1, 0, ft_strlen(newline + 1))))
		return (-1);
	return (1);
}

int			ft_lbjoin(char **line, char *buf)
{
	char *tmp;

	tmp = *line;
	*line = ft_strjoin(*line, buf);
	free(tmp);
	if (!(*line))
		return (-1);
	return (-2);
}

int			ft_process(int fd, char **line, char **save)
{
	ssize_t				read_size;
	int					ret;
	char				*buf;

	ret = -2;
	if (!(buf = malloc(BUFFER_SIZE + 1)))
		return (-1);
	while (ret == -2 && (read_size = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[read_size] = '\0';
		if (ft_strchr(buf, '\n'))
			ret = ft_nlsplit(line, save, buf);
		else
			ret = ft_lbjoin(line, buf);
	}
	free(buf);
	if (ret == -2 && read_size == 0)
		ret = 0;
	else if (ret == -2 && read_size < 0)
		ret = -1;
	return (ret);
}

int			get_next_line(int fd, char **line)
{
	int				ret;
	static	char	*save;

	if (fd < 0 || !line || BUFFER_SIZE <= 0 || !(*line = ft_strdup("")))
		return (-1);
	ret = -2;
	if (save)
		ret = ft_lsjoin(line, &save);
	if (ret == -2)
		ret = ft_process(fd, line, &save);
	return (ret);
}
