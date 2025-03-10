

/*Copyright (C) 2009 Naisse Gr�goire

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

#include "../constantes.h"

#ifndef MOTEURSONSH
#define MOTEURSONSH

struct Infos_son
{
    Infos_son(int n, bool u = false, bool p = false, int v = 100) : no(n), unique(u), preserv(p), volume(v)
    {
    }
    int no;
    bool unique;
    bool preserv;
    int volume;
};

struct Infos_buffer
{
    Infos_buffer() : buffer(), volume(100), pitchable(true)
    {
    }
   sf::SoundBuffer buffer;
   int volume;
   bool pitchable;
};

class MoteurSons : public CSingleton<MoteurSons>
{
	public:
	friend MoteurSons* CSingleton<MoteurSons>::GetInstance();
    friend void CSingleton<MoteurSons>::Kill();


	void Gerer(float temps);
	void Vider();
	int AjouterBuffer(std::string);
	bool JouerSon(int ID,coordonnee position,bool unique=0,bool preserv = 0,int volume=100);
	bool IsPlayingSound(int ID);
	void StopAllSounds();

	void PlayNewMusic(const std::string &chemin);

	void setVolumeMusique(int volume);

	sf::Sound::Status GetMusicStatus();

	std::string getCheminSon(int IDimage);

	void DebugRefreshSound();

	protected:

	MoteurSons();
	~MoteurSons();

	int sonEnCours;
	sf::Sound m_sons[NOMBRE_SONS];
	bool m_sons_preserv[NOMBRE_SONS];
	int m_IDSons[NOMBRE_SONS];
	std::vector <Infos_buffer *> m_buffers;
	std::vector <std::string> m_cheminsSons;//Permet de s'assurer d'avoir des singletons

	sf::Music m_music;

	std::string m_curMusic;
	std::string m_nextMusic;
	float change_volume;
	int m_music_volume;
};

#endif




