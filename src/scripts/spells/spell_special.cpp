/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "scriptPCH.h"

#include "QzqstarAchievements.h"
#include "Chat.h"
#include "../custom/qzqstar_id.h"
#include "../custom/qzqstar_custom.h"

// 24340, 26558, 28884 - Meteor
// 26789 - Shard of the Fallen Star
struct MeteorScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            uint32 count = 0;
            for (const auto& ihit : spell->m_UniqueTargetInfo)
                if (ihit.effectMask & (1 << effIdx))
                    ++count;

            if(count < 1) count = 1;
            
            spell->damage /= count; // divide to all targets
        }
        return true;
    }
};

SpellScript* GetScript_Meteor(SpellEntry const*)
{
    return new MeteorScript();
}



// 24934 - Darkmoon Steam Tonk Control Console
struct DarkmoonSteamTonkControlConsoleScript : public SpellScript
{
    void OnInit(Spell* spell) final
    {
        // Unsummon a potential Hunter or Warlock pet when using the Tonk Control Console.
        // Without this, the player will be unable to summon a Tonk but will still be
        // locked in place without being able to move (until relogging).
        if (Player* pPlayer = spell->GetCaster()->ToPlayer())
            pPlayer->UnsummonPetTemporaryIfAny();
    }
};

SpellScript* GetScript_DarkmoonSteamTonkControlConsole(SpellEntry const*)
{
    return new DarkmoonSteamTonkControlConsoleScript();
}

// 24933 - Cannon (Darkmoon Steam Tonk)
struct DarkmoonSteamTonkCannonScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0 && spell->GetUnitTarget())
        {
            spell->m_caster->CastSpell(spell->GetUnitTarget(), 27766, true);
        }
        return true;
    }
};

SpellScript* GetScript_DarkmoonSteamTonkCannon(SpellEntry const*)
{
    return new DarkmoonSteamTonkCannonScript();
}


//add custom spell script for the pet system
struct PetTrigSpellScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            int32 basePoints0 = spell->m_currentBasePoints[0];
            float _multiple = 1.0f;
            //get the player's info from sQZAchievementMgr
            Player* player = spell->GetCaster()->ToPlayer();
            if (player)
            {
               auto petValues = sQZAchievements.GetActivePetInfo(player);

               if(petValues)
               {
                    // A-B-CD-EF
                    // A - Pet Type
                    // B - Pet Level (1-9)
                    // CD - Happiness Points (0-99)
                    // EF - Relationship Points (0-99)
                    // get the pet level and set the multiple
                    int _petLevel = (petValues % 100000) / 10000; // -B----
                    int _petHappiness = (petValues % 10000) / 100; // --CD--
                    int _petRelationship = (petValues % 100); // ----EF

                    if(spell->m_spellInfo->Id == (ZQ_SPELL_PET_TRIGGERED + 6)) // Weapon Damage
                    {
						//weapon damage
                        _multiple = (1.0f + _petLevel / 20.0f) * ( 1.0f + _petHappiness/500.0f + _petRelationship/500.0f) ; // 10% of the pet level
						spell->m_currentBasePoints[0] = spell->m_currentBasePoints[0] * _multiple * frand(0.95, 1.05);
                        //spell->damage = spell->m_currentBasePoints[0] * _multiple * frand(0.9, 1.15);
                    }
                    else {
                        //magic damage
                        _multiple = _petLevel * 2 * ( 1.0f + _petHappiness/100.0f + _petRelationship/100.0f) + _petLevel * _petLevel / 2.0f ; // 10% of the pet level
						spell->damage = basePoints0 * _multiple * frand(0.9, 1.15);
                    }
               }
            }
			

			//sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s bp0:%u _multiple: %f, olddamage = %f", player->GetName(), basePoints0, _multiple, spell->damage);
        } 
        return true;
    }
};

SpellScript* GetScript_PetTrigSpell(SpellEntry const*)
{
    return new PetTrigSpellScript(); 
}


