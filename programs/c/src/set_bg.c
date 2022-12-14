/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stoupin <stoupin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/18 22:13:29 by bmickael          #+#    #+#             */
/*   Updated: 2018/02/01 16:50:35 by stoupin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <stropts.h>

#include "internal_bmp.h"

static void paste_fileheader(t_bitmap *s, char *filename)
{
	printf("{green}Chargement de l'image %s:{eoc}\n", filename);
	printf("%s %c%c\n", "signature", s->fileheader.signature[0], s->fileheader.signature[1]);
	printf("%s: %i\n", "filesize", s->fileheader.filesize);
	printf("%s: %i\n", "offset", s->fileheader.fileoffset_to_pixelarray);
	printf("%s: %i\n", "header_size", s->bitmapinfoheader.dibheadersize);
	printf("%s: %i\n", "width", s->bitmapinfoheader.width);
	printf("%s: %i\n", "height", s->bitmapinfoheader.height);
	printf("%s: %i\n", "planes", s->bitmapinfoheader.planes);
	printf("%s: %i\n", "bpp", s->bitmapinfoheader.bitsperpixel);
	printf("%s: %i\n", "compression", s->bitmapinfoheader.compression);
	printf("%s: %i\n", "imagesize", s->bitmapinfoheader.imagesize);
	printf("%s: %i\n", "xpermeter", s->bitmapinfoheader.ypixelpermeter);
	printf("%s: %i\n", "ypermeter", s->bitmapinfoheader.xpixelpermeter);
	printf("%s: %i\n", "numcolorpal", s->bitmapinfoheader.numcolorspallette);
}

static void fill_image(uint8_t *data, uint8_t *pixelbuffer, int width, int height, size_t out_bpp)
{
	size_t i;
	int p;
	int c;
	uint8_t *ptr;

	p = height - 1;
	ptr = pixelbuffer + (p * width * 3);
	c = 0;
	i = 0;
	while (p >= 0)
	{
		/*
		 * This is applicable only if a 32 bits/pixels screen on output
		 */
		if (out_bpp == 32) {
			if ((i % 4) == 3)
				i++;
		}
		data[i] = ptr[c++];
		if (c == (width * 3))
		{
			p--;
			ptr = pixelbuffer + (p * width * 3);
			c = 0;
		}
		i++;
	}
}

#define BUFFER_SIZE (4096 * 16)

uint8_t *read_to_end(int fd) {
	uint8_t *tmp_buffer = malloc(BUFFER_SIZE);
	uint8_t *buffer = malloc(BUFFER_SIZE);
	size_t	size = 0;
	int len_readen;

	while ((len_readen = read(fd, tmp_buffer, BUFFER_SIZE)) > 0) {
		buffer = realloc(buffer, size + len_readen);
		if (buffer == NULL) {
			printf("no memory to allocate buffer");
			exit(1);
		}
		memcpy(buffer + size, tmp_buffer, len_readen);
		size += len_readen;
	}
	if (len_readen == -1) {
		perror("read");
		exit(1);
	}
	free(tmp_buffer);
	return buffer;
}

int				bmp_load(char *filename, int *width, int *height, int **data, size_t out_bpp)
{
	t_bitmap *s;

	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(1);
	}
	s = (t_bitmap *)read_to_end(fd);

	paste_fileheader((t_bitmap *)s, filename);
	*width = s->bitmapinfoheader.width;
	*height = s->bitmapinfoheader.height;
	if (!(*data = (int *)calloc((*width) * (*height), sizeof(int))))
		exit(EXIT_FAILURE);
	fill_image((uint8_t *)*data, (uint8_t *)
			((char*)s + s->fileheader.fileoffset_to_pixelarray),
		   *width, *height, out_bpp);
	free(s);
	return 0;

	/* 
	 * if (!(infos = (struct stat *)malloc(sizeof(struct stat))))
	 * 	exit(EXIT_FAILURE);
	 * if ((stat(filename, infos)) == -1 || (!(file = fopen(filename, "rb"))))
	 * {
	 * 	eprintf("%s\n", strerror(errno));
	 * 	exit(EXIT_FAILURE);
	 * }
	 * if (!(s = (t_bitmap *)malloc(infos->st_size)))
	 * 	exit(EXIT_FAILURE);
	 * fread(s, infos->st_size, 1, file);
	 */
}

int main(int ac, char **av)
{
	if (ac != 2) {
		dprintf(STDERR_FILENO, "usage: bmploader image_file\n");
		return 1;
	}
	struct local_buffer local_buffer = {0};

	ioctl(0, GET_FRAME_BUFFER_PTR, (void *)&local_buffer);
	printf("local_buffer: %p len: %zu bpp: %zu\n", local_buffer.buf, local_buffer.len, local_buffer.bpp);

	void *data = NULL;
	int width = 0;
	int height = 0;

	if (bmp_load(av[1], &width, &height, (int **)&data, local_buffer.bpp) == -1) {
		printf("bmp load failed\n");
		exit(1);
	}

	memcpy(local_buffer.buf, data, local_buffer.len);

	ioctl(0, REFRESH_SCREEN, &local_buffer);
	return 0;
}

/* 
 * int fd = open(av[1], O_RDONLY);
 * if (fd == -1) {
 * 	perror("open");
 * 	exit(1);
 * }
 *
 * uint8_t* data = read_to_end(fd);
 * struct winsize win;
 * memset(&win, 0, sizeof(struct winsize));
 * int ret = ioctl(0, TIOCGWINSZ, &win);
 * if (ret == -1) {
 * 	perror("ioctl");
 * 	exit(1);
 * }
 *
 * size_t width = win.ws_xpixel;
 * size_t height =  win.ws_ypixel;
 *
 *
 * size_t bpp = win.bpp;
 * uint8_t *buffer = malloc(width * height * bpp / 8);
 * if (buffer == NULL) {
 * 	printf("no memory to allocate buffer");
 * 	exit(1);
 * }
 * draw_image((struct BmpImage *)data, buffer, width,height, bpp);
 */
