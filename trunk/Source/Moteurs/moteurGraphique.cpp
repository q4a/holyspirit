

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
    m_images.front().img = new sf::Texture();
    sf::Image img;
    img.create(8, 8, sf::Color(255, 255, 255));
    m_images.front().img->loadFromImage(img);


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
}

MoteurGraphique::~MoteurGraphique()
{
    LightManager->Kill();
    Vider();
    for (unsigned i = 0; i < m_tileset.size(); ++i)
        if(m_tileset[i].tileset)
            delete m_tileset[i].tileset;
    m_systemeParticules.clear();
    m_modeleSystemeParticules.clear();
    m_ecran.close();

    for (unsigned i = 0; i < m_images.size(); ++i)
        if(m_images[i].img)
            delete m_images[i].img;
    m_images.clear();
}

void MoteurGraphique::CreateNewWindow()
{
    if (!configuration->mode_fenetre)
        m_ecran.create(sf::VideoMode(configuration->Resolution.x, configuration->Resolution.y),"HolySpirit : Act of Faith",sf::Style::Fullscreen);
    else
        m_ecran.create(sf::VideoMode(configuration->Resolution.x, configuration->Resolution.y),"HolySpirit : Act of Faith",sf::Style::Titlebar);

  // if (configuration->syncronisation_verticale)
     //   m_ecran.EnableVerticalSync(true);

    //FIXME m_ecran.showMouseCursor(false);
    m_ecran.setMouseCursorVisible(false);

    if (configuration->postFX)
    {
        /*EffectBlur.SetCurrentTexture("texture");
        EffectBlur2.SetCurrentTexture("texture");
        EffectBlurScreen.SetCurrentTexture("texture");
        EffectMort.SetCurrentTexture("framebuffer");
        EffectFiltre.SetCurrentTexture("framebuffer");
        EffectWater.SetCurrentTexture("framebuffer");
        EffectDistortion.SetCurrentTexture("framebuffer");*/

        EffectDistortion.setParameter("distortion_map",m_distortion_screen.getTexture());
    }

    m_ecran.setActive();

    bufferImage.create(m_ecran.getSize().x, m_ecran.getSize().y);
    m_water_screen.create(m_ecran.getSize().x, m_ecran.getSize().y);
    m_distortion_screen.create(m_ecran.getSize().x, m_ecran.getSize().y);
    m_light_screen.create(m_ecran.getSize().x+64, m_ecran.getSize().y+64);
    m_light_screen2.create(m_ecran.getSize().x+64, m_ecran.getSize().y+64);
}

