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


// 12472 - Cold Snap
struct MageColdSnapScript : SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (spell->m_caster->GetTypeId() != TYPEID_PLAYER)
                return false;

            // immediately finishes the cooldown on Frost spells
            auto cdCheck = [](SpellEntry const & spellEntry) -> bool
            {
                if (spellEntry.SpellFamilyName != SPELLFAMILY_MAGE)
                    return false;
                if ((spellEntry.GetSpellSchoolMask() & SPELL_SCHOOL_MASK_FROST) && spellEntry.GetRecoveryTime() > 0)
                    return true;
                return false;
            };
            static_cast<Player*>(spell->m_caster)->RemoveSomeCooldown(cdCheck);
        }
        return true;
    }
};

SpellScript* GetScript_MageColdSnap(SpellEntry const*)
{
    return new MageColdSnapScript();
}

// 31040 - Frost Echo
//qzqstar, 241212, cool down the frost echo merged.
struct MageFrostEchoScript : SpellScript
{
    bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (spell->m_caster->GetTypeId() != TYPEID_PLAYER)
                return false;

            // immediately finishes the cooldown on frost nova
            auto cdCheck = [](SpellEntry const & spellEntry) -> bool
            {
                if (spellEntry.SpellFamilyName != SPELLFAMILY_MAGE)
                    return false;
                /*122, 865, 6131, 10230*/
                if ((spellEntry.Id == 122 || spellEntry.Id == 865 || spellEntry.Id == 6131 || spellEntry.Id == 10230) && spellEntry.GetRecoveryTime() > 0)
                    return true;
                return false;
            };
            static_cast<Player*>(spell->m_caster)->RemoveSomeCooldown(cdCheck);
        }
        return true;
    }
};
SpellScript* GetScript_MageFrostEcho(SpellEntry const*)
{
    return new MageFrostEchoScript();
}



// 31055 - mage, - Meteor
struct MageMeteorScript : public SpellScript
{
	bool OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const final
	{
		if (effIdx == EFFECT_INDEX_0)
		{
			uint32 count = 0;
			for (const auto& ihit : spell->m_UniqueTargetInfo)
				if (ihit.effectMask & (1 << effIdx))
					++count;

			if (count < 1) count = 1;

			//mcaster unit
			auto _player = static_cast<Player*>(spell->m_casterUnit);

			//get the mana points
			auto _mana = _player->GetPower(POWER_MANA);

			//check mana
			if (_mana < 100) _mana = 100;

			//set the spell damage to be half of mana
			spell->damage = _mana / count; // divide to all targets
		}

		return true;
	}

	void OnSuccessfulFinish(Spell* spell) const final
	{
		//mcaster unit
		auto _player = static_cast<Player*>(spell->m_casterUnit);

		//get the mana points
		auto _mana = _player->GetPower(POWER_MANA);

		//set half of mana to be mana points
		_player->SetPower(POWER_MANA, _mana / 2);
	}
};

SpellScript* GetScript_MageMeteor(SpellEntry const*)
{
	return new MageMeteorScript();
}


void AddSC_mage_spell_scripts()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "spell_mage_cold_snap";
    newscript->GetSpellScript = &GetScript_MageColdSnap;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_mage_frost_echo";
    newscript->GetSpellScript = &GetScript_MageFrostEcho;
    newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "spell_mage_meteor";
	newscript->GetSpellScript = &GetScript_MageMeteor;
	newscript->RegisterSelf();
}
