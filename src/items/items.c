#include "items.h"

#include "../text/texts.h"

ItemDef item_defs[] = {
	{
		.id = II_SmallHeal,
		.type = IT_Heal,
		.name = TEXT_SMALL_HEAL,
		.value = 10,
	},
};

void item_bag_init(ItemBag *bag) {
	bag->cur_item_bag_count = 0;
	bag->money = 0;

	for (u16 i = 0; i < ITEM_BAG_MAX_ITEM_COUNT; ++i) {
		bag->items[i].item_def = NULL;
		bag->items[i].qty = 0;
	}
}
