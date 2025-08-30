#include    <YSI_Coding\y_hooks>

timer Timer_CheckAccount[1000](playerid) {
    new 
        query[128],
        user_name[MAX_PLAYER_NAME]
    ;

    GetPlayerName(playerid, user_name, MAX_PLAYER_NAME);
    mysql_format(dbCon, query, sizeof(query), "SELECT * FROM `userdata` WHERE `username` = '%e'", Player_GetName(playerid));
    mysql_tquery(dbCon, query, "Account_Check", "d", playerid);
    return 1;
}

hook OnPlayerRequestClass(playerid, classid) {
    if(!PlayerInfo[playerid][pLoggedIn]) {
        TogglePlayerSpectating(playerid, true);      
        defer Timer_CheckAccount(playerid);
    }
    else {
        SpawnPlayer(playerid);
    }
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    if(!Player_LoggedIn(playerid)) return 0;

    Player_UpdateData(playerid);
    Player_Process(playerid, 0);
    return 1;
}

forward Account_Check(playerid);
public Account_Check(playerid) {
    SetPlayerVirtualWorld(playerid, 3000+random(5000));
	InterpolateCameraPos(playerid, 1977.038940, 1221.614013, 51.226306, 1979.325439, 1223.661132, 50.634807, 20000);
	InterpolateCameraLookAt(playerid, 1980.721069, 1224.903442, 50.437683, 1983.007568, 1226.950561, 49.846183, 20000);

    if(cache_num_rows() == 1) {
        cache_get_value_name_int(0,"id",PlayerInfo[playerid][pUID]);
        cache_get_value_name(0,"pass",PlayerInfo[playerid][pPassword],BCRYPT_HASH_LENGTH);

        #if defined AUTO_LOGIN
            Player_AuthLogin(playerid);
		#else
			Password_Verify(playerid, true);
		#endif
    }
    else {
        Player_AuthRegister(playerid);
    }

    Clear_Message(playerid, 20);
    Send_Message(playerid, "ยินดีต้อนรับสู่ {AA0000}GTA RedEyes {FFFFFF}(1.0 เบต้า), {AA0000}%s", Player_GetName(playerid));
    return 1;
}

forward Account_Create(playerid);
public Account_Create(playerid) {
    PlayerInfo[playerid][pUID] = cache_insert_id();
    Player_AuthLogin(playerid);
    return 1;
}

forward Account_Load(playerid);
public Account_Load(playerid) {
    cache_get_value_name(0, "last_login", PlayerInfo[playerid][pLastLogin], 24);
    cache_get_value_name_int(0,"gender",PlayerInfo[playerid][pGender]);
    cache_get_value_name_int(0,"tutorial",PlayerInfo[playerid][pTutorial]);
    cache_get_value_name(0, "birthday", PlayerInfo[playerid][pBirthday], 24);
    cache_get_value_name_int(0,"admin",PlayerInfo[playerid][pAdmin]);
    cache_get_value_name_int(0,"cash",PlayerInfo[playerid][pCash]);
    cache_get_value_name_int(0,"bankaccount",PlayerInfo[playerid][pBankAccount]);
    cache_get_value_name_int(0,"skin",PlayerInfo[playerid][pSkin]);
    cache_get_value_name_float(0,"lastposx",PlayerInfo[playerid][pLastPosX]);
    cache_get_value_name_float(0,"lastposy",PlayerInfo[playerid][pLastPosY]);
    cache_get_value_name_float(0,"lastposz",PlayerInfo[playerid][pLastPosZ]);
    cache_get_value_name_float(0,"lastposa",PlayerInfo[playerid][pLastPosA]);
    cache_get_value_name_int(0,"lastint",PlayerInfo[playerid][pLastInt]);
    cache_get_value_name_int(0,"lastword",PlayerInfo[playerid][pLastVW]);
    cache_get_value_name_float(0,"lasthp",PlayerInfo[playerid][pLastHP]);
    cache_get_value_name_float(0,"lastap",PlayerInfo[playerid][pLastAP]);
    cache_get_value_name_int(0,"point",PlayerInfo[playerid][pPoint]);
    cache_get_value_name_int(0,"leader",PlayerInfo[playerid][pLeader]);
    cache_get_value_name_int(0,"member",PlayerInfo[playerid][pMember]);
    cache_get_value_name_int(0,"rank",PlayerInfo[playerid][pRank]);

    MySQL_Stat();

    /*--------------------------*/
    // LoadPlayerFaction(playerid);
    return 1;
}