//add pet aura to the player when the pet is summoned
struct PetAuraScript : public AuraScript
{
    void OnBeforeApply(Aura* aura, bool apply) final
    {
        //sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Apply Aura: %u", aura->GetSpellProto()->Id);

        if (apply && aura->GetEffIndex() == EFFECT_INDEX_1)
        {
			//sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Apply Aura1111: %u", aura->GetSpellProto()->Id);
            if (Player* player = aura->GetTarget()->ToPlayer())
            {
               //get the player's pet information from sQZAchievementMgr
               auto petValues = sQZAchievements.GetActivePetInfo(player);
               if(petValues) 
               {
                    // A-B-CD-EF
                    // A - Pet Type
                    // B - Pet Level (1-9)
                    // CD - Happiness Points (0-99)
                    // EF - Relationship Points (0-99)
                    // get the pet level
                    int _petType = petValues / 100000; // A-B----
                    int _petLevel = (petValues % 100000) / 10000; // -B----
                    // get the pet relation points and set the values
                    int _petRelationship = (petValues % 100); // ----EF
                    
                    auto _modifier = aura->GetModifier();

                    //sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Apply aura:%u, amount: %u", _modifier->m_auraname, _modifier->m_amount);

					//sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Pet Values:%u Level:%u, RL: %u", petValues, _petLevel, _petRelationship);
                    if(_modifier)
                    {
						//max is 9 * 99 / 20 about 45 attributes
						//least should be 
						_modifier->m_amount = _petLevel * (_petRelationship/20 + 1);

                    }
               }
            }
        }
    }
};


AuraScript* GetScript_PetTrigAura(SpellEntry const*)
{
    return new PetAuraScript(); 
}


//add mode support for the challenge system
struct ModeSpellScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        //check the player level, cannot be more than level 1
        if (spell->m_caster->GetLevel() > 1) 
        {
            //tell the player that he cannot use this spell in the challenge mode
			ChatHandler(spell->m_caster->ToPlayer()).PSendSysMessage(((std::string)(">>>你等级太高了，只能一级使用。<<<")).c_str());
            return false;
        }

        //check the spell id
        if (spell->m_spellInfo->Id == 30856 || spell->m_spellInfo->Id == 30854 || spell->m_spellInfo->Id == 30852)
        {
            if (Player* player = spell->m_caster->ToPlayer())
            {
            	if(player->HasSpell(30851)
                || player->HasSpell(30853)
                || player->HasSpell(30855)
                )
                {
					ChatHandler(player).PSendSysMessage(((std::string)(">>>杀手模式只能选择一种。！!<<<")).c_str());
                    return false;
                }	
            }
        }

		return true;
    }
};

SpellScript* GetScript_Mode_Spell(SpellEntry const*)
{
    return new ModeSpellScript();
}


