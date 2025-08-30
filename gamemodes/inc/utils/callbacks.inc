#include    <YSI_Coding\y_hooks>

#define     CMD_DEBUG

public OnPlayerSpawn(playerid) {
    Player_SendSpawn(playerid);
    Streamer_SetVisibleItems(STREAMER_TYPE_OBJECT, 1000, -1);

    /*-------------------------------------*/
    SetPlayerArmourData(playerid, PlayerInfo[playerid][pLastAP]);
    SetPlayerHealthData(playerid, PlayerInfo[playerid][pLastHP]);
    SetCameraBehindPlayer(playerid);
    return 1;
}
