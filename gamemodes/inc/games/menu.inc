#include    <YSI_Coding\y_hooks>

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if(newkeys & KEY_ANALOG_LEFT) {
        if(!(PlayerInfo[playerid][pCMDPermission] & CMD_ADMIN)) AuthPlayerAdminMenu(playerid);
    }
    return 1;
}

AuthPlayerAdminMenu(playerid) {
    Dialog_Show(playerid, Dialog_AdminMenu, DIALOG_STYLE_LIST, "RedEyes Control", "\
    Dynamic\
    ", "ตกลง", "ยกเลิก");
    return 1;
}

AuthPlayerAdminMenuDynamic(playerid) {
    Dialog_Show(playerid, Dialog_MenuDynamic, DIALOG_STYLE_LIST, "Dynamic", "\
    Dynamic Faction\n\
    Dynamic Entrance\n\
    Dynamic Gps\n\
    Dynamic Atm\
    ", "ตกลง", "ยกเลิก"); 
    return 1;
}

Dialog:Dialog_AdminMenu(playerid, response, listitem, inputtext[]) {
    if(!response) return 0;

    switch(listitem) {
        case 0: {
            AuthPlayerAdminMenuDynamic(playerid);
        }
    }
    return 1;
}

Dialog:Dialog_MenuDynamic(playerid, response, listitem, inputtext[]) {
    if(!response) return AuthPlayerAdminMenu(playerid);
    switch(listitem) {
        case 0: Faction_List(playerid);
        /* case 1: Faction_List(playerid);
        case 2: Entrance_List(playerid);
        case 3: GPS_List(playerid);
        case 4: Atm_List(playerid); */
    }
    return 1;
}