void MoteurGraphique::Charger()
{
    configuration->effetMort=0;
    if (configuration->postFX)
    {
        console->Ajouter("");
        console->Ajouter("Loading postFX :");

        if (!EffectBlur.loadFromFile(configuration->chemin_fx+configuration->nom_effetBlur, sf::Shader::Fragment))
            console->Ajouter("Unable to load : "+configuration->chemin_fx+configuration->nom_effetBlur,1);
        else
            console->Ajouter("Loading : "+configuration->chemin_fx+configuration->nom_effetBlur,0);

        if (!EffectBlur2.loadFromFile(configuration->chemin_fx+configuration->nom_effetBlur, sf::Shader::Fragment))
            console->Ajouter("Unable to load : "+configuration->chemin_fx+configuration->nom_effetBlur,1);
        else
            console->Ajouter("Loading : "+configuration->chemin_fx+configuration->nom_effetBlur,0);

        if (!EffectBlurScreen.loadFromFile(configuration->chemin_fx+configuration->nom_effetBlur, sf::Shader::Fragment))
            console->Ajouter("Unable to load : "+configuration->chemin_fx+configuration->nom_effetBlur,1);
        else
            console->Ajouter("Loading : "+configuration->chemin_fx+configuration->nom_effetBlur,0);

        EffectBlur.setParameter("offset",0.02);
        EffectBlur2.setParameter("offset",0.005);

        if (!EffectMort.loadFromFile(configuration->chemin_fx+configuration->nom_effetMort, sf::Shader::Fragment))
            console->Ajouter("Unable to load : "+configuration->chemin_fx+configuration->nom_effetMort,1);
        else
            console->Ajouter("Loading : "+configuration->chemin_fx+configuration->nom_effetMort,0);

        EffectMort.setParameter("offset", 0);
        EffectMort.setParameter("color",1, 1, 1);



        if (!EffectFiltre.loadFromFile(configuration->chemin_fx+configuration->nom_effetFiltre, sf::Shader::Fragment))
            console->Ajouter("Unable to load : "+configuration->chemin_fx+configuration->nom_effetFiltre,1);
        else
            console->Ajouter("Loading : "+configuration->chemin_fx+configuration->nom_effetFiltre,0);

        if (!EffectWater.loadFromFile(configuration->chemin_fx+configuration->nom_effetWater, sf::Shader::Fragment))
            console->Ajouter("Unable to load : "+configuration->chemin_fx+configuration->nom_effetWater,1);
        else
            console->Ajouter("Loading : "+configuration->chemin_fx+configuration->nom_effetWater,0);

        if (!EffectDistortion.loadFromFile(configuration->chemin_fx+configuration->nom_effetDistortion, sf::Shader::Fragment))
            console->Ajouter("Unable to load : "+configuration->chemin_fx+configuration->nom_effetDistortion,1);
        else
            console->Ajouter("Loading : "+configuration->chemin_fx+configuration->nom_effetDistortion,0);

        m_img_water = AjouterImage(configuration->water_map,-1);
        EffectWater.setParameter("water_map", *getImage(m_img_water));
    }

    console->Ajouter("");
    console->Ajouter("Loading writing policies :");

    if (!m_font.loadFromFile(configuration->chemin_fonts+configuration->font))
        console->Ajouter("Unable to load : "+configuration->chemin_fonts+configuration->font,1);
    else
        console->Ajouter("Loading : "+configuration->chemin_fonts+configuration->font,0);

    if (!m_font_titre.loadFromFile(configuration->chemin_fonts+configuration->font_titre))
        console->Ajouter("Unable to load : "+configuration->chemin_fonts+configuration->font_titre,1);
    else
        console->Ajouter("Loading : "+configuration->chemin_fonts+configuration->font_titre,0);

    special_typo.Load(configuration->chemin_fonts+configuration->spec_font);
    special_typo_h.Load(configuration->chemin_fonts+configuration->spec_font_h);
    special_typo_p.Load(configuration->chemin_fonts+configuration->spec_font_p);

    m_img_item_background = AjouterImage(configuration->chemin_menus + configuration->nom_item_background,-1);
    m_img_item_unusable = AjouterImage(configuration->chemin_menus + configuration->nom_item_unusable,-1);

    m_ambientShadowTileset.Charger(configuration->ambientShadow_tileset);
}

