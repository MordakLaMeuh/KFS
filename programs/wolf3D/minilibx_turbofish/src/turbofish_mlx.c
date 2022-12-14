
#ifndef GNU

#include "turbofish_mlx.h"
#include "bmp/bmp.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stropts.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>

const char DEFAULT_WINDOW_NAME[] = "Turbofish window";
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int BPP = 32;

struct window {
	int width;
	int height;
	char *title;
	struct local_buffer local_buffer;

	int key_fd;
	int (*key_release_hook)(int keycode, void *param);
	int (*key_press_hook)(int keycode, void *param);
	void *key_release_env;
	void *key_press_env;
};

struct image {
	int width;
	int height;

	u8 *pix_map;
};

struct mlx {
	struct window *window;
	struct image *image;
	int (*callback)();
	void *env;
};

static void set_raw_mode(int fd);
static void set_cooked_mode(struct window *window);
static int handle_escape_scancode(int scancode);
static void display_char_32(u8 c, u32 *location, u32 test_color);

/*
**  needed before everything else.
**  return (void *)0 if failed
*/
void *mlx_init(void)
{
	struct mlx *mlx = (struct mlx *)calloc(1, sizeof(struct mlx));
	if (mlx == NULL) {
		dprintf(STDERR_FILENO, "Cannot allocate memory for main mlx structure\n");
	}
	return (void *)mlx;
}

/*
** Basic actions
*/
void *mlx_new_window(void *mlx_ptr, int size_x, int size_y, char *title)
{
	struct mlx *mlx = (struct mlx *)mlx_ptr;

	if (mlx == NULL) {
		dprintf(STDERR_FILENO, "Sending NUll ptr is not a good idea in %s\n", "mlx_new_window");
		return NULL;
	}
	if (size_x != WINDOW_WIDTH || size_y != WINDOW_HEIGHT) {
		dprintf(STDERR_FILENO, "Only supported window of 1024*768px\n");
		return NULL;
	}
	struct window *window = (struct window *)calloc(1, sizeof(struct window));
	if (window == NULL) {
		dprintf(STDERR_FILENO, "Cannot allocate enough memory for window\n");
		return NULL;
	}
	window->width = size_x;
	window->height = size_y;
	if (title != NULL) {
		window->title = strdup(title);
	} else {
		window->title = strdup(DEFAULT_WINDOW_NAME);
	}
	if (window->title == NULL) {
		dprintf(STDERR_FILENO, "Cannot allocate memory for title\n");
		free(window);
		return NULL;
	}
	ioctl(0, GET_FRAME_BUFFER_PTR, (void *)&window->local_buffer);
	printf("local_buffer: %p len: %zu bpp: %zu\n", window->local_buffer.buf, window->local_buffer.len, window->local_buffer.bpp);

	// Set key binding
	int fd = open("/proc/self/fd/0", O_RDWR | O_NONBLOCK);
	if (fd == -1) {
		perror("open fb failed");
		free(window->title);
		free(window);
		return NULL;
	}

	set_raw_mode(fd);
	window->key_fd = fd;
	mlx->window = window;
	return (void *)window;
}

int mlx_destroy_window(void *mlx_ptr, void *win_ptr)
{
	struct window *window = (struct window *)win_ptr;
	struct mlx *mlx = (struct mlx *)mlx_ptr;

	if (window == NULL || mlx == NULL) {
		dprintf(STDERR_FILENO, "Sending NUll(s) ptr(s) is not a good idea in %s\n", "mlx_destroy_window");
		return -1;
	}
	set_cooked_mode(window);
	free(window->title);
	free(window);
	return 0;
}

void *mlx_new_image(void *mlx_ptr, int width, int height)
{
	struct mlx *mlx = (struct mlx *)mlx_ptr;

	if (mlx == NULL) {
		dprintf(STDERR_FILENO, "Sending NUll ptr is not a good idea in %s\n", "mlx_new_image");
		return NULL;
	}

	struct image *image = (struct image *)calloc(1, sizeof(struct image));
	if (image == NULL) {
		dprintf(STDERR_FILENO, "Cannot allocate memory for basic image\n");
		return NULL;
	}
	image->width = width;
	image->height = height;
	image->pix_map = (u8 *)calloc(1, width * height * BPP / 8);
	if (image->pix_map == NULL) {
		dprintf(STDERR_FILENO, "Cannot allocate memory for image pixel map\n");
		free(image);
		return NULL;
	}
	mlx->image = image;
	return (void *)image;
}

