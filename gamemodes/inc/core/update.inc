#include    <YSI_Coding\y_hooks>

Player_SaveLastLoginData(playerid) {
    new 
        query[70 + 1]
    ;

    mysql_format(dbCon, query,sizeof(query),"UPDATE userdata SET last_login = CURRENT_TIMESTAMP() WHERE id = %d", PlayerInfo[playerid][pUID]);
    mysql_tquery(dbCon, query);    
    return 1;
}

Player_UpdateData(playerid) {
    if(!Player_LoggedIn(playerid)) return 0;
    GetPlayerHealth(playerid, PlayerInfo[playerid][pLastHP]);
    GetPlayerArmour(playerid, PlayerInfo[playerid][pLastAP]);

    Player_SavePostionData(playerid);
    MySQL_UpdatePlayerInfoFloat(playerid, "lasthp", PlayerInfo[playerid][pLastHP]);
    MySQL_UpdatePlayerInfoFloat(playerid, "lastap", PlayerInfo[playerid][pLastAP]);
    return 1;
}

Player_SavePostionData(playerid) {
    if(!Player_LoggedIn(playerid)) return 0;
    GetPlayerPos(playerid, PlayerInfo[playerid][pLastPosX], PlayerInfo[playerid][pLastPosY], PlayerInfo[playerid][pLastPosZ]);
    GetPlayerFacingAngle(playerid, PlayerInfo[playerid][pLastPosA]);
    PlayerInfo[playerid][pLastInt] = GetPlayerInterior(playerid);
    PlayerInfo[playerid][pLastVW] = GetPlayerVirtualWorld(playerid);

    MySQL_UpdatePlayerInfoFloat(playerid, "lastposx", PlayerInfo[playerid][pLastPosX]);
    MySQL_UpdatePlayerInfoFloat(playerid, "lastposy", PlayerInfo[playerid][pLastPosY]);
    MySQL_UpdatePlayerInfoFloat(playerid, "lastposz", PlayerInfo[playerid][pLastPosZ]);
    MySQL_UpdatePlayerInfoFloat(playerid, "lastposa", PlayerInfo[playerid][pLastPosA]);
    MySQL_UpdatePlayerInfoInt(playerid, "lastint", PlayerInfo[playerid][pLastInt]);
    MySQL_UpdatePlayerInfoInt(playerid, "lastword", PlayerInfo[playerid][pLastVW]);
    return 1;
}