void MoteurGraphique::Gerer(float temps)
{
    m_fps = (int)( 1.f /m_fps_clock.getElapsedTime().asSeconds());
    m_fps_clock.restart();

    if(!configuration->mode_fenetre)
    {
        if(sf::Mouse::getPosition(m_ecran).x > configuration->Resolution.x - 1)
            sf::Mouse::setPosition(sf::Vector2i(configuration->Resolution.x - 1,sf::Mouse::getPosition(m_ecran).y),m_ecran);
        if(sf::Mouse::getPosition(m_ecran).y > configuration->Resolution.y - 1)
            sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition(m_ecran).x,configuration->Resolution.y - 1),m_ecran);
    }


    m_transWater.x += temps*0.005;
    m_transWater.x += decalageReflection.x / configuration->Resolution.x / configuration->zoom * 0.014 * 0.5;
    decalageReflection.x = 0;
    if(m_transWater.x > 0.5)
        m_transWater.x = 0;
    if(m_transWater.x < 0)
        m_transWater.x = 0.5;

    m_transWater.y += temps*0.005;
    m_transWater.y -= decalageReflection.y / configuration->Resolution.y / configuration->zoom * 0.014 * 0.5;
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
            if (!m_systemeParticules_iter->Afficher(&m_modeleSystemeParticules[m_systemeParticules_iter->m_modele],temps))
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

        if(m_effetsEcran_iter->type == TREMBLEMENT)
        {
            if(m_effetsEcran_iter->temps > 0.05 && cameraDecale)
            {
                decalage = true;

                m_effetsEcran_iter->temps   = 0;

                int valeur = (int)m_effetsEcran_iter->info1 ;
                if(((decalage && (fabs(decalageCameraSouhaite.x) < abs(valeur) && abs(valeur) != m_effetsEcran_iter->info1))
                 || abs(valeur) == m_effetsEcran_iter->info1) || !decalage)
                        decalageCameraSouhaite.x = (1 - 2 * (rand()%100<50))*valeur;

                valeur = (int)m_effetsEcran_iter->info1 ;
                if(((decalage && (fabs(decalageCameraSouhaite.y) < abs(valeur) && abs(valeur) != m_effetsEcran_iter->info1))
                 || abs(valeur) == m_effetsEcran_iter->info1) || !decalage)
                        decalageCameraSouhaite.y = (1 - 2 * (rand()%100<50))*valeur;

                m_effetsEcran_iter->info1  -= 1;
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
        else if(m_effetsEcran_iter->type == ENVOLEMENT)
        {
            for(m_systemeParticules_iter=m_systemeParticules.begin();
                m_systemeParticules_iter!=m_systemeParticules.end();
                ++m_systemeParticules_iter)
                    m_systemeParticules_iter->Envoler(m_effetsEcran_iter->pos,(int)m_effetsEcran_iter->info1,(int)m_effetsEcran_iter->info2);

            m_effetsEcran.erase (m_effetsEcran_iter);
            if((int)m_effetsEcran.size()>k)
                m_effetsEcran_iter=m_effetsEcran.begin()+k;
        }
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

    if (configuration->RafraichirOmbre==1&&configuration->Ombre&&m_soleil.intensite>32)
    {
        sf::View temp = m_camera;
        temp.setSize(configuration->Resolution.x + 64, configuration->Resolution.y + 64);
        temp.zoom(configuration->zoom);

        decalageOmbre=temp.getCenter();



        m_light_screen2.setView(temp);

        m_light_screen2.clear(sf::Color(255,255,255,255));

        LightManager->DrawWallShadow(&m_light_screen2,&temp,m_angleOmbreSoleil,m_soleil);

        for (IterCommande=m_commandes[9].begin();IterCommande!=m_commandes[9].end();++IterCommande)
        {
            IterCommande->m_sprite.setColor(sf::Color(0,0,0,IterCommande->m_sprite.getColor().a));
            m_light_screen2.draw(IterCommande->m_sprite);
        }

        m_light_screen2.setView(m_light_screen2.getDefaultView());

        MySprite sprite3;
        sprite3.setTexture(*getImage(0));
        sprite3.resize(configuration->Resolution.w+64,configuration->Resolution.h+64);
        sprite3.setColor(sf::Color(sf::Color((int)(128+128-m_soleil.intensite*0.5),(int)(128+128-m_soleil.intensite*0.5),(int)(128+128-m_soleil.intensite*0.5))));
        m_light_screen2.draw(sprite3, sf::BlendAdd);

        m_light_screen2.display();


        if(configuration->postFX)
        {
            EffectBlur2.setParameter("direction_x", 1.0);
            sf::Sprite buf;
            buf.setTexture(m_light_screen2.getTexture());
            m_light_screen2.setView(m_light_screen2.getDefaultView());
            m_light_screen2.draw(buf, &EffectBlur2);
            m_light_screen2.display();
            EffectBlur2.setParameter("direction_x", 0);
        }
    }

    if (configuration->Lumiere > 0 && configuration->RafraichirLumiere)
    {
        m_light_screen.setView(m_light_screen.getDefaultView());

        m_light_screen.clear(sf::Color(m_soleil.rouge * m_soleil.intensite/255,
                                       m_soleil.vert  * m_soleil.intensite/255,
                                       m_soleil.bleu  * m_soleil.intensite/255,LIGHT_ALPHA));


        sf::View temp = m_camera;
        temp.setSize(configuration->Resolution.x + 64, configuration->Resolution.y + 64);
        temp.zoom(configuration->zoom);

        decalageLumiere=temp.getCenter();

        m_light_screen.setView(temp);

        LightManager->Draw(&m_light_screen,&temp);

        m_light_screen.display();

        if(configuration->postFX)
        {
            EffectBlur.setParameter("direction_x", 1.0);
            sf::Sprite buf;
            buf.setTexture(m_light_screen.getTexture());
            m_light_screen.setView(m_light_screen.getDefaultView());
            m_light_screen.draw(buf, &EffectBlur);
            m_light_screen.display();
            EffectBlur.setParameter("direction_x", 0);
        }
    }

    if(configuration->Reflection && configuration->postFX)
        m_water_screen.clear(sf::Color(0,0,0,255));
    else
        bufferImage.clear(sf::Color(0,0,0,255));

    if(configuration->Distortion)
        m_distortion_screen.clear(sf::Color(128,128,128,255));

    if(configuration->postFX && configuration->Distortion)
    for (IterCommande=m_distortion_commandes.begin();IterCommande!=m_distortion_commandes.end();++IterCommande)
    {
        if (IterCommande->m_utiliserCamera)
            m_distortion_screen.setView(m_camera);
        else
            m_distortion_screen.setView(m_distortion_screen.getDefaultView());

        m_distortion_screen.draw(IterCommande->m_sprite);
    }
    m_distortion_commandes.clear();

    for (int k=0;k<=20;++k)
    {
        if (k==12 && configuration->Lumiere)
        {
            sf::Sprite screen(m_light_screen.getTexture());

            sf::RenderStates r(sf::BlendMultiply);

            screen.setPosition(decalageLumiere.x-m_camera.getCenter().x-32 ,
                               decalageLumiere.y-m_camera.getCenter().y-32);

            bufferImage.setView(bufferImage.getDefaultView());

            if(configuration->postFX)
                r.shader = &EffectBlur;

            bufferImage.draw(screen, r);
        }

        if (k==10 && configuration->Ombre && m_soleil.intensite>32)
        {
            sf::Sprite screen(m_light_screen2.getTexture());

            sf::RenderStates r(sf::BlendMultiply);

            screen.setPosition(decalageOmbre.x-m_camera.getCenter().x-32 ,
                               decalageOmbre.y-m_camera.getCenter().y-32);

            bufferImage.setView(bufferImage.getDefaultView());

            if(configuration->postFX)
                r.shader = &EffectBlur2;

            bufferImage.draw(screen,r);
        }

        if (k==12 && configuration->postFX && configuration->Distortion)
        {
            bufferImage.setView(bufferImage.getDefaultView());

            m_distortion_screen.display();

           // EffectDistortion.SetCurrentTexture("framebuffer");
            EffectDistortion.setParameter("distortion_map", m_distortion_screen.getTexture());

            bufferImage.display();

            bufferImage.draw(sf::Sprite(bufferImage.getTexture()), &EffectDistortion);
        }

        if (k!=9)
        {
            for (IterCommande=m_commandes[k].begin();IterCommande!=m_commandes[k].end();++IterCommande)
            {
                if(k == 0)
                    IterCommande->m_sprite.setColor(sf::Color(128+m_soleil.rouge*m_soleil.intensite/512,
                                                              128+m_soleil.vert*m_soleil.intensite/512,
                                                              128+m_soleil.bleu*m_soleil.intensite/512,IterCommande->m_sprite.getColor().a));

                if(k == 0 && configuration->postFX && configuration->Reflection)
                {
                    if (IterCommande->m_utiliserCamera)
                        m_water_screen.setView(m_camera);
                    else
                        m_water_screen.setView(m_water_screen.getDefaultView());

                    sf::RenderStates r(IterCommande->m_states);
                    r.shader = &EffectFiltre;

                    m_water_screen.draw(IterCommande->m_sprite,r);
                }
                else
                {
                    if (IterCommande->m_utiliserCamera)
                        bufferImage.setView(m_camera);
                    else
                        bufferImage.setView(bufferImage.getDefaultView());


                    sf::RenderStates r(IterCommande->m_states);

                    if(k < 12 && configuration->postFX)
                        r.shader = &EffectFiltre;

                    bufferImage.draw(IterCommande->m_sprite,r);
                }
            }
        }

        bufferImage.setView(bufferImage.getDefaultView());

        if(k == 0 && configuration->postFX && configuration->Reflection)
        {
            m_water_screen.display();
            EffectWater.setParameter("translation", m_transWater.x, m_transWater.y);
            EffectWater.setParameter("offset", 1/configuration->zoom , 1/configuration->zoom );
            bufferImage.draw(sf::Sprite (m_water_screen.getTexture()), &EffectWater);
        }

        for (IterTextes=m_textes[k].begin();IterTextes!=m_textes[k].end();++IterTextes)
            bufferImage.draw(*IterTextes);


        if (k==13)
        {
            EffectFiltre.setParameter("color", configuration->contrastes-1, configuration->contrastes-1, configuration->contrastes-1);
            EffectFiltre.setParameter("luminosity", configuration->luminosite/128);

            if (configuration->effetMort>0 && configuration->postFX)
            {
                bufferImage.setView(bufferImage.getDefaultView());
                bufferImage.display();
                bufferImage.draw(sf::Sprite(bufferImage.getTexture()), &EffectMort);
            }
        }


        if (k==18 && configuration->postFX)
        {
            if (m_blur>0)
            {
                bufferImage.setView(bufferImage.getDefaultView());
                EffectBlurScreen.setParameter("offset",(float)m_blur);
                bufferImage.display();
                bufferImage.draw(sf::Sprite(bufferImage.getTexture()), &EffectBlurScreen);
            }
        }

        if (configuration->effetNoir>0 && k == 18)
        {
            MySprite sprite2;
            sprite2.setTexture(*getImage(0));
            sprite2.resize(configuration->Resolution.x,configuration->Resolution.y);
            sprite2.setColor(sf::Color((int)(configuration->effetNoir*255),(int)(configuration->effetNoir*255),(int)(configuration->effetNoir*255),255));
            bufferImage.draw(sprite2,sf::BlendMultiply);
        }

        m_commandes[k].clear();
        m_textes[k].clear();
    }

    bufferImage.display();
    m_ecran.draw(sf::Sprite(bufferImage.getTexture()));
    m_ecran.display();
}

int MoteurGraphique::AjouterImage(const char *Data, std::size_t SizeInBytes, std::string nom,int importance, bool newimage)
{
    if(!newimage)
    {
        for (unsigned i=0; i < m_images.size(); i++)
        {
            if (m_images[i].nom==nom && m_images[i].img != NULL)
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

                m_images[i].img = new sf::Texture();
                m_images[i].img->setSmooth(true);

                if (!configuration->lissage)
                    m_images[i].img->setSmooth(false);

                if (!m_images[i].img->loadFromMemory(Data,SizeInBytes))
                {
                    console->Ajouter("Unable to load : "+nom,1);
                    return -1;
                }
                else
                    console->Ajouter("Loading : "+nom,0);

                m_images[i].importance=importance;

                return i;
            }
        }
    }


    m_images.push_back(Image_moteur ());
    m_images.back().nom=nom;

    m_images.back().img = new sf::Texture();
    m_images.back().img->setSmooth(true);

    if (!configuration->lissage)
        m_images.back().img->setSmooth(false);

    if (!m_images.back().img->loadFromMemory(Data,SizeInBytes))
    {
        console->Ajouter("Unable to load from memory : "+nom,1);
        return -1;
    }
    else
        console->Ajouter("Loading : "+nom,0);

    m_images.back().importance=importance;

    return m_images.size()-1;
}

