#include    <YSI_Coding\y_hooks>

timer Timer_AuthTutorial[5500](playerid) {
    AuthPlayerTutorial(playerid);
    return 1;
}

timer Timer_CameraTutorial[3000](playerid) {
    InterpolateCameraPos(playerid, 1701.110473, -2297.639648, 60.035995, 1682.634643, -2289.006835, 15.390919, 5000);
    InterpolateCameraLookAt(playerid, 1701.153564, -2297.206054, 55.055023, 1682.594360, -2284.017089, 15.072607, 5000);

    defer Timer_AuthTutorial(playerid);
    return 1;
}

timer Timer_StartGame[3000](playerid) {
    Player_FirstStartGame(playerid);
    return 1;
}

StartPlayerTutorial(playerid) {
    defer Timer_CameraTutorial(playerid);
    return 1;
}

AuthPlayerTutorial(playerid) {
    static 
        const fmat_gender[3][5] = {
            "-", 
            "ชาย", 
            "หญิง"
        }
    ;

    Dialog_Show(playerid, Dialog_Tutorial, DIALOG_STYLE_TABLIST, "กรอกข้อมูลสำหรับตัวละคร", "\
    เพศ\t|\t%s\n\
    วว/ดด/ปป\t|\t%s\n\
    - ยืนยันข้อมูลทั้งหมด {AA0000}(หากคุณแน่ใจแล้วสามารถกดตกลงเพื่อยืนยันข้อมูล)\
    ", "ตกลง", "ยกเลิก", fmat_gender[PlayerInfo[playerid][pGender]], PlayerInfo[playerid][pBirthday]);
    return 1;
}

AuthPlayerGender(playerid) {
    Dialog_Show(playerid, Dialog_Gender, DIALOG_STYLE_LIST, "เลือกเพศสำหรับตัวละคร", "\
    ชาย\n\
    หญิง\
    ", "ตกลง", "กลับ"); 
    return 1;
}

AuthPlayerBirthday(playerid) {
    Dialog_Show(playerid, Dialog_Birthday, DIALOG_STYLE_INPUT, "กรอกวันเดือนปีเกิดสำหรับตัวละคร", "\
    {FFFFFF}ตัวอย่าง: {AA0000}28/02/1990 {FFFFFF}(ใช้ปีคริสตศักราช เช่น พศ 2533 -> คศ 1990)\n\n\
    {FFFFFF}ป้อนวันเดือนปีเกิดของคุณด้านล่างนี้:\
    ", "ตกลง", "กลับ"); 
    return 1;
}

ConfirmPlayerTutorial(playerid) {
    Send_Message(playerid, "{AA0000}! {FFFFFF}คุณได้กรอกข้อมูลตัวละครเสร็จเรียบร้อย....");
    defer Timer_StartGame(playerid);
    return 1;
}

Dialog:Dialog_Tutorial(playerid, response, listitem, inputtext[]) {
    if(!response) return AuthPlayerTutorial(playerid);

    switch(listitem) {
        case 0: AuthPlayerGender(playerid);
        case 1: AuthPlayerBirthday(playerid);
        case 2: {
            if(!PlayerInfo[playerid][pGender]) return AuthPlayerTutorial(playerid);
            if(!strcmp(PlayerInfo[playerid][pBirthday], "00/00/00", true)) return AuthPlayerTutorial(playerid);

            ConfirmPlayerTutorial(playerid);
        }
    }
    return 1;
}

Dialog:Dialog_Gender(playerid, response, listitem, inputtext[]) {
    if(!response) return AuthPlayerTutorial(playerid);

    PlayerInfo[playerid][pGender] = listitem + 1;
    AuthPlayerTutorial(playerid);
    return 1;
}

Dialog:Dialog_Birthday(playerid, response, listitem, inputtext[]) {
    if(!response) return AuthPlayerTutorial(playerid);

    new
        day[2],
        month[2],
        year[2]
    ;

    getdate(year[0], month[0], day[0]);
    
    if(sscanf(inputtext, "p</>iii", day[1], month[1], year[1])) return AuthPlayerBirthday(playerid);

    new
        check = year[0] - year[1]
    ;

    if(check > 50 || check < 1 || day[1] > GetMonthDays(year[1], month[1]) || day[1] < 1 || month[1] > 12 || month[1] < 1) return AuthPlayerBirthday(playerid);
    if(month[1] > month[0] || (month[1] == month[0] && day[1] > day[0])) check --;

    format(PlayerInfo[playerid][pBirthday], 24, "%1d/%1d/%d", day[1], month[1], year[1]);
    AuthPlayerTutorial(playerid);
    return 1;
}