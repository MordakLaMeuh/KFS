#include <ltrace.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#define __GROW 16
#define __GETDELIM_MIN_LINE 4

/*
 * getdelime- delimited string input
 * ssize_t  getdelim(char **line, size_t *n, int delim, FILE *stream)
 * The buffer should be freed by the user program even if getline()/getdelim() failed
 */
int  getdelim(char **line, size_t *n, int delim,
		  FILE *stream)
{
	TRACE
	unsigned char	del = (unsigned char)delim;
	size_t		count = 0;
	int		read_c;
	unsigned char	c;

	if (!line || !n || !stream) {
		errno = EINVAL;
		return -1;
	}

	if (!*line) {
		char	*new_line = malloc(__GETDELIM_MIN_LINE);

		if (!new_line) {
			errno = ENOMEM;
			return -1;
		}
		*line = new_line;
	}

	while (EOF != (read_c = fgetc(stream))) {
		c = (unsigned char)read_c;

		if (count + 1 >= SSIZE_MAX) {
			errno = EOVERFLOW;
			return -1;
		}

		if (count + 1 > *n) {
			size_t	new_size = count + __GROW;
			char	*new_line;
			new_line = realloc(*line, new_size + 1);

			if (!new_line) {
				errno = ENOMEM;
				return -1;
			}
			*line = new_line;
		}
		(*line)[count] = c;
		count++;
		if (c == del) {
			break;
		}
	}
	*n = count;
	(*line)[count] = '\0';

	if (ferror(stream)) {
		return -1;
	}
	else if (feof(stream) && count == 0) {
		return -1;
	}

	return (int)count;
}

#undef __GROW
#undef __GETDELIM_MIN_LINE
