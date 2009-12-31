

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

#include <iostream>
#include <sstream>

MoteurGraphique::MoteurGraphique()
{
    LightManager = Light_Manager::GetInstance();

    m_images.push_back(Image_moteur ());
    m_images.front().img = new sf::Image();
    m_images.front().img->Create(1024, 1024, sf::Color(255, 255, 255));

    m_images.front().nom         = "O";
    m_images.front().importance  = -1;


    m_tileset.push_back(Tileset_moteur ());
    m_tileset.front().tileset = new Tileset();

    m_tileset.front().nom         = "O";
    m_tileset.front().importance  = -1;

    m_soleil.rouge               = 255;
    m_soleil.vert                = 255;
    m_soleil.bleu                = 255;
    m_soleil.intensite           = 255;

    decalageCamera.x             = 0;
    decalageCamera.y             = 0;
    decalageCameraSouhaite.x     = 0;
    decalageCameraSouhaite.y     = 0;

    cameraDecale                 = false;

   // m_ecran                 = NULL;
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

    m_ecran.Close();
}

void MoteurGraphique::CreateNewWindow()
{
    if (!configuration->mode_fenetre)
        m_ecran.Create(sf::VideoMode(configuration->Resolution.x, configuration->Resolution.y),"HolySpirit : Act of Faith",sf::Style::Fullscreen);
    else
        m_ecran.Create(sf::VideoMode(configuration->Resolution.x, configuration->Resolution.y),"HolySpirit : Act of Faith",sf::Style::Titlebar);

    if (configuration->syncronisation_verticale)
        m_ecran.SetFramerateLimit(60);

    m_ecran.ShowMouseCursor(false);

    if (configuration->postFX)
    {
        EffectBlur.SetTexture("framebuffer", sf::Shader::CurrentTexture);
        EffectBlur2.SetTexture("framebuffer", sf::Shader::CurrentTexture);
        EffectBlurScreen.SetTexture("framebuffer", sf::Shader::CurrentTexture);
        EffectNoir.SetTexture("framebuffer", sf::Shader::CurrentTexture);
        EffectMort.SetTexture("framebuffer", sf::Shader::CurrentTexture);
        EffectContrastes.SetTexture("framebuffer", sf::Shader::CurrentTexture);
        EffectFiltre.SetTexture("framebuffer", sf::Shader::CurrentTexture);
        EffectShadow.SetTexture("framebuffer", sf::Shader::CurrentTexture);
        EffectWater.SetTexture("framebuffer", sf::Shader::CurrentTexture);
    }

    m_ecran.SetActive();

    bufferImage.Create(m_ecran.GetWidth(), m_ecran.GetHeight());
    m_water_screen.Create(m_ecran.GetWidth(), m_ecran.GetHeight());
    m_light_screen.Create(m_ecran.GetWidth()+64, m_ecran.GetHeight()+64);
    m_light_screen2.Create(m_ecran.GetWidth()+64, m_ecran.GetHeight()+64);
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

        if (!EffectBlur2.LoadFromFile(configuration->chemin_fx+configuration->nom_effetBlur))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetBlur,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetBlur,0);

        if (!EffectBlurScreen.LoadFromFile(configuration->chemin_fx+configuration->nom_effetBlur))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetBlur,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetBlur,0);

        EffectBlur.SetParameter("offset",0.02);
        EffectBlur2.SetParameter("offset",0.0075);

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

         if (!EffectShadow.LoadFromFile(configuration->chemin_fx+configuration->nom_effetShadow))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetShadow,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetShadow,0);

        if (!EffectWater.LoadFromFile(configuration->chemin_fx+configuration->nom_effetWater))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetWater,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetWater,0);

        m_img_water = AjouterImage(configuration->water_map,-1);
        EffectWater.SetTexture("water_map", *getImage(m_img_water));
    }

    console->Ajouter("");
    console->Ajouter("Chargement des polices d'écriture :");

    if (!m_font.LoadFromFile(configuration->chemin_fonts+configuration->font))
        console->Ajouter("Impossible de charger : "+configuration->chemin_fonts+configuration->font,1);
    else
        console->Ajouter("Chargement de : "+configuration->chemin_fonts+configuration->font,0);

    if (!m_font_titre.LoadFromFile(configuration->chemin_fonts+configuration->font_titre))
        console->Ajouter("Impossible de charger : "+configuration->chemin_fonts+configuration->font_titre,1);
    else
        console->Ajouter("Chargement de : "+configuration->chemin_fonts+configuration->font_titre,0);



    m_img_corner = AjouterImage(configuration->chemin_menus + configuration->nom_corner,-1);
}

