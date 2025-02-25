/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "TotemAI.h"
#include "Totem.h"
#include "Creature.h"
#include "ObjectAccessor.h"
#include "SpellMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"

int32 TotemAI::Permissible(Creature const* creature)
{
    if (creature->IsTotem())
        return PERMIT_BASE_PROACTIVE;

    return PERMIT_BASE_NO;
}

TotemAI::TotemAI(Creature* c, uint32 scriptId) : CreatureAI(c, scriptId), i_victimGuid()
{
    ASSERT(c->IsTotem());
}

void TotemAI::MoveInLineOfSight(Unit* /*who*/) { }

void TotemAI::EnterEvadeMode(EvadeReason /*why*/)
{
    me->CombatStop(true);
}

void TotemAI::UpdateAI(uint32 /*diff*/)
{
    if (me->ToTotem()->GetTotemType() != TOTEM_ACTIVE)
        return;

    if (!me->IsAlive() || me->IsNonMeleeSpellCast(false))
        return;

    // Search spell
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(me->ToTotem()->GetSpell(), me->GetMap()->GetDifficultyID());
    if (!spellInfo)
        return;

    // Get spell range
    float max_range = spellInfo->GetMaxRange(false);

    // SpellModOp::Range not applied in this place just because not existence range mods for attacking totems

    // pointer to appropriate target if found any
    Unit* victim = !i_victimGuid.IsEmpty() ? ObjectAccessor::GetUnit(*me, i_victimGuid) : nullptr;

    // Search victim if no, not attackable, or out of range, or friendly (possible in case duel end)
    if (!victim ||
        !victim->isTargetableForAttack() || !me->IsWithinDistInMap(victim, max_range) ||
        me->IsFriendlyTo(victim) || !me->CanSeeOrDetect(victim))
    {
        victim = nullptr;
        Trinity::NearestAttackableUnitInObjectRangeCheck u_check(me, me->GetCharmerOrOwnerOrSelf(), max_range);
        Trinity::UnitLastSearcher<Trinity::NearestAttackableUnitInObjectRangeCheck> checker(me, victim, u_check);
        Cell::VisitAllObjects(me, checker, max_range);
    }

    // If have target
    if (victim)
    {
        // remember
        i_victimGuid = victim->GetGUID();

        // attack
        me->CastSpell(victim, me->ToTotem()->GetSpell());
    }
    else
        i_victimGuid.Clear();
}

void TotemAI::AttackStart(Unit* /*victim*/)
{
}
