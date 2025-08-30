#include <YSI_Coding\y_hooks>

#define     MAX_FACTION         (50)
#define     MAX_FACTION_RANK    (6)
#define     MAX_FACTION_SKINS   (6)
#define     MAX_FACTION_SKIP    (20)

enum eFactionData {
    gID,
    gName[32],
    gColor[6+1],
    gType,
    Float:gSpawnX,
    Float:gSpawnY,
    Float:gSpawnZ,
    gSpawnInt,
    gSpawnVW,
    gNewbie,

    gPickup,
	Text3D:gd3d,
};
new DynamicFaction[MAX_FACTION][eFactionData];

new 
    Faction_RankName[MAX_FACTION][MAX_FACTION_RANK][24+1],
    Faction_Skins[MAX_FACTION][MAX_FACTION_SKINS],
    Faction_EditRankIndex[MAX_PLAYERS
];

new 
    Iterator:Faction_Array<MAX_FACTION>,
    Iterator:Faction_Member[MAX_FACTION]<MAX_PLAYERS>
;

Faction_Clear() {
    Iter_Clear(Faction_Array);
    return 1;
}

LoadPlayerFaction(playerid) {
    if(PlayerInfo[playerid][pMember] == -1)
        return 0;

    // PlayerInfo[playerid][pMember] = Faction_Install(PlayerInfo[playerid][pMember]);
    Player_InstallFaction(playerid, Player_GetFaction(playerid));
    return 1;
}

stock Faction_Install(FacID) {
    foreach(new i : Faction_Array) if(DynamicFaction[i][gID] == FacID) 
        return i;

    return -1;
}

Faction_Create() {
    new FacID = Iter_Free(Faction_Array), query[218];

    DynamicFaction[FacID][gType] = 0;

    format(DynamicFaction[FacID][gName], 24+1, "ว่างเปล่า");
    format(DynamicFaction[FacID][gColor], 6+1, "FFFFFF");

    for(new rank = 0; rank < MAX_FACTION_RANK; rank++)
		format(Faction_RankName[FacID][rank], 24+1, "Rank %d", rank + 1);
    
    mysql_format(dbCon, query, sizeof(query), "INSERT INTO `factions` (`groupName`,`groupColor`,`groupType`,`groupNewbie`) VALUES ('%e','%e','%d','%d')", DynamicFaction[FacID][gName], DynamicFaction[FacID][gColor], DynamicFaction[FacID][gType], DynamicFaction[FacID][gNewbie]);
    mysql_tquery(dbCon, query, "Faction_Insert", "i", FacID);
    Iter_Add(Faction_Array, FacID);  
    return FacID;
}

Faction_Destroy(FacID) {
    if(FacID == -1)
        return 0;

    DynamicFaction[FacID][gType] = 0;
    format(DynamicFaction[FacID][gName], 24+1, "ว่างเปล่า");
    format(DynamicFaction[FacID][gColor], 6+1, "FFFFFF");
    
    Faction_SaveTypeData(FacID);
    Faction_SaveNameData(FacID);
    Faction_SaveHexColorData(FacID);
    return 1;
}

Faction_UpdateLoc(id)
{
	new string[528];
	if(IsValidDynamic3DTextLabel(DynamicFaction[id][gd3d])) 
		DestroyDynamic3DTextLabel(DynamicFaction[id][gd3d]);

	if(IsValidDynamicPickup(DynamicFaction[id][gPickup])) 
		DestroyDynamicPickup(DynamicFaction[id][gPickup]);

	format(string, sizeof(string), "{ffffff}• ตำแหน่งที่ตั้งฐานเกิดของ%s • \n{%s}• %s (%d) •", Faction_GetTypeName(DynamicFaction[id][gType]), DynamicFaction[id][gColor], DynamicFaction[id][gName], id);
	DynamicFaction[id][gd3d] = CreateDynamic3DTextLabel(string, -1, DynamicFaction[id][gSpawnX], DynamicFaction[id][gSpawnY], DynamicFaction[id][gSpawnZ], 25, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, DynamicFaction[id][gSpawnVW], DynamicFaction[id][gSpawnInt], -1, 25.0);
	DynamicFaction[id][gPickup] = CreateDynamicPickup(1314, 23, DynamicFaction[id][gSpawnX], DynamicFaction[id][gSpawnY], DynamicFaction[id][gSpawnZ], DynamicFaction[id][gSpawnVW], DynamicFaction[id][gSpawnInt], -1, 25.0);
	return 1;
}