int MoteurGraphique::AjouterImage(std::string chemin,int importance,bool newimage)
{
    if(!newimage)
    {
        for (unsigned i=0; i < m_images.size(); i++)
        {
            if (m_images[i].nom==chemin && m_images[i].img != NULL)
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

                m_images[i].img = new sf::Texture();
                m_images[i].img->setSmooth(true);

                if (!configuration->lissage)
                    m_images[i].img->setSmooth(false);

                if (!m_images[i].img->loadFromFile(chemin.c_str()))
                {
                    console->Ajouter("Unable to load : "+chemin,1);
                    return -1;
                }
                else
                    console->Ajouter("Loading : "+chemin,0);

                m_images[i].importance=importance;

                return i;
            }
        }
    }


    m_images.push_back(Image_moteur ());
    m_images.back().nom=chemin;

    m_images.back().img = new sf::Texture();
    m_images.back().img->setSmooth(true);

    if (!configuration->lissage)
        m_images.back().img->setSmooth(false);

    if (!m_images.back().img->loadFromFile(chemin.c_str()))
    {
        console->Ajouter("Unable to load : "+chemin,1);
        return -1;
    }
    else
        console->Ajouter("Loading : "+chemin,0);

    m_images.back().importance=importance;

    return m_images.size()-1;
}


