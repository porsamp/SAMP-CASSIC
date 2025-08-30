#include    <a_samp>

#include "config"

#include 	<a_mysql>
#include    <streamer>
#include 	<Pawn.RakNet>
#define 	cec_auto
#include	<cec>
#include    <easyDialog>
#include    <sscanf2>
#include    <strlib>
#include 	<samp_bcrypt>	
// #include    <progress2>	
#include    <distance>
#include    <cuf>

#include 	<YSI_Data\y_iterate>
#include 	<YSI_Coding\y_timers>

DEFINE_HOOK_REPLACEMENT(OnPlayer, OP_);

main () {  }

public OnGameModeInit() {
    printf("Hell Worad");
    return 1;
}

public OnGameModeExit() {
    return 1;
}