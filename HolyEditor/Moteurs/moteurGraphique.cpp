

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



#include "moteurGraphique.h"
#include "../globale.h"

#include <iostream>
#include <sstream>

sf::FloatRect GetViewRect(const sf::View& view)
{
    sf::FloatRect temp;

    temp.Left   = view.GetCenter().x - view.GetSize().x * 0.5;
    temp.Width  = view.GetSize().x;
    temp.Top    = view.GetCenter().y - view.GetSize().y * 0.5;
    temp.Height = view.GetSize().y;

    return temp;
};


MoteurGraphique::MoteurGraphique()
{
    LightManager = Light_Manager::GetInstance();

    m_images.push_back(Image_moteur ());
    m_images[0].img = new sf::Image();
    m_images[0].img->Create(1024, 1024, sf::Color(255, 255, 255));

    m_images[0].nom         = "O";
    m_images[0].importance  = -1;

    m_soleil.rouge          = 255;
    m_soleil.vert           = 255;
    m_soleil.bleu           = 255;
    m_soleil.intensite      = 255;

    decalageCamera.x        = 0;
    decalageCamera.y        = 0;
}
MoteurGraphique::~MoteurGraphique()
{
    LightManager->Kill();
    Vider();
    for (unsigned i = 0; i < m_images.size(); ++i)
        if(m_images[i].img != NULL)
            delete m_images[i].img;
    m_images.clear();
    m_systemeParticules.clear();
    m_modeleSystemeParticules.clear();
}

void MoteurGraphique::CreateNewWindow()
{

}

void MoteurGraphique::Charger()
{
    if (configuration->postFX)
    {
        console->Ajouter("");
        console->Ajouter("Chargement des postFX :");

        if (!EffectBlur.LoadFromFile(configuration->chemin_fx+configuration->nom_effetBlur))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetBlur,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetBlur,0);
        if (!EffectMort.LoadFromFile(configuration->chemin_fx+configuration->nom_effetMort))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetMort,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetMort,0);

        EffectMort.SetParameter("offset", 0);
        EffectMort.SetParameter("color",1, 1, 1);

        if (!EffectContrastes.LoadFromFile(configuration->chemin_fx+configuration->nom_effetContrastes))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetContrastes,1);
        else
        {
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetContrastes,0);
            EffectContrastes.SetParameter("color", 0.f, 0.f, 0.f);
        }

        if (!EffectNoir.LoadFromFile(configuration->chemin_fx+configuration->nom_effetNoir))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetNoir,1);
        else
        {
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetNoir,0);
            EffectNoir.SetParameter("color", 0.f, 0.f, 0.f);
        }
        configuration->effetMort=0;


        if (!EffectFiltre.LoadFromFile(configuration->chemin_fx+configuration->nom_effetFiltre))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetFiltre,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetFiltre,0);

    }

    console->Ajouter("");
    console->Ajouter("Chargement des polices d'�criture :");

    if (!m_font.LoadFromFile(configuration->chemin_fonts+configuration->font))
        console->Ajouter("Impossible de charger : "+configuration->chemin_fonts+configuration->font,1);
    else
        console->Ajouter("Chargement de : "+configuration->chemin_fonts+configuration->font,0);

    if (!m_font_titre.LoadFromFile(configuration->chemin_fonts+configuration->font_titre))
        console->Ajouter("Impossible de charger : "+configuration->chemin_fonts+configuration->font_titre,1);
    else
        console->Ajouter("Chargement de : "+configuration->chemin_fonts+configuration->font_titre,0);

    m_img_corner = AjouterImage(configuration->chemin_menus + configuration->nom_corner);
}

