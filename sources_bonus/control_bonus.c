/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:31:39 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/17 15:39:05 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	player_control(t_game *game)
{
	player_control_rotate(game);
	player_control_move(game);
	player_control_weapon(game);
	player_control_extra(game);
	player_control_borders(game);
}

void	player_control_rotate(t_game *game)
{
	mlx_mouse_get_pos(game->win, &game->key.mpos.x, &game->key.mpos.y);
	mlx_mouse_move(game->win, game->win_center.x, game->win_center.y);
	game->key.mdir.x = game->key.mdir.x / 2 + game->key.mpos.x -
					   game->win_center.x;
	game->p.angle += game->key.mdir.x / MOUSE_SPEED;
	if (game->key.k[K_TURN_LEFT])
		game->p.angle -= PL_SPEED / 3;
	if (game->key.k[K_TURN_RIGHT])
		game->p.angle += PL_SPEED / 3;
	if (game->p.angle >= PI2)
		game->p.angle -= PI2;
	else if (game->p.angle < 0)
		game->p.angle += PI2;
	__sincos(game->p.angle, &game->p.vector.y, &game->p.vector.x);
}

void	player_control_move(t_game *game)
{
	if (game->key.k[K_MOVE_FORWARD])
		game->p.pos = (t_fpoint){
		game->p.pos.x + (game->key.k[K_RUN] + 1) * PL_SPEED * game->p.vector.x,
		game->p.pos.y + (game->key.k[K_RUN] + 1) * PL_SPEED * game->p.vector.y};
	if (game->key.k[K_MOVE_BACK])
		game->p.pos = (t_fpoint){
	game->p.pos.x - (game->key.k[K_RUN] + 1) * PL_SPEED / 1.5 * game->p.vector.x,
	game->p.pos.y - (game->key.k[K_RUN] + 1) * PL_SPEED / 1.5 * game->p.vector.y};
	if (game->key.k[K_MOVE_LEFT])
		game->p.pos = (t_fpoint){
		game->p.pos.x + (game->key.k[K_RUN] + 1) * PL_SPEED * game->p.vector.y,
		game->p.pos.y - (game->key.k[K_RUN] + 1) * PL_SPEED * game->p.vector.x};
	if (game->key.k[K_MOVE_RIGHT])
		game->p.pos = (t_fpoint){
		game->p.pos.x - (game->key.k[K_RUN] + 1) * PL_SPEED * game->p.vector.y,
		game->p.pos.y + (game->key.k[K_RUN] + 1) * PL_SPEED * game->p.vector.x};
}

void	player_control_weapon(t_game *game)
{
	if (game->p.weapon.frame == 0)
	{
		if (game->key.k[K_KNIFE] && game->p.weapon_cur != W_KNIFE)
			player_set_weapon(game, W_KNIFE);
		else if (game->key.k[K_PISTOL] && game->p.weapon_cur != W_PISTOL &&
				 game->p.ammo && (game->p.weapons_mask & W_PISTOL_MASK))
			player_set_weapon(game, W_PISTOL);
		else if (game->key.k[K_RIFLE] && game->p.weapon_cur != W_RIFLE &&
				 game->p.ammo && (game->p.weapons_mask & W_RIFLE_MASK))
			player_set_weapon(game, W_RIFLE);
	}
	if (game->key.m[M_SHOOT] && game->p.weapon.lock == false &&
		(game->p.ammo || game->p.weapon_cur == W_KNIFE))
	{
		game->p.weapon.lock = true;
		game->p.weapon.tick = ANIM_TICKS - 1;
	}
	if (game->key.m[M_SHOOT] == false && game->p.weapon.lock == true
		&& game->p.weapon.frame == 0)
		game->p.weapon.lock = false;
}

void	player_set_fov(t_game *game, double fov, bool reset)
{
	if (reset)
		fov = ((game->img.aspect >= 1.77) - (game->img.aspect < 1.77)) *
			  sqrt(fabs(M_PI_4 * (game->img.aspect - 1.77) / 2)) + M_PI_2;
	game->col_step = tan(fov / (game->img.size.x - 1));
	game->col_center = (float)game->img.size.x / 2;
	game->col_scale = 1 / game->col_step;
	printf("Real FOV: %.0f\n", 114 * atan(game->col_step * game->col_center));
	if (reset == true || (M_PI_4 / 4 < fov && fov < PI2))
		game->fov = fov;
}

void	player_control_toggler(t_game *game, int key_code)
{
	if (key_code == K_MAP_TOGGLE && game->key.k[key_code] == false)
		game->map.show = !game->map.show;
}

