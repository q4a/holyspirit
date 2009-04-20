

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



#include "moteurGraphique.h"
#include "../globale.h"

#include <iostream.h>

MoteurGraphique::MoteurGraphique()
{
    LightManager=Light_Manager::GetInstance();

    m_images.push_back(Image_moteur ());
    m_images[0].img.Create(1024, 1024, sf::Color(255, 255, 255));
    m_images[0].nom="O";
    m_images[0].importance=-1;

    m_soleil.rouge=255;
    m_soleil.vert=255;
    m_soleil.bleu=255;
    m_soleil.intensite=255;

    m_ecran=NULL;
}
MoteurGraphique::~MoteurGraphique()
{
    LightManager->Kill();
    Vider();
    m_images.clear();
    m_systemeParticules.clear();
    m_modeleSystemeParticules.clear();

    if(m_ecran!=NULL)
    {
        m_ecran->Close();
        delete m_ecran;
    }
}

void MoteurGraphique::createWindow()
{
    if(m_ecran!=NULL)
    {
        m_ecran->Close();
        delete m_ecran;
    }

    m_ecran = new sf::RenderWindow();

    if(!configuration->mode_fenetre)
        m_ecran->Create(sf::VideoMode(configuration->Resolution.x, configuration->Resolution.y, 32),"HolySpirit : Act of Faith",sf::Style::Fullscreen);
    else
        m_ecran->Create(sf::VideoMode(configuration->Resolution.x, configuration->Resolution.y, 32),"HolySpirit : Act of Faith",sf::Style::Titlebar);

    if(configuration->syncronisation_verticale)
    {
        m_ecran->UseVerticalSync(true);
        m_ecran->SetFramerateLimit(60);
    }

    m_ecran->ShowMouseCursor(false);
}

void MoteurGraphique::Charger()
{
    if (configuration->postFX)
    {
        console->Ajouter("");
        console->Ajouter("Chargement des postFX :");

        if(!EffectBlur.LoadFromFile(configuration->chemin_fx+configuration->nom_effetBlur))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetBlur,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetBlur,0);
        EffectBlur.SetTexture("framebuffer", NULL);

        if(!EffectMort.LoadFromFile(configuration->chemin_fx+configuration->nom_effetMort))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetMort,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetMort,0);

        EffectMort.SetParameter("offset", 0);
        EffectMort.SetTexture("framebuffer", NULL);
        EffectMort.SetParameter("color",1, 1, 1);

        if(!EffectContrastes.LoadFromFile(configuration->chemin_fx+configuration->nom_effetContrastes))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetContrastes,1);
        else
        {
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetContrastes,0);
            EffectContrastes.SetTexture("framebuffer", NULL);
            EffectContrastes.SetParameter("color", 0.f, 0.f, 0.f);
        }

        if(!EffectNoir.LoadFromFile(configuration->chemin_fx+configuration->nom_effetNoir))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetNoir,1);
        else
        {
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetNoir,0);
            EffectNoir.SetTexture("framebuffer", NULL);
            EffectNoir.SetParameter("color", 0.f, 0.f, 0.f);
        }
        configuration->effetMort=0;
    }

    console->Ajouter("");
    console->Ajouter("Chargement des polices d'écriture :");

    if(!m_font_titre.LoadFromFile(configuration->chemin_fonts+configuration->font_titre))
        console->Ajouter("Impossible de charger : "+configuration->chemin_fonts+configuration->font_titre,1);
    else
        console->Ajouter("Chargement de : "+configuration->chemin_fonts+configuration->font_titre,0);
}

void MoteurGraphique::Gerer(float temps,int tailleMapY)
{
    int k=0;
    for(m_systemeParticules_iter=m_systemeParticules.begin();m_systemeParticules_iter!=m_systemeParticules.end();++m_systemeParticules_iter,++k)
        if(m_systemeParticules_iter->m_modele>=0&&m_systemeParticules_iter->m_modele<(int)m_modeleSystemeParticules.size())
        {
            m_systemeParticules_iter->Afficher(&m_modeleSystemeParticules[m_systemeParticules_iter->m_modele]);

            if(!m_systemeParticules_iter->Gerer(temps,tailleMapY))
            {
                m_systemeParticules.erase (m_systemeParticules_iter);
                if(k!=0&&!m_systemeParticules.empty())
                    m_systemeParticules_iter=m_systemeParticules.begin()+(k-1);
                else if(k==0&&!m_systemeParticules.empty())
                    m_systemeParticules_iter=m_systemeParticules.begin();
                else
                    break;
            }
        }
}

