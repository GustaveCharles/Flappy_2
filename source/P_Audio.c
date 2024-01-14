#include "P_Audio.h"


void Audio_Init(){
    mmInitDefaultMem((mm_addr)soundbank_bin);
    mmLoadEffect(SFX_JUMP);
}