forward Account_Register(playerid);
public Account_Register(playerid) {
    new 
        Password[64],
        query[256]
    ;

    bcrypt_get_hash(Password);
    mysql_format(dbCon, query, sizeof(query), "INSERT INTO `userdata` (`username`, `pass`) VALUES ('%e', '%e')",Player_GetName(playerid),Password);
    mysql_tquery(dbCon,query,"Account_Create","i",playerid);

    strcat(PlayerInfo[playerid][pPassword],Password);
    return 1;
}

forward Password_Verify(playerid,bool:success);
public Password_Verify(playerid,bool:success) {
    if(success) {
        new
            query[64]
        ;

        mysql_format(dbCon,query,sizeof(query),"SELECT * FROM userdata WHERE id = '%d' LIMIT 1", PlayerInfo[playerid][pUID]);
        mysql_tquery(dbCon,query,"Account_Load","i",playerid);

        mysql_format(dbCon, query, sizeof(query), "SELECT * FROM `banned` WHERE id = '%d' LIMIT 1", PlayerInfo[playerid][pUID]);
	    mysql_tquery(dbCon, query, "Banned_Verify", "d", playerid);

        PlayerPlaySound(playerid, 1150, 0.0, 0.0, 0.0);
    }
    else {
        Player_AuthLogin(playerid);
    }
    return 1;
}

forward Banned_Verify(playerid);
public Banned_Verify(playerid) {
    if(cache_num_rows()) {
        new rows, banDate[32], banner[24], reason[128];
		cache_get_row_count(rows);

		cache_get_value_name(0, "date", banDate, 32);
		cache_get_value_name(0, "bannedby", banner, 24);
		cache_get_value_name(0, "reason", reason, 128);

        new dailog_out[512];
        format(dailog_out, sizeof(dailog_out), "\
            {C68DFF}วันที่ : %s\n\n\
            {FFFFFF}[ชื่อผู้ใช้งาน]\n\
            {84FF35}%s\n\n\
            {FFFFFF}[แบนโดย]\n\
            {359DFF}%s\n\n\
            {FF354A}(สาเหตุ) : %s", banDate, Player_GetName(playerid), banner, reason);

        Dialog_Show(playerid, DIALOG_BANNED, DIALOG_STYLE_MSGBOX, "{AA0000}บรรชีของคุณโดนระงับการใช้งาน", dailog_out, "เติมปลดแบน", "ออกเกมส์");
    }
    else
    {
        Player_Process(playerid, 1);
    }
}

Player_AuthLogin(playerid) {
    Dialog_Show(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "{FFFFFF}หน้าเข้าสู่เซิร์ฟเวอร์", "\
    {ffc600}Welcome To The RedEyes\n\n\
    {FFFFFF}[ชื่อผู้ใช้งาน]\n{ff802b}%s (%d)\n\n\
    {FFFFFF}[ข้อแนะนำ]\n{66CCFF}หากคุณพบกับปัญหาสามารถสอบถามได้ในภายดิสคอร์ด\n\n\
    {FF6666}(กรุณาใส่รหัสผ่านผู้ใช้งานเพื่อเข้าสู่เซิร์ฟเวอร์)\
    ", "ตกลง", "ออก", Player_GetName(playerid), playerid);
    return 1;
}

Player_AuthRegister(playerid) {
    Dialog_Show(playerid, DIALOG_REGISTER, DIALOG_STYLE_INPUT, "{FFFFFF}หน้าสมัครผู้ใช้งาน", "\
    {ffc600}Welcome To The RedEyes\n\n\
    {FFFFFF}[ชื่อผู้ใช้งาน]\n{ff802b}%s (%d)\n\n\
    {FFFFFF}[การตั้งรหัสผ่าน]\n{66CCFF}รหัสผ่านควรยาวไม่เกิน 24 และไม่น้อยกว่า 6 ตัวอักษร\nรหัสผ่านควรมีตัวเลขและตัวอักษรปะปนกันไป\nรหัสผ่านควรตั้งที่ไม่สามารถเดาได้ง่ายดาย !\n\n\
    {FF6666}(กรุณาใส่รหัสผ่านผู้ใช้งานเพื่อเข้าสู่เซิร์ฟเวอร์)\
    ", "ตกลง", "ออก", Player_GetName(playerid), playerid);
    return 1;
}

