#include "global.h"
#include "constants/battle.h"
#include "constants/battle_frontier.h"
#include "constants/items.h"
#include "constants/rogue.h"

#include "event_data.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "item_menu.h"
#include "main.h"
#include "money.h"
#include "overworld.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "script.h"
#include "shop.h"
#include "sound.h"
#include "string_util.h"

#include "rogue.h"
#include "rogue_adventurepaths.h"
#include "rogue_assistant.h"
#include "rogue_baked.h"
#include "rogue_campaign.h"
#include "rogue_controller.h"
#include "rogue_charms.h"
#include "rogue_followmon.h"
#include "rogue_hub.h"
#include "rogue_safari.h"
#include "rogue_script.h"
#include "rogue_timeofday.h"
#include "rogue_trainers.h"
#include "rogue_multiplayer.h"
#include "rogue_player_customisation.h"
#include "rogue_pokedex.h"
#include "rogue_popup.h"
#include "rogue_query.h"
#include "rogue_quest.h"
#include "rogue_questmenu.h"
#include "rogue_settings.h"

void DoSpecialTrainerBattle(void);

bool8 Rogue_CheckPartyHasRoomForMon(void)
{
    if(Rogue_IsRunActive())
    {
        u8 partySize = Rogue_GetMaxPartySize();

        // We don't actually want to shift around the party as that can cause issue in EX
        // Where stuff like megas keep track of exact indicies which mega evolve
        //RemoveAnyFaintedMons(FALSE);

        if (CalculatePlayerPartyCount() >= partySize)
        {
            return FALSE;
        }
    }

    return TRUE;
}

void Rogue_SeedRandomGenerators(void)
{
    SeedRng(gMain.vblankCounter1);
    SeedRng2(gMain.vblankCounter2);
}

u16 GetStartDifficulty(void);

static u8 Calc_RandomTradeLevel(struct Pokemon* mon)
{
    if(gRogueRun.enteredRoomCounter == 0)
    {
        u16 startDifficulty = GetStartDifficulty();

        if(startDifficulty == 0)
            return 7;
        else
            return 5 + GetStartDifficulty() * 10;
    }
    else
        return GetMonData(mon, MON_DATA_LEVEL);
}

void Rogue_RandomisePartyMon(void)
{
    u16 species;
    u32 temp;
    u8 monIdx = gSpecialVar_0x8004;
    u8 targetlevel = Calc_RandomTradeLevel(&gPlayerParty[0]);

    RogueMonQuery_Begin();
    RogueMonQuery_Reset(QUERY_FUNC_INCLUDE);

    if(Rogue_GetCurrentDifficulty() < ROGUE_GYM_MID_DIFFICULTY)
        RogueMonQuery_IsLegendary(QUERY_FUNC_EXCLUDE);

    RogueMonQuery_TransformIntoEggSpecies();
    RogueMonQuery_TransformIntoEvos(targetlevel, TRUE, TRUE);

    // Remove random entries until we can safely calcualte weights without going over
    while(RogueWeightQuery_IsOverSafeCapacity())
    {
        RogueMiscQuery_FilterByChance(Random(), QUERY_FUNC_INCLUDE, 50);
    }

    RogueWeightQuery_Begin();
    {
        // we can have dupes but just not as common
        RogueWeightQuery_FillWeights(20);

        if(monIdx == 255)
        {
            // Entire team
            u8 i;

            for(i = 0; i < gPlayerPartyCount; ++i)
            {
                targetlevel = Calc_RandomTradeLevel(&gPlayerParty[i]);
                temp = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);

                species = RogueWeightQuery_SelectRandomFromWeightsWithUpdate(Random(), 1);

                ZeroMonData(&gPlayerParty[i]);
                CreateMon(&gPlayerParty[i], species, targetlevel, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);

                SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &temp);

                temp = ITEM_SAFARI_BALL;
                SetMonData(&gPlayerParty[i], MON_DATA_POKEBALL, &temp);

                GetSetPokedexSpeciesFlag(species, IsMonShiny(&gPlayerParty[i]) ? FLAG_GET_CAUGHT_SHINY : FLAG_GET_CAUGHT);
            }
        }
        else
        {
            // Single mon in team
            targetlevel = Calc_RandomTradeLevel(&gPlayerParty[monIdx]);
            temp = GetMonData(&gPlayerParty[monIdx], MON_DATA_HELD_ITEM);

            species = RogueWeightQuery_SelectRandomFromWeightsWithUpdate(Random(), 1);

            ZeroMonData(&gPlayerParty[monIdx]);
            CreateMon(&gPlayerParty[monIdx], species, targetlevel, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);

            SetMonData(&gPlayerParty[monIdx], MON_DATA_HELD_ITEM, &temp);

            temp = ITEM_SAFARI_BALL;
            SetMonData(&gPlayerParty[monIdx], MON_DATA_POKEBALL, &temp);

            GetSetPokedexSpeciesFlag(species, IsMonShiny(&gPlayerParty[monIdx]) ? FLAG_GET_CAUGHT_SHINY : FLAG_GET_CAUGHT);
        }
    }
    RogueWeightQuery_End();

    RogueMonQuery_End();
}

