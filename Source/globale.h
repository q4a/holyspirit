

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


#include "Moteurs/moteurGraphique.h"
#include "Moteurs/moteurSons.h"
#include "Moteurs/eventManager.h"
#include "Network/network.h"
#include "configuration.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <libintl.h>

#include <string>
#include <iterator>

#pragma once

class MySprite : public sf::Sprite {
public:

/*MySprite();

MySprite(const Texture& texture);

MySprite(const Texture& texture, const IntRect& rectangle);*/

void resize(float x, float y);

};

std::basic_string<sf::Uint32> toUtf32(std::string);

extern Configuration *configuration;
extern Console *console;
extern MoteurGraphique *moteurGraphique;
extern MoteurSons *moteurSons;
extern EventManager *eventManager;
extern Network *net;