void MoteurGraphique::Gerer(float temps,int tailleMapY)
{
    int k=0;
    m_systemeParticules_iter=m_systemeParticules.begin();
    while(m_systemeParticules_iter!=m_systemeParticules.end())
    {
        if (m_systemeParticules_iter->m_modele>=0&&m_systemeParticules_iter->m_modele<(int)m_modeleSystemeParticules.size())
        {


            if (!m_systemeParticules_iter->Afficher(&m_modeleSystemeParticules[m_systemeParticules_iter->m_modele],temps,tailleMapY))
            {
                m_systemeParticules.erase (m_systemeParticules_iter);
                if((int)m_systemeParticules.size()>k)
                    m_systemeParticules_iter=m_systemeParticules.begin()+k;
            }
            else
                ++m_systemeParticules_iter,++k;
        }
        else
            ++m_systemeParticules_iter,++k;
    }


    k=0;
    m_effetsEcran_iter=m_effetsEcran.begin();

    while(m_effetsEcran_iter!=m_effetsEcran.end())
    {
        m_effetsEcran_iter->temps += temps;

        if(m_effetsEcran_iter->type == TREMBLEMENT)
        {
            if(m_effetsEcran_iter->temps > 0.05)
            {
                m_effetsEcran_iter->temps   = 0;

                int valeur = (int)m_effetsEcran_iter->info1 * (1 - 2 * (rand()%2));
                if(fabs(decalageCamera.x) < fabs(valeur) && fabs(valeur) != m_effetsEcran_iter->info1 || fabs(valeur) == m_effetsEcran_iter->info1)
                    decalageCamera.x = valeur;

                valeur = (int)m_effetsEcran_iter->info1 * (1 - 2 * (rand()%2));
                if(fabs(decalageCamera.y) < fabs(valeur) && fabs(valeur) != m_effetsEcran_iter->info1 || fabs(valeur) == m_effetsEcran_iter->info1)
                    decalageCamera.y = valeur;

                m_effetsEcran_iter->info1  -= 1;
            }
        }

        if (m_effetsEcran_iter->info1 < 0)
        {
            m_effetsEcran.erase (m_effetsEcran_iter);
            if((int)m_effetsEcran.size()>k)
                m_effetsEcran_iter=m_effetsEcran.begin()+k;
        }
        else
            ++m_effetsEcran_iter,++k;
    }

}

void MoteurGraphique::Afficher()
{
    //configuration->Resolution.x = m_ecran->GetWidth();
    //configuration->Resolution.y = m_ecran->GetHeight();

    sf::Sprite sprite;
    sf::Sprite sprite2;

    m_ecran->SetView(m_camera);

    m_ecran->Clear(sf::Color(0,0,128));

    for (int k=0;k<=20;k++)
    {

        if (k!=9)
        {
            for (IterCommande=m_commandes[k].begin();IterCommande!=m_commandes[k].end();++IterCommande)
            {
                if (IterCommande->m_utiliserCamera)
                    m_ecran->SetView(m_camera);
                else
                    m_ecran->SetView(m_ecran->GetDefaultView());

                m_ecran->Draw(IterCommande->m_sprite);
            }
        }

        sf::View temp;
        temp.SetSize(m_ecran->GetWidth(),m_ecran->GetHeight());
        temp.SetCenter(m_ecran->GetWidth() * 0.5,m_ecran->GetHeight() * 0.5);
        m_ecran->SetView(temp);
        for (unsigned i=0;i<m_textes[k].size();i++)
            m_ecran->Draw(m_textes[k][i]);

    }

    m_ecran->Display();
    m_ecran->repaint();
    Vider();
}

int MoteurGraphique::AjouterImage(const char *Data, std::size_t SizeInBytes, std::string nom,int importance)
{
    for (unsigned i=0; i < m_images.size(); i++)
    {
        if (m_images[i].nom==nom)
        {
            m_images[i].importance=importance;
            return i;
        }
    }
    for (unsigned i=0; i < m_images.size(); i++)
    {
        if (m_images[i].img == NULL)
        {
            m_images[i].nom=nom;

            m_images[i].img = new sf::Image();

            if (!configuration->lissage)
                m_images[i].img->SetSmooth(false);

            if (!m_images[i].img->LoadFromMemory(Data,SizeInBytes))
            {
                console->Ajouter("Impossible de charger : "+nom,1);
                return -1;
            }
            else
                console->Ajouter("Chargement de : "+nom,0);

            m_images[i].importance=importance;

            return i;
        }
    }

    m_images.push_back(Image_moteur ());
    m_images.back().nom=nom;

    m_images.back().img = new sf::Image();

    if (!configuration->lissage)
        m_images.back().img->SetSmooth(false);

    if (!m_images.back().img->LoadFromMemory(Data,SizeInBytes))
    {
        console->Ajouter("Impossible de charger depuis la m�moire : "+nom,1);
        return -1;
    }
    else
        console->Ajouter("Chargement de : "+nom,0);

    m_images.back().importance=importance;

    return m_images.size()-1;
}