void Rogue_AlterMonIVs(void)
{
    const u16 delta = 10;

    u16 statId;
    u16 ivAmount;
    u16 monIdx = gSpecialVar_0x8004;
    u16 statOp = gSpecialVar_0x8005;

    if(monIdx == 255)
    {
        // Entire team
        u8 i;

        for(i = 0; i < gPlayerPartyCount; ++i)
        {
            for(statId = MON_DATA_HP_IV; statId <= MON_DATA_SPDEF_IV; ++statId)
            {
                ivAmount = GetMonData(&gPlayerParty[i], statId);

                if(statOp == 0)
                {
                    ivAmount += delta;
                    ivAmount = min(31, ivAmount);
                }
                else
                {
                    if(ivAmount < delta)
                        ivAmount = 0;
                    else
                        ivAmount -= delta;
                }

                SetMonData(&gPlayerParty[i], statId, &ivAmount);
                CalculateMonStats(&gPlayerParty[i]);
            }
        }
    }
    else
    {
        // Modify just 1 mon
        for(statId = MON_DATA_HP_IV; statId <= MON_DATA_SPDEF_IV; ++statId)
        {
            ivAmount = GetMonData(&gPlayerParty[monIdx], statId);

            if(statOp == 0)
            {
                ivAmount += delta;
                ivAmount = min(31, ivAmount);
            }
            else
            {
                if(ivAmount < delta)
                    ivAmount = 0;
                else
                    ivAmount -= delta;
            }

            SetMonData(&gPlayerParty[monIdx], statId, &ivAmount);
            CalculateMonStats(&gPlayerParty[monIdx]);
        }
    }
}

void Rogue_ApplyStatusToMon(void)
{
    u32 statusAilment = 0;
    u16 monIdx = gSpecialVar_0x8004;

    switch(gSpecialVar_0x8005)
    {
        case 0:
            statusAilment = STATUS1_POISON;
            break;

        case 1:
            statusAilment = STATUS1_PARALYSIS;
            break;

        case 2:
            statusAilment = STATUS1_SLEEP;
            break;

        case 3:
            statusAilment = STATUS1_FREEZE;
            break;

        case 4:
            statusAilment = STATUS1_BURN;
            break;
    }

    if(monIdx == 255)
    {
        // Entire team
        u8 i;

        for(i = 0; i < gPlayerPartyCount; ++i)
        {
            SetMonData(&gPlayerParty[i], MON_DATA_STATUS, &statusAilment);
        }
    }
    else
    {
        SetMonData(&gPlayerParty[monIdx], MON_DATA_STATUS, &statusAilment);
    }
}

void Rogue_ReleaseMonInSlot(void)
{
    u16 monIdx = gSpecialVar_0x8004;

    if(monIdx < gPlayerPartyCount)
    {
        RemoveMonAtSlot(monIdx, TRUE, TRUE, TRUE);
    }
}

void Rogue_ReleaseMonInSlot_NoLabBuffering(void)
{
    u16 monIdx = gSpecialVar_0x8004;

    if(monIdx < gPlayerPartyCount)
    {
        RemoveMonAtSlot(monIdx, TRUE, TRUE, FALSE);
    }
}

u16 Rogue_CalcMaxPartySize(void)
{
    return Rogue_GetMaxPartySize();
}

u16 Rogue_GetMonEvoCount(void)
{
    u16 monIdx = gSpecialVar_0x8004;
    u16 species = GetMonData(&gPlayerParty[monIdx], MON_DATA_SPECIES);

    if(species != SPECIES_NONE)
    {
        u16 e;
        struct Evolution evo;
        u8 evoCount = Rogue_GetMaxEvolutionCount(species);
        u16 count = 0;

        for (e = 0; e < evoCount; e++)
        {
            Rogue_ModifyEvolution(species, e, &evo);
            Rogue_ModifyEvolution_ApplyCurses(species, e, &evo);

            if (evo.targetSpecies != SPECIES_NONE)
            {
                ++count;
            }
        }

        return count;
    }

    return 0;
}

void Rogue_GetMonEvoParams(void)
{
    u16 monIdx = gSpecialVar_0x8004;
    u16 evoIdx = gSpecialVar_0x8005;
    u16 species = GetMonData(&gPlayerParty[monIdx], MON_DATA_SPECIES);

    gSpecialVar_0x8006 = 0;
    gSpecialVar_0x8007 = 0;

    if(species != SPECIES_NONE)
    {        
        // evoIdx doesn't mean array idx annoyingly as evos can be toggled/changed
        u16 e;
        struct Evolution evo;
        u8 evoCount = Rogue_GetMaxEvolutionCount(species);
        u16 count = 0;

        for (e = 0; e < evoCount; e++)
        {
            Rogue_ModifyEvolution(species, e, &evo);
            Rogue_ModifyEvolution_ApplyCurses(species, e, &evo);

            if (evo.targetSpecies != SPECIES_NONE)
            {
                if(count++ == evoIdx)
                {
                    gSpecialVar_0x8006 = evo.method;
                    gSpecialVar_0x8007 = evo.param;
                    return;
                }
            }
        }
    }
}

