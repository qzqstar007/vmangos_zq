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

                    if(spell->m_spellInfo->Id == (33327)) // Weapon Damage
                    {
						//weapon damage
                        _multiple = (1.0f + _petLevel / 5.0f) * ( 1.0f + _petHappiness/200.0f + _petRelationship/100.0f) ; // 10% of the pet level
						spell->m_currentBasePoints[0] = spell->m_currentBasePoints[0] * _multiple * frand(0.9, 1.15);
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

                if(player->HasSpell(ZQ_SPELL_MOUNTS_REINDEER)) { basePoints0 += 10; basePoints1 += 5; } // 麋鹿新手坐骑 10攻强，5法伤
                if(player->HasSpell(ZQ_SPELL_MOUNTS_LAND)) { basePoints0 += 30; basePoints1 += 15; } // 坐骑100% 10攻强，5法伤
                if(player->HasSpell(ZQ_SPELL_MOUNTS_FLY)) { basePoints0 += 30; basePoints1 += 15; } // 飞行坐骑 10攻强，5法伤
                if(player->HasSpell(ZQ_SPELL_MOUNTS_TIGER)) { basePoints0 += 100; basePoints1 += 50; } 
                if(player->HasSpell(ZQ_SPELL_MOUNTS_GRIYP)) { basePoints0 += 100; basePoints1 += 50; } 
                if(player->HasSpell(ZQ_SPELL_MOUNTS_TURTLE)) { basePoints0 += 30; basePoints1 += 15; }                

                uint32 _doneCounter = PAIR32_HIPART(  sQZAchievements.GetQuestDoneCounters(player)  ); //each quest add 1 AP and 0.5 SP
                basePoints0 += _doneCounter;
                basePoints1 += (_doneCounter/2);

                sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[APSP Spell] for user %s : AP:%u, SP:%u. _doneCounter:%u.", 
                    player->GetName(), basePoints0, basePoints1, _doneCounter);

                uint32_t _EQBonus = sQZAchievements.GetEQCollectBonus(player);
				basePoints0 += _EQBonus;
				basePoints1 += (_EQBonus/2);

                ChatHandler(player).PSendSysMessage(((std::string)("你因为收集（如坐骑、专业、装备、任务等）而获得了[%d]点攻强和 [%d]点法伤。  ")).c_str(), basePoints0, basePoints1);

                spell->m_currentBasePoints[0] = basePoints0;
                spell->m_currentBasePoints[1] = basePoints0;
                spell->m_currentBasePoints[2] = basePoints1;
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
                //check the target level
                if (spell->GetUnitTarget()->GetLevel() > 60 ||  spell->GetUnitTarget()->GetLevel() > player->GetLevel())
                {
                    //tell the player that he cannot stole the spell from the target
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>目标等级太高，你无法从该目标身上偷取技能。<<<")).c_str());
                    return false;
                }
                //check the target is a player or a creature
                else if (!spell->GetUnitTarget()->IsCreature()) {
                    //tell the player that he cannot stole the spell from the target
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>你只能从怪物身上偷取技能。<<<")).c_str());
                    return false;
                }

                
                //check the player's achievemnts
                uint32_t empty_slot = sQZAchievements.GetSkillsCollectEmptySlot(player);
                
                if(empty_slot == 0)
                {
                    //tell the player that he cannot stole the spell from the target
                    ChatHandler(player).PSendSysMessage(((std::string)(">>>你技能已满，请删除一些在尝试。<<<")).c_str());
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
        }	


        return true;
    }	
};

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
}
