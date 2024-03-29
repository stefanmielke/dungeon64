#pragma once

#include "../../libs/ultra64-extensions/include/tween.h"

#include "party.h"
#include "../types.h"
#include "../items/items.h"

typedef enum PlayerFlags {
	FLAGS_NONE = 0,
	FLAGS_GUILD_START = 1,
} PlayerFlags;

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

	PlayerFlags flags;
} Player;

void player_dungeon_init(Player *player, Vec3 position);
void player_game_init(Player *player);
void player_party_add_member(Player *player, Class class, char name[], Gender gender);
void player_party_retire_member(Player *player, int index);
bool player_is_any_member_alive(Player *player);

void player_heal_all_party_members(Player *player);
void player_heal_hp_amount_all_party_members(Player *player, int amount);
void player_heal_hp_amount_party_member(Player *player, PartyMember *party_member, int amount);

void player_use_item(Player *player, u8 item_index);
void player_use_item_on_party_member(Player *player, u8 item_index, u8 member_index);
