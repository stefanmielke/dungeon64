#include "player.h"

void player_init(Player *player, Vec3 position) {
	player->pos.x = position.x;
	player->pos.y = position.y;
	player->pos.z = position.z;
	player->angle = 0;

	player->current_steps_taken = 0;
}

void player_party_init(Player *player) {
	player->party.current_member_count = 0;
	for (u8 i = 0; i < 4; ++i) {
		player->party.members[i].class = PC_None;
	}
}

void player_party_member_init(Player *player, int index, Class class, const char *name,
							  Gender gender) {
	player->party.members[index] = get_start_member_for_class(class, name, gender);
	player->party.current_member_count++;
}

void player_party_add_member(Player *player, Class class, const char *name, Gender gender) {
	if (player->party.current_member_count >= 4)
		return;

	for (u8 i = 0; i < 4; ++i) {
		if (player->party.members[i].class == PC_None) {
			player_party_member_init(player, i, class, name, gender);
			return;
		}
	}
}