Faction_List(playerid) {
    new 
        str[64], 
        dialog_out[1028] = "ไอดี\tชื่อ\tสมาชิกออนไลน์\tสล็อต\n", 
        count
    ;

    strcat(dialog_out, "{FFBF00}>{FFFFFF} หน้าถัดไป\n");
    new listitem_skip = (PlayerInfo[playerid][pSelectSkip]) * MAX_FACTION_SKIP;

    foreach(new i : Faction_Array) {
        if(listitem_skip) {
            listitem_skip--;
            continue;
        }

        if(count == MAX_FACTION_SKIP) break;
        format(str, sizeof(str), "({FFBF00}%i{FFFFFF})\t{AEFFBC}%s\t%d คน\n", i, DynamicFaction[i][gName],Iter_Count(Faction_Member[i]));
        strcat(dialog_out, str);
        count++;
        PlayerInfo[playerid][pSelect][count] = i;
        PlayerInfo[playerid][pSelectMax] = count;
    }

    strcat(dialog_out, "{FFFF00}[+]{FFFFFF} สร้างเพิ่ม"); 
    Dialog_Show(playerid, Dialog_FactionList, DIALOG_STYLE_TABLIST_HEADERS, sformat("{AEFFBC}Faction{FFFFFF} List   (%d/%d)",PlayerInfo[playerid][pSelectSkip] + 1 , MAX_FACTION / MAX_FACTION_SKIP), dialog_out, "ตกลง", "กลับ");
    return 1;
}

Faction_MenuEdit(playerid,FacID) {
    if(!Iter_Contains(Faction_Array, FacID))
        return -1;

    Dialog_Show(playerid,Dialog_FactionEdit,DIALOG_STYLE_TABLIST,sprintf("{AEFFBC}Faction - {%s}%s",DynamicFaction[FacID][gColor],DynamicFaction[FacID][gName]),"\
    > แก้ไขชื่อ\t%s\n\
    > แก้ไขประเภท\t%s\n\
    > แก้ไขสี\t{%s}%s\n\
    > ตั้งค่ายศ\t%d\n\
    > แก้ไขสกิน\t%d\n\
    > ยัดหัวหน้า\n\
    {FF6600}> {FFFFFF}ยุบ Faction ทิ้ง\
    ","ตกลง","กลับ",
        DynamicFaction[FacID][gName],
        Faction_GetTypeName(Faction_GetType(FacID)),
        DynamicFaction[FacID][gColor],
        DynamicFaction[FacID][gColor],
        MAX_FACTION_RANK,
        MAX_FACTION_SKINS
    );
    return 1;
}

Faction_AuthEditSkins(playerid) {
    new str[512];
    for (new i = 0; i < MAX_FACTION_SKINS; i ++)
        format(str,sizeof(str),"%s\n{AA0000}สกินตัวที่ %i:{FFFFFF} %d",str, i + 1, Faction_Skins[Player_GetListClick(playerid)][i]);

    Dialog_Show(playerid, Dialog_AuthEditSkin, DIALOG_STYLE_LIST, "{AEFFBC}Faction{FFFFFF} - จัดการสกินตัวลคร", str, "ตกลง", "กลับ");
    return 1;
}

Faction_AuthEditRank(playerid) {
    new str[512];
    for (new i = 0; i < MAX_FACTION_RANK; i ++)
        format(str,sizeof(str),"%s\n{AA0000}ระดับยศที่ %i:{FFFFFF} %s",str, i + 1, Faction_RankName[Player_GetListClick(playerid)][i]);

    Dialog_Show(playerid, Dialog_AuthEditRank, DIALOG_STYLE_LIST, "{AEFFBC}Faction{FFFFFF} - จัดการยศ", str, "ตกลง", "กลับ");
    return 1;
}

Faction_EditRankOnly(playerid,RankID) {
    Dialog_Show(playerid,Dialog_EditRankOnly,DIALOG_STYLE_INPUT,"{AEFFBC}Faction{FFFFFF} - แก้ไขชื่อยศ","\
    {FFFFFF}กรอกชื่อที่คุณจะตั้งค่ายศ {FFFF00}(%d)\
    ","ตกลง","กลับ",RankID+1);
    return 1;
}

Faction_EditSkinOnly(playerid,SkinID) {
    Dialog_Show(playerid,Dialog_EditSkinsOnly,DIALOG_STYLE_INPUT,"{AEFFBC}Faction{FFFFFF} - แก้ไขสกินตัวลคร","\
    {FFFFFF}กรอกไอดีสกินที่คุณจะตั้งค่าสกิน {FFFF00}(%d)\
    ","ตกลง","กลับ",SkinID+1);
    return 1;
}