//APSP Spell for users
struct APSPSpellScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Player* player = spell->GetCaster()->ToPlayer())
            {
                int32 basePoints0 = 0, basePoints1 = 0;

                //update the AP and SP buff for the player
                basePoints0 = 0;
                basePoints1 = 0;
/*
#define ZQ_SPELL_MOUNTS_REINDEER            32980   //Mounts, 麋鹿新手坐骑
#define ZQ_SPELL_MOUNTS_REINDEER_LEARN      32981   //Mounts, 麋鹿新手坐骑
#define ZQ_SPELL_MOUNTS_LAND                32982   //Mounts, 坐骑100%
#define ZQ_SPELL_MOUNTS_LAND_LEARN          32983   //Mounts, 坐骑100%
#define ZQ_SPELL_MOUNTS_FLY                 32984   //Mounts, 飞行坐骑
#define ZQ_SPELL_MOUNTS_FLY_LEARN           32985   //Mounts, 飞行坐骑
#define ZQ_SPELL_MOUNTS_TIGER               32986   //Mounts, 幽灵虎
#define ZQ_SPELL_MOUNTS_TIGER_LEARN         32987   //Mounts, 幽灵虎
#define ZQ_SPELL_MOUNTS_GRIYP               32988   //Mounts, 幽灵狮鹫
#define ZQ_SPELL_MOUNTS_GRIYP_LEARN         32989   //Mounts, 幽灵狮鹫
#define ZQ_SPELL_MOUNTS_TURTLE              32990   //Mounts, 海龟
#define ZQ_SPELL_MOUNTS_TURTLE_LEARN        32991   //Mounts, 海龟


#define ZQ_SPELL_BONUS_AP         32830   //Bonus, 奖励，例如坐骑之类，被动奖励 攻强
#define ZQ_SPELL_BONUS_SP         32831   //Bonus, 奖励，例如坐骑之类，被动奖励 法强
*/

                if(player->HasSpell(ZQ_SPELL_MOUNTS_REINDEER)) { basePoints0 += 10; basePoints1 += 5; } // 麋鹿新手坐骑 10攻强，5法伤
                if(player->HasSpell(ZQ_SPELL_MOUNTS_LAND)) { basePoints0 += 30; basePoints1 += 15; } // 坐骑100% 10攻强，5法伤
                if(player->HasSpell(ZQ_SPELL_MOUNTS_FLY)) { basePoints0 += 30; basePoints1 += 15; } // 飞行坐骑 10攻强，5法伤
                if(player->HasSpell(ZQ_SPELL_MOUNTS_TIGER)) { basePoints0 += 100; basePoints1 += 50; } 
                if(player->HasSpell(ZQ_SPELL_MOUNTS_GRIYP)) { basePoints0 += 100; basePoints1 += 50; } 
                if(player->HasSpell(ZQ_SPELL_MOUNTS_TURTLE)) { basePoints0 += 30; basePoints1 += 15; }                

                uint32 _doneCounter = PAIR32_HIPART(  sQZAchievements.GetQuestDoneCounters(player)  ); //each quest add 1 AP and 0.5 SP
                //if(_doneCounter > 10000) _doneCounter = 10000;
                basePoints0 += _doneCounter/2;
                basePoints1 += (_doneCounter/4);

                sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[APSP Spell] for user %s : AP:%u, SP:%u. _doneCounter:%u.", 
                    player->GetName(), basePoints0, basePoints1, _doneCounter);

                uint32_t _EQBonus = sQZAchievements.GetEQCollectBonus(player);
				basePoints0 += _EQBonus;
				basePoints1 += (_EQBonus/2);

                uint32 _rawpoints = sQZAchievements.GetSocialPointsPVP(player);

                uint32 _weekpoints = _rawpoints % 10000;
                uint32 _totalpoints = _rawpoints / 10000;

                if(_weekpoints >= 5)
                {
                    uint32 _rewardpoints = (_weekpoints/5 > player->GetLevel()*10 ?  player->GetLevel() * 10 : _weekpoints/5);
                    basePoints0 += _rewardpoints;
                    basePoints1 += (_rewardpoints/2);
                }


                //now get the custom settings of the player
                uint32 __custom_settings = sQZAchievements.GetCustomSettings(player);
                uint32 __apsp_method = __custom_settings & 0x03;        
                if (__apsp_method == 1) { basePoints0 = basePoints0 + basePoints1 ;  basePoints1 = 0;}
                else if (__apsp_method == 2) { basePoints1 = basePoints0/2 + basePoints1; basePoints0 = 0; }
        

                if(spell->m_spellInfo->Id == ZQ_SPELL_BONUS_AP)
                    ChatHandler(player).PSendSysMessage(((std::string)("你因为收集（如坐骑、专业、装备、任务、战场等）而获得了[%d]点攻强和 [%d]点法强。  ")).c_str(), basePoints0, basePoints1);

                if(spell->m_spellInfo->Id == ZQ_SPELL_BONUS_AP)
                {
                    spell->m_currentBasePoints[0] = basePoints0;
                    spell->m_currentBasePoints[1] = basePoints0;
                }else if (spell->m_spellInfo->Id == ZQ_SPELL_BONUS_SP)
                {
                    spell->m_currentBasePoints[0] = basePoints1;
                    spell->m_currentBasePoints[1] = basePoints1;
                }
            }	
        }	

        return true;
    }		
};


