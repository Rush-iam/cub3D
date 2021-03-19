/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_effects_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 18:14:51 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/19 23:40:45 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_effect(t_game *game, struct s_effect *ef)
{
	float	power;

	if (ef->frame_cur < ef->frames)
	{
		if (ef->frame_cur >= ef->frames)
			ef->frame_cur = ef->frames - 1;
		power = (float)ef->frame_cur / (ef->frames / 2.);
		if (power > 1)
			power = 2 - power;
		if (ef->type == EF_FLASH)
			effect_flash(game, game->effect.color, power * ef->max_power);
		else if (ef->type == EF_FIZZLEFADE)
			effect_fizzlefade(game, game->effect.color);
	}
}

void	effect_flash(t_game *game, unsigned color, float power)
{
	img_clear_rgb(&game->effect_img, color | ((int)(255 - 255. * power) << 24));
	mlx_put_image_to_window(game->mlx, game->win, game->effect_img.ptr, 0, 0);
}

void	effect_fizzlefade(t_game *game, unsigned color)
{
	static unsigned	rndval = 1;
	unsigned		lsb;
	t_point			pos;
	unsigned		i;
	unsigned		scale;

	scale = ft_umax(game->win_center.x, game->img.size.y) / 256 + 1;
	img_clear_rgb(&game->effect_img, 0xFF000000);
	i = 0;
	while (i < 512)
	{
		pos.x = ((rndval & 0x1FF00) >> 8) * scale;
		pos.y = (rndval & 0x000FF) * scale;
		lsb = rndval & 1;
		rndval >>= 1;
		if (lsb)
			rndval ^= 0x12000;
		draw_square_fill(&game->effect_img, pos, scale, color);
		i++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->effect_img.ptr, 0, 0);
}