Player_Process(playerid, ProcessType) {
    switch(ProcessType) {
        case 0: {
            PlayerInfo[playerid][pUID] = -1;
            PlayerInfo[playerid][pLastLogin] = '\0';
            PlayerInfo[playerid][pLoggedIn] = false;
            PlayerInfo[playerid][pGender] = 0;
            format(PlayerInfo[playerid][pBirthday], 24, "00/00/00");
            PlayerInfo[playerid][pTutorial] = 0;
            PlayerInfo[playerid][pAdmin] = 0;
            PlayerInfo[playerid][pCMDPermission] = CMD_PLAYER;
            PlayerInfo[playerid][pCash] = 0;
            PlayerInfo[playerid][pBankAccount] = 0;
            PlayerInfo[playerid][pSkin] = 0;
            PlayerInfo[playerid][pLastPosX] = 0.0;
            PlayerInfo[playerid][pLastPosY] = 0.0;
            PlayerInfo[playerid][pLastPosZ] = 0.0;
            PlayerInfo[playerid][pLastPosA] = 0.0;
            PlayerInfo[playerid][pLastInt] = 0;
            PlayerInfo[playerid][pLastVW] = 0;     
            PlayerInfo[playerid][pLastHP] = 0.0;
            PlayerInfo[playerid][pLastAP] = 0.0;            

            PlayerInfo[playerid][pSelectMax] = 0;
            PlayerInfo[playerid][pSelectEdit] = 0;
            PlayerInfo[playerid][pSelectSkip] = 0;
            PlayerInfo[playerid][pFreeze] = false;

            if(PlayerInfo[playerid][pFreezeTimer]) stop PlayerInfo[playerid][pFreezeTimer];
            // if(PlayerInfo[playerid][pTimer_PlayerLoop]) stop PlayerInfo[playerid][pTimer_PlayerLoop];

            // ClearDataPlayerNotify(playerid);

            /*---------------------------------*/
            // Faction_ClearData(playerid);
        }
        case 1: {
            Player_SaveLastLoginData(playerid);
            Player_Permission(playerid);

            PlayerInfo[playerid][pLoggedIn] = true;

            if(PlayerInfo[playerid][pTutorial] == 0) {
                Clear_Message(playerid, 20);

                StartPlayerTutorial(playerid);
                return 1;
            }

            SetSpawnInfo(playerid, NO_TEAM, PlayerInfo[playerid][pSkin], PlayerInfo[playerid][pLastPosX], PlayerInfo[playerid][pLastPosY], PlayerInfo[playerid][pLastPosZ], PlayerInfo[playerid][pLastPosA], 0, 0, 0, 0, 0, 0);
            TogglePlayerSpectating(playerid, false);

            /*--------------------------------------*/

            SetPlayerCashData(playerid, GetPlayerCashData(playerid));

            // PlayerInfo[playerid][pTimer_PlayerLoop] = repeat Timer_PlayerLoop(playerid);

            Player_Freeze(playerid, 5000);
            Clear_Message(playerid, 20);
            Send_Message(playerid, "คุณเข้าสู่ระบบ {AA0000}สำเร็จ {FFFFFF}ในฐานะ {AA0000}%s{FFFFFF}.", Player_GetName(playerid));
            Send_Message(playerid, "เข้าสู่ระบบครั้งล่าสุด: %s ที่ผ่านมา", PlayerInfo[playerid][pLastLogin]);
            CallLocalFunction("Player_JoinServer", "i", playerid);
        }
    }
    return 1;
}