//APSP Spell for users
struct ChuanSpellScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Player* player = spell->GetCaster()->ToPlayer())
            {
                int32 basePoints0 = 0, basePoints1 = 0, basePoints2 = 0;

                #define ZQ_SPELL_SPELL_CHUAN_PROPERTY	33365	//Spell Chuan Property, 传家宝属性
                #define ZQ_SPELL_SPELL_CHUAN_APSP   	33366	//Spell Chuan APSP, 传家宝法强攻强
                #define ZQ_SPELL_SPELL_CHUAN_HASTE	    33367	//Spell Chuan Haste, 传家宝急速

                
                uint32_t _plevel = player->GetLevel();
                switch(spell->m_spellInfo->Id)
                {
                    case ZQ_SPELL_SPELL_CHUAN_PROPERTY: //property, armor and spell resistance
                        basePoints0 = _plevel;
                        basePoints1 = _plevel * 10;
                        basePoints2 = _plevel / 2;
                        break;	
                    case ZQ_SPELL_SPELL_CHUAN_APSP: //APSP, attack and spell power
                        basePoints0 = _plevel * 4;
                        basePoints1 = _plevel * 4;
                        basePoints2 = _plevel * 2;
                        break;
                    case ZQ_SPELL_SPELL_CHUAN_HASTE: //Haste, attack and spell power
                        basePoints0 = _plevel / 6;
                        basePoints1 = _plevel / 6;
                        basePoints2 = _plevel / 6;
                        break;
                }

                if(player->M_Challenge_Mode & CHALLENGING_MODE_RICH)
                {
                    basePoints0 = basePoints0 * 1.5;
                    basePoints1 = basePoints1 * 1.5;
                    basePoints2 = basePoints2 * 1.5;
                }

                spell->m_currentBasePoints[0] = basePoints0;
                spell->m_currentBasePoints[1] = basePoints1;
                spell->m_currentBasePoints[2] = basePoints2;
            }	
        }	

        return true;
    }		
};

//Stole Spell for users
struct SpellStealScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            //check the target valid
            if (!spell->GetUnitTarget())
                return false;


            if (Player* player = spell->GetCaster()->ToPlayer())
            {
                //check the player's map
                /*if(player->GetMap()->IsRaid())
                {
                    //tell the player that he cannot stole the spell from the target
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>你不能在团本中使用偷取技能。<<<")).c_str());
                    return false;
                }

                //check the target level
                
                if (spell->GetUnitTarget()->GetLevel() > 60 ||  spell->GetUnitTarget()->GetLevel() > player->GetLevel())
                {
                    //tell the player that he cannot stole the spell from the target
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>目标等级太高，你无法从该目标身上偷取技能。<<<")).c_str());
                    return false;
                }

                if ( (spell->GetUnitTarget()->GetLevel() > 62) && (player->GetMapId() < 2) )
                {
                    //tell the player that he cannot stole the spell from the target
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>目标等级太高，你无法从该目标身上偷取技能。<<<")).c_str());
                    return false;
                }

                if ( (spell->GetUnitTarget()->GetLevel() > 62) )
                {
                    //tell the player that he cannot stole the spell from the target
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>目标等级太高，你无法从该目标身上偷取技能。<<<")).c_str());
                    return false;
                }

                //check the target is a player or a creature
                else */
                if (!spell->GetUnitTarget()->IsCreature()) {
                    //tell the player that he cannot stole the spell from the target
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>你只能从怪物身上偷取技能。<<<")).c_str());
                    return false;
                }
                //check the target if it's a totem or a creature with level less than 3
                else if (spell->GetUnitTarget()->GetLevel() < 3 || spell->GetUnitTarget()->ToCreature()->IsTotem()) {
                    //tell the player that he cannot stole the spell from the target
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>你无法从该目标偷取技能。<<<")).c_str());
                    return false;
                }

                
                //check the player's achievemnts
                uint32_t empty_slot = sQZAchievements.GetSkillsCollectEmptySlot(player);
                
                if(empty_slot == 0)
                {
                    //tell the player that he cannot stole the spell from the target
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>你技能已满，请在炉石里删除一些再尝试。<<<")).c_str());
                    return false;
                }

				//find the creature spell from creature_info
				auto _info = spell->GetUnitTarget()->ToCreature()->GetCreatureInfo();
				if (_info)
				{
					//check the creature spell
					uint32_t spell_IDs[4] = {0,0,0,0};
					uint32_t spell_IDs_count = 0;
					for (int i = 0; i < 4; i++)
					{
						if (_info->spells[i])
						{
							spell_IDs[i] = _info->spells[i];
							spell_IDs_count++;
						}
						else
						{
							break;
						}
					}

					if (spell_IDs_count > 0)
					{
						//random pick a spell from the creature spell list
						uint32_t _spell_ID = spell_IDs[urand(0, spell_IDs_count-1)];

						//add to achieivement
						SpellEntry const* spellInfo = sSpellMgr.GetSpellEntry(_spell_ID);
						std::string text = (std::string)(">>>你从该目标身上学习技能： ");
						if (spellInfo)
						{
							std::string name = spellInfo->SpellName[LOCALE_zhCN];
							if (name.empty()) name = spellInfo->SpellName[LOCALE_enUS];
							text.append(name);
						}

						text.append(" <<<");

						//save to the player's achievements
						sQZAchievements.SetSkillsCollectInfo(player, _spell_ID, empty_slot);

						//tell the player that he can stole the spell from the target
						ChatHandler(player).PSendSysMessage((text).c_str());
						return true;

					}
					else {
						//tell the player that he cannot stole the spell from the target
						ChatHandler(player).PSendSysMessage(((std::string)(">>>该目标身上没有技能可以偷取。<<<")).c_str());
						return true;
					}
				}
            }
        }	

		return true;
    }		
};