int mlx_destroy_image(void *mlx_ptr, void *img_ptr)
{
	struct mlx *mlx = (struct mlx *)mlx_ptr;
	struct image *image = (struct image *)img_ptr;

	if (mlx == NULL || image == NULL) {
		dprintf(STDERR_FILENO, "Sending NUll(s) ptr(s) is not a good idea in %s\n", "mlx_destroy_image");
		return -1;
	}
	free(image->pix_map);
	free(image);
	mlx->image = NULL;
	return 0;
}

/*
**  return void NULL if failed
*/
char *mlx_get_data_addr(void *img_ptr,
			int *bits_per_pixel,
			int *size_line,
			int *endian)
{
	struct image *image = (struct image *)img_ptr;

	if (image == NULL || bits_per_pixel == NULL || size_line == NULL || endian == NULL) {
		dprintf(STDERR_FILENO, "Sending NUll(s) ptr(s) is not a good idea in %s\n", "mlx_get_data_address");
		return NULL;
	}
	*bits_per_pixel = BPP;
	*size_line = *bits_per_pixel / 8 * image->width;
	*endian = 0;
	return (char *)image->pix_map;
}

int mlx_loop_hook(void *mlx_ptr, int (*funct_ptr)(), void *param)
{
	struct mlx *mlx = (struct mlx *)mlx_ptr;

	if (mlx == NULL || funct_ptr == NULL) {
		dprintf(STDERR_FILENO, "Sending NUll ptr is not a good idea in %s\n", "mlx_loop_hook");
		return -1;
	}
	mlx->callback = funct_ptr;
	mlx->env = param;
	return 0;
}

int mlx_loop(void *mlx_ptr)
{
	struct mlx *mlx = (struct mlx *)mlx_ptr;

	if (mlx == NULL || mlx->callback == NULL) {
		dprintf(STDERR_FILENO, "Sending NUll ptr is not a good idea in %s\n", "mlx_loop");
		return -1;
	}
	while (true) {
		struct window *window = (struct window *)mlx->window;
		if (window == NULL) {
			dprintf(STDERR_FILENO, "Cannot get window\n");
			exit(-1);
		}
		int scancode = 0;
		int len_readen;
		while ((len_readen = read(window->key_fd, &scancode, 2)) > 0) {
			// Pressed
			if (scancode >= 0x1 && scancode <= 0x58) {
				if (window->key_press_hook != NULL) {
					window->key_press_hook(scancode, window->key_press_env);
				}
			}
			// Released
			else if (scancode >= 0x81 && scancode <= 0xd8) {
				if (window->key_release_hook != NULL) {
					window->key_release_hook(scancode - 0x80, window->key_release_env);
				}
			}
			// Pressed
			else if (scancode >= 0xe010 && scancode <= 0xe06d) {
				int escaped_scancode = handle_escape_scancode(scancode & 0xFF);
				if (window->key_press_hook != NULL) {
					window->key_press_hook(escaped_scancode, window->key_press_env);
				}
			}
			// Released
			else if (scancode >= 0xe090 && scancode <= 0xe0ed) {
				int escaped_scancode = handle_escape_scancode((scancode & 0xFF) - 0x80);
				if (window->key_release_hook != NULL) {
					window->key_release_hook(escaped_scancode, window->key_release_env);
				}
			}
		}
		int _ret = mlx->callback(mlx->env);
		(void)_ret;
	}
	// _unreachable!()
	return 0;
}

void mlx_put_image_to_window(mlx_ptr_t *mlx_ptr,
			     mlx_win_list_t *win_ptr,
			     mlx_img_list_t *img_ptr,
			     int x,
			     int y)
{
	struct mlx *mlx = (struct mlx *)mlx_ptr;
	struct window *window = (struct window *)win_ptr;
	struct image *image = (struct image *)img_ptr;

	if (mlx == NULL || window == NULL || image == NULL) {
		dprintf(STDERR_FILENO, "Sending NUll(s) ptr(s) is not a good idea in %s\n", mlx_put_image_to_window);
		exit(1);
	}
	if (window->local_buffer.bpp == 24) {
		int j = 0;
		for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
			u32 content = ((u32 *)(image->pix_map))[i];
			window->local_buffer.buf[j++] = content & 0xff;
			window->local_buffer.buf[j++] = (content & 0xff00) >> 8;
			window->local_buffer.buf[j++] = (content & 0xff0000) >> 16;
		}
	} else {
		aligned_memcpy(window->local_buffer.buf, image->pix_map, WINDOW_WIDTH * WINDOW_HEIGHT * BPP / 8);
	}
	ioctl(0, REFRESH_SCREEN, &window->local_buffer);
	(void)x;
	(void)y;
}