Player_SendSpawn(playerid) {
    if(PlayerInfo[playerid][pDeath] != 0) {
        // if(PlayerInfo[playerid][pTimer_PlayerDeath]) stop PlayerInfo[playerid][pTimer_PlayerDeath];

        SetPlayerPos(playerid, PlayerInfo[playerid][pLastPosX], PlayerInfo[playerid][pLastPosY], PlayerInfo[playerid][pLastPosZ]);
        SetPlayerFacingAngle(playerid, PlayerInfo[playerid][pLastPosA]);
        SetPlayerInterior(playerid, PlayerInfo[playerid][pLastInt]);   
        SetPlayerVirtualWorld(playerid, PlayerInfo[playerid][pLastVW]);   
        SetPlayerHealthData(playerid, 50.0);

        // ShowPlayerDeathUI(playerid, true);
        // SetMutePlayerVoice(playerid, true);
        // PlayerInfo[playerid][pTimer_PlayerDeath] = repeat Timer_PlayerDeath(playerid);
    }
    else {
        SetPlayerPos(playerid, PlayerInfo[playerid][pLastPosX], PlayerInfo[playerid][pLastPosY], PlayerInfo[playerid][pLastPosZ]);
        SetPlayerFacingAngle(playerid, PlayerInfo[playerid][pLastPosA]);
        SetPlayerInterior(playerid, PlayerInfo[playerid][pLastInt]);   
        SetPlayerVirtualWorld(playerid, PlayerInfo[playerid][pLastVW]);   
    }
    return 1;
}

Player_Freeze(playerid, time = 3000) {
    if(PlayerInfo[playerid][pFreeze]) {
        stop PlayerInfo[playerid][pFreezeTimer];

        PlayerInfo[playerid][pFreeze] = false;
        TogglePlayerControllable(playerid, true);
    }

    TogglePlayerControllable(playerid, false);
    PlayerInfo[playerid][pFreeze] = true;
    PlayerInfo[playerid][pFreezeTimer] = defer Timer_Freeze[time](playerid);
    return 1;
}

Player_FirstStartGame(playerid) {

    Player_Freeze(playerid, 5000);
    Clear_Message(playerid, 20);
    GivePlayerCashData(playerid, 5000);

    PlayerInfo[playerid][pTutorial] = 1;

    PlayerInfo[playerid][pLastPosX] = 1682.843017;
    PlayerInfo[playerid][pLastPosY] = -2328.411376;
    PlayerInfo[playerid][pLastPosZ] = 13.546875;
    PlayerInfo[playerid][pLastPosA] = 0.0;
    PlayerInfo[playerid][pLastInt] = 0;
    PlayerInfo[playerid][pLastVW] = 0;     

    PlayerInfo[playerid][pLastHP] = 100.0;
    PlayerInfo[playerid][pLastAP] = 0.0;            

    // PlayerInfo[playerid][pTimer_PlayerLoop] = repeat Timer_PlayerLoop(playerid);

    SetSpawnInfo(playerid, NO_TEAM, PlayerInfo[playerid][pSkin], PlayerInfo[playerid][pLastPosX], PlayerInfo[playerid][pLastPosY], PlayerInfo[playerid][pLastPosZ], PlayerInfo[playerid][pLastPosA], 0, 0, 0, 0, 0, 0);
    TogglePlayerSpectating(playerid, false);

    SetPlayerCashData(playerid, GetPlayerCashData(playerid));
    SetPlayerSkinData(playerid, (PlayerInfo[playerid][pGender] == 1) ? 37: 93, true);

    MySQL_UpdatePlayerInfoStr(playerid, "birthday", PlayerInfo[playerid][pBirthday]);
    MySQL_UpdatePlayerInfoInt(playerid, "gender", PlayerInfo[playerid][pGender]);
    MySQL_UpdatePlayerInfoInt(playerid, "tutorial", PlayerInfo[playerid][pTutorial]);
    // Player_UpdateData(playerid);

    Send_Message(playerid, "ยินดีต้อนรับ, นี้เป็นการเริ่มต้นครั้งแรกของตัวละคร {AA0000}%s.", Player_GetName(playerid));
    Send_Message(playerid, "หากคุณพบปัญหาเกี่ยวกับระบบโปรดติดต่อนายก นายกจะช่วยเหลือคุณทันที !");
    return 1;
}

Dialog:DIALOG_REGISTER(playerid, response, listitem, inputtext[]) {
    if(!response) return Kick(playerid);
    if(!strlen(inputtext)) return Player_AuthRegister(playerid);
    if(!(6 <= strlen(inputtext) <= 24)) return Player_AuthRegister(playerid);
    bcrypt_hash(playerid,"Account_Register",inputtext,BCRYPT_COST);
    return 1;   
}

Dialog:DIALOG_LOGIN(playerid, response, listitem, inputtext[]) {
    if(!response) return Kick(playerid);
    if(!strlen(inputtext)) return Player_AuthLogin(playerid);
    bcrypt_verify(playerid,"Password_Verify",inputtext,PlayerInfo[playerid][pPassword]);    
    return 1;   
}