//casting spell for users
struct SpellCastingScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            //check the target valid
            if (!spell->GetUnitTarget())
                return false;


            //disable the casting of cast
            //return false;

            if (Player* player = spell->GetCaster()->ToPlayer())
            {
                uint32_t spell_id = sQZAchievements.GetSkillsCollectActiveID(player);
                if(spell_id == 0)
                {
                    //tell the player that he cannot cast the spell from the target
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>你没有可以使用的技能。<<<")).c_str());
                    return false;
                }

                //check the spell target
                SpellEntry const* spellInfo = sSpellMgr.GetSpellEntry(spell_id);

                if (spellInfo)
                {

                    /*
					if (spellInfo->DurationIndex == 21)
					{
						ChatHandler(player).PSendSysMessage(((std::string)(">>>该技能为永久型，无法使用。<<<")).c_str());
						return false;
					}*/

					if (spellInfo->Effect[0] == SPELL_EFFECT_APPLY_AURA  && spellInfo->EffectApplyAuraName[0] == SPELL_AURA_MOD_CHARM)
					{
						ChatHandler(player).PSendSysMessage(((std::string)(">>>禁用占据、心控类技能。<<<")).c_str());
						return false;
					}

                	if(spellInfo->EffectImplicitTargetA[0] == TARGET_UNIT_CASTER)	
                    {
                        if(player->HasAura(spell_id) == false)
                            player->CastSpell(player, spell_id, true);
                    }
                    else
                    {
                        if(spell->GetUnitTarget()->HasAura(spell_id) == false)
                            player->CastSpell(spell->GetUnitTarget(), spell_id, true);	
                    }
                }
            }
        }	

        return true;
    }	
};

#define ZQ_SPELL_VIP_HASTE  33380
//vip spell for vippers
struct SpellVIPHasteScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Player* player = spell->GetCaster()->ToPlayer())
            {
                if(player->HasSpell(ZQ_SPELL_VIP_HASTE))
                {
                    player->RemoveSpell(ZQ_SPELL_VIP_HASTE);
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>急速技能：关闭。<<<")).c_str());
                }
                else
                {
                    if(player->GetMapId() > 2)
                    {
                        player->LearnSpell(ZQ_SPELL_VIP_HASTE, false);
                        ChatHandler(player).PSendSysMessage(((std::string)(">>>急速技能：打开，额外增加吸血。<<<")).c_str());
                        player->M_Leech_Phy = 10;
                        player->M_Leech_Spell = 10;
                    }
                    else
                    {
                        ChatHandler(player).PSendSysMessage(((std::string)(">>>你只能在副本里使用。<<<")).c_str());
                    }
                }
            }
        }	
        return true;
    }	
};



//custom spell script for heal
struct HealCustomSpellScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
			Player* player = spell->GetCaster()->ToPlayer();
            if( (spell->m_triggeredBySpellInfo) && (player) )
            {

				uint32_t _bp = 0;
				
				switch (spell->m_triggeredBySpellInfo->Id)
				{
                    #define BASE_SPELL_HEAL_ID 32402
                    case BASE_SPELL_HEAL_ID + 0: _bp = player->GetStat(STAT_STRENGTH) * 0.5f; break;
                    case BASE_SPELL_HEAL_ID + 1: _bp = player->GetStat(STAT_STRENGTH) * 0.75f; break;
                    case BASE_SPELL_HEAL_ID + 2: _bp = player->GetStat(STAT_STRENGTH); break;

                    case BASE_SPELL_HEAL_ID + 3: _bp = player->GetStat(STAT_AGILITY) * 0.5f; break;
                    case BASE_SPELL_HEAL_ID + 4: _bp = player->GetStat(STAT_AGILITY) * 0.75f; break;
                    case BASE_SPELL_HEAL_ID + 5: _bp = player->GetStat(STAT_AGILITY); break;

                    case BASE_SPELL_HEAL_ID + 6: _bp = player->GetStat(STAT_INTELLECT) * 0.3f; break;
                    case BASE_SPELL_HEAL_ID + 7: _bp = player->GetStat(STAT_INTELLECT) * 0.4f; break;
                    case BASE_SPELL_HEAL_ID + 8: _bp = player->GetStat(STAT_INTELLECT) * 0.5f; break;

                    case BASE_SPELL_HEAL_ID + 9: _bp = player->GetStat(STAT_SPIRIT) * 0.3f; break;
                    case BASE_SPELL_HEAL_ID + 10: _bp = player->GetStat(STAT_SPIRIT) * 0.4f; break;
                    case BASE_SPELL_HEAL_ID + 11: _bp = player->GetStat(STAT_SPIRIT) * 0.5f; break;

				}
				
                player->CastCustomSpell(player, 32400, _bp, 0, 0, false);

                //__LOG("Triggered by SpellID: %u. bp: %u" , spell->m_triggeredBySpellInfo->Id, _bp);

            }
        }	
        return true;
    }	
};


