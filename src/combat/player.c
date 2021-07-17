#include "player.h"

void player_init(Player *player, Vec3 position) {
	player->pos.x = position.x;
	player->pos.y = position.y;
	player->pos.z = position.z;
	player->angle = 0;

	player->current_steps_taken = 0;

	player->party.current_member_count = 2;
	player->party.members[0] = get_start_member_for_class(PC_Warrior);
	player->party.members[1] = get_start_member_for_class(PC_Warrior);
}