void RogueDebug_FillGenPC(void)
{
#ifdef ROGUE_DEBUG
    u8 i;
    u16 species;
    u16 writeIdx = 0;
    u16 genId = gSpecialVar_0x8004;

    for(species = SPECIES_NONE + 1; species < NUM_SPECIES; ++species)
    {
        if(SpeciesToGen(species) == genId)
        {
            struct Pokemon mon;
            u16 currIdx = writeIdx++;
            u16 targetBox = currIdx / IN_BOX_COUNT;
            u16 boxIndex = currIdx % IN_BOX_COUNT;

            GetSetPokedexSpeciesFlag(species, FLAG_SET_SEEN);
            GetSetPokedexSpeciesFlag(species, FLAG_SET_CAUGHT);

            CreateMon(&mon, species, 95, MAX_PER_STAT_IVS, FALSE, 0, OT_ID_RANDOM_NO_SHINY, 0);

            SetBoxMonAt(targetBox, boxIndex, &mon.box);
        }
    }

    // Clear a box of space
    for(i = 0; i < IN_BOX_COUNT; ++i)
    {
        u16 currIdx = writeIdx++;
        u16 targetBox = currIdx / IN_BOX_COUNT;
        u16 boxIndex = currIdx % IN_BOX_COUNT;
        ZeroBoxMonAt(targetBox, boxIndex);
    }
#endif
}

void RogueDebug_ClearQuests(void)
{
#ifdef ROGUE_DEBUG
    RogueQuest_OnNewGame();
#endif
}

void Debug_RogueQuest_CompleteQuest(u16 questId);

void RogueDebug_CompleteAvaliableQuests(void)
{
#ifdef ROGUE_DEBUG
    u16 i;

    for(i = 0; i < QUEST_ID_COUNT; ++i)
    {
        if(RogueQuest_IsQuestUnlocked(i))
        {
            if(!RogueQuest_GetStateFlag(i, QUEST_STATE_HAS_COMPLETE))
                Debug_RogueQuest_CompleteQuest(i);
        }
    }
#endif
}

void RogueDebug_CollectAllQuests(void)
{
#ifdef ROGUE_DEBUG
    u16 i;

    for(i = 0; i < QUEST_ID_COUNT; ++i)
    {
        if(RogueQuest_HasPendingRewards(i))
        {
            RogueQuest_TryCollectRewards(i);
        }
    }
#endif
}

void RogueDebug_StartBattle(void)
{
#ifdef ROGUE_DEBUG
    u16 i;

    for(i = 0; i < PARTY_SIZE; ++i)
    {
        ZeroMonData(&gEnemyParty[i]);
        BoxMonAtToMon(TOTAL_BOXES_COUNT- 1, i, &gEnemyParty[i]);
    }

    CalculateEnemyPartyCount();

    gSpecialVar_0x8004 = SPECIAL_BATTLE_AUTOMATION;
    DoSpecialTrainerBattle();
#endif
}

void Rogue_ShowNewQuests()
{
    Rogue_OpenQuestMenu(CB2_ReturnToFieldContinueScript, FALSE);
}

void Rogue_QuestCollectNextReward()
{
    // 0 - Nothing to collect
    // 1 - Success
    // 2 - Cannot give reward
    u16 questId;

    for(questId = 0; questId < QUEST_ID_COUNT; ++questId)
    {
        if(RogueQuest_HasPendingRewards(questId))
        {
            if(RogueQuest_TryCollectRewards(questId))
                gSpecialVar_Result = 1;
            else
                gSpecialVar_Result = 2;
            return;
        }
    }

    gSpecialVar_Result = 0;
}

void Rogue_HasAnyNewQuests()
{
    gSpecialVar_Result = RogueQuest_HasPendingNewQuests();
}

void Rogue_DetermineItemPickupCount()
{
    u16 itemId = gSpecialVar_0x8001;
    gSpecialVar_0x8002 = Rogue_ModifyItemPickupAmount(itemId, 1);
}

enum BerryTreatBuff
{
    BERRY_BUFF_FRIEND,
    BERRY_BUFF_HP,
    BERRY_BUFF_ATK,
    BERRY_BUFF_DEF,
    BERRY_BUFF_SPD,
    BERRY_BUFF_SPATK,
    BERRY_BUFF_SPDEF,
    BERRY_BUFF_WEAKEN,
};