int MoteurGraphique::AjouterImage(std::string chemin,int importance)
{
    for (unsigned i=0; i < m_images.size(); i++)
    {
        if (m_images[i].nom==chemin)
        {
            m_images[i].importance=importance;
            return i;
        }
    }
    for (unsigned i=0; i < m_images.size(); i++)
    {
        if (m_images[i].img == NULL)
        {
            m_images[i].nom=chemin;

            m_images[i].img = new sf::Image();

            if (!configuration->lissage)
                m_images[i].img->SetSmooth(false);

            if (!m_images[i].img->LoadFromFile(chemin.c_str()))
            {
                console->Ajouter("Impossible de charger : "+chemin,1);
                return -1;
            }
            else
                console->Ajouter("Chargement de : "+chemin,0);

            m_images[i].importance=importance;

            return i;
        }
    }

    m_images.push_back(Image_moteur ());
    m_images.back().nom=chemin;

    m_images.back().img = new sf::Image();

    if (!configuration->lissage)
        m_images.back().img->SetSmooth(false);

    if (!m_images.back().img->LoadFromFile(chemin.c_str()))
    {
        console->Ajouter("Impossible de charger : "+chemin,1);
        return -1;
    }
    else
        console->Ajouter("Chargement de : "+chemin,0);

    m_images.back().importance=importance;

    return m_images.size()-1;
}

int MoteurGraphique::AjouterTileset(std::string chemin,int importance)
{
    std::ifstream fichier;
    fichier.open(chemin.c_str(), std::ios::in);
    int retour = AjouterTileset(fichier, chemin, importance);
    console->Ajouter("Chargement de : "+chemin,0);
    fichier.close();
    return retour;
}


int MoteurGraphique::AjouterTileset(std::ifstream &fichier, std::string chemin,int importance)
{
    m_tileset.push_back(Tileset_moteur ());

    m_tileset.back().nom = chemin;
    m_tileset.back().tileset = new Tileset(fichier);
    m_tileset.back().tileset->m_chemin = chemin;
    m_tileset.back().importance = importance;

    return m_tileset.size()-1;
}


void MoteurGraphique::DecrementerImportance()
{
    for (unsigned i=0; i < m_images.size(); i++)
        if (m_images[i].importance!=-1)
        {
            m_images[i].importance--;
            if (m_images[i].importance<=0)
            {
                delete m_images[i].img;
                m_images[i].img = NULL;
                m_images[i].nom="",m_images[i].importance=0;
            }
        }

    //m_tileset.clear();
}


