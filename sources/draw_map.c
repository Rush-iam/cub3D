/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 18:52:51 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/26 23:36:54 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_map(t_game *game)
{
	t_point	pt;

	img_clear_rgb(&game->map.img, 0xAA000000);
	pt.y = 0;
	while (pt.y < (int)game->map.img.size.y)
	{
		pt.x = 0;
		while (pt.x < (int)game->map.img.size.x)
		{
			draw_line(&game->map.img, pt,
					(t_point){pt.x + MAP_SCALE, pt.y}, 0x113322);
			draw_line(&game->map.img, pt,
					(t_point){pt.x, pt.y + MAP_SCALE}, 0x113322);
			if (game->map.grid[pt.y / MAP_SCALE][pt.x / MAP_SCALE] == '1')
				draw_square(&game->map.img, (t_point){pt.x + MAP_SCALE / 2,
								pt.y + MAP_SCALE / 2}, MAP_SCALE - 4, 0x33AA99);
			pt.x += MAP_SCALE;
		}
		pt.y += MAP_SCALE;
	}
	draw_map_player(game);
	draw_square(&game->map.img, (t_point){
		((t_object *)game->objects->content)->pos.x * MAP_SCALE,
		((t_object *)game->objects->content)->pos.y * MAP_SCALE}, 6, 0xFFFA80);
	mlx_put_image_to_window(game->mlx, game->win, game->map.img.ptr, 0, 0);
}

void	draw_map_player(t_game *game)
{
	unsigned	ray;

	draw_square(&game->map.img,
			(t_point){game->p.pos.x * MAP_SCALE,
						game->p.pos.y * MAP_SCALE}, 6, 0xFFFA80);
	ray = 0;
	while (ray < game->img.size.x)
	{
		draw_line(&game->map.img,
				(t_point){game->p.pos.x * MAP_SCALE, game->p.pos.y * MAP_SCALE},
				(t_point){game->column[ray].cell.x * MAP_SCALE,
						game->column[ray].cell.y * MAP_SCALE}, 0x888015);
		ray += 32;
	}
	draw_line(&game->map.img, (t_point){game->p.pos.x * MAP_SCALE,
										game->p.pos.y * MAP_SCALE}, (t_point){
		game->p.pos.x * MAP_SCALE + MAP_SCALE * cos(game->p.angle),
		game->p.pos.y * MAP_SCALE + MAP_SCALE * sin(game->p.angle)}, 0xFF4020);
}