void MoteurGraphique::Gerer(float temps,int tailleMapY)
{
    m_transWater.x += temps*0.005;
    m_transWater.x += decalageReflection.x / 800 / configuration->zoom * 0.014 * 0.5;
    decalageReflection.x = 0;
    if(m_transWater.x > 0.5)
        m_transWater.x = 0;
    if(m_transWater.x < 0)
        m_transWater.x = 0.5;

    m_transWater.y += temps*0.005;
    m_transWater.y -= decalageReflection.y / 600 / configuration->zoom * 0.014 * 0.5;
    decalageReflection.y = 0;
    if(m_transWater.y > 0.5)
        m_transWater.y = 0;
    if(m_transWater.y < 0)
        m_transWater.y = 0.5;


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

    bool decalage = false;
    while(m_effetsEcran_iter!=m_effetsEcran.end())
    {
        m_effetsEcran_iter->temps += temps;

        if(m_effetsEcran_iter->type == TREMBLEMENT && cameraDecale)
        {
            if(m_effetsEcran_iter->temps > 0.05)
            {
                decalage = true;

                m_effetsEcran_iter->temps   = 0;

                int valeur = (int)m_effetsEcran_iter->info1 ;
                if(decalage && (fabs(decalageCameraSouhaite.x) < fabs(valeur) && fabs(valeur) != m_effetsEcran_iter->info1 || fabs(valeur) == m_effetsEcran_iter->info1) || !decalage)
                        decalageCameraSouhaite.x = (1 - 2 * (rand()%100<50))*valeur;

                valeur = (int)m_effetsEcran_iter->info1 ;
                if(decalage && (fabs(decalageCameraSouhaite.y) < fabs(valeur) && fabs(valeur) != m_effetsEcran_iter->info1 || fabs(valeur) == m_effetsEcran_iter->info1) || !decalage)
                        decalageCameraSouhaite.y = (1 - 2 * (rand()%100<50))*valeur;

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
    if(decalage)
        cameraDecale = false;

    if(decalageCameraSouhaite.x < decalageCamera.x)
    {
        decalageCamera.x -= temps * 500 * (fabs(decalageCameraSouhaite.x ) + 1);
        if(decalageCamera.x < decalageCameraSouhaite.x)
            decalageCamera.x = decalageCameraSouhaite.x;
    }
    else if(decalageCameraSouhaite.x > decalageCamera.x)
    {
        decalageCamera.x += temps * 500 * (fabs(decalageCameraSouhaite.x) + 1);
        if(decalageCamera.x > decalageCameraSouhaite.x)
            decalageCamera.x = decalageCameraSouhaite.x;
    }

    if(decalageCameraSouhaite.y < decalageCamera.y)
    {
        decalageCamera.y -= temps * 500 * (fabs(decalageCameraSouhaite.y) + 1);
        if(decalageCamera.y < decalageCameraSouhaite.y)
            decalageCamera.y = decalageCameraSouhaite.y;
    }
    else if(decalageCameraSouhaite.y > decalageCamera.y)
    {
        decalageCamera.y += temps * 500 * (fabs(decalageCameraSouhaite.y) + 1);
        if(decalageCamera.y > decalageCameraSouhaite.y)
            decalageCamera.y = decalageCameraSouhaite.y;
    }

    if(decalageCameraSouhaite.y == decalageCamera.y && decalageCameraSouhaite.x == decalageCamera.x)
        cameraDecale = true;
}

void MoteurGraphique::Afficher()
{
    sf::Sprite sprite;
    sf::Sprite sprite2;

    if (configuration->RafraichirOmbre==1&&configuration->Ombre&&m_soleil.intensite>32)
    {
        sf::View temp = m_camera;
        temp.SetSize(864, 664);
        temp.Zoom(configuration->zoom);

        decalageOmbre=temp.GetCenter();

        m_light_screen2.SetView(temp);

        m_light_screen2.Clear(sf::Color(255,255,255));

        LightManager->DrawWallShadow(&m_light_screen2,&temp);

        sprite.SetBlendMode(sf::Blend::Alpha);

        for (IterCommande=m_commandes[9].begin();IterCommande!=m_commandes[9].end();++IterCommande)
        {
            sprite=IterCommande->m_sprite;
            sprite.SetColor(sf::Color(0,0,0,sprite.GetColor().a));
            m_light_screen2.Draw(sprite);
        }

        m_light_screen2.SetView(m_light_screen2.GetDefaultView());

        sf::Sprite sprite3;
        sprite3.SetImage(*getImage(0));
        sprite3.Resize(configuration->Resolution.w+64,configuration->Resolution.h+64);
        sprite3.SetColor(sf::Color(sf::Color((int)(128+128-m_soleil.intensite*0.5),(int)(128+128-m_soleil.intensite*0.5),(int)(128+128-m_soleil.intensite*0.5))));
        sprite3.SetBlendMode(sf::Blend::Add);
        m_light_screen2.Draw(sprite3);

        m_light_screen2.Display();

        if(configuration->postFX)
            EffectShadow.SetTexture("shadow_map", m_light_screen2.GetImage());

        configuration->RafraichirOmbre=2;
    }

    if (configuration->Lumiere && configuration->RafraichirLumiere)
    {
        sf::View temp = m_camera;
        temp.SetSize(864, 664);
        temp.Zoom(configuration->zoom);

        decalageLumiere=temp.GetCenter();

        m_light_screen.SetView(temp);

        m_light_screen.Clear(sf::Color(m_soleil.rouge*m_soleil.intensite/255,m_soleil.vert*m_soleil.intensite/255,m_soleil.bleu*m_soleil.intensite/255));

        LightManager->Draw(&m_light_screen,&temp);

        m_light_screen.Display();

        configuration->RafraichirLumiere=false;
    }

    bufferImage.Clear();
    m_water_screen.Clear();

    for (int k=0;k<=20;k++)
    {
        if (k==12 && configuration->Lumiere)
        {
            sf::Sprite screen(m_light_screen.GetImage());

            screen.SetBlendMode(sf::Blend::Multiply);

            screen.SetX(decalageLumiere.x-m_camera.GetCenter().x-32);
            screen.SetY(decalageLumiere.y-m_camera.GetCenter().y-32);

            bufferImage.SetView(bufferImage.GetDefaultView());

            if(configuration->postFX)
                bufferImage.Draw(screen, EffectBlur);
            else
                bufferImage.Draw(screen);
        }

        if (k==10 && configuration->Ombre && m_soleil.intensite>32)
        {
            sf::Sprite screen(m_light_screen2.GetImage());

            screen.SetBlendMode(sf::Blend::Multiply);

            screen.SetX(decalageOmbre.x-m_camera.GetCenter().x-32);
            screen.SetY(decalageOmbre.y-m_camera.GetCenter().y-32);

            bufferImage.SetView(bufferImage.GetDefaultView());

            if(configuration->postFX)
                bufferImage.Draw(screen, EffectBlur2);
            else
                bufferImage.Draw(screen);
        }

        if (k!=9)
        {
            for (IterCommande=m_commandes[k].begin();IterCommande!=m_commandes[k].end();++IterCommande)
            {
                if(k==0)
                    IterCommande->m_sprite.SetColor(sf::Color(m_soleil.rouge*m_soleil.intensite/255,m_soleil.vert*m_soleil.intensite/255,m_soleil.bleu*m_soleil.intensite/255));

                if(k == 0 && configuration->postFX && configuration->Reflection)
                {
                    if (IterCommande->m_utiliserCamera)
                        m_water_screen.SetView(m_camera);
                    else
                        m_water_screen.SetView(m_water_screen.GetDefaultView());

                    m_water_screen.Draw(IterCommande->m_sprite,EffectFiltre);
                }
                else
                {
                    if (IterCommande->m_utiliserCamera)
                        bufferImage.SetView(m_camera);
                    else
                        bufferImage.SetView(bufferImage.GetDefaultView());

                    if(k < 12 && configuration->postFX)
                        bufferImage.Draw(IterCommande->m_sprite,EffectFiltre);
                    else
                        bufferImage.Draw(IterCommande->m_sprite);
                    /*EffectShadow.SetParameter("coord", IterCommande->m_sprite.TransformToGlobal(sf::Vector2f(0,0)).x/configuration->Resolution.x
                                                     , IterCommande->m_sprite.TransformToGlobal(sf::Vector2f(0,0)).y/configuration->Resolution.y);
                    EffectShadow.SetParameter("size" , IterCommande->m_sprite.GetSize().x/configuration->Resolution.x
                                                     , IterCommande->m_sprite.GetSize().y/configuration->Resolution.y);
                    bufferImage.Draw(IterCommande->m_sprite,EffectShadow);*/
                }
            }
        }

        bufferImage.SetView(bufferImage.GetDefaultView());

        if(k == 0 && configuration->postFX && configuration->Reflection)
        {
            m_water_screen.Display();
            EffectWater.SetParameter("translation", m_transWater.x, m_transWater.y);
            EffectWater.SetParameter("offset", configuration->zoom , configuration->zoom );
            bufferImage.Draw(sf::Sprite (m_water_screen.GetImage()), EffectWater);
        }

        for (unsigned i=0;i<m_textes[k].size();i++)
            bufferImage.Draw(m_textes[k][i]);


        if (k==13)
        {
            if (configuration->luminosite>0)
            {
                sf::Sprite sprite2;
                sprite2.SetImage(*getImage(0));
                sprite2.Resize(configuration->Resolution.w,configuration->Resolution.h);
                sprite2.SetColor(sf::Color((int)configuration->luminosite*2,(int)configuration->luminosite*2,(int)configuration->luminosite*2,255));
                sprite2.SetBlendMode(sf::Blend::Add);
                bufferImage.Draw(sprite2);
            }

            if (configuration->contrastes>1 && configuration->postFX)
            {
                bufferImage.SetView(bufferImage.GetDefaultView());
                bufferImage.Draw(sf::Sprite(bufferImage.GetImage()), EffectContrastes);
                EffectContrastes.SetParameter("color", configuration->contrastes-1, configuration->contrastes-1, configuration->contrastes-1);
            }

            if (configuration->effetMort>0 && configuration->postFX)
            {
                bufferImage.SetView(bufferImage.GetDefaultView());
                bufferImage.Draw(sf::Sprite(bufferImage.GetImage()), EffectMort);
            }
        }


        if (k==18 && configuration->postFX)
        {
            if (m_blur>0)
            {
                bufferImage.SetView(bufferImage.GetDefaultView());
                bufferImage.Draw(sf::Sprite(bufferImage.GetImage()), EffectBlurScreen);
                EffectBlurScreen.SetParameter("offset",(float)m_blur);
            }
        }

        if (configuration->effetNoir>0 && k == 18)
        {
            sf::Sprite sprite2;
            sprite2.SetImage(*getImage(0));
            sprite2.Resize(configuration->Resolution.w,configuration->Resolution.h);
            sprite2.SetColor(sf::Color((int)(configuration->effetNoir*255),(int)(configuration->effetNoir*255),(int)(configuration->effetNoir*255),255));
            sprite2.SetBlendMode(sf::Blend::Multiply);
            bufferImage.Draw(sprite2);
        }

        m_commandes[k].clear();
        m_textes[k].clear();
    }



   // m_ecran.Clear();
    bufferImage.Display();
    m_ecran.Draw(sf::Sprite(bufferImage.GetImage()));
    m_ecran.Display();
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
    if(chemin != "")
        for (unsigned i=0; i < m_tileset.size(); i++)
        {
            if (m_tileset[i].nom == chemin)
            {
                m_tileset[i].tileset->ChargerImages();
                m_tileset[i].importance = importance;

                Tileset buffer(fichier);

                return i;
            }
        }

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

void MoteurGraphique::AjouterEntiteGraphique(Entite_graphique *entite)
{
    if(entite->m_tileset != NULL)
    {
        if(entite->m_sprite.GetPosition().x + entite->m_sprite.GetSize().x - entite->m_sprite.GetOrigin().x     >= m_camera.GetRect().Left
        && entite->m_sprite.GetPosition().x - entite->m_sprite.GetOrigin().x                                    <  m_camera.GetRect().Right
        && entite->m_sprite.GetPosition().y + entite->m_sprite.GetSize().y - entite->m_sprite.GetOrigin().y     >= m_camera.GetRect().Top
        && entite->m_sprite.GetPosition().y - entite->m_sprite.GetOrigin().y                                    <  m_camera.GetRect().Bottom
        || entite->m_sprite.GetRotation() != 0)
            AjouterCommande(&entite->m_sprite, entite->m_couche + entite->m_decalCouche, true);

        if(entite->m_shadow)
        {
            sf::Sprite sprite;
            sprite = entite->m_sprite;

            sprite.SetScale(1, (100-(float)m_soleil.hauteur)/50);
            sprite.SetRotation(m_angleOmbreSoleil);

            AjouterCommande(&sprite, 9, true);
        }

        if(entite->m_reflect)
        {
            sf::Sprite sprite;
            sprite = entite->m_sprite;

            sprite.FlipY(true);
            sprite.SetOrigin(sprite.GetOrigin().x, sprite.GetSize().y - sprite.GetOrigin().y);

            if(sprite.GetPosition().x + sprite.GetSize().x - sprite.GetOrigin().x     >= m_camera.GetRect().Left
            && sprite.GetPosition().x - sprite.GetOrigin().x                          <  m_camera.GetRect().Right
            && sprite.GetPosition().y + sprite.GetSize().y - sprite.GetOrigin().y     >= m_camera.GetRect().Top
            && sprite.GetPosition().y - sprite.GetOrigin().y                          <  m_camera.GetRect().Bottom)
                AjouterCommande(&sprite, 0, true);
        }
    }
}

void MoteurGraphique::AjouterCommande(sf::Sprite *sprite, int couche, bool camera)
{
    if (couche>=0&&couche<=20)
        m_commandes[couche].push_back(Commande (sprite,camera));
}

void MoteurGraphique::AjouterTexte(std::string txt, coordonnee pos, int couche, bool titre, int size, sf::Color color, bool fond)
{
    sf::Text temp;
    temp.SetFont(m_font);
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
        temp2.Resize(temp.GetRect().Right - temp.GetRect().Left + 4,
                     temp.GetRect().Bottom - temp.GetRect().Top + 4);
        temp2.SetColor(sf::Color(0,0,0,224));
        AjouterCommande(&temp2, couche, 0);
    }
}

void MoteurGraphique::AjouterTexteNonChevauchable(sf::Text* string, int couche, bool titre)
{
    if (couche>=0&&couche<=20)
    {
        for (int i=0;i<(int)m_textes[couche].size();i++)
        {
            if(string->GetRect().Right  > m_textes[couche][i].GetRect().Left
            && string->GetRect().Left   < m_textes[couche][i].GetRect().Right
            && string->GetRect().Bottom > m_textes[couche][i].GetRect().Top - 3
            && string->GetRect().Top    < m_textes[couche][i].GetRect().Bottom)
                string->SetPosition(string->GetPosition().x, m_textes[couche][i].GetRect().Top - string->GetRect().Bottom + string->GetRect().Top - 5), i = -1;
        }

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

            /*temp.SetStyle(sf::Text::Bold);
            temp.SetColor(string->GetColor());

            m_textes[couche].push_back(temp);

            temp.SetColor(sf::Color((int)(string->GetColor().r*0.15),(int)(string->GetColor().g*0.15),(int)(string->GetColor().b*0.15),string->GetColor().a));
            temp.SetStyle(sf::Text::Regular);*/
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

sf::Image* MoteurGraphique::getImage(int IDimage)
{
    if (IDimage>=0&&IDimage<(int)m_images.size())
        return m_images[(unsigned)IDimage].img;
    return m_images.front().img;
}

Tileset* MoteurGraphique::getTileset(int IDtileset)
{
    if (IDtileset>=0 && IDtileset<(int)m_tileset.size())
        return m_tileset[(unsigned)IDtileset].tileset;
    return m_tileset.front().tileset;
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
        return m_images[(unsigned)IDimage].nom;
    return "";
}

bool MoteurGraphique::getEvent(sf::Event &EventReceived)
{
    return m_ecran.GetEvent(EventReceived);
}

coordonnee MoteurGraphique::getPositionSouris()
{
//   m_ecran.SetView(m_camera);
    coordonnee pos;

    pos.x=(int)m_ecran.ConvertCoords(m_ecran.GetInput().GetMouseX(), m_ecran.GetInput().GetMouseY(), m_camera).x;
    pos.y=(int)m_ecran.ConvertCoords(m_ecran.GetInput().GetMouseX(), m_ecran.GetInput().GetMouseY(), m_camera).y;

    return pos;
}

void MoteurGraphique::Printscreen()
{
    std::ostringstream buf;
    buf<<"screenshot"<<configuration->numero_screen<<".png";
    sf::Image Screen;
    Screen.CopyScreen(m_ecran);
    Screen.SaveToFile(buf.str());
    configuration->numero_screen++;
}

int MoteurGraphique::GetFPS()
{
    return (int)( 1.f / m_ecran.GetFrameTime());
}

