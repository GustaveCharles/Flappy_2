
#include "P_Initializer.h"

void P_InitNDS()
{
	//Initialize the main engine
	P_Graphics_setup_main();
	P_Map16x16_Init( 12, 16 );

	//Audio_Init();
	//Audio_PlayMusic();
}