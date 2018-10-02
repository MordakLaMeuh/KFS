
#include "kernel_io.h"
#include "dynamic_allocator.h"
#include "vesa.h"
#include "libft.h"
#include "libasm_i386.h"

extern const struct modifier_list g_modifier_list[MODIFIER_QUANTITY];

void		init_kernel_io(void)
{
	g_kernel_io_ctx.term_mode = kernel;
	g_kernel_io_ctx.tty = NULL;
	g_kernel_io_ctx.current_tty = NULL;
	g_kernel_io_ctx.nb_tty = 0;
}

struct k_tty	*create_tty(u8 *background_img, u32 default_color)
{
	struct k_tty *tty;

	g_kernel_io_ctx.nb_tty += 1;
	g_kernel_io_ctx.tty = krealloc(
			g_kernel_io_ctx.tty,
			sizeof(struct k_tty) * g_kernel_io_ctx.nb_tty);
	if (g_kernel_io_ctx.tty == NULL)
		goto panic;

	tty = &g_kernel_io_ctx.tty[g_kernel_io_ctx.nb_tty - 1];
	tty->background_img = background_img;
	tty->default_color = default_color;
	tty->nb_line = 0;
	tty->line = kmalloc(sizeof(struct k_line) * 16);
	if (tty->line == NULL)
		goto panic;

	tty->line[0].nb_char = 0;
	tty->line[0].str = kmalloc(sizeof(u8) * 16);
	if (tty->line[0].str == NULL)
		goto panic;

	return tty;

panic:
	return NULL;
}

static int	memmove_tty(u32 i)
{
	g_kernel_io_ctx.nb_tty -= 1;
	while (i < g_kernel_io_ctx.nb_tty) {
		if (&g_kernel_io_ctx.tty[i + 1] == g_kernel_io_ctx.current_tty)
			g_kernel_io_ctx.current_tty = &g_kernel_io_ctx.tty[i];
		memcpy(
				&g_kernel_io_ctx.tty[i],
				&g_kernel_io_ctx.tty[i + 1],
				sizeof(struct k_tty));
	}
	return 0;
}

int		remove_tty(u32 index)
{
	struct k_tty *tty;

	if (index >= g_kernel_io_ctx.nb_tty)
		return -1;

	tty = &g_kernel_io_ctx.tty[index];

	for (size_t line = 0; line < tty->nb_line; line++)
		kfree(tty->line[line].str);
	kfree(tty->line);

	return memmove_tty(index);
}

void		copy_tty_content(struct k_tty *tty)
{
	size_t first_line;

	if (tty->nb_line > 48)
		first_line = tty->nb_line - 48;
	else
		first_line = 0;

	for (size_t i = first_line; i < tty->nb_line; i++)
		write_direct(1, tty->line[i].str, tty->line[i].nb_char);
}

void		fill_tty_background(struct k_tty *tty)
{
	sse2_memcpy(
			(u32 *)DB_FRAMEBUFFER_ADDR,
			(void *)tty->background_img,
			vesa_ctx.mode.pitch
			* vesa_ctx.mode.height);
}

int		select_tty(u32 index)
{
	if (index >= g_kernel_io_ctx.nb_tty)
		return -1;

	g_kernel_io_ctx.current_tty = &g_kernel_io_ctx.tty[index];

	fill_tty_background(g_kernel_io_ctx.current_tty);

	set_cursor_location(0, 0);
	set_text_color(g_kernel_io_ctx.current_tty->default_color);
	copy_tty_content(g_kernel_io_ctx.current_tty);
	refresh_screen();
	return 0;
}

void		*add_tty_char(u8 c)
{
	struct k_tty *tty;
	struct k_line *line;

	tty = g_kernel_io_ctx.current_tty;

	line = &tty->line[tty->nb_line];
	line->nb_char += 1;
	if (line->nb_char % 16 == 0) {
		line->str = krealloc(
				line->str,
				sizeof(u8)
				* 16 * (line->nb_char / 16 + 1));
		if (line->str == NULL)
			goto panic;
	}
	line->str[line->nb_char - 1] = c;
	return line;
panic:
	return NULL;
}

static void	mark_color(struct k_tty *tty)
{
	for (size_t i = 0; i < MODIFIER_QUANTITY; i++) {
		if (get_text_color() == g_modifier_list[i].color) {
			size_t len = strlen(g_modifier_list[i].string);
			memcpy(
					tty->line[tty->nb_line].str,
					 g_modifier_list[i].string,
					 len
			);
			tty->line[tty->nb_line].nb_char += len;
		}
	}
}

void		*new_tty_line()
{
	struct k_tty *tty;

	tty = g_kernel_io_ctx.current_tty;
	tty->nb_line += 1;
	if (tty->nb_line % 16 == 0) {
		tty->line = krealloc(
				tty->line,
				sizeof(struct k_line)
				* 16 * (tty->nb_line / 16 + 1));
		if (tty->line == NULL)
			goto panic;
	}
	tty->line[tty->nb_line].nb_char = 0;
	tty->line[tty->nb_line].str = kmalloc(sizeof(u8) * 16);
	if (tty->line[tty->nb_line].str == NULL)
		goto panic;
	if (get_text_color() != tty->default_color)
		mark_color(tty);
	return tty->line;
panic:
	return NULL;
}