void MoteurGraphique::AjouterEntiteGraphique(Entite_graphique *entite)
{
    //if(entite->m_tileset != NULL)
    {
        sf::Sprite sprite_final;
        sprite_final = entite->m_sprite;
        sprite_final.Scale(fabs(entite->m_scale.x*0.01),fabs(entite->m_scale.y*0.01));
        (entite->m_scale.x < 0) ? sprite_final.FlipX(true), sprite_final.SetOrigin(sprite_final.GetSubRect().Width - sprite_final.GetOrigin().x, sprite_final.GetOrigin().y) : sprite_final.FlipX(false);
        (entite->m_scale.y < 0) ? sprite_final.FlipY(true) : sprite_final.FlipY(false);



        sprite_final.Rotate(entite->m_rotation);
        sprite_final.SetColor(sf::Color(sprite_final.GetColor().r * entite->m_color.r / 255,
                                        sprite_final.GetColor().g * entite->m_color.g / 255,
                                        sprite_final.GetColor().b * entite->m_color.b / 255,
                                        sprite_final.GetColor().a * entite->m_color.a / 255));

        sprite_final.Move(entite->m_decalage.x, entite->m_decalage.y);

       if(sprite_final.GetPosition().x + sprite_final.GetSize().x - sprite_final.GetOrigin().x     >= GetViewRect(m_camera).Left
        && sprite_final.GetPosition().x - sprite_final.GetOrigin().x                                <  GetViewRect(m_camera).Left + GetViewRect(m_camera).Width
        && sprite_final.GetPosition().y + sprite_final.GetSize().y - sprite_final.GetOrigin().y     >= GetViewRect(m_camera).Top
        && sprite_final.GetPosition().y - sprite_final.GetOrigin().y                                <  GetViewRect(m_camera).Top + GetViewRect(m_camera).Height)
            AjouterCommande(&sprite_final, entite->m_couche + entite->m_decalCouche, true);

        if(entite->m_shadow)
        {
            sf::Sprite sprite;
            sprite = sprite_final;

            sprite.SetScale(1, (100-(float)m_soleil.hauteur)/50);
            sprite.SetRotation(m_angleOmbreSoleil);

            AjouterCommande(&sprite, 9, true);
        }

        if(entite->m_reflect)
        {
            sf::Sprite sprite;
            sprite = sprite_final;

            sprite.FlipY(true);
            sprite.SetOrigin(sprite.GetOrigin().x, sprite.GetSize().y - sprite.GetOrigin().y);

            if(sprite.GetPosition().x + sprite.GetSize().x - sprite.GetOrigin().x     >= GetViewRect(m_camera).Left
            && sprite.GetPosition().x - sprite.GetOrigin().x                          <  GetViewRect(m_camera).Left + GetViewRect(m_camera).Width
            && sprite.GetPosition().y + sprite.GetSize().y - sprite.GetOrigin().y     >= GetViewRect(m_camera).Top
            && sprite.GetPosition().y - sprite.GetOrigin().y                          <  GetViewRect(m_camera).Top + GetViewRect(m_camera).Height)
                AjouterCommande(&sprite, 0, true);
        }
    }
}


Tileset* MoteurGraphique::getTileset(int IDtileset)
{
    if (IDtileset>=0 && IDtileset<(int)m_tileset.size())
        return m_tileset[(unsigned)IDtileset].tileset;
    return m_tileset.front().tileset;
}

Entite_graphique MoteurGraphique::getEntiteGraphique(int noTileset, int noTile, int couche)
{
    Entite_graphique entite;

    if(noTileset >= 0 && noTileset < (int)m_tileset.size())
        entite.m_tileset    = m_tileset[noTileset].tileset;
    else
        entite.m_tileset    = NULL;

    entite.m_noAnimation    = noTile;
    entite.m_couche         = couche;
    //entite.Generer();

    return entite;
}


int MoteurGraphique::AjouterModeleSystemeParticules(std::string chemin)
{
    for (unsigned i=0;i<m_modeleSystemeParticules.size();i++)
        if (m_modeleSystemeParticules[i].m_chemin==chemin)
            return i;

    m_modeleSystemeParticules.push_back(ModeleParticuleSysteme (chemin));

    return m_modeleSystemeParticules.size()-1;
}


void MoteurGraphique::AjouterSystemeParticules(int ID,coordonnee position,sf::Color color,float force,float angle)
{
    if (ID>=0&&ID<(int)m_modeleSystemeParticules.size())
        m_systemeParticules.push_back(ParticuleSysteme (ID,&m_modeleSystemeParticules[ID],position,color,force,angle));
}

void MoteurGraphique::AjouterCommande(sf::Sprite *sprite, int couche, bool camera)
{
    if (couche>=0&&couche<=20)
        m_commandes[couche].push_back(Commande (sprite,camera));
}

void MoteurGraphique::AjouterTexte(std::string txt, coordonnee pos, int couche, bool titre, int size, sf::Color color, bool fond)
{
    sf::Text temp;
    temp.SetString(txt);
    temp.SetPosition(pos.x, pos.y);
    temp.SetCharacterSize(size);
    temp.SetColor(color);

    AjouterTexte(&temp, couche, titre);

    if(fond)
    {
        sf::Sprite temp2;
        temp2.SetImage(*getImage(0));
        temp2.SetPosition(pos.x-2, pos.y-2);
        temp2.Resize(temp.GetRect().Width + 4,
                     temp.GetRect().Height + 4);
        temp2.SetColor(sf::Color(0,0,0,224));
        AjouterCommande(&temp2, couche, 0);
    }
}