void MoteurGraphique::Afficher(coordonnee dimensionsMap)
{
    configuration->Resolution.x=m_ecran->GetWidth();
    configuration->Resolution.y=m_ecran->GetHeight();

    sf::Sprite sprite;
    sf::Sprite sprite2;

    if(configuration->postFX)
    {
        EffectBlur.SetTexture("framebuffer", NULL);
        EffectNoir.SetTexture("framebuffer", NULL);
        EffectMort.SetTexture("framebuffer", NULL);
        EffectContrastes.SetTexture("framebuffer", NULL);
    }

    m_ecran->SetView(m_camera);

        if(configuration->RafraichirOmbre==1&&configuration->Ombre&&m_soleil.intensite>32)
        {
            decalageLumiere=m_camera.GetCenter();

            m_ecran->Clear(sf::Color(255,255,255));

            LightManager->DrawWallShadow(m_ecran,&m_camera,dimensionsMap);

            if (configuration->postFX)
            {
                EffectBlur.SetParameter("offset",0.02);
                m_ecran->Draw(EffectBlur);
            }

            m_ecran->SetView(m_camera);
            sprite.SetBlendMode(sf::Blend::Alpha);

            for(IterCommande=m_commandes[9].begin();IterCommande!=m_commandes[9].end();++IterCommande)
            {
                sprite=IterCommande->m_sprite;
                sprite.SetColor(sf::Color(0,0,0,255));
                m_ecran->Draw(sprite);
            }

            m_ecran->SetView(m_ecran->GetDefaultView());

            sf::Sprite sprite3;
            sprite3.SetX(0);
            sprite3.SetY(0);
            sprite3.SetImage(*getImage(0));
            sprite3.Resize(configuration->Resolution.w,configuration->Resolution.h);
            sprite3.SetColor(sf::Color(sf::Color((int)(128+128-m_soleil.intensite*0.5),(int)(128+128-m_soleil.intensite*0.5),(int)(128+128-m_soleil.intensite*0.5))));
            sprite3.SetBlendMode(sf::Blend::Add);
            m_ecran->Draw(sprite3);

            if (configuration->postFX)
            {
                EffectBlur.SetParameter("offset",0.005);
                m_ecran->Draw(EffectBlur);
            }

            m_light_screen2.CopyScreen(*m_ecran);

            configuration->RafraichirOmbre=2;
        }

    if(configuration->Lumiere && configuration->RafraichirLumiere)
    {
        m_ecran->SetView(m_camera);

        m_ecran->Clear(sf::Color(m_soleil.rouge*m_soleil.intensite/255,m_soleil.vert*m_soleil.intensite/255,m_soleil.bleu*m_soleil.intensite/255));

        sprite2.SetColor(sf::Color(255,255,255,255));

        LightManager->Draw(m_ecran,&m_camera,dimensionsMap);

        if (configuration->postFX)
        {
            EffectBlur.SetParameter("offset",0.01);
            m_ecran->Draw(EffectBlur);
            EffectBlur.SetParameter("offset",0.005);
            m_ecran->Draw(EffectBlur);
        }

        m_light_screen.CopyScreen(*m_ecran);

        configuration->RafraichirLumiere=false;
    }

    m_ecran->Clear();

    for(int k=0;k<=20;k++)
    {
        if(k==12&&configuration->Lumiere)
        {
            sprite2.FlipY(true);
            sprite2.SetImage(m_light_screen);
            sprite2.SetBlendMode(sf::Blend::Multiply);
            sprite2.SetColor(sf::Color(255,255,255,255));

            sprite2.SetX(0);
            sprite2.SetY(0);

            m_ecran->SetView(m_ecran->GetDefaultView());
            m_ecran->Draw(sprite2);
        }

        if(k==10&&configuration->Ombre&&configuration->Ombre&&m_soleil.intensite>32)
        {
            sprite2.FlipY(true);
            sprite2.SetImage(m_light_screen2);
            sprite2.SetBlendMode(sf::Blend::Multiply);
            sprite2.SetColor(sf::Color(255,255,255));

            sprite2.SetX(decalageLumiere.x-m_camera.GetCenter().x);
            sprite2.SetY(decalageLumiere.y-m_camera.GetCenter().y);

            m_ecran->SetView(m_ecran->GetDefaultView());
            m_ecran->Draw(sprite2);
        }


        if(k!=9)
        {
            for(IterCommande=m_commandes[k].begin();IterCommande!=m_commandes[k].end();++IterCommande)
            {
                if(IterCommande->m_utiliserCamera)
                    m_ecran->SetView(m_camera);
                else
                    m_ecran->SetView(m_ecran->GetDefaultView());

                m_ecran->Draw(IterCommande->m_sprite);
            }
        }


        for(int i=0;i<(int)m_textes[k].size();i++)
        {
            m_ecran->SetView(m_ecran->GetDefaultView());
            m_ecran->Draw(m_textes[k][i]);
        }

        if(k==18&&configuration->postFX)
        {
            if(m_blur>0)
            {
                EffectBlur.SetParameter("offset",(float)m_blur);
                m_ecran->Draw(EffectBlur);
                m_ecran->Draw(EffectBlur);
                m_ecran->Draw(EffectBlur);
            }
            if(configuration->effetMort>0)
                m_ecran->Draw(EffectMort);
        }


    }

    if(configuration->effetNoir>0)
    {
        if (configuration->postFX)
        {
            EffectNoir.SetParameter("color", configuration->effetNoir, configuration->effetNoir, configuration->effetNoir);
            m_ecran->Draw(EffectNoir);
        }
        else
        {
            sf::Sprite sprite2;
            sprite2.SetImage(*getImage(0));
            sprite2.Resize(configuration->Resolution.w,configuration->Resolution.h);
            sprite2.SetColor(sf::Color((int)(configuration->effetNoir*255),(int)(configuration->effetNoir*255),(int)(configuration->effetNoir*255),255));
            sprite2.SetBlendMode(sf::Blend::Multiply);
            m_ecran->Draw(sprite2);
        }
    }


    if(configuration->contrastes>1&&configuration->postFX)
    {
        m_ecran->Draw(EffectContrastes);
        EffectContrastes.SetParameter("color", configuration->contrastes, configuration->contrastes, configuration->contrastes);
    }
    if(configuration->luminosite>0)
    {
        sf::Sprite sprite2;
        sprite2.SetImage(*getImage(0));
        sprite2.Resize(configuration->Resolution.w,configuration->Resolution.h);
        sprite2.SetColor(sf::Color((int)configuration->luminosite,(int)configuration->luminosite,(int)configuration->luminosite,255));
        sprite2.SetBlendMode(sf::Blend::Add);
        m_ecran->Draw(sprite2);
    }

    m_ecran->Display();
    Vider();
}

