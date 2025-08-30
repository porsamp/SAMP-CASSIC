#include <YSI_Coding\y_hooks>

LoadDataFaction() {
    mysql_tquery(dbCon, "SELECT * FROM `factions`", "Faction_Load", "");
    return 1;
}

forward Faction_Load();
public Faction_Load() {
    static rows, tick;
    cache_get_row_count(rows);
    tick = GetTickCount();

	for (new i = 0; i < rows; i ++) if (i < MAX_FACTION)
	{
		cache_get_value_name_int(i, "groupID", DynamicFaction[i][gID]);
	    cache_get_value_name(i, "groupName", DynamicFaction[i][gName], 32);
		cache_get_value_name(i, "groupColor", DynamicFaction[i][gColor], 6);
		cache_get_value_name_int(i, "groupType", DynamicFaction[i][gType]);
		cache_get_value_name_float(i, "SpawnX", DynamicFaction[i][gSpawnX]);
		cache_get_value_name_float(i, "SpawnY", DynamicFaction[i][gSpawnY]);
		cache_get_value_name_float(i, "SpawnZ", DynamicFaction[i][gSpawnZ]);
		cache_get_value_name_int(i, "SpawnInt", DynamicFaction[i][gSpawnInt]);
		cache_get_value_name_int(i, "SpawnVW", DynamicFaction[i][gSpawnVW]);
		cache_get_value_name_int(i, "groupNewbie", DynamicFaction[i][gNewbie]);

        for(new rank = 0; rank < MAX_FACTION_RANK; rank++)
            cache_get_value_name(i, sprintf("Rank_%d", rank+1), Faction_RankName[i][rank], 24+1);

        for(new skin = 0; skin < MAX_FACTION_SKINS; skin++)
            cache_get_value_name_int(i, sprintf("Skins_%d", skin+1), Faction_Skins[i][skin]);

		if(DynamicFaction[i][gSpawnX] != 0 && DynamicFaction[i][gSpawnY] != 0) {
			Faction_UpdateLoc(i);	
		}

		Iter_Add(Faction_Array, i);
	}

	printfc(AQUA, "[Cassic:SQL] -> Load %d for Faction | Take %d m/s", Iter_Count(Faction_Array), GetTickCount() - tick);
	MySQL_Stat();
	return 1;
}


forward Faction_Insert(FacID);
public Faction_Insert(FacID) {
    if(FacID == -1) return 0;
    DynamicFaction[FacID][gID] = cache_insert_id();
    /*----------------------------------*/
    for(new rank = 0; rank < MAX_FACTION_RANK; rank++)
        Faction_SaveRankNameData(rank, FacID);
    /*----------------------------------*/
    return 1;
}

stock Faction_SaveNameData(FacID) {
    new query[128];
    mysql_format(dbCon,query,sizeof(query),"UPDATE `factions` SET `groupName` = '%e' WHERE `groupID` = '%d'",DynamicFaction[FacID][gName],DynamicFaction[FacID][gID]);
    mysql_tquery(dbCon,query);
    return 1;
}

stock Faction_SaveHexColorData(FacID) {
    new query[64];
    mysql_format(dbCon,query,sizeof(query),"UPDATE `factions` SET `groupColor` = '%e' WHERE `groupID` = '%d'",DynamicFaction[FacID][gColor],DynamicFaction[FacID][gID]);
    mysql_tquery(dbCon,query);
    return 1;
}

stock Faction_SaveTypeData(FacID) {
    new query[64];
    mysql_format(dbCon,query,sizeof(query),"UPDATE `factions` SET `groupType` = '%d' WHERE `groupID` = '%d'",DynamicFaction[FacID][gType],DynamicFaction[FacID][gID]);
    mysql_tquery(dbCon,query);
    return 1;
}

stock Faction_SaveRankNameData(RankID,FacID) {
    new query[256];
    mysql_format(dbCon,query,sizeof(query),"UPDATE `factions` SET `Rank_%d` = '%e' WHERE `groupID` = '%d'",RankID+1,Faction_RankName[FacID][RankID],DynamicFaction[FacID][gID]);
    mysql_tquery(dbCon,query);    
    return 1;
}

stock Faction_SaveSkinsData(SkinID,FacID) {
    new query[256];
    mysql_format(dbCon,query,sizeof(query),"UPDATE `factions` SET `Skins_%d` = '%d' WHERE `groupID` = '%d'",SkinID+1,Faction_Skins[FacID][SkinID],DynamicFaction[FacID][gID]);
    mysql_tquery(dbCon,query);    
    return 1;
}

stock Player_SaveFactionData(playerid) {
    new query[128];
    mysql_format(myDB,query,sizeof(query),"UPDATE userdata SET Faction = %d WHERE UID = %d",PlayerData[playerid][pFactionID], PlayerData[playerid][pUID]);
    print(query);
    mysql_tquery(myDB,query);
    return 1;
}

stock Player_SaveFactionRankData(playerid) {
    new query[128];
    mysql_format(myDB,query,sizeof(query),"UPDATE userdata SET Rank = %d WHERE id = %d",Player_GetRank(playerid), PlayerData[playerid][pUID]);
    mysql_tquery(myDB,query);
    return 1;
}