void MoteurGraphique::AjouterTexteNonChevauchable(sf::Text* string, int couche, bool titre)
{
    if (couche>=0&&couche<=20)
    {
        /*for (int i=0;i<(int)m_textes[couche].size();i++)
        {
            if(string->GetRect().Right  > m_textes[couche][i].GetRect().Left
            && string->GetRect().Left   < m_textes[couche][i].GetRect().Right
            && string->GetRect().Bottom > m_textes[couche][i].GetRect().Top - 3
            && string->GetRect().Top    < m_textes[couche][i].GetRect().Bottom)
                string->SetPosition(string->GetPosition().x, m_textes[couche][i].GetRect().Top - string->GetRect().Bottom + string->GetRect().Top - 5), i = -1;
        }*/

        AjouterTexte(string, couche, titre);
    }
}

void MoteurGraphique::AjouterTexte(sf::Text* string, int couche,bool titre)
{

    if (couche>=0&&couche<=20)
    {
        sf::Text temp(*string);
        temp.SetFont(m_font);

        if (titre)
        {
            temp.SetFont(m_font_titre);

            temp.SetStyle(sf::Text::Bold);
            temp.SetColor(string->GetColor());

            m_textes[couche].push_back(temp);

            temp.SetColor(sf::Color((int)(string->GetColor().r*0.15),(int)(string->GetColor().g*0.15),(int)(string->GetColor().b*0.15),string->GetColor().a));
            temp.SetStyle(sf::Text::Regular);
            m_textes[couche].push_back(temp);
        }
        else
        {
            temp.Move(1,1);
            temp.SetColor(sf::Color((int)(string->GetColor().r*0.05),(int)(string->GetColor().g*0.05),(int)(string->GetColor().b*0.05),string->GetColor().a));
            m_textes[couche].push_back(temp);
            temp.Move(-1,-1);
            temp.SetColor(string->GetColor());
        }

        m_textes[couche].push_back(temp);
    }
}

void MoteurGraphique::Vider()
{
    for (int i=0;i<=20;i++)
    {
        m_commandes[i].clear();
        m_textes[i].clear();
    }
}
void MoteurGraphique::ViderParticules()
{
    m_systemeParticules.clear();
}


sf::Image* MoteurGraphique::getImage(int IDimage)
{
    if (IDimage>=0&&IDimage<(int)m_images.size())
        return m_images[IDimage].img;
    else
        return m_images[0].img;
}

ModeleParticuleSysteme* MoteurGraphique::getModeleMoteurParticules(int ID)
{
    if (ID>=0&&ID<(int)m_modeleSystemeParticules.size())
        return &m_modeleSystemeParticules[ID];
    else
        return &m_modeleSystemeParticules[0];
}

std::string MoteurGraphique::getCheminImage(int IDimage)
{
    if (IDimage>=0&&IDimage<(int)m_images.size())
        return m_images[IDimage].nom;
    else
        return "";
}

bool MoteurGraphique::getEvent(sf::Event &EventReceived)
{
    return m_ecran->GetEvent(EventReceived);
}

coordonnee MoteurGraphique::getPositionSouris()
{
    coordonnee pos;

    pos.x=(int)m_ecran->ConvertCoords(m_ecran->GetInput().GetMouseX(), m_ecran->GetInput().GetMouseY(), m_camera).x;
    pos.y=(int)m_ecran->ConvertCoords(m_ecran->GetInput().GetMouseX(), m_ecran->GetInput().GetMouseY(), m_camera).y;

    return pos;
}

void MoteurGraphique::Printscreen()
{
    std::ostringstream buf;
    buf<<"screenshot"<<configuration->numero_screen<<".png";
    //sf::Image Screen = m_ecran->Capture();
    //Screen.SaveToFile(buf.str());
    configuration->numero_screen++;
}

int MoteurGraphique::GetFPS()
{
    return (int)( 1.f / m_ecran->GetFrameTime());
}

