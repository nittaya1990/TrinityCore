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

#ifndef TRINITY_TOTEMAI_H
#define TRINITY_TOTEMAI_H

#include "CreatureAI.h"
#include "Timer.h"

class Creature;
class Totem;

class TC_GAME_API TotemAI : public CreatureAI
{
    public:

        explicit TotemAI(Creature* c, uint32 scriptId = {});

        void MoveInLineOfSight(Unit* who) override;
        void AttackStart(Unit* victim) override;
        void EnterEvadeMode(EvadeReason /*why*/) override;

        void UpdateAI(uint32 diff) override;
        static int32 Permissible(Creature const* creature);

    private:
        ObjectGuid i_victimGuid;
};
#endif
