#include "player.h"

void player_init(Player *player, Vec3 position) {
	player->pos.x = position.x;
	player->pos.y = position.y;
	player->pos.z = position.z;
	player->angle = 0;

	player->current_steps_taken = 0;

	player->party.current_member_count = 4;
	player->party.members[0] = get_start_member_for_class(PC_Warrior, "Warrior", G_Male);
	player->party.members[1] = get_start_member_for_class(PC_Wizard, "Wizard", G_Male);
	player->party.members[2] = get_start_member_for_class(PC_Cleric, "Cleric", G_Female);
	player->party.members[3] = get_start_member_for_class(PC_Thief, "Thief", G_Male);
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
}
