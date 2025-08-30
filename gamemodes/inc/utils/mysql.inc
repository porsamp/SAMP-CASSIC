#include    <YSI_Coding\y_hooks>

new 
    MySQL:dbCon
;

hook OnGameModeInit() {
    Credit();
    MySQL_Conn();
    return 1;
}

hook OnGameModeExit() {
    MySQL_Disconn();
    return 1;
}

MySQL_Conn() {
    new 
        MySQLOpt:options = mysql_init_options()
    ;

    mysql_log(ERROR | WARNING);
    mysql_set_option(options, AUTO_RECONNECT, true);
    mysql_set_option(options, POOL_SIZE, 12);
    mysql_set_option(options, SERVER_PORT, 3306);
    mysql_global_options(DUPLICATE_CONNECTION_WARNING, false);
    dbCon = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB);
    printfc(LIGHTBLUE, "[Cassic:SQL] -> USER:(%s) DATABAS:(%s)", MYSQL_USER, MYSQL_DB);
    if(mysql_errno(dbCon)) return printc(RED,"[RedEyes:SQL] -> Connection failed! Please check the connection settings...");
    mysql_set_charset("tis620",dbCon);   

    printc(GREEN,"[Cassic:SQL] -> Connection is successful!");

    /*------------------------------------------*/
    MySQL_SystemLoad();
    return 1; 
}

MySQL_Disconn() {
    if(dbCon) mysql_close(dbCon);
    return 1;
}

MySQL_SystemLoad() {
    /* ------------------ */
    // LoadDataFaction();
    return 1;
}

stock MySQL_Stat() {
    new 
        stats[150]
    ;

    mysql_stat(stats, sizeof(stats), dbCon);
    print(stats);
    return 1;
}

stock MySQL_UpdatePlayerInfoInt(playerid, const field[], data) {
    new
        query[128]
    ;

    mysql_format(dbCon, query, sizeof(query), "UPDATE `userdata` SET `%e` = '%d' WHERE `id` = '%d'", field, data, PlayerInfo[playerid][pUID]);
    mysql_tquery(dbCon, query);
    return 1;
}

stock MySQL_UpdatePlayerInfoFloat(playerid, const field[], Float:data) {
    new
        query[128]
    ;

    mysql_format(dbCon, query, sizeof(query), "UPDATE `userdata` SET `%e` = '%.4f' WHERE `id` = '%d'", field, data, PlayerInfo[playerid][pUID]);
    mysql_tquery(dbCon, query);
    return 1;
}

stock MySQL_UpdatePlayerInfoStr(playerid, const field[], const data[]) {
    new
        query[128]
    ;

    mysql_format(dbCon, query, sizeof(query), "UPDATE `userdata` SET `%e` = '%s' WHERE `id` = '%d'", field, data, PlayerInfo[playerid][pUID]);
    mysql_tquery(dbCon, query);
    return 1;
}