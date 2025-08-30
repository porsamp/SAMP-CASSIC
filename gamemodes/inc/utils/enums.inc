//--------------------------------[ENUMS.PWN]--------------------------------

enum (<<= 1) {
    CMD_PLAYER = 1, // 
    CMD_FACTION, // 
    CMD_AGENCY, // 
    CMD_ADMIN, // 
    CMD_DEV // 
}

enum ePlayerData {
    pUID,
    pPassword[BCRYPT_HASH_LENGTH],
    pLastLogin[24],
    bool:pLoggedIn,

    pGender,
    pBirthday[24],
    pTutorial,

    pAdmin,
    pCMDPermission,

    pSkin,

    Float:pLastPosX,
    Float:pLastPosY,
    Float:pLastPosZ,
    Float:pLastPosA,
    pLastInt,
    pLastVW,

    Float:pLastHP,
    Float:pLastAP,

    pCash,
    pBankAccount,
    pPoint,

    pLeader,
    pMember,
    pRank,

    pDeath,
    pDeathTime,    

    pSelect[200],
    pSelectMax,
    pSelectEdit,
    pSelectSkip,
    pSelect_click,

    bool:pFreeze,
    Timer:pFreezeTimer
};

new PlayerInfo[MAX_PLAYERS][ePlayerData];