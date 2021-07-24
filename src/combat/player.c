#include "player.h"

void player_dungeon_init(Player *player, Vec3 position) {
	player->pos.x = position.x;
	player->pos.y = position.y;
	player->pos.z = position.z;

	player->current_steps_taken = 0;
}

void player_game_init(Player *player) {
	item_bag_init(&player->item_bag);

	player->party.current_member_count = 0;
	for (u8 i = 0; i < 4; ++i) {
		player->party.members[i].class = PC_None;
	}

	player_party_add_member(player, PC_Warrior, "MIELKE", G_Male);
}

void player_party_member_init(Player *player, int index, Class class, char name[], Gender gender) {
	player->party.members[index] = get_start_member_for_class(class, name, gender);
	player->party.current_member_count++;
}

void player_party_add_member(Player *player, Class class, char name[], Gender gender) {
	if (player->party.current_member_count >= 4)
		return;

	for (u8 i = 0; i < 4; ++i) {
		if (player->party.members[i].class == PC_None) {
			player_party_member_init(player, i, class, name, gender);
			return;
		}
	}
}

void player_party_move_member(Player *player, u8 slot_from, u8 slot_to) {
	// only copies to empty spots
	if (player->party.members[slot_to].class != PC_None)
		return;

	PartyMember *member_from = &player->party.members[slot_from];
	PartyMember *member_to = &player->party.members[slot_to];

	member_to->class = member_from->class;
	member_to->current_health = member_from->current_health;
	member_to->current_tp = member_from->current_tp;
	member_to->damage_range = member_from->damage_range;
	member_to->gender = member_from->gender;
	member_to->level = member_from->level;
	member_to->max_health = member_from->max_health;
	member_to->max_tp = member_from->max_tp;
	for (u8 i = 0; i < 9; ++i) {
		member_to->name[i] = member_from->name[i];
	}

	member_from->class = PC_None;
}

void player_party_retire_member(Player *player, int index) {
	player->party.members[index].class = PC_None;

	// compacting party
	for (u8 i = 0; i < 4; ++i) {
		if (player->party.members[i].class == PC_None) {
			for (u8 j = i + 1; j < 4; ++j) {
				if (player->party.members[j].class != PC_None) {
					player_party_move_member(player, j, i);
					break;
				}
			}
		}
	}

	player->party.current_member_count--;
}

bool player_is_any_member_alive(Player *player) {
	for (u8 i = 0; i < player->party.current_member_count; ++i) {
		if (player->party.members[i].current_health > 0) {
			return true;
		}
	}
	return false;
}

void player_heal_all_party_members(Player *player) {
	for (u8 i = 0; i < player->party.current_member_count; ++i) {
		player->party.members[i].current_health = player->party.members[i].max_health;
		player->party.members[i].current_tp = player->party.members[i].max_tp;
	}
}

void player_heal_hp_amount_all_party_members(Player *player, int amount) {
	for (u8 i = 0; i < player->party.current_member_count; ++i) {
		player->party.members[i].current_health += amount;
		if (player->party.members[i].current_health > player->party.members[i].max_health)
			player->party.members[i].current_health = player->party.members[i].max_health;
	}
}

void player_use_item(Player *player, u8 item_index) {
	Item *item = &player->item_bag.items[item_index];
	if (!item || !item->item_def)
		return;

	switch (item->item_def->type) {
		case IT_Heal:
			player_heal_hp_amount_all_party_members(player, item->item_def->value);
			item_bag_remove_item_by_index(&player->item_bag, item_index);
			break;
		default:
			break;
	}
}