static u8 BerryItemToTreatBuff(u16 berryItem)
{
    switch(berryItem)
    {
        case ITEM_ORAN_BERRY:
        case ITEM_SITRUS_BERRY:
        case ITEM_POMEG_BERRY:
            return BERRY_BUFF_HP;

        case ITEM_KELPSY_BERRY:
            return BERRY_BUFF_ATK;

        case ITEM_QUALOT_BERRY:
            return BERRY_BUFF_DEF;

        case ITEM_HONDEW_BERRY:
            return BERRY_BUFF_SPATK;

        case ITEM_GREPA_BERRY:
            return BERRY_BUFF_SPDEF;

        case ITEM_TAMATO_BERRY:
        case ITEM_SALAC_BERRY:
            return BERRY_BUFF_SPD;

        case ITEM_LEPPA_BERRY:
            return BERRY_BUFF_WEAKEN;

#ifdef ROGUE_EXPANSION
        case ITEM_LIECHI_BERRY:
            return BERRY_BUFF_ATK;

        case ITEM_GANLON_BERRY:
            return BERRY_BUFF_DEF;

        case ITEM_PETAYA_BERRY:
            return BERRY_BUFF_SPATK;

        case ITEM_APICOT_BERRY:
            return BERRY_BUFF_SPDEF;
#endif
        default:
            return BERRY_BUFF_FRIEND;
    }
}

void Rogue_CheckBerryTreat(void)
{
    u16 itemId = gSpecialVar_ItemId;
    gSpecialVar_Result = BerryItemToTreatBuff(itemId);
}

void Rogue_ApplyBerryTreat(void)
{
    u16 monIdx = gSpecialVar_0x8004;
    u16 itemId = gSpecialVar_ItemId;
    u16 buffAmount = gSpecialVar_0x8005;
    u16 berryBuff = BerryItemToTreatBuff(itemId);

    if(berryBuff == BERRY_BUFF_FRIEND)
    {
        u16 friendship = GetMonData(&gPlayerParty[monIdx], MON_DATA_FRIENDSHIP);

        if(friendship < MAX_FRIENDSHIP)
        {
            gSpecialVar_Result = TRUE;

            friendship += buffAmount * 10;
            if(friendship > MAX_FRIENDSHIP)
                friendship = MAX_FRIENDSHIP;

            SetMonData(&gPlayerParty[monIdx], MON_DATA_FRIENDSHIP, &friendship);
            CalculateMonStats(&gPlayerParty[monIdx]);
        }
        else
        {
            gSpecialVar_Result = FALSE;
        }
    }
    else if(berryBuff == BERRY_BUFF_WEAKEN)
    {
        u16 statOffset;

        gSpecialVar_Result = FALSE;

        for(statOffset = 0; statOffset < 6; ++statOffset)
        {
            u16 ivCount = GetMonData(&gPlayerParty[monIdx], MON_DATA_HP_IV + statOffset);

            if(ivCount != 0)
            {
                gSpecialVar_Result = TRUE;

                if(ivCount < buffAmount)
                    ivCount = 0;
                else
                    ivCount -= buffAmount;

                SetMonData(&gPlayerParty[monIdx], MON_DATA_HP_IV + statOffset, &ivCount);
            }
        }

        CalculateMonStats(&gPlayerParty[monIdx]);
    }
    else
    {
        u16 statOffset = berryBuff - 1;
        u16 ivCount = GetMonData(&gPlayerParty[monIdx], MON_DATA_HP_IV + statOffset);

        if(ivCount < 31)
        {
            gSpecialVar_Result = TRUE;

            ivCount += buffAmount;
            if(ivCount > 31)
                ivCount = 31;

            SetMonData(&gPlayerParty[monIdx], MON_DATA_HP_IV + statOffset, &ivCount);
            CalculateMonStats(&gPlayerParty[monIdx]);
        }
        else
        {
            gSpecialVar_Result = FALSE;
        }
    }
}

void Rogue_ChangeMonBall(void)
{
    u16 monIdx = gSpecialVar_0x8004;
    u16 itemId = gSpecialVar_ItemId;

    SetMonData(&gPlayerParty[monIdx], MON_DATA_POKEBALL, &itemId);
}

void Rogue_AnyNewQuestsPending(void)
{
    gSpecialVar_Result = AnyNewQuestsPending();
}

void Rogue_BufferLabMonName(void)
{
    u16 index = gSpecialVar_0x8002;
    Rogue_CopyLabEncounterMonNickname(index, gStringVar1);
}

void Rogue_GiveLabMon(void)
{
    u16 index = gSpecialVar_0x8002;
    gSpecialVar_Result = Rogue_GiveLabEncounterMon(index);
}

void Rogue_ChooseMiniBossRewardMons(void)
{
    Rogue_SelectMiniBossRewardMons();
}

void Rogue_ClearCharmsAndCurses(void)
{
    Rogue_RemoveCharmsFromBag();
    Rogue_RemoveCursesFromBag();
}

void Rogue_IsRoamerActive(void)
{
    gSpecialVar_Result = gRogueRun.wildEncounters.roamer.species != SPECIES_NONE;
}

