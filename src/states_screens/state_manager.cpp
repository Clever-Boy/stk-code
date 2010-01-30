//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2009 Marianne Gagnon
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


#include "states_screens/state_manager.hpp"

#include "guiengine/engine.hpp"
#include "guiengine/modaldialog.hpp"
#include "guiengine/screen.hpp"
#include "input/input_manager.hpp"
#include "states_screens/dialogs/race_paused_dialog.hpp"
#include "utils/translation.hpp"

using namespace GUIEngine;

StateManager* state_manager_singleton = NULL;

StateManager* StateManager::get()
{
    if (state_manager_singleton == NULL) state_manager_singleton = new StateManager();
    return state_manager_singleton;
}

#if 0
#pragma mark -
#pragma mark Player Management
#endif

ptr_vector<ActivePlayer, HOLD>& StateManager::getActivePlayers()
{
    return m_active_players;
}
ActivePlayer* StateManager::getActivePlayer(const int id)
{
    ActivePlayer *returnPlayer = NULL;
    if (id < m_active_players.size() && id >= 0)
    {
        returnPlayer = m_active_players.get(id);
    }
    else
    {
        fprintf(stderr, "getActivePlayer(): id out of bounds\n");
    }
    
    assert( returnPlayer->m_id == id );
    
    return returnPlayer;
}
void StateManager::updateActivePlayerIDs()
{
    const int amount = m_active_players.size();
    for (int n=0; n<amount; n++)
    {
        m_active_players[n].m_id = n;
    }
}

int StateManager::createActivePlayer(PlayerProfile *profile, InputDevice *device)
{
    ActivePlayer *p;
    int i;
    p = new ActivePlayer(profile, device);
    i = m_active_players.size();
    m_active_players.push_back(p);
    
    updateActivePlayerIDs();
    
    return i;
}

void StateManager::removeActivePlayer(int id)
{
    m_active_players.erase(id);
    updateActivePlayerIDs();
}
int StateManager::activePlayerCount()
{
    return m_active_players.size();
}

void StateManager::resetActivePlayers()
{
    const int amount = m_active_players.size();
    for(int i=0; i<amount; i++)
    {
        m_active_players[i].setDevice(NULL);
    }
    m_active_players.clearWithoutDeleting();
}

#if 0
#pragma mark -
#pragma mark misc stuff
#endif

bool StateManager::throttleFPS()
{
    return m_game_mode != GUIEngine::GAME  &&  GUIEngine::getCurrentScreen()->throttleFPS;
}

void StateManager::escapePressed()
{
    // in input sensing mode
    if(input_manager->isInMode(InputManager::INPUT_SENSE_KEYBOARD) ||
       input_manager->isInMode(InputManager::INPUT_SENSE_GAMEPAD) )
    {
        ModalDialog::dismiss();
        input_manager->setMode(InputManager::MENU);
    }
    // when another modal dialog is visible
    else if(ModalDialog::isADialogActive())
    {
        ModalDialog::dismiss();
    }
    // In-game
    else if(m_game_mode == GAME)
    {
        new RacePausedDialog(0.8f, 0.6f);
        //resetAndGoToMenu("main.stkgui");
        //input_manager->setMode(InputManager::MENU);
    }
    // In menus
    else
    {
        popMenu();
    }
}


