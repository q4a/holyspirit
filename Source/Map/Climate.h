

/*Copyright (C) 2009 Naisse Grégoire

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#ifndef CLIMATEH
#define CLIMATEH

#include "tileset.h"
#include "../globale.h"

class Climate
{
    public:
        Climate(std::string temp = "");
        ~Climate();

        void Charger(std::string);

        void Update(float time);
        void Draw();

        float GetState();
        float GetTransitionTime();

        void Stop();
        void Continue();

        bool m_actif;
        bool m_passif;

        std::string m_chemin;

        Lumiere m_lumiereModificater;
        float m_cur_time;

    private:
        std::vector<Entite_graphique> m_entities;
        std::vector<int> m_noAnimationStart;

        float m_max_time;
        float m_transition_time;

        std::vector<int> m_tileset;
};


#endif