void	player_control_extra(t_game *game)
{
	if (game->key.k[K_FOV_WIDE])
		player_set_fov(game, game->fov * FOV_ZOOMSPEED, false);
	if (game->key.k[K_FOV_TELE])
		player_set_fov(game, game->fov / FOV_ZOOMSPEED, false);
	if (game->key.k[K_FOV_RESET])
		player_set_fov(game, 0, true);
}

void	player_control_borders(t_game *g)
{
	const t_point	plus = {g->p.pos.x + PL_RADIUS, g->p.pos.y + PL_RADIUS};
	const t_point	minus = {g->p.pos.x - PL_RADIUS, g->p.pos.y - PL_RADIUS};

	g->p.pos.x = fmax(g->p.pos.x, PL_RADIUS);
	g->p.pos.y = fmax(g->p.pos.y, PL_RADIUS);
	g->p.pos.x = fmin(g->p.pos.x, g->map.size.x - PL_RADIUS);
	g->p.pos.y = fmin(g->p.pos.y, g->map.size.y - PL_RADIUS);
	if (ft_isdigit(g->map.grid[(int)g->p.pos.y][minus.x]) || ft_memchr(
		CHAR_SOLID, g->map.grid[(int)g->p.pos.y][minus.x], sizeof(CHAR_SOLID)))
		g->p.pos.x = minus.x + 1 + PL_RADIUS + FLOAT_FIX;
	if (ft_isdigit(g->map.grid[(int)g->p.pos.y][plus.x]) || ft_memchr(
		CHAR_SOLID, g->map.grid[(int)g->p.pos.y][plus.x], sizeof(CHAR_SOLID)))
		g->p.pos.x = plus.x - PL_RADIUS - FLOAT_FIX;
	if (ft_isdigit(g->map.grid[minus.y][(int)g->p.pos.x]) || ft_memchr(
		CHAR_SOLID, g->map.grid[minus.y][(int)g->p.pos.x], sizeof(CHAR_SOLID)))
		g->p.pos.y = minus.y + 1 + PL_RADIUS + FLOAT_FIX;
	if (ft_isdigit(g->map.grid[plus.y][(int)g->p.pos.x]) || ft_memchr(
		CHAR_SOLID, g->map.grid[plus.y][(int)g->p.pos.x], sizeof(CHAR_SOLID)))
		g->p.pos.y = plus.y - PL_RADIUS - FLOAT_FIX;
	player_control_borders_diag(g);
}

void	player_control_borders_diag(t_game *g)
{
	const t_point	plus = {g->p.pos.x + PL_RADIUS, g->p.pos.y + PL_RADIUS};
	const t_point	minus = {g->p.pos.x - PL_RADIUS, g->p.pos.y - PL_RADIUS};

	if (ft_isdigit(g->map.grid[minus.y][minus.x]) || ft_memchr(
			CHAR_SOLID, g->map.grid[minus.y][minus.x], sizeof(CHAR_SOLID)))
		(g->p.pos.x - (int)g->p.pos.x > g->p.pos.y - (int)g->p.pos.y) ?
		(g->p.pos.x = minus.x + 1 + PL_RADIUS) :
		(g->p.pos.y = minus.y + 1 + PL_RADIUS);
	if (ft_isdigit(g->map.grid[plus.y][plus.x]) || ft_memchr(
			CHAR_SOLID, g->map.grid[plus.y][plus.x], sizeof(CHAR_SOLID)))
		(g->p.pos.x - (int)g->p.pos.x < g->p.pos.y - (int)g->p.pos.y) ?
		(g->p.pos.x = plus.x - PL_RADIUS) :
		(g->p.pos.y = plus.y - PL_RADIUS);
	if (ft_isdigit(g->map.grid[minus.y][plus.x]) || ft_memchr(
			CHAR_SOLID, g->map.grid[minus.y][plus.x], sizeof(CHAR_SOLID)))
		(1. - (g->p.pos.x - (int)g->p.pos.x) > g->p.pos.y - (int)g->p.pos.y) ?
		(g->p.pos.x = plus.x - PL_RADIUS) :
		(g->p.pos.y = minus.y + 1 + PL_RADIUS);
	if (ft_isdigit(g->map.grid[plus.y][minus.x]) || ft_memchr(
			CHAR_SOLID, g->map.grid[plus.y][minus.x], sizeof(CHAR_SOLID)))
		(g->p.pos.x - (int)g->p.pos.x > 1. - (g->p.pos.y - (int)g->p.pos.y)) ?
		(g->p.pos.x = minus.x + 1 + PL_RADIUS) :
		(g->p.pos.y = plus.y - PL_RADIUS);
}