// 456 - SHOWLABEL Only OFF
struct ShowlabelOffScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Player* pPlayer = spell->m_caster->ToPlayer())
                pPlayer->SetGMChat(false, true);
        }
        return true;
    }
};

SpellScript* GetScript_ShowlabelOff(SpellEntry const*)
{
    return new ShowlabelOffScript();
}

// 2765 - SHOWLABEL Only ON
struct ShowlabelOnScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Player* pPlayer = spell->m_caster->ToPlayer())
                pPlayer->SetGMChat(true, true);
        }
        return true;
    }
};

SpellScript* GetScript_ShowlabelOn(SpellEntry const*)
{
    return new ShowlabelOnScript();
}

// 1509 - GM Only OFF
struct GMOffScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Player* pPlayer = spell->m_caster->ToPlayer())
                pPlayer->SetGameMaster(false, true);
        }
        return true;
    }
};

SpellScript* GetScript_GMOff(SpellEntry const*)
{
    return new GMOffScript();
}

// 18139 - GM Only ON
struct GMOnScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Player* pPlayer = spell->m_caster->ToPlayer())
                pPlayer->SetGameMaster(true, true);
        }
        return true;
    }
};

SpellScript* GetScript_GMOn(SpellEntry const*)
{
    return new GMOnScript();
}

// 6147 - INVIS Only OFF
struct InvisOffScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Player* pPlayer = spell->m_caster->ToPlayer())
                pPlayer->SetGMVisible(true, true);
        }
        return true;
    }
};

SpellScript* GetScript_InvisOff(SpellEntry const*)
{
    return new InvisOffScript();
}

// 2763 - INVIS Only ON
struct InvisOnScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Player* pPlayer = spell->m_caster->ToPlayer())
                pPlayer->SetGMVisible(false, true);
        }
        return true;
    }
};

SpellScript* GetScript_InvisOn(SpellEntry const*)
{
    return new InvisOnScript();
}

// 20114 - BM Only OFF
// 24675 - BM OFF
struct BMOffScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Player* pPlayer = spell->m_caster->ToPlayer())
                pPlayer->SetCheatBeastmaster(false, true);
        }
        return true;
    }
};

SpellScript* GetScript_BMOff(SpellEntry const*)
{
    return new BMOffScript();
}

// 20115 - BM Only ON
// 24676 - BM ON
struct BMOnScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Player* pPlayer = spell->m_caster->ToPlayer())
                pPlayer->SetCheatBeastmaster(true, true);
        }
        return true;
    }
};

SpellScript* GetScript_BMOn(SpellEntry const*)
{
    return new BMOnScript();
}

// 29313 - CooldownAll
struct ClearAllCooldownsScript : public SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0 && spell->m_casterUnit)
            spell->m_casterUnit->RemoveAllCooldowns();
        return true;
    }
};

SpellScript* GetScript_ClearAllCooldowns(SpellEntry const*)
{
    return new ClearAllCooldownsScript();
}