Faction_AuthEditType(playerid) {
    new str[128];
    for(new i=0;i<6;i++) {
        format(str, sizeof(str),"%s\n%s%s", str, Faction_GetTypeName(i), (Faction_GetType(Player_GetListClick(playerid))==i) ? "{FFFF00} - กำลังใช้งาน":"");
    }
    Dialog_Show(playerid,Dialog_FactionEditType, DIALOG_STYLE_LIST,"{AEFFBC}Faction{FFFFFF} - แก้ไขประเภท",str,"ตกลง","กลับ");
    return 1;
}

Faction_AuthEditName(playerid) {
    Dialog_Show(playerid,Dialog_FactionEditName,DIALOG_STYLE_INPUT,"{AEFFBC}Faction{FFFFFF} - แก้ไขชื่อ","\
    {FFFFFF}กรุณากรอกชื่อที่ต้องการจะตั้ง Faction\
    ","ตกลง","กลับ");
    return 1;
}

Faction_AuthEditHexColor(playerid) {
    Dialog_Show(playerid,Dialog_FactionEditColor,DIALOG_STYLE_INPUT,"{AEFFBC}Faction{FFFFFF} - แก้ไขสี","\
    {FFFFFF}กรุณากรอกโค้ดสี Hex 6 หลัก เพื่อต้องการจะตั้ง Faction\
    ","ตกลง","กลับ");
    return 1;
}

stock Faction_GetID(FacID) {
    return DynamicFaction[FacID][Faction_ID];
}

stock Faction_GetTypeName(type)
{
	new string[32] = "ไม่ทราบ";

	switch(type)
	{
		case 1: format(string, 32, "ตำรวจ");
		case 2: format(string, 32, "หมอ");
		case 3: format(string, 32, "หน่วยงานรัฐ");
		case 4: format(string, 32, "แก๊งค์");
		case 5: format(string, 32, "นายทะเบียน");
	}
	return string;
}

stock Faction_GetType(FacID) {
    return DynamicFaction[FacID][gType];
}

stock Faction_GetName(FacID) {
    new name[32] = "ประชาชน";

    if(FacID == -1)
        return name;
    
    format(name, 32, DynamicFaction[FacID][gName]);
    return name;
}

stock Faction_SetPlayerColor(playerid) {
    if(Player_GetFaction(playerid) != -1) 
        SetPlayerColor(playerid, HexToInt(sprintf("0x%sFF",DynamicFaction[Player_GetFaction(playerid)][gColor])));
    else SetPlayerColor(playerid,-1);
    return 1;
}

stock Player_GetRank(playerid) {
    return PlayerInfo[playerid][pFactionRank];
}

stock Player_GetRankName(playerid) {
    return Faction_RankName[Player_GetFaction(playerid)][Player_GetRank(playerid)-1];
}

stock Player_GetFaction(playerid) {
    return PlayerInfo[playerid][pMember];
}

stock Player_GetFactionName(playerid) {
    new 
        FacID = PlayerInfo[playerid][pMember],
        name[32] = "ประชาชน"
    ;

 	if (FacID == -1)
	    return name;

	format(name, 32, DynamicFaction[FacID][gName]);
	return name;
}

stock Player_SetFaction(playerid, FacID) {
    // PlayerInfo[playerid][pMember] = FacID;
    PlayerInfo[playerid][pMember] = (FacID == -1) ? -1 : DynamicFaction[FacID][Faction_ID];
    Faction_SetPlayerColor(playerid);
    Player_SaveFactionData(playerid);
    return 1;
}

stock Player_SetRank(playerid,RankID) {
    if(Player_GetFaction(playerid) == -1) 
        return 0;

    PlayerInfo[playerid][pFactionRank] = RankID;
    Player_SaveFactionRankData(playerid);
    return 1;
}

Player_InstallFaction(playerid,FacID, Add = 1) {
    if(Add) {
        Iter_Add(Faction_Member[FacID], playerid);
    }
    else Iter_Remove(Faction_Member[FacID], playerid);
    return 1;
}


Dialog:Dialog_FactionList(playerid,response,listitem,inputtext[]) {
    if(!response) {
        if(PlayerInfo[playerid][pSelectSkip]) {
            PlayerInfo[playerid][pSelectSkip]--;
            Faction_List(playerid);
        }
        else AuthPlayerAdminMenuDynamic(playerid);
        return 1;
    }

    if(!strcmp(inputtext,"> หน้าถัดไป",true)) {
        if(PlayerInfo[playerid][pSelectMax] >= MAX_FACTION_SKIP)
            PlayerInfo[playerid][pSelectSkip]++;

        Faction_List(playerid);
        return 1;
    }

    if(!strcmp(inputtext,"[+] สร้างเพิ่ม",true)) {
        new ret = Faction_Create();
        if(ret != -1) Faction_List(playerid);
        return 1;
    }

    Player_SetListClick(playerid, PlayerInfo[playerid][pSelect][listitem]);
    Faction_MenuEdit(playerid, PlayerInfo[playerid][pSelect][listitem]);
    return 1;
}

