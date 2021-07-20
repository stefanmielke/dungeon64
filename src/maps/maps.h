
extern unsigned short map1_1[];
extern unsigned long map1_1_size;
extern unsigned long map1_1_width;

typedef enum {
	TT_Empty_Space = 0,
	// walls
	TT_Wall_Full = 1,
	TT_Wall_East = 2,
	TT_Wall_South,
	TT_Wall_West,
	TT_Wall_North,
	TT_Upstairs_East = 6,
	TT_Upstairs_South,
	TT_Upstairs_West,
	TT_Upstairs_North,
	TT_Downstairs_East = 9,
	TT_Downstairs_South,
	TT_Downstairs_West,
	TT_Downstairs_North,

	// grounds
	TT_Ground_1 = 100,
	TT_Ground_2,
	TT_Ground_3,
	TT_Ground_4,
	TT_Ground_5,
	TT_Ground_6,

	// objects
	TT_StartPos = 255,
} TileTypes;

typedef enum {
	TL_Empty_Space = 0,

	TL_Wall_Start = 1,
	TL_Wall_End = 10,

	TL_Ground_Start = 100,
	TL_Ground_End = 254,

	TL_Objects_Start = 255,
	TL_Objects_End = 65535,
} TileLimits;