static const u8 gText_PreviewUnlocksBuilding[] = _("New HUB Building");
static const u8 gText_PreviewUnlocksAdventureSetting[] = _("New Adventure Options"); 
static const u8 gText_GiveUnlocksShops[] = _("New HUB Unlocks:\nMart & Clothes Shop!");
static const u8 gText_GiveUnlocksSafari[] = _("New HUB Unlocks:\nSafari Zone & Name Rater!");
static const u8 gText_GiveUnlocksTravellingMart[] = _("New HUB Unlocks:\nTravelling Mart!");
static const u8 gText_GiveUnlocksTravellingBank[] = _("New HUB Unlocks:\nBank!");
static const u8 gText_GiveUnlocksBikeShop[] = _("New HUB Unlocks:\nBike Shop!");
static const u8 gText_GiveUnlocksTutors[] = _("New HUB Unlocks:\nMove Tutors!");
static const u8 gText_GiveUnlocksDaycare[] = _("New HUB Unlocks:\nDay Care!");
static const u8 gText_GiveUnlocksBerries[] = _("New HUB Unlocks:\nBerry Patch!");
static const u8 gText_GiveUnlocksBakery[] = _("New HUB Unlocks:\nBakery!");

static const u8 gText_PreviewUnlocksLegendarySafari[] = _("SAFARI Upgrade");
static const u8 gText_GiveLegendarySafari[] = _("Legendary POKéMON can\nnow appear in the Safari Zone!");
static const u8 gText_PreviewUnlocksBikeShop[] = _("Bike Shop Upgrade");
static const u8 gText_GiveLegendaryBikeShop[] = _("You can now collect and carry\nboth Bikes!");
static const u8 gText_PreviewUnlocksMrRandoStart[] = _("Mr. Randoman will now offer a free\ntrade at the start of Adventures!");

