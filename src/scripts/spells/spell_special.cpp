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

                    if(spell->m_spellInfo->Id == 31717) // Weapon Damage
                    {
						//weapon damage
                        _multiple = (_petLevel / 4.0f) * ( 1.0f + _petHappiness/200.0f) ; // 10% of the pet level
						spell->m_currentBasePoints[0] = spell->m_currentBasePoints[0] * _multiple * frand(0.9, 1.15);
                    }
                    else {
                        //magic damage
                        _multiple = _petLevel * 7 * ( 1.0f + _petHappiness/100.0f) + _petLevel * _petLevel ; // 10% of the pet level
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
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Apply Aura1111: %u", aura->GetSpellProto()->Id);
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
}