Dialog:Dialog_FactionEdit(playerid,response,listitem,inputtext[]) {
    if(!response) return Faction_List(playerid);
    switch(listitem) {
        case 0: Faction_AuthEditName(playerid);
        case 1: Faction_AuthEditType(playerid);
        case 2: Faction_AuthEditHexColor(playerid);
        case 3: Faction_AuthEditRank(playerid);
        case 4: Faction_AuthEditSkins(playerid);
        // case 5: callcmd::setleader(playerid, sformat("%d %d", playerid, Player_GetListClick(playerid)));
    }
    if(!strcmp(inputtext,"> ยุบ Faction ทิ้ง",true)) {
        new ret = Faction_Destroy(Player_GetListClick(playerid));
        if(ret) SendClientMessage(playerid,-1,"ยุบ Faction สำเร็จ {33FF00}!");
        else SendClientMessage(playerid,-1,"{FF0000}ไม่สามารถลบ Faction นี้ได้ !");
    }
    return 1;
}

Dialog:Dialog_FactionEditType(playerid,response,listitem,inputtext[]) {
    if(!response) 
        return Faction_MenuEdit(playerid,Player_GetListClick(playerid));

    if(listitem == Faction_GetType(Player_GetListClick(playerid))) 
        return Faction_AuthEditType(playerid);

    DynamicFaction[Player_GetListClick(playerid)][gType] = listitem;
    Faction_AuthEditType(playerid); 
    Faction_SaveTypeData(Player_GetListClick(playerid));
    return 1;
}

Dialog:Dialog_FactionEditName(playerid,response,listitem,inputtext[]) {
    if(!response) 
        return Faction_MenuEdit(playerid,Player_GetListClick(playerid));

    if(!(strlen(inputtext) <= 25)) 
        return Faction_AuthEditName(playerid);

    format(DynamicFaction[Player_GetListClick(playerid)][gName],24+1,inputtext);
    Faction_SaveNameData(Player_GetListClick(playerid));
    Faction_List(playerid);
    return 1;
}

Dialog:Dialog_FactionEditColor(playerid,response,listitem,inputtext[]) {
    if(!response) 
        return Faction_MenuEdit(playerid,Player_GetListClick(playerid));

    if(!(strlen(inputtext) <= 6)) 
        return Faction_AuthEditHexColor(playerid);

    format(DynamicFaction[Player_GetListClick(playerid)][gColor],6+1,inputtext);
    Faction_SaveHexColorData(Player_GetListClick(playerid));
    Faction_List(playerid);
    return 1;
}

Dialog:Dialog_AuthEditSkin(playerid,response,listitem,inputtext[]) {
    if(!response) 
        return Faction_MenuEdit(playerid,Player_GetListClick(playerid));
    
    Faction_EditRankIndex[playerid] = listitem;
    Faction_EditSkinOnly(playerid, listitem);
    return 1;
}

Dialog:Dialog_EditSkinsOnly(playerid,response, listitem,inputtext[]) {
    if(!response) 
        return Faction_AuthEditSkins(playerid);

    if(!(strlen(inputtext) <= 25)) 
        return Faction_EditSkinOnly(playerid,Faction_EditRankIndex[playerid]);

    if(strval(inputtext) < 0 || strval(inputtext) > 311)
        return Faction_EditSkinOnly(playerid,Faction_EditRankIndex[playerid]);

    Faction_Skins[Player_GetListClick(playerid)][Faction_EditRankIndex[playerid]] = strval(inputtext);
    Faction_SaveSkinsData(Faction_EditRankIndex[playerid], Player_GetListClick(playerid));
    Faction_AuthEditSkins(playerid);
    return 1;
}

Dialog:Dialog_AuthEditRank(playerid,response,listitem,inputtext[]) {
    if(!response) 
        return Faction_MenuEdit(playerid,Player_GetListClick(playerid));

    Faction_EditRankIndex[playerid] = listitem;
    Faction_EditRankOnly(playerid, listitem);
    return 1;
}

Dialog:Dialog_EditRankOnly(playerid,response, listitem,inputtext[]) {
    if(!response) 
        return Faction_AuthEditRank(playerid);

    if(!(strlen(inputtext) <= 25)) 
        return Faction_EditRankOnly(playerid,Faction_EditRankIndex[playerid]);

    format(Faction_RankName[Player_GetListClick(playerid)][Faction_EditRankIndex[playerid]],24+1,inputtext);
    Faction_SaveRankNameData(Faction_EditRankIndex[playerid],Player_GetListClick(playerid));
    Faction_AuthEditRank(playerid);
    return 1;
}