const struct RogueQuestConstants gRogueQuests[QUEST_CAPACITY + 1] = 
{
    [QUEST_NONE] = 
    {
        .title = _("-"),
        .desc = _("-"),
        .flags = QUEST_FLAGS_NONE
    },

    [QUEST_FirstAdventure] = 
    {
        .title = _("To adventure!"),
        .desc = _(
                    "Embark on your first\n"
                    "adventure!"
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 3 } },
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 1000 } },
            { .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksBuilding, .giveText=gText_GiveUnlocksShops },
        },
        .unlockedQuests = { 
            QUEST_Collector1,
            QUEST_ShoppingSpree,
            QUEST_NoFainting1,
            QUEST_MrRandoman,
            QUEST_BerryCollector
        }
    },

    [QUEST_Gym1] = 
    {
        .title = _("BADGE 1"),
        .desc = _(
                    "Defeat the 1st GYM."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 1 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_HM01_CUT, 1 } },
        }
    },
    [QUEST_Gym2] = 
    {
        .title = _("BADGE 2"),
        .desc = _(
                    "Defeat the 2nd GYM."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 1 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_HM05_FLASH, 1 } },
        }
    },
    [QUEST_Gym3] = 
    {
        .title = _("BADGE 3"),
        .desc = _(
                    "Defeat the 3rd GYM."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 1 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_HM06_ROCK_SMASH, 1 } },
        }
    },
    [QUEST_Gym4] = 
    {
        .title = _("BADGE 4"),
        .desc = _(
                    "Defeat the 4th GYM."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 1 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_HM04_STRENGTH, 1 } },
        }
    },
    [QUEST_Gym5] = 
    {
        .title = _("BADGE 5"),
        .desc = _(
                    "Defeat the 5th GYM."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 1 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_HM08_DIVE, 1 } },
        }
    },
    [QUEST_Gym6] = 
    {
        .title = _("BADGE 6"),
        .desc = _(
                    "Defeat the 6th GYM."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 1 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_HM02_FLY, 1 } },
        }
    },
    [QUEST_Gym7] = 
    {
        .title = _("BADGE 7"),
        .desc = _(
                    "Defeat the 7th GYM."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 1 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_HM07_WATERFALL, 1 } },
        }
    },
    [QUEST_Gym8] = 
    {
        .title = _("BADGE 8"),
        .desc = _(
                    "Defeat the 8th GYM."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 1 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_HM03_SURF, 1 } },
        }
    },

    [QUEST_GymChallenge] = 
    {
        .title = _("Gym Challenge"),
        .desc = _(
                    "Defeat the first 4\n"
                    "GYM leaders."
                ),
        .flags = QUEST_FLAGS_REPEATABLE,
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 2 } },
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 2500 } } 
        }
    },
    [QUEST_GymMaster] = 
    {
        .title = _("Gym Master"),
        .desc = _(
                    "Defeat all 8\n"
                    "GYM leaders."
                ),
        .flags = QUEST_FLAGS_REPEATABLE,
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 2 } },
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 2500 } } 
        },
        .unlockedQuests = { 
            QUEST_Electric_Master, 
            QUEST_Electric_Champion
        }
    },
    [QUEST_EliteMaster] = 
    {
        .title = _("Elite Master"),
        .desc = _(
                    "Defeat all of the\n"
                    "Elite 4."
                ),
        .flags = QUEST_FLAGS_REPEATABLE,
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 2 } },
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 5000 } } 
        }
    },
    [QUEST_Champion] = 
    {
        .title = _("The Champion"),
        .desc = _(
                    "Defeat the final\n"
                    "champion."
                ),
        .flags = QUEST_FLAGS_REPEATABLE,
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 3 } },
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 15000 } } 
        }
    },

    [QUEST_Collector1] = 
    {
        .title = _("Collector"),
        .desc = _(
                    "Fill out 15 POKéDEX\n"
                    "entries."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 500 } },
            { .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksBuilding, .giveText=gText_GiveUnlocksSafari },
        },
        .unlockedQuests = { 
            QUEST_Collector2, 
        }
    },
    [QUEST_Collector2] = 
    {
        .title = _("Collector +"),
        .desc = _(
                    "Defeat the Elite 4\n"
                    "with a Legendary POKéMON\n"
                    "in your party."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 500 } },
            { .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksLegendarySafari, .giveText=gText_GiveLegendarySafari },
        }
    },

    [QUEST_ShoppingSpree] = 
    {
        .title = _("Shop Spree"),
        .desc = _(
                    "Spend at least ¥20000\n"
                    "In a single visit to any\n"
                    "REST STOP."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_AMULET_COIN, 1 } },
            { .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksBuilding, .giveText=gText_GiveUnlocksTravellingMart },
        },
        .unlockedQuests = { 
            QUEST_BigSaver,
            QUEST_Bike1,
        }
    },
    [QUEST_BigSaver] = 
    {
        .title = _("Big Saver"),
        .desc = _(
                    "Exit a REST STOP with\n"
                    "¥50000 in your pocket."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_STAR_PIECE, 1 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_SITRUS_BERRY, 3 } },
            { .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksBuilding, .giveText=gText_GiveUnlocksTravellingBank },
        }
    },
    
    [QUEST_Bike1] = 
    {
        .title = _("Can't Stop!"),
        .desc = _(
                    "Exit a Tough Route within\n"
                    "2 minutes of entering."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 500 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_TAMATO_BERRY, 3 } },
            { .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksBuilding, .giveText=gText_GiveUnlocksBikeShop },
        },
        .unlockedQuests = { 
            QUEST_Bike2, 
        }
    },
    [QUEST_Bike2] = 
    {
        .title = _("Wont't Stop!"),
        .desc = _(
                    "After collecting the 8th\n"
                    "GYM badge, exit a Tough\n"
                    "Route within 1 minute."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 500 } },
            { .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksBikeShop, .giveText=gText_GiveLegendaryBikeShop },
        }
    },

    [QUEST_NoFainting1] = 
    {
        .title = _("Care Tactics"),
        .desc = _(
                    "Defeat 4 GYM leaders\n"
                    "without your PARTNER\n"
                    "POKéMON fainting."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 500 } },
            { .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksBuilding, .giveText=gText_GiveUnlocksTutors },
        },
        .unlockedQuests = { 
            QUEST_NoFainting2,
            QUEST_NoFainting3,
        }
    },
    [QUEST_NoFainting2] = 
    {
        .title = _("Smart Tactics"),
        .desc = _(
                    "Defeat all 8 GYM\n"
                    "leaders without any\n"
                    "POKéMON fainting."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 500 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_KELPSY_BERRY, 3 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_HONDEW_BERRY, 3 } },
            //{ .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksBuilding, .giveText=gText_GiveUnlocksDaycare },
        }
    },
    [QUEST_NoFainting3] = 
    {
        .title = _("True Tactics"),
        .desc = _(
                    "Win a full Run\n"
                    "without any POKéMON\n"
                    "ever fainting."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 20000 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_LUM_BERRY, 3 } }
        }
    },

    [QUEST_MrRandoman] = 
    {
        .title = _("Mr. Randoman"),
        .desc = _(
                    "Trade a POKéMON with\n"
                    "Mr. Randoman."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 500 } },
            { .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksAdventureSetting, .giveText=gText_PreviewUnlocksMrRandoStart },
        },
        .unlockedQuests = { 
            QUEST_ChaosChampion,
        }
    },
    [QUEST_ChaosChampion] = 
    {
        .title = _("Chaos Champ"),
        .desc = _(
                    "Win a full Run, always\n"
                    "doing a Random Party\n"
                    "Trade whenever possible."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 20000 } }
        }
    },

    [QUEST_BerryCollector] = 
    {
        .title = _("Forager"),
        .desc = _(
                    "Collect 10 different types\n"
                    "of Berry whilst on an\n"
                    "Adventure."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 500 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_ORAN_BERRY, 3 } },
            { .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksBuilding, .giveText=gText_GiveUnlocksBerries },
        },
        .unlockedQuests = { 
            QUEST_DenExplorer
        }
    },

    [QUEST_DenExplorer] = 
    {
        .title = _("Wild Den"),
        .desc = _(
                    "Capture a POKéMON from\n"
                    "a Wild Den."
                ),
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_QUALOT_BERRY, 3 } },
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_GREPA_BERRY, 3 } },
            { .type=QUEST_REWARD_CUSTOM_TEXT, .previewText=gText_PreviewUnlocksBuilding, .giveText=gText_GiveUnlocksBakery },
        }
    },

    [QUEST_Electric_Master] = 
    {
        .title = _("Elec Master"),
        .desc = _(
                    "Only use Electric POKéMON\n"
                    "during an adventure and\n"
                    "defeat all GYM Leaders."
                ),
        .flags = QUEST_FLAGS_NONE,
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 1 } } ,
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_POTION, 1 } } ,
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 15020 } } 
        }
    },
    [QUEST_Electric_Champion] = 
    {
        .title = _("Elec Champion"),
        .desc = _(
                    "Only use Electric POKéMON\n"
                    "during an adventure and\n"
                    "defeat the Final Champion."
                ),
        .flags = QUEST_FLAGS_NONE,
        .rewards = { 
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_RARE_CANDY, 1 } } ,
            { .type=QUEST_REWARD_GIVE_ITEM, .params={ ITEM_POTION, 1 } } ,
            { .type=QUEST_REWARD_GIVE_MONEY, .params={ 15020 } } 
        }
    },
};