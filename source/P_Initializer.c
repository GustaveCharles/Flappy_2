
#include "P_Initializer.h"
#include "P_Graphics.h"
#include "P_Map16x16.h"
#include "P_Time.h"
#include "P_Score.h"
#include "P_Audio.h"

void P_InitNDS()
{
	//Initialize the main engine
	P_Graphics_setup_main();
	P_Map16x16_Init( 12, 16 );
	Audio_Init();
}