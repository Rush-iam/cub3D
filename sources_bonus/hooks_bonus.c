/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:10 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/06 17:33:10 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	hook_key_press(int key_code, t_game *game)
{
	if (key_code >= (int)sizeof(game->key.k))
		return (1);
	if (key_code == KEY_ESCAPE)
		terminate(game, EXIT_SUCCESS, NULL);
//	printf("Key pressed: %d\n", key_code);
	game->key.k[key_code] = true;
	return (0);
}

int	hook_key_release(int key_code, struct s_key *key)
{
	if (key_code >= (int)sizeof(key->k))
		return (1);
	key->k[key_code] = false;
	return (0);
}

int	hook_mouse_press(int btn, int x, int y, struct s_key *key)
{
	(void)x;
	(void)y;
	if (btn >= (int)sizeof(key->m))
		return (1);
	key->m[btn] = true;
	return (0);
}

int	hook_mouse_release(int btn, int x, int y, struct s_key *key)
{
	(void)x;
	(void)y;
	if (btn >= (int)sizeof(key->m))
		return (1);
	key->m[btn] = false;
	return (0);
}

int	hook_exit(t_game *game)
{
	return (terminate(game, 0, NULL));
}