void Rogue_BufferRoamerName(void)
{
    StringCopyN(gStringVar1, RoguePokedex_GetSpeciesName(gRogueRun.wildEncounters.roamer.species), POKEMON_NAME_LENGTH);
}

void Rogue_GetUnlockedCampaignCount(void)
{
    u16 i;
    u16 count = 0;

    for(i = ROGUE_CAMPAIGN_FIRST; i <= ROGUE_CAMPAIGN_LAST; ++i)
    {
        if(gRogueSaveBlock->campaignData[i - ROGUE_CAMPAIGN_FIRST].isUnlocked)
            ++count;
    }

    gSpecialVar_Result = count;
}

void Rogue_GetNextUnlockedCampaign(void)
{
    u16 i = gSpecialVar_0x8004;

    if(i == ROGUE_CAMPAIGN_NONE)
        i = ROGUE_CAMPAIGN_FIRST;
    else
        ++i;

    for(; i <= ROGUE_CAMPAIGN_LAST; ++i)
    {
        if(gRogueSaveBlock->campaignData[i - ROGUE_CAMPAIGN_FIRST].isUnlocked)
        {
            gSpecialVar_0x8004 = i;
            return;
        }
    }

    gSpecialVar_0x8004 = ROGUE_CAMPAIGN_NONE;
}

void Rogue_GetCampaignHighScore(void)
{
    u16 i = VarGet(VAR_ROGUE_DESIRED_CAMPAIGN);

    if(i != ROGUE_CAMPAIGN_NONE)
    {
        if(gRogueSaveBlock->campaignData[i - ROGUE_CAMPAIGN_FIRST].isCompleted)
        {
            gSpecialVar_Result = gRogueSaveBlock->campaignData[i - ROGUE_CAMPAIGN_FIRST].bestScore;
            return;
        }
    }

    gSpecialVar_Result = 0;
    return;
}

void Rogue_BufferCampaignName(void)
{
    StringCopy(&gStringVar1[0], GetCampaignTitle(VarGet(VAR_ROGUE_DESIRED_CAMPAIGN)));
}

static bool8 CheckSpeciesCombo(u16 speciesCheckA, u16 speciesCheckB, u16 speciesTargetA, u16 speciesTargetB)
{
    return (speciesCheckA == speciesTargetA && speciesCheckB == speciesTargetB)
        || (speciesCheckB == speciesTargetA && speciesCheckA == speciesTargetB);
}

static u16 GetSpeciesComboOutput(u16 speciesA, u16 speciesB, bool8 getItem)
{
    //if(CheckSpeciesCombo(speciesA, speciesB, SPECIES_EEVEE, SPECIES_CHARMANDER))
    //    return getItem ? ITEM_PECHA_BERRY : SPECIES_ABSOL;

#ifdef ROGUE_EXPANSION
    if(CheckSpeciesCombo(speciesA, speciesB, SPECIES_KYUREM, SPECIES_RESHIRAM))
        return getItem ? ITEM_DNA_SPLICERS : SPECIES_KYUREM_WHITE;

    if(CheckSpeciesCombo(speciesA, speciesB, SPECIES_KYUREM, SPECIES_ZEKROM))
        return getItem ? ITEM_DNA_SPLICERS : SPECIES_KYUREM_BLACK;

    if(CheckSpeciesCombo(speciesA, speciesB, SPECIES_ZYGARDE_COMPLETE, SPECIES_ZYGARDE_COMPLETE))
        return getItem ? ITEM_ZYGARDE_CUBE : SPECIES_ZYGARDE_10;

    if(CheckSpeciesCombo(speciesA, speciesB, SPECIES_ZYGARDE_10, SPECIES_ZYGARDE_10))
        return getItem ? ITEM_ZYGARDE_CUBE : SPECIES_ZYGARDE;

    if(CheckSpeciesCombo(speciesA, speciesB, SPECIES_ZYGARDE, SPECIES_ZYGARDE))
        return getItem ? ITEM_ZYGARDE_CUBE : SPECIES_ZYGARDE_COMPLETE;

    if(CheckSpeciesCombo(speciesA, speciesB, SPECIES_NECROZMA, SPECIES_SOLGALEO))
        return getItem ? ITEM_N_SOLARIZER : SPECIES_NECROZMA_DUSK_MANE;

    if(CheckSpeciesCombo(speciesA, speciesB, SPECIES_NECROZMA, SPECIES_LUNALA))
        return getItem ? ITEM_N_LUNARIZER : SPECIES_NECROZMA_DAWN_WINGS;

    if(CheckSpeciesCombo(speciesA, speciesB, SPECIES_CALYREX, SPECIES_GLASTRIER))
        return getItem ? ITEM_REINS_OF_UNITY : SPECIES_CALYREX_ICE_RIDER;

    if(CheckSpeciesCombo(speciesA, speciesB, SPECIES_CALYREX, SPECIES_SPECTRIER))
        return getItem ? ITEM_REINS_OF_UNITY : SPECIES_CALYREX_SHADOW_RIDER;
#endif

    return 0;
}