int MoteurGraphique::AjouterImage(const char *Data, std::size_t SizeInBytes, std::string nom,int importance)
{
    for(int i=0;i<(int)m_images.size();i++)
    {
        if(m_images[i].nom==nom)
        {
            m_images[i].importance=importance;
            return i;
        }
        if(m_images[i].nom=="")
        {
            m_images[i].nom=nom;

            if(!configuration->lissage)
                m_images[i].img.SetSmooth(false);

            if(!m_images[i].img.LoadFromMemory(Data,SizeInBytes))
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

    if(!configuration->lissage)
        m_images.back().img.SetSmooth(false);

    if(!m_images.back().img.LoadFromMemory(Data,SizeInBytes))
    {
        console->Ajouter("Impossible de charger depuis la mémoire : "+nom,1);
        return -1;
    }
    else
        console->Ajouter("Chargement de : "+nom,0);

    m_images.back().importance=importance;

    return m_images.size()-1;
}

int MoteurGraphique::AjouterImage(std::string chemin,int importance)
{
    for(int i=0;i<(int)m_images.size();i++)
    {
        if(m_images[i].nom==chemin)
        {
            m_images[i].importance=importance;
            return i;
        }

        if(m_images[i].nom=="")
        {
            m_images[i].nom=chemin;

            if(!configuration->lissage)
                m_images[i].img.SetSmooth(false);

            if(!m_images[i].img.LoadFromFile(chemin.c_str()))
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

    if(!configuration->lissage)
        m_images.back().img.SetSmooth(false);

    if(!m_images.back().img.LoadFromFile(chemin.c_str()))
    {
        console->Ajouter("Impossible de charger : "+chemin,1);
        return -1;
    }
    else
        console->Ajouter("Chargement de : "+chemin,0);

    m_images.back().importance=importance;

    return m_images.size()-1;
}

void MoteurGraphique::DecrementerImportance()
{
    for(int i=0;i<(int)m_images.size();i++)
        if(m_images[i].importance!=-1)
        {
            m_images[i].importance--;
            if(m_images[i].importance<=0)
                m_images[i].nom="",m_images[i].importance=0;
        }
}

int MoteurGraphique::AjouterModeleSystemeParticules(std::string chemin)
{
    for(int i=0;i<(int)m_modeleSystemeParticules.size();i++)
        if(m_modeleSystemeParticules[i].m_chemin==chemin)
            return i;

    m_modeleSystemeParticules.push_back(ModeleParticuleSysteme (chemin));

    return m_modeleSystemeParticules.size()-1;
}


void MoteurGraphique::AjouterSystemeParticules(int ID,coordonnee position,sf::Color color,float force,float angle)
{
    if(ID>=0&&ID<(int)m_modeleSystemeParticules.size())
        m_systemeParticules.push_back(ParticuleSysteme (ID,&m_modeleSystemeParticules[ID],position,color,force,angle));
}

void MoteurGraphique::AjouterCommande(sf::Sprite *sprite, int couche, bool camera)
{
    if(couche>=0&&couche<=20)
        m_commandes[couche].push_back(Commande (sprite,camera));
}

void MoteurGraphique::AjouterTexte(sf::String* string, int couche,bool titre)
{
    sf::String temp(*string);
    if(couche>=0&&couche<=20)
    {

       /* if(temp.GetRect().Right>configuration->Resolution.w)
            temp.SetX(configuration->Resolution.w-(temp.GetRect().Right-temp.GetRect().Left));

        if(temp.GetRect().Left<0)
            temp.SetX(0);

        if(temp.GetRect().Top<0)
            temp.SetY(0);

        if(temp.GetRect().Bottom>configuration->Resolution.h)
            temp.SetY(configuration->Resolution.h-(temp.GetRect().Bottom-temp.GetRect().Top));*/

        if(titre)
        {
            temp.SetFont(m_font_titre);

            temp.SetStyle(sf::String::Bold);
            temp.SetColor(string->GetColor());

            m_textes[couche].push_back(temp);

            temp.SetColor(sf::Color((int)(string->GetColor().r*0.15),(int)(string->GetColor().g*0.15),(int)(string->GetColor().b*0.15),string->GetColor().a));
            temp.SetStyle(sf::String::Regular);
            m_textes[couche].push_back(temp);
        }

        m_textes[couche].push_back(temp);
    }
}

void MoteurGraphique::Vider()
{
    for(int i=0;i<=20;i++)
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
    if(IDimage>=0&&IDimage<(int)m_images.size())
        return &m_images[IDimage].img;
    else
        return &m_images[0].img;
}

ModeleParticuleSysteme* MoteurGraphique::getModeleMoteurParticules(int ID)
{
    if(ID>=0&&ID<(int)m_modeleSystemeParticules.size())
        return &m_modeleSystemeParticules[ID];
    else
        return &m_modeleSystemeParticules[0];
}

std::string MoteurGraphique::getCheminImage(int IDimage)
{
    if(IDimage>=0&&IDimage<(int)m_images.size())
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
    m_ecran->SetView(m_camera);
    coordonnee pos;

    pos.x=(int)m_ecran->ConvertCoords(m_ecran->GetInput().GetMouseX(), m_ecran->GetInput().GetMouseY()).x;
    pos.y=(int)m_ecran->ConvertCoords(m_ecran->GetInput().GetMouseX(), m_ecran->GetInput().GetMouseY()).y;

    return pos;
}

void MoteurGraphique::printscreen()
{
    char buffer[255];
    sf::Image Screen = m_ecran->Capture();
    sprintf(buffer,"screenshot%i.png",configuration->numero_screen);
    Screen.SaveToFile(buffer);
    configuration->numero_screen++;
}