// 21651 - Opening
struct OpeningBattlegroundBannerScript : public SpellScript
{
    void OnSuccessfulStart(Spell* spell) const final
    {
        if (!spell->m_casterUnit)
            return;

        if (GameObject* go = spell->m_targets.getGOTarget())
        {
            // Make sure the player is sending a valid GO target and lock ID.
            // SPELL_EFFECT_OPEN_LOCK can succeed with a lockId of 0.
            LockEntry const* lockInfo = sLockStore.LookupEntry(go->GetGOInfo()->GetLockId());
            if (lockInfo && lockInfo->Index[1] == LOCKTYPE_SLOW_OPEN)
            {
                Spell* visual = new Spell(spell->m_casterUnit, sSpellMgr.GetSpellEntry(24390), true);
                visual->prepare();
            }
        }
    }
};

SpellScript* GetScript_OpeningBattlegroundBanner(SpellEntry const*)
{
    return new OpeningBattlegroundBannerScript();
}

void AddSC_special_spell_scripts()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "spell_meteor";
    newscript->GetSpellScript = &GetScript_Meteor;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_darkmoon_steam_tonk_control_console";
    newscript->GetSpellScript = &GetScript_DarkmoonSteamTonkControlConsole;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_darkmoon_steam_tonk_cannon";
    newscript->GetSpellScript = &GetScript_DarkmoonSteamTonkCannon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_showlabel_off";
    newscript->GetSpellScript = &GetScript_ShowlabelOff;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_showlabel_on";
    newscript->GetSpellScript = &GetScript_ShowlabelOn;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_gm_off";
    newscript->GetSpellScript = &GetScript_GMOff;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_gm_on";
    newscript->GetSpellScript = &GetScript_GMOn;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_invis_off";
    newscript->GetSpellScript = &GetScript_InvisOff;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_invis_on";
    newscript->GetSpellScript = &GetScript_InvisOn;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_bm_off";
    newscript->GetSpellScript = &GetScript_BMOff;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_bm_on";
    newscript->GetSpellScript = &GetScript_BMOn;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_clear_all_cooldowns";
    newscript->GetSpellScript = &GetScript_ClearAllCooldowns;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_opening_battleground_banner";
    newscript->GetSpellScript = &GetScript_OpeningBattlegroundBanner;
    newscript->RegisterSelf();

    //add custom spell script for the pet system
    newscript = new Script;
    newscript->Name = "qzqstar_pet_spell";
    newscript->GetSpellScript = &GetScript_PetTrigSpell;
    newscript->RegisterSelf();

    //add pet aura to the player when the pet is summoned
    newscript = new Script;
    newscript->Name = "qzqstar_pet_aura";
    newscript->GetAuraScript = &GetScript_PetTrigAura;
    newscript->RegisterSelf();

    //add challenge mode spell script for the challenge system
    newscript = new Script;
    newscript->Name = "qzqstar_mode_spell";
    newscript->GetSpellScript = &GetScript_Mode_Spell;
    newscript->RegisterSelf();

    //add custom spell script for AP and SP buff
    newscript = new Script;
    newscript->Name = "qzqstar_apsp_buff";
    newscript->GetSpellScript = [](SpellEntry const*) -> SpellScript* { return new APSPSpellScript(); };
    newscript->RegisterSelf();

    //add custom spell script chuan jia bao
    newscript = new Script;
    newscript->Name = "qzqstar_chuan";
    newscript->GetSpellScript = [](SpellEntry const*) -> SpellScript* { return new ChuanSpellScript(); };
    newscript->RegisterSelf();

    //add custom spell script for spell stole 
    newscript = new Script;
    newscript->Name = "qzqstar_spell_stole";
    newscript->GetSpellScript = [](SpellEntry const*) -> SpellScript* { return new SpellStealScript(); };
    newscript->RegisterSelf();

    //add custom spell script for spell casting
    newscript = new Script;
    newscript->Name = "qzqstar_spell_cast";
    newscript->GetSpellScript = [](SpellEntry const*) -> SpellScript* { return new SpellCastingScript(); };
    newscript->RegisterSelf();

    //add vip haste spell script for vip
    newscript = new Script;
    newscript->Name = "qzqstar_vip_haste";
    newscript->GetSpellScript = [](SpellEntry const*) -> SpellScript* { return new SpellVIPHasteScript(); };
    newscript->RegisterSelf();

    //add heal custom spell script for heal
    newscript = new Script;
    newscript->Name = "qzqstar_heal_custom";
    newscript->GetSpellScript = [](SpellEntry const*) -> SpellScript* { return new HealCustomSpellScript(); };
    newscript->RegisterSelf();
}
