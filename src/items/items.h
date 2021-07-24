#pragma once

#include <ultratypes.h>

typedef enum ItemId {
	II_SmallHeal,
	II_Max,
} ItemId;

typedef enum ItemType {
	IT_None,
	IT_Heal,
} ItemType;

typedef struct ItemDef {
	ItemId id;
	ItemType type;
	const char *name;
	f32 value;
} ItemDef;

typedef struct Item {
	ItemDef *item_def;
	s32 qty;
} Item;

#define ITEM_BAG_MAX_ITEM_COUNT 99
typedef struct ItemBag {
	s32 money;
	u16 cur_item_bag_count;
	Item items[ITEM_BAG_MAX_ITEM_COUNT];
} ItemBag;

extern ItemDef item_defs[];

void item_bag_init(ItemBag *bag);
void item_bag_add_item(ItemBag *bag, ItemId id, s32 qty);
void item_bag_remove_item(ItemBag *bag, ItemId id, s32 qty);
void item_bag_move_item(ItemBag *bag, u8 slot_from, u8 slot_to);