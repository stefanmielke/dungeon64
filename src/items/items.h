#pragma once

#include <stdbool.h>
#include <ultratypes.h>

typedef enum ItemId {
	II_SmallHeal,
	II_LargeHeal,
	II_Max,
} ItemId;

typedef enum ItemType {
	IT_None,
	IT_Heal,
} ItemType;

typedef enum ItemUsageType {
	IUT_SingleMember,
	IUT_AllMembers,
	IUT_SingleEnemy,
	IUT_AllEnemies,
} ItemUsageType;

typedef struct ItemDef {
	ItemType type;
	ItemUsageType usage_type;
	const char *name;
	f32 value;
	u32 buy_value;
} ItemDef;

typedef struct Item {
	ItemDef *item_def;
} Item;

#define ITEM_BAG_MAX_ITEM_COUNT 20
typedef struct ItemBag {
	s32 money;
	u16 cur_item_bag_count;
	Item items[ITEM_BAG_MAX_ITEM_COUNT];
} ItemBag;

#define ITEM_NAME_LENGTH 30
extern ItemDef item_defs[];
void item_def_get_name_and_price(ItemDef *item_def, char *item_desc);

void item_bag_init(ItemBag *bag);
void item_bag_add_money(ItemBag *bag, u32 value);
bool item_bag_spend_money(ItemBag *bag, u32 value);
void item_bag_add_item(ItemBag *bag, ItemId id);
void item_bag_remove_item_by_index(ItemBag *bag, u8 index);
void item_bag_move_item(ItemBag *bag, u8 slot_from, u8 slot_to);