int MoteurGraphique::AjouterImage(const sf::Texture &img,int importance)
{
    m_images.push_back(Image_moteur ());
    m_images.back().nom="";

    m_images.back().img = new sf::Texture();
    (*m_images.back().img) = img;
    m_images.back().img->setSmooth(true);

    //if (!configuration->lissage)
       // m_images.back().img->setSmooth(false);

    m_images.back().importance=importance;

    return m_images.size()-1;
}

int MoteurGraphique::AjouterTileset(std::string chemin,int importance)
{
    std::ifstream fichier;
    fichier.open(chemin.c_str(), std::ios::in);
    int retour = AjouterTileset(fichier, chemin, importance);
    console->Ajouter("Loading : "+chemin,0);
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
                m_tileset[i].tileset->Charger(fichier);
                m_tileset[i].importance = importance;

               // Tileset buffer(fichier);

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
        if (m_images[i].importance!=-1 && m_images[i].img != NULL)
        {
            m_images[i].importance--;
            if (m_images[i].importance<=0)
            {
                delete m_images[i].img;
                m_images[i].img = NULL;
                m_images[i].nom="",m_images[i].importance=0;
            }
        }


    for (unsigned i = 0; i < m_tileset.size(); ++i)
        if(m_tileset[i].tileset)
            delete m_tileset[i].tileset;
    m_tileset.clear();
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

void MoteurGraphique::AjouterEntiteGraphique(Entite_graphique *entite,  const sf::RenderStates& states)
{
    if(entite->m_tileset != NULL)
    {
        if(entite->m_ambientShadow >= 0)
        {
            sf::Sprite sprite2;
            coordonnee positionPartieDecor = m_ambientShadowTileset.getPositionDuTile(entite->m_ambientShadow);

            sprite2.setTexture(*moteurGraphique->getImage(m_ambientShadowTileset.getImage(entite->m_ambientShadow)));
            sprite2.setTextureRect(sf::IntRect(positionPartieDecor.x, positionPartieDecor.y,
                                          positionPartieDecor.w, positionPartieDecor.h - 1));

            sprite2.setOrigin(m_ambientShadowTileset.getCentreDuTile(entite->m_ambientShadow).x,
                              m_ambientShadowTileset.getCentreDuTile(entite->m_ambientShadow).y);

            sprite2.setPosition(entite->getPosition());

            sprite2.move(entite->m_decalage.x,entite->m_decalage.y);

            AjouterCommande(&sprite2, 10 + entite->m_decalCouche, !entite->m_fixed);
        }

        if(entite->getGlobalBounds().width > 0 && entite->getGlobalBounds().height > 0)
        {
            {
                sf::Sprite sprite = *entite;
                sprite.move(entite->m_decalage.x,entite->m_decalage.y);

                if(sprite.getScale().x < 0)
                    sprite.setOrigin(sprite.getLocalBounds().width - sprite.getOrigin().x,
                                     sprite.getOrigin().y);

                if((sprite.getPosition().x + sprite.getGlobalBounds().width - sprite.getOrigin().x     >= GetViewRect(m_camera).left
                 && sprite.getPosition().x - sprite.getOrigin().x                          <  GetViewRect(m_camera).left + GetViewRect(m_camera).width
                 && sprite.getPosition().y + sprite.getGlobalBounds().height - sprite.getOrigin().y     >= GetViewRect(m_camera).top
                 && sprite.getPosition().y - sprite.getOrigin().y                          <  GetViewRect(m_camera).top + GetViewRect(m_camera).height)
                || sprite.getRotation() != 0 || entite->m_fixed)
                    AjouterCommande(&sprite, entite->m_couche + entite->m_decalCouche, !entite->m_fixed, states);
            }

            if(entite->m_shadow && configuration->Ombre && entite->m_couche + entite->m_decalCouche >= 10)
            {
                sf::Sprite sprite;
                sprite = *entite;
                sprite.move(entite->m_decalage.x,entite->m_decalage.y);

                sprite.setScale(1, (100-(float)m_soleil.hauteur)/50);
                sprite.setRotation(-m_angleOmbreSoleil);

                AjouterCommande(&sprite, 9, !entite->m_fixed);
            }

            if(entite->m_reflect && configuration->Reflection)
            {
                sf::Sprite sprite;
                sprite = *entite;
                sprite.move(entite->m_decalage.x,entite->m_decalage.y);

                sprite.scale(1,-1);

                //sprite.FlipX(true);
                sprite.setOrigin(sprite.getLocalBounds().width - sprite.getOrigin().x,
                                 sprite.getGlobalBounds().height - sprite.getOrigin().y);

                if(sprite.getPosition().x + sprite.getGlobalBounds().width - sprite.getOrigin().x     >= GetViewRect(m_camera).left
                && sprite.getPosition().x - sprite.getOrigin().x                          <  GetViewRect(m_camera).left + GetViewRect(m_camera).width
                && sprite.getPosition().y + sprite.getGlobalBounds().height - sprite.getOrigin().y     >= GetViewRect(m_camera).top
                && sprite.getPosition().y - sprite.getOrigin().y                          <  GetViewRect(m_camera).top + GetViewRect(m_camera).height)
                    AjouterCommande(&sprite, 0, !entite->m_fixed);
            }
        }



        if(entite->m_distort && configuration->Distortion)
        {
           if((entite->m_sprite_distortion.getPosition().x + entite->m_sprite_distortion.getGlobalBounds().width - entite->m_sprite_distortion.getOrigin().x    >= GetViewRect(m_camera).left
            && entite->m_sprite_distortion.getPosition().x - entite->m_sprite_distortion.getOrigin().x                                              <  GetViewRect(m_camera).left + GetViewRect(m_camera).width
            && entite->m_sprite_distortion.getPosition().y + entite->m_sprite_distortion.getGlobalBounds().height - entite->m_sprite_distortion.getOrigin().y    >= GetViewRect(m_camera).top
            && entite->m_sprite_distortion.getPosition().y - entite->m_sprite_distortion.getOrigin().y)                                           <  GetViewRect(m_camera).top + GetViewRect(m_camera).height
            || entite->m_sprite_distortion.getRotation() != 0)
                m_distortion_commandes.push_back(Commande (&entite->m_sprite_distortion, !entite->m_fixed, sf::BlendAlpha));
        }



        for(unsigned i = 0 ; i < entite->m_sprite_shadowmap.size() ; ++i)
        {
            if(fabs((float)entite->m_angle_shadowmap[i] - m_angleOmbreSoleil + 135) > 90)
            {
                sf::Sprite sprite = entite->m_sprite_shadowmap[i];
                sprite.move(entite->m_decalage.x,entite->m_decalage.y);
                float intensity = (fabs((float)entite->m_angle_shadowmap[i] - m_angleOmbreSoleil + 135) - 90)/45 * 96;

                if(fabs((float)entite->m_angle_shadowmap[i] - m_angleOmbreSoleil + 135) > 135)
                    intensity = 96;

                if(m_soleil.intensite < 128)
                     intensity *= (float)m_soleil.intensite / 128;

                sprite.setColor(sf::Color(255,255,255,(int)intensity));
                AjouterCommande(&sprite, entite->m_couche + entite->m_decalCouche, !entite->m_fixed);
            }

        }
    }
}

void MoteurGraphique::AjouterCommande(sf::Sprite *sprite, int couche, bool camera, const sf::RenderStates &s)
{
    if (couche>=0&&couche<=20)
        m_commandes[couche].push_back(Commande (sprite,camera,s));
}

void MoteurGraphique::AjouterTexte(const std::string &txt, coordonnee pos, Border &border, int couche, bool titre, int size, sf::Color color)
{
    sf::Text temp;
    temp.setFont(m_font);
    temp.setString(toUtf32(txt));
    temp.setPosition(pos.x, pos.y);
    temp.setCharacterSize(size);
    temp.setColor(color);
    AjouterTexte(&temp, couche, titre);

    pos.x -= 6;
    pos.y -= 2;

    coordonnee s;
    s.x = (int)temp.getGlobalBounds().width + 13;
    s.y = (int)temp.getGlobalBounds().height + 6;
    border.Afficher(pos, s, couche, sf::Color(224,224,224,color.a));
}

void MoteurGraphique::AjouterTexte(const std::string &txt, coordonnee pos, int couche, bool titre, int size, sf::Color color)
{
    sf::Text temp;
    temp.setFont(m_font);
    temp.setString(toUtf32(txt));
    temp.setPosition(pos.x, pos.y);
    temp.setCharacterSize(size);
    temp.setColor(color);
    AjouterTexte(&temp, couche, titre);
}

void MoteurGraphique::AjouterTexteNonChevauchable(sf::Text* string, int couche, bool titre)
{
    if (couche>=0&&couche<=20)
    {
        for (IterTextes=m_textes[couche].begin();IterTextes!=m_textes[couche].end();++IterTextes)
        {
            if(string->getGlobalBounds().left + string->getGlobalBounds().width > IterTextes->getGlobalBounds().left - 8
            && string->getGlobalBounds().left                           < IterTextes->getGlobalBounds().left + IterTextes->getGlobalBounds().width + 8
            && string->getGlobalBounds().top  + string->getGlobalBounds().height> IterTextes->getGlobalBounds().top - 8
            && string->getGlobalBounds().top                            < IterTextes->getGlobalBounds().top + IterTextes->getGlobalBounds().height + 8)
                string->setPosition(string->getPosition().x, IterTextes->getGlobalBounds().top - string->getGlobalBounds().height - 17), IterTextes=m_textes[couche].begin();
        }

        AjouterTexte(string, couche, titre);
    }
}

void MoteurGraphique::AjouterTexte(sf::Text* string, int couche,bool titre)
{

    if (couche>=0&&couche<=20)
    {
        sf::Text temp(*string);
        temp.setPosition((int)temp.getPosition().x,(int)temp.getPosition().y);
        temp.setFont(m_font);

        if (titre)
        {
            temp.setFont(m_font_titre);

            /*temp.SetStyle(sf::Text::Bold);
            temp.setColor(string->getColor());

            m_textes[couche].push_back(temp);

            temp.setColor(sf::Color((int)(string->getColor().r*0.15),(int)(string->getColor().g*0.15),(int)(string->getColor().b*0.15),string->getColor().a));
            temp.SetStyle(sf::Text::Regular);*/
            //m_textes[couche].push_back(temp);
        }
      //  else
        {
            temp.move(2,2);
            temp.setColor(sf::Color((int)(string->getColor().r*0.05),(int)(string->getColor().g*0.05),(int)(string->getColor().b*0.05),string->getColor().a));
            m_textes[couche].push_back(temp);
            temp.move(-1,-1);
            temp.setColor(string->getColor());
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

sf::Texture* MoteurGraphique::getImage(int IDimage)
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
    return m_ecran.pollEvent(EventReceived);
}

sf::Vector2f MoteurGraphique::getPositionSouris()
{
    return m_ecran.convertCoords(sf::Mouse::getPosition(m_ecran), m_camera);
}

sf::Vector2f MoteurGraphique::getPositionSourisEcran()
{
    return sf::Vector2f(sf::Mouse::getPosition(m_ecran).x, sf::Mouse::getPosition(m_ecran).y);
}

void MoteurGraphique::Printscreen()
{
    std::ostringstream buf;
    buf<<"screenshot"<<configuration->numero_screen<<".png";
    m_ecran.capture().saveToFile(buf.str());
    configuration->numero_screen++;
}

int MoteurGraphique::GetFPS()
{
    return m_fps;
}