int mlx_string_put(void *mlx_ptr,
		   void *win_ptr,
		   int x,
		   int y,
		   int color,
		   char *string)
{
	struct mlx *mlx = (struct mlx *)mlx_ptr;

	if (mlx == NULL || string == NULL) {
		dprintf(STDERR_FILENO, "Sending NUll(s) ptr(s) is not a good idea in \n", "mlx_string_put");
		return -1;
	}
	u32 *location = (u32 *)(mlx->image->pix_map + y * WINDOW_WIDTH * BPP / 8 + x * BPP / 8);
	while (*string != '\0') {
		if ((u8 *)location >= mlx->image->pix_map + WINDOW_WIDTH * BPP / 8 * WINDOW_HEIGHT) {
			dprintf(STDERR_FILENO, "attempting to write out of bound\n");
			return -1;
		}
		display_char_32(*string++, location, color);
		location += CHAR_WIDTH;
	}
	(void)win_ptr;
	return 0;
}

int mlx_hook(t_win_list *win,
	     int x_event,
	     int x_mask,
	     int (*funct)(),
	     void *param)
{
	struct window *window = (struct window *)win;

	if (window == NULL || funct == NULL) {
		dprintf(STDERR_FILENO, "Sending NUll(s) ptr(s) is not a good idea in %s\n", "mlx_hook");
		return -1;
	}

	if (x_event == TURBOFISH_KEY_PRESS) {
		window->key_press_hook = funct;
		window->key_press_env = param;
	} else if (x_event == TURBOFISH_KEY_RELEASE) {
		window->key_release_hook = funct;
		window->key_release_env = param;
	}
	(void)x_mask;
	return 0;
}

static void set_raw_mode(int fd)
{
	struct termios termios_p;
	int ret = tcgetattr(fd, &termios_p);
	if(ret == -1) {
		perror("tcgetattr failed");
	}

	termios_p.c_lflag &= (~(ICANON | ECHO | TOSTOP));
	ret = tcsetattr(fd, TCSANOW, &termios_p);
	if(ret == -1) {
		perror("tcsetattr failed");
	}
	ioctl(fd, RAW_SCANCODE_MODE, 1);
}

static void set_cooked_mode(struct window *window)
{
	struct termios termios_p;

	close(window->key_fd);

	int ret = tcgetattr(0, &termios_p);
	if (ret == -1) {
		perror("tcgetattr failed");
	}

	termios_p.c_lflag |= (ICANON | ECHO | TOSTOP);
	ret = tcsetattr(0, TCSANOW, &termios_p);
	if (ret == -1) {
		perror("tcsetattr failed");
	}
}

static int handle_escape_scancode(int scancode)
{
	switch (scancode) {
		case 0x38:
			return 100;
		case 0x48:
			return KEYB_ARROW_UP;
		case 0x4b:
			return KEYB_ARROW_LEFT;
		case 0x4d:
			return KEYB_ARROW_RIGHT;
		case 0x50:
			return KEYB_ARROW_DOWN;
		default:
			return -1;
	}
}

extern u8 _font;

static void display_char_32(u8 c, u32 *location, u32 text_color)
{
	u8 *bitmap;
	u8 line;

	bitmap = (u8 *)&_font + (c << 4);
	for (int i = 0; i < CHAR_HEIGHT; i++) {
		line = *bitmap;
		for (int j = 0; j < CHAR_WIDTH; j++) {
			if (line & 0x80)
				*location = text_color;
			line <<= 1;
			location++;
		}
		location += (WINDOW_WIDTH * BPP / 8 - BPP) >> 2;
		bitmap++;
	}
}

void	*mlx_xpm_file_to_image(void *mlx_ptr, char *filename,
							int *width, int *height) {
	int *data = NULL;
	struct mlx *mlx = (struct mlx *)mlx_ptr;

	if (mlx == NULL) {
		dprintf(STDERR_FILENO, "Sending NUll ptr is not a good idea in %s\n", "mlx_xpm_file_to_image");
		return NULL;
	}

	if (!bmp_load(filename, width, height, &data)) {
		printf("bmp load error\n");
		return NULL;
	}
	struct image *new = mlx_new_image((void *)mlx, *width, *height);
	for (int i = 0; i < *width * *height; i++) {
		((u32 *)new->pix_map)[i] = data[i];
	}
	return new;
}

#endif
