AntiDeAMX() {
   new b;
   #emit load.pri b
   #emit stor.pri b
}

stock Player_LoggedIn(playerid)
{
   return (PlayerInfo[playerid][pLoggedIn]);
}

stock ReturnName(playerid) {
	static name[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}

stock Player_GetName(playerid) {
   new 
     name[25]
   ;

   GetPlayerName(playerid, name, sizeof(name));
   return name;
}

stock Send_Message(playerid, const fmat[],  va_args<>) {
	new 
     str[145]
   ;

	va_format(str, sizeof (str), fmat, va_start<2>);
	return SendClientMessage(playerid, -1, str);
}

stock Clear_Message(playerid, line) {
   for(new i = 0; i != line; i++) {
     SendClientMessage(playerid, -1, "");
   }
   return 1;
}

stock Player_Permission(playerid) {
   switch(PlayerInfo[playerid][pAdmin]) {
      case 1: PlayerInfo[playerid][pCMDPermission] = CMD_PLAYER | CMD_ADMIN;
      case 2: PlayerInfo[playerid][pCMDPermission] = CMD_PLAYER | CMD_ADMIN | CMD_DEV; 
      case 3: PlayerInfo[playerid][pCMDPermission] = CMD_PLAYER | CMD_ADMIN | CMD_DEV;
      default: PlayerInfo[playerid][pCMDPermission] = CMD_PLAYER;
   }
   return 1;
}

stock GetMonthDays(year, month) {
   switch(month) {
     case 0: return 31;
     case 1: return 31;
     case 2: return FebruaryDays(year);
     case 3: return 31;
     case 4: return 30;
     case 5: return 31;
     case 6: return 30;
     case 7: return 31;
     case 8: return 31;
     case 9: return 30;
     case 10: return 31;
     case 11: return 30;
     case 12: return 31;
     default: return -1;     
   }
   return -1;
}

stock FebruaryDays(year) {
   if(!(year % 4)) return 29;
   else return 28;   
}

stock GetPlayerCashData(playerid) {
   return (PlayerInfo[playerid][pCash]);
}

stock GivePlayerCashData(playerid, cash) {
   PlayerInfo[playerid][pCash] += cash;
   SetPlayerCashData(playerid, PlayerInfo[playerid][pCash]);   
   MySQL_UpdatePlayerInfoInt(playerid, "cash", PlayerInfo[playerid][pCash]);
   return 1;
}

SetPlayerCashData(playerid, cash) {
   ResetPlayerMoney(playerid);
   GivePlayerMoney(playerid, cash);
   return 1;
}

stock Float:Player_HealthData(playerid) {
   new 
      Float:Cur_HP
   ;

   GetPlayerHealth(playerid, Cur_HP);
   return (Cur_HP);
}

stock SetPlayerHealthData(playerid, Float:Hp) {
   PlayerInfo[playerid][pLastHP] = Hp;
   SetPlayerHealth(playerid, Hp);
   return 1;
}

stock GivePlayerHealthData(playerid, Float:Hp) {
   new 
      Float:Cur_HP
   ;

   GetPlayerHealth(playerid, Cur_HP);
   if(Cur_HP + Hp > 100.0) SetPlayerHealthData(playerid, 100.0);
   else SetPlayerHealthData(playerid, Cur_HP + Hp);
   return 1;
}

stock Float:Player_ArmourData(playerid) {
   new 
      Float:Cur_AP
   ;

   GetPlayerArmour(playerid, Cur_AP);
   return (Cur_AP);
}

SetPlayerArmourData(playerid, Float:Ap) {
   PlayerInfo[playerid][pLastAP] = Ap;
   SetPlayerArmour(playerid, Ap);
   return 1;
}

stock GivePlayerArmourData(playerid, Float:Ap) {
   new 
      Float:Cur_AP
   ;
   
   GetPlayerArmour(playerid, Cur_AP);
   if(Cur_AP + Ap > 100.0) SetPlayerArmourData(playerid, 100.0);
   else SetPlayerArmourData(playerid, Cur_AP + Ap);
   return 1;
}

stock SetPlayerSkinData(playerid, skin, save = false) {
   SetPlayerSkin(playerid, skin);
   
   if(save) {
      PlayerInfo[playerid][pSkin] = skin;
      MySQL_UpdatePlayerInfoInt(playerid, "skin", PlayerInfo[playerid][pSkin]);
   }
   return 1;
}

stock sformat(const fmat[],va_args<>) {
    new retstr[256];
    va_format(retstr,sizeof(retstr),fmat,va_start<1>);
    return retstr;
}

stock Player_SetListClick(playerid, i) {
   PlayerInfo[playerid][pSelect_click] = i;
}

stock Player_GetListClick(playerid) {
   return (PlayerInfo[playerid][pSelect_click]);
}

Credit() {
   print("\n\n");
   printf("      _______   _______   _______      ");
   printf("      |   _   | |   _   | |   _   |     	");
   printf("      |.  1   | |.  |   | |.  l   /     	");
   printf("      |.  ____| |.  |   | |.  _   1     	");
   printf("      |:  |    |:  1   | |:  |   |     	");
   printf("      |::.|    |::.. . | |::.|:. |     	");
   printf("  											       ");
   printf("    _______   _______   ___ ___   _______	");
   printf("   |   _   | |   _   | |   Y   | |   _   |");
   printf("   |   1___| |.  1   | |.     | |.  1   |");
   printf("   |____   | |.  _   | |. |_/  | |.  ____|");
   printf("   |:  1   | |:  |   | |:  |   | |:  	  	");
   printf("   |::.. . | |::.|:. | |::.|:. | |::.	  	");   
   printf("\n											       ");
   printf("   Successfully initiated the gamemode...");
   printf("\n\n");
}