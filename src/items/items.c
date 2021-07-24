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

void item_bag_add_item(ItemBag *bag, ItemId id, s32 qty) {
	Item *item_to_change = NULL;
	for (u8 i = 0; i < ITEM_BAG_MAX_ITEM_COUNT; ++i) {
		if (bag->items[i].item_def && bag->items[i].item_def->id == id) {
			item_to_change = &bag->items[i];
			break;
		}
	}
	if (!item_to_change) {
		item_to_change = &bag->items[bag->cur_item_bag_count];
		item_to_change->item_def = &item_defs[id];
		bag->cur_item_bag_count++;
	}

	item_to_change->qty += qty;
}

void item_bag_remove_item_by_index(ItemBag *bag, u8 index, s32 qty) {
	item_bag_remove_item(bag, bag->items[index].item_def->id, qty);
}

void item_bag_remove_item(ItemBag *bag, ItemId id, s32 qty) {
	Item *item_to_change = NULL;
	for (u8 i = 0; i < ITEM_BAG_MAX_ITEM_COUNT; ++i) {
		if (bag->items[i].item_def->id == id) {
			item_to_change = &bag->items[i];
			break;
		}
	}

	if (!item_to_change)
		return;

	item_to_change->qty -= qty;
	if (item_to_change->qty <= 0) {
		item_to_change->qty = 0;
		item_to_change->item_def = NULL;

		// compacting bag
		for (u8 i = 0; i < 4; ++i) {
			if (bag->items[i].item_def == NULL) {
				for (u8 j = i + 1; j < 4; ++j) {
					if (bag->items[i].item_def != NULL) {
						item_bag_move_item(bag, j, i);
					}
				}
			}
		}
		bag->cur_item_bag_count--;
	}
}

void item_bag_move_item(ItemBag *bag, u8 slot_from, u8 slot_to) {
	// only copies to empty spots
	if (bag->items[slot_to].item_def != NULL)
		return;

	Item *item_from = &bag->items[slot_from];
	Item *item_to = &bag->items[slot_to];

	item_to->item_def = item_from->item_def;
	item_to->qty = item_from->qty;

	item_from->item_def = NULL;
	item_from->qty = 0;
}
