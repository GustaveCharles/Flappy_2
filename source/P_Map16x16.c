
#include "P_Map16x16.h"
#include "numbers.h"
#include "sky.h"
#include "levelSel.h"

u16 *mapMemory;
int MapCols;
int MapRows;

// Declare the tiles emptyTile and fullTile as explained in the manual
u8 emptyTile[64] =
	{
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0};

u8 fullTile[64] =
	{
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 254, 254, 254, 254, 254, 254, 255,
		255, 254, 255, 255, 255, 255, 254, 255,
		255, 254, 255, 255, 255, 255, 254, 255,
		255, 254, 255, 255, 255, 255, 254, 255,
		255, 254, 255, 255, 255, 255, 254, 255,
		255, 254, 254, 254, 254, 254, 254, 255,
		255, 255, 255, 255, 255, 255, 255, 255};

void P_Map16x16_configureBG0_Sub()
{
	// Configure the Background
	BGCTRL_SUB[0] = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(24) | BG_TILE_BASE(4);
	// Copy tiles and palette
	dmaCopy(numbersTiles, BG_TILE_RAM_SUB(4), numbersTilesLen);
	dmaCopy(numbersPal, &BG_PALETTE_SUB[128], numbersPalLen);
	dmaCopy(numbersPal, &BG_PALETTE_SUB[144], numbersPalLen);
	BG_PALETTE_SUB[129] = ARGB16(1, 31, 0, 0);
	BG_PALETTE_SUB[145] = ARGB16(1, 0, 31, 0);

	// Set map to 0 (clean)
	memset(BG_MAP_RAM_SUB(24), 0, 32 * 32 * 2);
}

void P_Map16x16_configureBG2_Sub()
{
	// Configure BG 2 Sub. (Do not forget the BG_BMP_BASE configuration)
	BGCTRL_SUB[2] = BG_BMP_BASE(0) | BgSize_B8_256x256;

	// Affine Marix Transformation
	REG_BG2PA_SUB = 256;
	REG_BG2PC_SUB = 0;
	REG_BG2PB_SUB = 0;
	REG_BG2PD_SUB = 256;

	swiCopy(levelSelPal, BG_PALETTE_SUB, levelSelPalLen);
	swiCopy(levelSelBitmap, BG_GFX_SUB, levelSelBitmapLen);
}

void P_Map16x16_configureBG3()
{
	/*GRAPHICS III
	 *
	 * This function is called in the initialization of the P_Map16x16 to
	 * configure the Background 3.
	 *
	 * This background is going to be used to background wall-paper.
	 *
	 * In exercise 2 the Background 3 (BG3) should be configure in rotoscale
	 * mode with a 8 bits of pixel depth. Do not forget to copy the palette
	 * correctly.
	 *
	 * In exercise 5 Background 3 (BG3) should be configured in the tiled mode
	 * as a 32x32 tile grid and using one general color palette of 256
	 * components (256 colors).
	 */

	// Configure BG 3 for the background image as explained before
	BGCTRL[3] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(26) | BG_TILE_BASE(0);

	// Copy tiles, map and palette in the memory (use swicopy or memcpy)
	swiCopy(skyTiles, BG_TILE_RAM(0), skyTilesLen);
	swiCopy(skyMap, BG_MAP_RAM(26), skyMapLen);
	swiCopy(skyPal, BG_PALETTE, skyPalLen);
}

void P_Map16x16_Init(int cols, int rows)
{
	MapCols = cols;
	MapRows = rows;
	// Configure BG3 for background image
	P_Map16x16_configureBG3();

	// Configure Bottom background
	P_Map16x16_configureBG2_Sub();
	// Configure BG0 in the sub engine
	P_Map16x16_configureBG0_Sub();
}