void Rogue_CheckMonCombo(void)
{
    u16 speciesA = GetMonData(&gPlayerParty[gSpecialVar_0x8003], MON_DATA_SPECIES);
    u16 speciesB = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPECIES);

    gSpecialVar_0x8005 = GetSpeciesComboOutput(speciesA, speciesB, TRUE);
    gSpecialVar_0x8006 = GetSpeciesComboOutput(speciesA, speciesB, FALSE);
}

void Rogue_ApplyMonCombo(void)
{
    u16 speciesA = GetMonData(&gPlayerParty[gSpecialVar_0x8003], MON_DATA_SPECIES);
    u16 speciesB = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPECIES);
    u16 outputSpecies = GetSpeciesComboOutput(speciesA, speciesB, FALSE);

    if(outputSpecies)
    {
        u8 speciesName[POKEMON_NAME_LENGTH + 1];
        StringCopyN(speciesName, RoguePokedex_GetSpeciesName(outputSpecies), ARRAY_COUNT(speciesName));

        SetMonData(&gPlayerParty[gSpecialVar_0x8003], MON_DATA_SPECIES, &outputSpecies);
        SetMonData(&gPlayerParty[gSpecialVar_0x8003], MON_DATA_NICKNAME, speciesName);
        RemoveMonAtSlot(gSpecialVar_0x8004, TRUE, TRUE, FALSE);

        gSpecialVar_Result = TRUE;
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }
}

void Rogue_GetFollowMonSpecies(void)
{
    u16 species;
    bool8 isShiny;
    u8 spawnSlot;
    FollowMon_GetSpeciesFromLastInteracted(&species, &isShiny, &spawnSlot);

    gSpecialVar_0x800A = species;
    gSpecialVar_0x800B = isShiny;
    gSpecialVar_0x8009 = spawnSlot;
}

void Rogue_SetupFollowMonFromParty(void)
{
    FollowMon_SetGraphicsFromParty();
}

void Rogue_TryEnqueueWildBattleMon(void)
{
    u8 spawnSlot = gSpecialVar_0x8009;
    RogueSafari_EnqueueBattleMon(spawnSlot);
}

void Rogue_GetTrainerNum(void)
{
    u16 trainerNum = Rogue_GetTrainerNumFromLastInteracted();
    if(trainerNum != TRAINER_NONE)
    {
        gSpecialVar_0x8004 = trainerNum;
        gSpecialVar_Result = TRUE;
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }
}

void Rogue_PlayStaticTrainerEncounterBGM(void)
{
    u16 trainerNum = VarGet(VAR_ROGUE_SPECIAL_ENCOUNTER_DATA);

    struct RogueBattleMusic music;
    Rogue_ModifyBattleMusic(BATTLE_MUSIC_TYPE_TRAINER, trainerNum, &music);

    //PlayBGM();
    PlayNewMapMusic(music.encounterMusic);
    //playbgm(MUS_ENCOUNTER_INTENSE, FALSE)
}

void Rogue_PlayRivalTrainerEncounterBGM(void)
{
    u16 trainerNum = gRogueRun.rivalTrainerNum;

    struct RogueBattleMusic music;
    Rogue_ModifyBattleMusic(BATTLE_MUSIC_TYPE_TRAINER, trainerNum, &music);

    //PlayBGM();
    PlayNewMapMusic(music.encounterMusic);
    //playbgm(MUS_ENCOUNTER_INTENSE, FALSE)
}

extern const u8 gPlaceholder_Gym_PreBattleOpenning[];

void Rogue_ShouldSkipTrainerOpenningMsg(void)
{
    // Skip openning text if the message is empty
    const u8* str = Rogue_ModifyFieldMessage(gPlaceholder_Gym_PreBattleOpenning);
    gSpecialVar_Result = (str[0] == 0xFF);
}

void Rogue_EnterPartnerMonCapacity()
{
    gSpecialVar_Result = Rogue_GetStartingMonCapacity();
}

void Rogue_IsMultiplayerActive(void)
{
    gSpecialVar_Result = RogueMP_IsActive();
}

void Rogue_IsMultiplayerHost(void)
{
    gSpecialVar_Result = RogueMP_IsHost();
}

void Rogue_HostMultiplayer()
{
    RogueMP_OpenHost();
    RogueMP_WaitForConnection();
}

void Rogue_JoinMultiplayer()
{
    RogueMP_OpenClient();
    RogueMP_WaitForConnection();
}

void Rogue_CloseMultiplayer()
{
    RogueMP_Close();
    //RogueMP_WaitForConnection();
}

void Rogue_IsRogueAssistantConnected()
{
    gSpecialVar_Result = Rogue_IsAssistantConnected();
}

void Rogue_SetupFollowParterMonObjectEvent()
{
    SetupFollowParterMonObjectEvent();
}

void Rogue_RegisterRideMon()
{
    u16 gfxId = FollowMon_GetMonGraphics(&gPlayerParty[0]);
    VarSet(VAR_ROGUE_REGISTERED_RIDE_MON, gfxId);
}

