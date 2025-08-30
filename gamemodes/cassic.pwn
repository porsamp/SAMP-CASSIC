#include    <a_samp>

#include    "config"

#include 	<a_mysql>
#include    <streamer>

#include 	<YSI_Data\y_iterate>
#include 	<YSI_Coding\y_timers>
#include 	<YSI_Visual\y_commands>
#include 	<YSI_Data\y_bit>

#include 	<Pawn.RakNet>
#include    <sscanf2>
#include    <strlib>
#include 	<samp_bcrypt>	
// #include    <progress2>	
#include    <distance>
#include    <cuf>

#define 	cec_auto
#include	<cec>
#include    <easyDialog>

DEFINE_HOOK_REPLACEMENT(OnPlayer, OP_);

// Config
#include "inc\config\data.inc"

// General
#include "inc\utils\enums.inc"
#include "inc\utils\functions.inc"
#include "inc\utils\mysql.inc"
#include "inc\utils\callbacks.inc"
#include "inc\utils\timers.inc"

// Core
#include "inc\core\setup.inc"
#include "inc\core\auth.inc"
#include "inc\core\update.inc"

// System
#include "inc\games\inventory_db.inc"
#include "inc\games\inventory_items.inc"
#include "inc\games\inventory_list.inc"
#include "inc\games\inventory_main.inc"
#include "inc\games\faction_main.inc"
#include "inc\games\faction_edittor.inc"
#include "inc\games\faction_db.inc"
#include "inc\games\tutorial.inc"
#include "inc\games\menu.inc"

// CMD
#include "inc\cmd\admin.inc"
#include "inc\cmd\player.inc"

// MAP

main () {  }

public OnGameModeInit() {
	InitiateGamemode();    
    return 1;
}

public OnGameModeExit() {
	print("Exiting the gamemode, please wait...");
	return 1;
}