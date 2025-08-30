timer Timer_Freeze[3000](playerid) {
    stop PlayerInfo[playerid][pFreezeTimer];

    PlayerInfo[playerid][pFreeze] = false;
    TogglePlayerControllable(playerid, true);
    return 1;
}
