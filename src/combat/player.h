#pragma once

#include "../../libs/ultra64-extensions/include/tween.h"

#include "party.h"
#include "../types.h"
#include "../items/items.h"

typedef struct Player {
	Vec3f pos;
	float view_speed;
	float move_forward;
	float move_lateral;
	Vec3f forward;
	float angle;
	u32 current_tile;

	u32 current_steps_taken;
	u32 next_combat_at;

	Tween *movement_tween;
	Tween *view_tween;

	Party party;
	ItemBag item_bag;
} Player;

void player_dungeon_init(Player *player, Vec3 position);
void player_game_init(Player *player);
void player_party_add_member(Player *player, Class class, char name[], Gender gender);
void player_party_retire_member(Player *player, int index);
bool player_is_any_member_alive(Player *player);
void player_heal_all_party_members(Player *player);
