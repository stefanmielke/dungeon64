#include "items.h"

#include <nustd/stdio.h>
#include <nustd/string.h>

#include "../text/texts.h"

// follows the order on 'ItemId'
ItemDef item_defs[] = {
	{
		.type = IT_Heal,
		.usage_type = IUT_SingleMember,
		.name = TEXT_SMALL_HEAL,
		.value = 10,
		.buy_value = 20,
	},
	{
		.type = IT_Heal,
		.usage_type = IUT_SingleMember,
		.name = TEXT_LARGE_HEAL,
		.value = 40,
		.buy_value = 100,
	},
};

void item_def_get_name_and_price(ItemDef *item_def, char *item_desc) {
	char value_text[6];
	int name_length = strlen(item_def->name);

	{
		u8 c = 0;
		for (; c < name_length; ++c)
			item_desc[c] = item_def->name[c];
		for (; c < ITEM_NAME_LENGTH - 1; ++c)
			item_desc[c] = ' ';
		item_desc[c] = '\0';
	}

	sprintf(value_text, "%d", item_def->buy_value);
	int value_length = strlen(value_text);
	int value_start = ITEM_NAME_LENGTH - value_length - 1;
	for (u8 c = 0; c < value_length; ++c) {
		item_desc[value_start + c] = value_text[c];
	}
}

void item_bag_init(ItemBag *bag) {
	bag->cur_item_bag_count = 0;
	bag->money = 0;

	for (u16 i = 0; i < ITEM_BAG_MAX_ITEM_COUNT; ++i) {
		bag->items[i].item_def = NULL;
	}
}

void item_bag_add_money(ItemBag *bag, u32 value) {
	bag->money += value;
}

bool item_bag_spend_money(ItemBag *bag, u32 value) {
	if (bag->money < value)
		return false;

	bag->money -= value;
	return true;
}

void item_bag_add_item(ItemBag *bag, ItemId id) {
	Item *item_to_change = &bag->items[bag->cur_item_bag_count];
	item_to_change->item_def = &item_defs[id];
	bag->cur_item_bag_count++;
}

void item_bag_remove_item_by_index(ItemBag *bag, u8 index) {
	Item *item_to_change = &bag->items[index];
	if (!item_to_change->item_def)
		return;

	item_to_change->item_def = NULL;
	// compacting bag
	for (u8 i = 0; i < bag->cur_item_bag_count; ++i) {
		if (bag->items[i].item_def == NULL) {
			for (u8 j = i + 1; j < bag->cur_item_bag_count; ++j) {
				if (bag->items[j].item_def != NULL) {
					item_bag_move_item(bag, j, i);
					break;
				}
			}
		}
	}
	bag->cur_item_bag_count--;
}

void item_bag_move_item(ItemBag *bag, u8 slot_from, u8 slot_to) {
	// only copies to empty spots
	if (bag->items[slot_to].item_def != NULL)
		return;

	Item *item_from = &bag->items[slot_from];
	Item *item_to = &bag->items[slot_to];

	item_to->item_def = item_from->item_def;
	item_from->item_def = NULL;
}