void Rogue_RunRewardLvls()
{
    gSpecialVar_Result = Rogue_PostRunRewardLvls();
}

void Rogue_RunRewardMoney()
{
    gSpecialVar_Result = Rogue_PostRunRewardMoney();
}

void ReloadWarpSilent()
{
    u8 mapGroup = gSaveBlock1Ptr->location.mapGroup;
    u8 mapNum = gSaveBlock1Ptr->location.mapNum;
    u8 warpId = (u8)-1;
    u16 x = gSaveBlock1Ptr->pos.x;
    u16 y = gSaveBlock1Ptr->pos.y;

    StoreInitialPlayerAvatarStateForReloadWarp();
    SetWarpDestination(mapGroup, mapNum, warpId, x, y);
    DoDiveWarp();
}

void Rogue_SetTimeAndSeason()
{
    u8 tod = min(gSpecialVar_0x8004, TIME_PRESET_COUNT);
    u8 season = min(gSpecialVar_0x8005, SEASON_COUNT);

    if(tod != TIME_PRESET_COUNT || season != SEASON_COUNT)
    {
        RogueToD_SetTimePreset(tod, season);
        gSpecialVar_Result = TRUE;
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }
}

void Popup_CannotTakeItem()
{
    u16 itemId = gSpecialVar_0x8004;
    u16 quantity = gSpecialVar_0x8005;

    Rogue_PushPopup_CannotTakeItem(itemId, quantity);
}

void Popup_NewBadgeGet()
{
    Rogue_PushPopup_NewBadgeGet(Rogue_GetCurrentDifficulty() - 1);
}

u16 Rogue_GetBagCapacityUpgradeLevel()
{
    return gSaveBlock1Ptr->bagCapacityUpgrades;
}

void Rogue_IncreaseBagCapacityUpgradeLevel()
{
    gSpecialVar_Result = FALSE;

    if(gSaveBlock1Ptr->bagCapacityUpgrades < ITEM_BAG_MAX_CAPACITY_UPGRADE)
    {
        ++gSaveBlock1Ptr->bagCapacityUpgrades;
        gSpecialVar_Result = TRUE;
        ShrinkBagItems();
    }
}

void Rogue_BufferBagUpgradeCost()
{
    u32 cost = Rogue_CalcBagUpgradeCost();
    ConvertUIntToDecimalStringN(gStringVar2, cost, STR_CONV_MODE_LEFT_ALIGN, 7);
}

void Rogue_CheckBagUpgradeCost()
{
    u32 cost = Rogue_CalcBagUpgradeCost();
    if(IsEnoughMoney(&gSaveBlock1Ptr->money, cost))
        gSpecialVar_Result = TRUE;
    else
        gSpecialVar_Result = FALSE;
}

void Rogue_RemoveBagUpgradeCost()
{
    u32 cost = Rogue_CalcBagUpgradeCost();
    RemoveMoney(&gSaveBlock1Ptr->money, cost);
}

void Rogue_SeedRng()
{
    SeedRng(gMain.vblankCounter1);
    SeedRng2(gMain.vblankCounter2);
}

void Rogue_CheckHubConnectionDir()
{
    u8 dir;
    u8 checkArea = gSpecialVar_0x8004;
    u8 currentArea = RogueHub_GetAreaFromCurrentMap();

    for(dir = HUB_AREA_CONN_SOUTH; dir < HUB_AREA_CONN_COUNT; ++dir)
    {
        if(RogueHub_FindAreaInDir(currentArea, dir) == checkArea)
        {
            gSpecialVar_Result = dir;
            return;
        }
    }

    gSpecialVar_Result = HUB_AREA_CONN_COUNT;
}

void Rogue_AssignDefaultRegion()
{
    u32 flags = RoguePlayer_GetOutfitTrainerFlags();
    bool8 anySet = FALSE;

    if(flags & TRAINER_FLAG_REGION_KANTO)
    {
        Rogue_SetConfigToggle(CONFIG_TOGGLE_TRAINER_KANTO, TRUE);
        anySet = TRUE;
    }

    if(flags & TRAINER_FLAG_REGION_JOHTO)
    {
        Rogue_SetConfigToggle(CONFIG_TOGGLE_TRAINER_JOHTO, TRUE);
        anySet = TRUE;
    }

    if(flags & TRAINER_FLAG_REGION_HOENN)
    {
        Rogue_SetConfigToggle(CONFIG_TOGGLE_TRAINER_HOENN, TRUE);
        anySet = TRUE;
    }

#ifdef ROGUE_EXPANSION
    if(flags & TRAINER_FLAG_REGION_SINNOH)
    {
        Rogue_SetConfigToggle(CONFIG_TOGGLE_TRAINER_SINNOH, TRUE);
        anySet = TRUE;
    }

    if(flags & TRAINER_FLAG_REGION_UNOVA)
    {
        Rogue_SetConfigToggle(CONFIG_TOGGLE_TRAINER_UNOVA, TRUE);
        anySet = TRUE;
    }

    if(flags & TRAINER_FLAG_REGION_KALOS)
    {
        Rogue_SetConfigToggle(CONFIG_TOGGLE_TRAINER_KALOS, TRUE);
        anySet = TRUE;
    }

    if(flags & TRAINER_FLAG_REGION_ALOLA)
    {
        Rogue_SetConfigToggle(CONFIG_TOGGLE_TRAINER_ALOLA, TRUE);
        anySet = TRUE;
    }

    if(flags & TRAINER_FLAG_REGION_GALAR)
    {
        Rogue_SetConfigToggle(CONFIG_TOGGLE_TRAINER_GALAR, TRUE);
        anySet = TRUE;
    }
#endif

    // Fallback to Kanto for "nostalgia" I guess?
    // (It doesn't matter too much, just need it to not break)
    if(!anySet)
    {
        Rogue_SetConfigToggle(CONFIG_TOGGLE_TRAINER_KANTO, TRUE);
    }

    RoguePokedex_SetDexVariant(POKEDEX_VARIANT_DEFAULT);
}

void Rogue_IsFinalQuestActive()
{
    gSpecialVar_Result = Rogue_UseFinalQuestEffects() != 0;
}

void Rogue_TryScatterPokeblockNearHoneyTree()
{
    if(Rogue_IsRunActive() && gRogueAdvPath.currentRoomType == ADVPATH_ROOM_HONEY_TREE)
    {
        gSpecialVar_Result = Rogue_TryAddHoneyTreePokeblock(VarGet(VAR_ROGUE_ACTIVE_POKEBLOCK));
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }
}

void Rogue_ScatterPokeblockItem()
{
    if(VarGet(VAR_ROGUE_ACTIVE_POKEBLOCK) == ITEM_POKEBLOCK_SHINY)
    {
        // For the shiny pokeblock always scatter it
        gSpecialVar_Result = TRUE;
    }
    else
    {
        u8 type = ItemId_GetSecondaryId(VarGet(VAR_ROGUE_ACTIVE_POKEBLOCK));
        gSpecialVar_Result = Rogue_RerollSingleWildSpecies(type);
    }
}

void Rogue_CanPlantBerries()
{
    gSpecialVar_Result = !Rogue_IsRunActive() || gRogueAdvPath.currentRoomType == ADVPATH_ROOM_RESTSTOP;
}

void Rogue_CheckDaycareCount()
{
    u8 i = 0;
    u8 count = 0;
    struct BoxPokemon* mon;

    for(i = 0; i < DAYCARE_SLOT_COUNT; ++i)
    {
        mon = Rogue_GetDaycareBoxMon(i);
        if(GetBoxMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE)
            ++count;
    }

    gSpecialVar_Result = count;
}

void Rogue_IsDaycareSlotEmpty()
{
    u8 daycareSlot = gSpecialVar_0x8005;
    struct BoxPokemon* mon = Rogue_GetDaycareBoxMon(daycareSlot);
    gSpecialVar_Result = (GetBoxMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE);
}

void Rogue_SwapDaycareMon()
{
    u16 partySlot = gSpecialVar_0x8004;
    u8 daycareSlot = gSpecialVar_0x8005;
    Rogue_SwapMonInDaycare(&gPlayerParty[partySlot], Rogue_GetDaycareBoxMon(daycareSlot));

    // Resetup followmon
    if(partySlot == 0)
        SetupFollowParterMonObjectEvent();
}

void Rogue_HealAlivePlayerParty()
{
    u8 i, j;
    u8 ppBonuses;
    u8 arg[4];

    CalculatePlayerPartyCount();

    // restore HP.
    for(i = 0; i < gPlayerPartyCount; i++)
    {
        u16 currHp = GetMonData(&gPlayerParty[i], MON_DATA_HP);
        if(currHp != 0)
        {
            u16 maxHP = GetMonData(&gPlayerParty[i], MON_DATA_MAX_HP);
            arg[0] = maxHP;
            arg[1] = maxHP >> 8;
            SetMonData(&gPlayerParty[i], MON_DATA_HP, arg);
            ppBonuses = GetMonData(&gPlayerParty[i], MON_DATA_PP_BONUSES);

            // restore PP.
            for(j = 0; j < MAX_MON_MOVES; j++)
            {
                arg[0] = CalculatePPWithBonus(GetMonData(&gPlayerParty[i], MON_DATA_MOVE1 + j), ppBonuses, j);
                SetMonData(&gPlayerParty[i], MON_DATA_PP1 + j, arg);
            }

            // since status is u32, the four 0 assignments here are probably for safety to prevent undefined data from reaching SetMonData.
            arg[0] = 0;
            arg[1] = 0;
            arg[2] = 0;
            arg[3] = 0;
            SetMonData(&gPlayerParty[i], MON_DATA_STATUS, arg);
        }
    }
}