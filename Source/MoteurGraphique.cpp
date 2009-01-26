
#include "MoteurGraphique.h"
#include "Globale.h"

#include <iostream>

MoteurGraphique::MoteurGraphique()
{
    sf::Image temp(1, 1, sf::Color(255, 255, 255));
    m_images.push_back(temp);
    std::string temp2="";
    m_cheminsImages.push_back(temp2);
}
MoteurGraphique::~MoteurGraphique()
{
    Vider();
    m_cheminsImages.clear();
    m_images.clear();
}

void MoteurGraphique::Charger()
{
    if (sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
    {
        console.Ajouter("");
        console.Ajouter("Chargement des postFX :");

        if(!EffectBlur.LoadFromFile(configuration.chemin_fx+configuration.nom_effetBlur))
            console.Ajouter("Impossible de charger : "+configuration.chemin_fx+configuration.nom_effetBlur,1);
        else
            console.Ajouter("Chargement de : "+configuration.chemin_fx+configuration.nom_effetBlur,0);
        EffectBlur.SetTexture("framebuffer", NULL);

        if(!EffectMort.LoadFromFile(configuration.chemin_fx+configuration.nom_effetMort))
            console.Ajouter("Impossible de charger : "+configuration.chemin_fx+configuration.nom_effetMort,1);
        else
            console.Ajouter("Chargement de : "+configuration.chemin_fx+configuration.nom_effetMort,0);

        EffectMort.SetParameter("offset", 0);
        EffectMort.SetTexture("framebuffer", NULL);
        EffectMort.SetParameter("color",1, 1, 1);

        if(!EffectContrastes.LoadFromFile(configuration.chemin_fx+configuration.nom_effetContrastes))
            console.Ajouter("Impossible de charger : "+configuration.chemin_fx+configuration.nom_effetContrastes,1);
        else
        {
            console.Ajouter("Chargement de : "+configuration.chemin_fx+configuration.nom_effetContrastes,0);
            EffectContrastes.SetTexture("framebuffer", NULL);
            EffectContrastes.SetParameter("color", 0.f, 0.f, 0.f);
        }

        if(!EffectNoir.LoadFromFile(configuration.chemin_fx+configuration.nom_effetNoir))
            console.Ajouter("Impossible de charger : "+configuration.chemin_fx+configuration.nom_effetNoir,1);
        else
        {
            console.Ajouter("Chargement de : "+configuration.chemin_fx+configuration.nom_effetNoir,0);
            EffectNoir.SetTexture("framebuffer", NULL);
            EffectNoir.SetParameter("color", 0.f, 0.f, 0.f);
        }
        configuration.effetMort=0;
    }

    console.Ajouter("");
    console.Ajouter("Chargement des polices d'écriture :");

    if(!m_font_titre.LoadFromFile(configuration.chemin_fonts+configuration.font_titre))
        console.Ajouter("Impossible de charger : "+configuration.chemin_fonts+configuration.font_titre,1);
    else
        console.Ajouter("Chargement de : "+configuration.chemin_fonts+configuration.font_titre,0);

    //Luminosite.Create(configuration.Resolution.x, configuration.Resolution.y, sf::Color(255,255,255));
}

void MoteurGraphique::Gerer(sf::RenderWindow *ecran,float temps,int tailleMapY)
{
    for(int i=0;i<m_systemeParticules.size();i++)
        if(m_systemeParticules[i].m_modele>=0&&m_systemeParticules[i].m_modele<m_modeleSystemeParticules.size())
        {
            if(!m_systemeParticules[i].Gerer(temps,tailleMapY))
                m_systemeParticules.erase (m_systemeParticules.begin()+i);

            m_systemeParticules[i].Afficher(ecran,&m_modeleSystemeParticules[m_systemeParticules[i].m_modele]);
        }
}

void MoteurGraphique::CalculerLumiereParticules(LumiereOmbrage tableauDesLampes[30][30],coordonnee vueMin,sf::RenderWindow *ecran, int tailleMapY)
{
    coordonnee positionCase;
    for(int i=0;i<m_systemeParticules.size();i++)
        for(int j=0;j<m_systemeParticules[i].m_particules.size();j++)
        {
            if((float)((m_systemeParticules[i].m_particules[j].position.y*2-m_systemeParticules[i].m_particules[j].position.x)/2)/64+tailleMapY/2<(float)tailleMapY/2)
                positionCase.y=(int)((m_systemeParticules[i].m_particules[j].position.y*2-m_systemeParticules[i].m_particules[j].position.x)/2)/64+tailleMapY/2-1;
            else
                positionCase.y=(int)((m_systemeParticules[i].m_particules[j].position.y*2-m_systemeParticules[i].m_particules[j].position.x)/2)/64+tailleMapY/2;

            positionCase.x=(int)(m_systemeParticules[i].m_particules[j].position.x+((m_systemeParticules[i].m_particules[j].position.y*2-m_systemeParticules[i].m_particules[j].position.x)/2))/64-tailleMapY/2;

            if(positionCase.x-vueMin.x>=0&&positionCase.x-vueMin.x<30&&positionCase.y-vueMin.y>=0&&positionCase.y-vueMin.y<30)
            {
                m_systemeParticules[i].m_particules[j].color.r=tableauDesLampes[positionCase.y-vueMin.y][positionCase.x-vueMin.x].rouge*tableauDesLampes[positionCase.y-vueMin.y][positionCase.x-vueMin.x].intensite/255;
                m_systemeParticules[i].m_particules[j].color.g=tableauDesLampes[positionCase.y-vueMin.y][positionCase.x-vueMin.x].vert*tableauDesLampes[positionCase.y-vueMin.y][positionCase.x-vueMin.x].intensite/255;
                m_systemeParticules[i].m_particules[j].color.b=tableauDesLampes[positionCase.y-vueMin.y][positionCase.x-vueMin.x].bleu*tableauDesLampes[positionCase.y-vueMin.y][positionCase.x-vueMin.x].intensite/255;
            }
        }
}

void MoteurGraphique::Afficher(sf::RenderWindow *ecran, sf::View *camera)
{

    sf::Sprite sprite;

    for(int k=0;k<20;k++)
    {
        for(int i=0;i<m_commandes[k].size();i++)
        {
            if(m_commandes[k][i].m_utiliserCamera)
                ecran->SetView(*camera);
            else
                ecran->SetView(ecran->GetDefaultView());

            sprite=m_commandes[k][i].m_sprite;
            ecran->Draw(sprite);
        }



        for(int i=0;i<m_textes[k].size();i++)
        {
            ecran->SetView(ecran->GetDefaultView());
            ecran->Draw(m_textes[k][i]);
        }

        if(k==17)
        {
            if(m_blur>0)
            {
                EffectBlur.SetTexture("framebuffer", NULL);
                EffectBlur.SetParameter("offset",m_blur);
                ecran->Draw(EffectBlur);
                ecran->Draw(EffectBlur);
                ecran->Draw(EffectBlur);
                ecran->Draw(EffectBlur);
                ecran->Draw(EffectBlur);
            }
            if(configuration.effetMort>0)
            {
                EffectMort.SetTexture("framebuffer", NULL);
                ecran->Draw(EffectMort);
            }
        }
    }

    EffectNoir.SetTexture("framebuffer", NULL);
    EffectNoir.SetParameter("color", configuration.effetNoir, configuration.effetNoir, configuration.effetNoir);
    ecran->Draw(EffectNoir);

    if(configuration.contrastes>1&&sf::PostFX::CanUsePostFX() == true&&configuration.postFX)
    {
        EffectContrastes.SetTexture("framebuffer", NULL);
        ecran->Draw(EffectContrastes);
        EffectContrastes.SetParameter("color", configuration.contrastes, configuration.contrastes, configuration.contrastes);
    }
    if(configuration.luminosite>0)
    {
        sf::Sprite sprite2;
        sprite2.SetImage(*getImage(0));
        sprite2.Resize(configuration.Resolution.w,configuration.Resolution.h);
        sprite2.SetColor(sf::Color((int)configuration.luminosite,(int)configuration.luminosite,(int)configuration.luminosite,255));
        sprite2.SetBlendMode(sf::Blend::Add);
        ecran->Draw(sprite2);
    }
}

int MoteurGraphique::AjouterImage(const char *Data, std::size_t SizeInBytes, std::string nom)
{
    for(int i=0;i<m_cheminsImages.size();i++)
        if(m_cheminsImages[i]==nom.c_str())
            return i;

    m_images.push_back(sf::Image ());
    m_cheminsImages.push_back(nom);

    if(!configuration.lissage)
        m_images[m_images.size()-1].SetSmooth(false);

    if(!m_images[m_images.size()-1].LoadFromMemory(Data,SizeInBytes))
    {
        console.Ajouter("Impossible de charger depuis la mémoire : "+nom,1);
        return -1;
    }
    else
        console.Ajouter("Chargement de : "+nom,0);


    return m_images.size()-1;
}

int MoteurGraphique::AjouterImage(std::string chemin)
{
    for(int i=0;i<m_cheminsImages.size();i++)
        if(m_cheminsImages[i]==chemin.c_str())
            return i;

    m_images.push_back(sf::Image ());
    m_cheminsImages.push_back(chemin);

    if(!configuration.lissage)
        m_images[m_images.size()-1].SetSmooth(false);

    if(!m_images[m_images.size()-1].LoadFromFile(chemin.c_str()))
    {
        console.Ajouter("Impossible de charger : "+chemin,1);
        return -1;
    }
    else
        console.Ajouter("Chargement de : "+chemin,0);


    return m_images.size()-1;
}

int MoteurGraphique::AjouterModeleSystemeParticules(std::string chemin)
{
    for(int i=0;i<m_modeleSystemeParticules.size();i++)
        if(m_modeleSystemeParticules[i].m_chemin==chemin)
            return i;

    m_modeleSystemeParticules.push_back(ModeleParticuleSysteme (chemin));

    return m_modeleSystemeParticules.size()-1;
}


void MoteurGraphique::AjouterSystemeParticules(int ID,coordonnee position,sf::Color color,float force,float angle)
{
    if(ID>=0&&ID<m_modeleSystemeParticules.size())
    {
        m_systemeParticules.push_back(ParticuleSysteme (ID,&m_modeleSystemeParticules[ID],position,color,force,angle));
    }
}

void MoteurGraphique::AjouterCommande(sf::Sprite *sprite, int couche, bool camera)
{
    if(couche>=0&&couche<20)
        m_commandes[couche].push_back(Commande (sprite,camera));
}

void MoteurGraphique::AjouterTexte(sf::String* string, int couche,bool titre)
{
    sf::String temp(*string);
    if(couche>=0&&couche<20)
    {
        /*while(m_textes.size()<=couche)
            m_textes.push_back(std::vector <sf::String> ());*/

        if(temp.GetRect().Right>configuration.Resolution.w)
            temp.SetX(configuration.Resolution.w-(temp.GetRect().Right-temp.GetRect().Left));

        if(temp.GetRect().Left<0)
            temp.SetX(0);

        if(temp.GetRect().Top<0)
            temp.SetY(0);

        if(temp.GetRect().Bottom>configuration.Resolution.h)
            temp.SetY(configuration.Resolution.h-(temp.GetRect().Bottom-temp.GetRect().Top));

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
    for(int i=0;i<20;i++)
        m_commandes[i].clear();
    for(int i=0;i<20;i++)
        m_textes[i].clear();
}
void MoteurGraphique::ViderParticules()
{
    m_systemeParticules.clear();
}


sf::Image* MoteurGraphique::getImage(int IDimage)
{
    if(IDimage>=0&&IDimage<m_images.size())
        return &m_images[IDimage];
    else
        return &m_images[0];
}

ModeleParticuleSysteme* MoteurGraphique::getModeleMoteurParticules(int ID)
{
    if(ID>=0&&ID<m_modeleSystemeParticules.size())
        return &m_modeleSystemeParticules[ID];
    else
        return &m_modeleSystemeParticules[0];
}

std::string MoteurGraphique::getCheminImage(int IDimage)
{
    if(IDimage>=0&&IDimage<m_images.size())
        return m_cheminsImages[IDimage];
    else
        return "";
}

void MoteurGraphique::calculerOmbresEtLumieres(sf::RenderWindow* ecran,Hero *hero,sf::View *camera,Map *map)
{
    /* m_lumiereHero.detruire();

    //La, ça se complique, je vais essayer d'être clair, mais il ne faut pas hésiter à me redemander des explications ^^
    Lumiere lumiere,lumiereTile,lumiereMap;
    float angleOmbreMap;

    coordonnee vueMin,vueMax,lampesMin,lampesMax,distanceActuelle,position,positionPartieDecor;

     double lumiereTemp,Xtemp,Ytemp,angle=0;

     //Calcul des sources de lumière qui peuvent se trouver les plus loin du perso, tout en influant sur des tiles dans le champs de vision
    lampesMin.x=(int)((float)hero->m_personnage.getCoordonnee().x-15);
    lampesMin.y=(int)((float)hero->m_personnage.getCoordonnee().y-15);
    lampesMax.x=(int)((float)hero->m_personnage.getCoordonnee().x+15);
    lampesMax.y=(int)((float)hero->m_personnage.getCoordonnee().y+15);

    //Vérification que ces tiles soit bien dans la map
    if(lampesMin.x<0) { lampesMin.x=0; }
    if(lampesMin.y<0) { lampesMin.y=0; }
    if(lampesMax.x>map->getDimensions().x) { lampesMax.x=map->getDimensions().x; }
    if(lampesMax.y>map->getDimensions().y) { lampesMax.y=map->getDimensions().y; }


    //Calcul des tile qui peuvent se trouver les plus loin du perso, tout en restant dans le champs de vision
    vueMin.x=hero->m_personnage.getCoordonnee().x-15;
    vueMin.y=hero->m_personnage.getCoordonnee().y-15;
    vueMax.x=hero->m_personnage.getCoordonnee().x+15;
    vueMax.y=hero->m_personnage.getCoordonnee().y+15;

    //Vérification que ces tiles soit bien dans la map
    if(vueMin.x<0) { vueMin.x=0; }
    if(vueMin.y<0) { vueMin.y=0; }
    if(vueMax.x>map->getDimensions().x) { vueMax.x=map->getDimensions().x; }
    if(vueMax.y>map->getDimensions().y) { vueMax.y=map->getDimensions().y; }
    if(configuration.heure+1<24)
    {
        lumiereMap.intensite=(int)(((float)m_lumiere[configuration.heure].intensite*(60-configuration.minute)+((float)m_lumiere[configuration.heure+1].intensite*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.rouge=(int)(((float)m_lumiere[configuration.heure].rouge*(60-configuration.minute)+((float)m_lumiere[configuration.heure+1].rouge*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.vert=(int)(((float)m_lumiere[configuration.heure].vert*(60-configuration.minute)+((float)m_lumiere[configuration.heure+1].vert*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.bleu=(int)(((float)m_lumiere[configuration.heure].bleu*(60-configuration.minute)+((float)m_lumiere[configuration.heure+1].bleu*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.hauteur=((float)m_lumiere[configuration.heure].hauteur*(60-configuration.minute)+((float)m_lumiere[configuration.heure+1].hauteur*configuration.minute))*0.016666666666666666666666666666667;
    }
    else
    {
        lumiereMap.intensite=(int)(((float)m_lumiere[configuration.heure].intensite*(60-configuration.minute)+((float)m_lumiere[0].intensite*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.rouge=(int)(((float)m_lumiere[configuration.heure].rouge*(60-configuration.minute)+((float)m_lumiere[0].rouge*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.vert=(int)(((float)m_lumiere[configuration.heure].vert*(60-configuration.minute)+((float)m_lumiere[0].vert*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.bleu=(int)(((float)m_lumiere[configuration.heure].bleu*(60-configuration.minute)+((float)m_lumiere[0].bleu*configuration.minute))*0.016666666666666666666666666666667);
        lumiereMap.hauteur=((float)m_lumiere[configuration.heure].hauteur*(60-configuration.minute)+((float)m_lumiere[0].hauteur*configuration.minute))*0.016666666666666666666666666666667;
    }

    angleOmbreMap=((float)configuration.heure*60+configuration.minute)*180/720;

    m_lumiereHero=hero->m_modelePersonnage.getPorteeLumineuse();

    m_lumiereHero.rouge=(lumiereMap.rouge*lumiereMap.intensite+m_lumiereHero.rouge*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiereMap.intensite);
    m_lumiereHero.vert=(lumiereMap.vert*lumiereMap.intensite+m_lumiereHero.vert*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiereMap.intensite);
    m_lumiereHero.bleu=(lumiereMap.bleu*lumiereMap.intensite+m_lumiereHero.bleu*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiereMap.intensite);

    m_lumiereHero.intensite+=lumiereMap.intensite;
    if(m_lumiereHero.intensite>255) m_lumiereHero.intensite=255;
    if(m_lumiereHero.intensite<0) m_lumiereHero.intensite=0;

    if(configuration.Ombre)
        m_lumiereHero.AjouterOmbre(lumiereMap.intensite/4,angleOmbreMap,lumiereMap.hauteur);


        // Je fonctionne comme pour en bas, juste que je prend le héro comme source de lumière, pour la portée lumineuse
    for(int i=0;i<30;i++)
        for(int j=0;j<30;j++)
        {
            m_tableauDesLampes[i][j].detruire();
             m_tableauDesLampes[i][j]=lumiereMap;
            if(configuration.Ombre)
                m_tableauDesLampes[i][j].AjouterOmbre((int)((float)lumiereMap.intensite*0.25),angleOmbreMap,lumiereMap.hauteur);

            if(i>=0&&i<vueMax.y-vueMin.y&&j>=0&&j<vueMax.x-vueMin.x)
            {

                lumiereTemp=0;
                lumiereTemp=(float)hero->m_modelePersonnage.getPorteeLumineuse().intensite-((((float)gpl::sqrt((hero->m_personnage.getCoordonneePixel().x-(j+vueMin.x)*COTE_TILE)*(hero->m_personnage.getCoordonneePixel().x-(j+vueMin.x)*COTE_TILE)+(hero->m_personnage.getCoordonneePixel().y-(i+vueMin.y)*COTE_TILE)*(hero->m_personnage.getCoordonneePixel().y-(i+vueMin.y)*COTE_TILE))))*DIVISEUR_COTE_TILE)*30;

                lumiere=hero->m_modelePersonnage.getPorteeLumineuse();

                lumiere.intensite=(int)lumiereTemp;

                float lumiereTemp2=0;

                 bool aMoitieSombre=true;

                lumiereTemp=0;
                lumiereTemp2=0;
                if(lumiere.intensite>0)
                    for(int o=1;o<10;o++)
                    {
                        coordonneeDecimal positionHero;

                        positionHero.x=(hero->m_personnage.getCoordonneePixel().x+1)*DIVISEUR_COTE_TILE;
                        positionHero.y=(hero->m_personnage.getCoordonneePixel().y+1)*DIVISEUR_COTE_TILE;

                        Xtemp=((float)((hero->m_personnage.getCoordonneePixel().x+(hero->m_personnage.getProchaineCase().x-hero->m_personnage.getCoordonnee().x)*8)*DIVISEUR_COTE_TILE-(j+vueMin.x))*0.1)*(float)o;
                        Ytemp=((float)((hero->m_personnage.getCoordonneePixel().y-(hero->m_personnage.getProchaineCase().y-hero->m_personnage.getCoordonnee().y)*8)*DIVISEUR_COTE_TILE-(i+vueMin.y))*0.1)*(float)o;

                        if(Xtemp-(int)Xtemp>0.5)
                            Xtemp=(int)Xtemp+1;

                        if(Ytemp-(int)Ytemp>0.5)
                            Ytemp=(int)Ytemp+1;


                        distanceActuelle.x=(j+vueMin.x)+(int)Xtemp;
                        distanceActuelle.y=(i+vueMin.y)+(int)Ytemp;

                        if(distanceActuelle.x>0&&distanceActuelle.x<m_decor[0][0].size()&&distanceActuelle.y>0&&distanceActuelle.y<m_decor[0].size())
                                if(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()>=0&&m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()<m_tileset.size())
                                   if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<0&&m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<lumiereTemp)
                                    {


                                        bool erreur=false;
                                        if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='b')
                                            if((j+vueMin.x)==distanceActuelle.x&&(j+vueMin.x)<positionHero.x)
                                                erreur=true;

                                        if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='b')
                                            if((i+vueMin.y)==distanceActuelle.y&&(i+vueMin.y)<=positionHero.y)
                                                erreur=true;

                                        if(!erreur)
                                        if(!((i+vueMin.y)==distanceActuelle.y&&(j+vueMin.x)==distanceActuelle.x))
                                            lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite,aMoitieSombre=true;
                                        else
                                        {

                                            if(distanceActuelle.x>positionHero.x&&distanceActuelle.y>positionHero.y)
                                                lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite,aMoitieSombre=true;

                                            if(distanceActuelle.x>positionHero.x&&distanceActuelle.y<=positionHero.y)
                                            {
                                                if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='h')
                                                    lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite,aMoitieSombre=true;
                                            }
                                            if(distanceActuelle.x<=positionHero.x&&distanceActuelle.y>positionHero.y)
                                            {
                                                if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='h')
                                                    lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite,aMoitieSombre=true;
                                            }

                                        }

                                    }
                    }


                if((int)lumiereTemp<0||lumiereTemp2<0)
                {
                    lumiere.intensite+=(int)(lumiereTemp);
                }

                if(lumiere.intensite>255)
                    lumiere.intensite=255;
                if(lumiere.intensite<0)
                    lumiere.intensite=0;


                if(lumiere.intensite>0)
                {
                    m_tableauDesLampes[i][j].intensite+=lumiere.intensite;

                   // if(!m_decor[1][i+vueMin.y][j+vueMin.x].getTileset()>0)
                    //if(m_tileset[m_decor[1][i+vueMin.y][j+vueMin.x].getTileset()].getLumiereDuTile(m_decor[1][i+vueMin.y][j+vueMin.x].getTile()).intensite==0)
                }

                if(lumiere.intensite>0)
                {
                    m_tableauDesLampes[i][j].rouge=(lumiere.rouge*lumiere.intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite)+m_tableauDesLampes[i][j].rouge*m_tableauDesLampes[i][j].intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite));
                    m_tableauDesLampes[i][j].vert=(lumiere.vert*lumiere.intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite)+m_tableauDesLampes[i][j].vert*m_tableauDesLampes[i][j].intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite));
                    m_tableauDesLampes[i][j].bleu=(lumiere.bleu*lumiere.intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite)+m_tableauDesLampes[i][j].bleu*m_tableauDesLampes[i][j].intensite/(m_tableauDesLampes[i][j].intensite+lumiere.intensite));

                }

                if(m_tableauDesLampes[i][j].intensite>255)
                    m_tableauDesLampes[i][j].intensite=255;
                if(m_tableauDesLampes[i][j].intensite<0)
                    m_tableauDesLampes[i][j].intensite=0;
                if(m_tableauDesLampes[i][j].rouge>255)
                    m_tableauDesLampes[i][j].rouge=255;
                if(m_tableauDesLampes[i][j].vert>255)
                    m_tableauDesLampes[i][j].vert=255;
                if(m_tableauDesLampes[i][j].bleu>255)
                    m_tableauDesLampes[i][j].bleu=255;

                if(i==hero->m_personnage.getCoordonnee().y&&j==hero->m_personnage.getCoordonnee().x)
                {
                    if(lumiere.intensite>0)
                        m_lumiereHero.intensite+=lumiere.intensite;

                    if(lumiere.intensite>0)
                    {
                        m_lumiereHero.rouge=(lumiere.rouge*lumiere.intensite/(m_lumiereHero.intensite+lumiere.intensite)+m_lumiereHero.rouge*m_lumiereHero.intensite/(m_lumiereHero.intensite+lumiere.intensite));
                        m_lumiereHero.vert=(lumiere.vert*lumiere.intensite/(m_lumiereHero.intensite+lumiere.intensite)+m_lumiereHero.vert*m_lumiereHero.intensite/(m_lumiereHero.intensite+lumiere.intensite));
                        m_lumiereHero.bleu=(lumiere.bleu*lumiere.intensite/(m_lumiereHero.intensite+lumiere.intensite)+m_lumiereHero.bleu*m_lumiereHero.intensite/(m_lumiereHero.intensite+lumiere.intensite));

                    }

                    if(m_lumiereHero.intensite>255)
                        m_lumiereHero.intensite=255;
                    if(m_lumiereHero.intensite<0)
                        m_lumiereHero.intensite=0;
                    if(m_lumiereHero.rouge>255)
                        m_lumiereHero.rouge=255;
                    if(m_lumiereHero.vert>255)
                        m_lumiereHero.vert=255;
                    if(m_lumiereHero.bleu>255)
                        m_lumiereHero.bleu=255;
                }

                if(configuration.Ombre)
                {
                    coordonnee coord1,coord2;

                    coord1.x=(int)(hero->m_personnage.getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().y-1*COTE_TILE+m_decor[0].size()*COTE_TILE)*2;
                    coord1.y=(int)(hero->m_personnage.getCoordonneePixel().x+hero->m_personnage.getCoordonneePixel().y);
                    coord2.x=(int)((j+vueMin.x)*COTE_TILE-(i+vueMin.y)*COTE_TILE-1*COTE_TILE+m_decor[0].size()*COTE_TILE)*2;
                    coord2.y=(int)((j+vueMin.x)*COTE_TILE+(i+vueMin.y)*COTE_TILE);

                    angle=atan(valeurAbsolue((float)coord2.y-(float)coord1.y)/valeurAbsolue((float)coord2.x-(float)coord1.x))*360/(2*M_PI)-90;

                    if(coord2.y>=coord1.y&&coord2.x>=coord1.x)
                        angle=180-angle;
                    else if(coord2.y>=coord1.y&&coord2.x<coord1.x)
                        angle+=180;
                    else if(coord2.y<coord1.y&&coord2.x<coord1.x)
                        angle=-angle;

                    m_tableauDesLampes[i][j].AjouterOmbre(lumiere.intensite/2,(int)angle,lumiere.hauteur);
                }
            }
        }






        for(int couche=1;couche==1;couche++)
        {
            for(int j=lampesMin.y;j<lampesMax.y;j++)
            {
                for(int k=lampesMin.x;k<lampesMax.x;k++)
                {
                    bool ok=false;
                    if(m_decor[0][j][k].getMonstre()>=0&&m_decor[couche][j][k].getMonstre()<m_monstre.size())// Je vérifie que le tile est initialisé ^^
                        ok=true;
                    if(m_decor[0][j][k].getEvenement()>=0&&m_decor[couche][j][k].getEvenement()<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                        if(m_evenement[m_decor[0][j][k].getEvenement()].getType()==LUMIERE)
                            ok=true;
                    if(m_decor[1][j][k].getMonstre()>=0&&m_decor[couche][j][k].getMonstre()<m_monstre.size())// Je vérifie que le tile est initialisé ^^
                        ok=true;
                    if(m_decor[1][j][k].getEvenement()>=0&&m_decor[couche][j][k].getEvenement()<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                        if(m_evenement[m_decor[1][j][k].getEvenement()].getType()==LUMIERE)
                            ok=true;
                    if(m_decor[couche][j][k].getTileset()>=0&&m_decor[couche][j][k].getTileset()<m_tileset.size())
                        if(m_tileset[m_decor[couche][j][k].getTileset()].getLumiereDuTile(m_decor[couche][j][k].getTile()).intensite>0)// Je ragarde si le tile est une source de lumière
                            ok=true;
                    if(ok)
                    {
                        lumiereTile.intensite=0;
                        lumiereTile.rouge=0;
                        lumiereTile.vert=0;
                        lumiereTile.bleu=0;
                        lumiereTile.hauteur=50;

                        if(m_decor[couche][j][k].getTileset()>=0&&m_decor[couche][j][k].getTileset()<m_tileset.size())
                            if(m_tileset[m_decor[couche][j][k].getTileset()].getLumiereDuTile(m_decor[couche][j][k].getTile()).intensite>0)
                                lumiereTile=m_tileset[m_decor[couche][j][k].getTileset()].getLumiereDuTile(m_decor[couche][j][k].getTile());// Je dis que lumièreTiles à la valeur de la lumière de la source de lumière

                        if(m_decor[0][j][k].getEvenement()>=0&&m_decor[0][j][k].getEvenement()<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                            if(m_evenement[m_decor[0][j][k].getEvenement()].getType()==LUMIERE)
                            {
                                lumiereTile.rouge=(lumiereTile.rouge*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()].getInformation(0)*m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);
                                lumiereTile.vert=(lumiereTile.vert*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()].getInformation(1)*m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);
                                lumiereTile.bleu=(lumiereTile.bleu*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()].getInformation(2)*m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);

                                lumiereTile.hauteur=(lumiereTile.hauteur*lumiereTile.intensite+m_evenement[m_decor[0][j][k].getEvenement()].getInformation(4)*m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);

                                lumiereTile.intensite+=m_evenement[m_decor[0][j][k].getEvenement()].getInformation(3);
                                if(lumiereTile.intensite>255)
                                    lumiereTile.intensite=255;
                                if(lumiereTile.intensite<0)
                                    lumiereTile.intensite=0;
                            }
                        if(m_decor[1][j][k].getEvenement()>=0&&m_decor[1][j][k].getEvenement()<m_evenement.size())// Je vérifie que le tile est initialisé ^^
                            if(m_evenement[m_decor[1][j][k].getEvenement()].getType()==LUMIERE)
                            {
                                lumiereTile.rouge=(lumiereTile.rouge*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()].getInformation(0)*m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);
                                lumiereTile.vert=(lumiereTile.vert*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()].getInformation(1)*m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);
                                lumiereTile.bleu=(lumiereTile.bleu*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()].getInformation(2)*m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);

                                lumiereTile.hauteur=(lumiereTile.hauteur*lumiereTile.intensite+m_evenement[m_decor[1][j][k].getEvenement()].getInformation(4)*m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3))/(m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3)+lumiereTile.intensite);

                                lumiereTile.intensite+=m_evenement[m_decor[1][j][k].getEvenement()].getInformation(3);
                                if(lumiereTile.intensite>255)
                                    lumiereTile.intensite=255;
                                if(lumiereTile.intensite<0)
                                    lumiereTile.intensite=0;
                            }


                        //Maintenant, je vais calculer la lumière à ajouter à tout les tiles affiché à l'écran
                        for(int l=vueMin.y;l<vueMax.y;l++)
                            for(int m=vueMin.x;m<vueMax.x;m++)
                                {
                                    bool ombre=false;
                                    position.x=(m-l-1+m_decor[0].size())*64;
                                    position.y=(m+l)*32; // Conversion des coord iso en cartésienne
                                    const sf::FloatRect& ViewRect = camera->GetRect();
                                    if(m_decor[0][l][m].getTileset()>-1)
                                        positionPartieDecor=m_tileset[m_decor[0][l][m].getTileset()].getPositionDuTile(m_decor[0][l][m].getTile());
                                    if(m_decor[1][l][m].getTileset()>-1)
                                    {
                                        positionPartieDecor=m_tileset[m_decor[1][l][m].getTileset()].getPositionDuTile(m_decor[1][l][m].getTile());
                                        if(m_tileset[m_decor[1][l][m].getTileset()].getOmbreDuTile(m_decor[1][l][m].getTile())&&configuration.Ombre)
                                            ombre=true;
                                    }

                                    if(position.x+positionPartieDecor.h*2+positionPartieDecor.w>=ViewRect.Left&&position.x-positionPartieDecor.h*2-positionPartieDecor.w-64<ViewRect.Right&&position.y+positionPartieDecor.h+positionPartieDecor.w>=ViewRect.Top&&position.y-positionPartieDecor.h-32<ViewRect.Bottom+100&&ombre
                                    ||position.x+positionPartieDecor.w/configuration.zoom>=ViewRect.Left&&position.x-128/configuration.zoom<ViewRect.Right&&position.y+64+positionPartieDecor.h/configuration.zoom>=ViewRect.Top&&position.y-positionPartieDecor.h/configuration.zoom+64<ViewRect.Bottom)
                                  //  if(position.x>=ViewRect.Left-384&&position.x<ViewRect.Right+384&&position.y>=ViewRect.Top-384&&position.y<ViewRect.Bottom+384)// Je test si le tile va êtrz affiché à l'écran ou pas
                                    {

                                    //lumiere=m_lumiere;
                                    lumiere.intensite=0;
                                    lumiereTemp=0;
                                    // Je réduit la quantité de lumière reçue, en fonction de la distance, qui est calculée avec un simple pythagore ^^
                                    lumiereTemp=(float)lumiereTile.intensite-(float)(gpl::sqrt((k*COTE_TILE-m*COTE_TILE)*(k*COTE_TILE-m*COTE_TILE)+(j*COTE_TILE-l*COTE_TILE)*(j*COTE_TILE-l*COTE_TILE)))/5;
                                    lumiere.hauteur=lumiereTile.hauteur;

                                    if(lumiereTemp>0)
                                    lumiere.intensite=(int)lumiereTemp;
                                    else
                                    lumiere.intensite=0;

                                    //Je vais tester s'il y a un obstacle entre la source de lumière et le tile
                                    if(!(l==j&&m==k))
                                    if(lumiere.intensite>0)
                                    for(int o=1;o<10;o++)
                                    {
                                        Xtemp=((double)(k-m)*0.1)*(double)o;
                                        Ytemp=((double)(j-l)*0.1)*(double)o;

                                        distanceActuelle.x=m+(int)Xtemp;
                                        distanceActuelle.y=l+(int)Ytemp;

                                        if(distanceActuelle.x>0&&distanceActuelle.x<m_decor[0][0].size()&&distanceActuelle.y>0&&distanceActuelle.y<m_decor[0].size())
                                            if(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()>=0&&m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()<m_tileset.size())
                                            if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<0&&m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite<lumiereTemp)
                                                    {
                                                        bool erreur=false;
                                                        //if(m_decor[1][l][m].getTileset()>0)
                                                            if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d')
                                                                //if(m_tileset[m_decor[1][l][m].getTileset()].getOrientationDuTile(m_decor[1][l][m].getTile())!='x')
                                                                    if(m==distanceActuelle.x&&m<k)
                                                                        erreur=true;


                                                       // if(m_decor[1][l][m].getTileset()>0)
                                                            if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g')
                                                                //if(m_tileset[m_decor[1][l][m].getTileset()].getOrientationDuTile(m_decor[1][l][m].getTile())!='y')
                                                                    if(l==distanceActuelle.y&&l<j)
                                                                        erreur=true;

                                                        if(!erreur)
                                                        if(!(l==distanceActuelle.y&&m==distanceActuelle.x))
                                                            lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                        else
                                                        {
                                                            if(distanceActuelle.x>=k&&distanceActuelle.y>=j)
                                                               lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;

                                                            if(distanceActuelle.x>=k&&distanceActuelle.y<j)
                                                            {
                                                                if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='y'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='d')
                                                                        lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                            }
                                                            if(distanceActuelle.x<k&&distanceActuelle.y>=j)
                                                            {

                                                                if(m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='x'||m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getOrientationDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile())=='g')
                                                                    lumiereTemp=m_tileset[m_decor[1][distanceActuelle.y][distanceActuelle.x].getTileset()].getLumiereDuTile(m_decor[1][distanceActuelle.y][distanceActuelle.x].getTile()).intensite;
                                                            }
                                                        }
                                                    }

                                    }
                                    // SI oui, je retire de la lumière au tile
                                    if((int)lumiereTemp<0)
                                    lumiere.intensite+=(int)lumiereTemp;

                                    lumiere.rouge=lumiereTile.rouge;
                                    lumiere.vert=lumiereTile.vert;
                                    lumiere.bleu=lumiereTile.bleu;

                                    if(lumiere.intensite>255)
                                        lumiere.intensite=255;
                                    if(lumiere.intensite<0)
                                        lumiere.intensite=0;

                                    if(lumiere.intensite>0)
                                    {
                                        // Je calcule la couleur de la lumière, par rapport à l'ancienne lumière et à la nouvelle, pour faire un fondu
                                        m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=(lumiere.rouge*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge*m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite)/(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+lumiere.intensite);
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=(lumiere.vert*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert*m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite)/(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+lumiere.intensite);
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=(lumiere.bleu*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu*m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite)/(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+lumiere.intensite);

                                        if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge>255)
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=255;
                                        if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert>255)
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=255;
                                        if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu>255)
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=255;

                                        m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+=lumiere.intensite;

                                        if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite>255)
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=255;
                                        if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite<0)
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=0;

                                        // Je calcule les ombres
                                        if(!(l==j&&m==k))
                                        if(configuration.Ombre)
                                        {
                                            coordonnee coord1,coord2;

                                            coord1.x=(k-j-1+m_decor[0].size())*64;
                                            coord1.y=(k+j)*32;

                                            coord2.x=(m-l-1+m_decor[0].size())*64;
                                            coord2.y=(m+l)*32;

                                            // L'angle de l'ombre, merci Mme la trigonométrie :D
                                            angle=atan(valeurAbsolue((float)coord2.y-(float)coord1.y)/valeurAbsolue((float)coord2.x-(float)coord1.x))*360/(2*M_PI)-90;

                                            if(coord2.y>=coord1.y&&coord2.x>=coord1.x)
                                                angle=180-angle;
                                            else if(coord2.y>=coord1.y&&coord2.x<coord1.x)
                                                angle+=180;
                                            else if(coord2.y<coord1.y&&coord2.x<coord1.x)
                                                angle=-angle;

                                            // Et j'ajoute cette ombre
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].AjouterOmbre(lumiere.intensite,(int)angle,lumiere.hauteur);
                                        }

                                        // J'applique la même chose au héro
                                        if(l==hero->m_personnage.getCoordonnee().y&&m==hero->m_personnage.getCoordonnee().x)
                                        {
                                           m_lumiereHero.rouge=(lumiere.rouge*lumiere.intensite+m_lumiereHero.rouge*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiere.intensite);
                                           m_lumiereHero.vert=(lumiere.vert*lumiere.intensite+m_lumiereHero.vert*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiere.intensite);
                                           m_lumiereHero.bleu=(lumiere.bleu*lumiere.intensite+m_lumiereHero.bleu*m_lumiereHero.intensite)/(m_lumiereHero.intensite+lumiere.intensite);

                                            if(m_lumiereHero.rouge>255)
                                               m_lumiereHero.rouge=255;
                                            if(m_lumiereHero.vert>255)
                                               m_lumiereHero.vert=255;
                                            if(m_lumiereHero.bleu>255)
                                               m_lumiereHero.bleu=255;

                                            if(configuration.Ombre)
                                            {
                                                coordonnee coord1,coord2;

                                                coord1.x=(int)(k*COTE_TILE-j*COTE_TILE-1+m_decor[0].size())*64;
                                                coord1.y=(int)(k*COTE_TILE+j*COTE_TILE)*32;

                                                coord2.x=((int)hero->m_personnage.getCoordonneePixel().x-(int)hero->m_personnage.getCoordonneePixel().y-1+m_decor[0].size())*64;
                                                coord2.y=((int)hero->m_personnage.getCoordonneePixel().x+(int)hero->m_personnage.getCoordonneePixel().y)*32;

                                                angle=atan(valeurAbsolue((float)coord2.y-(float)coord1.y)/valeurAbsolue((float)coord2.x-(float)coord1.x))*360/(2*M_PI)-90;

                                                if(coord2.y>=coord1.y&&coord2.x>=coord1.x)
                                                    angle=180-angle;
                                                else if(coord2.y>=coord1.y&&coord2.x<coord1.x)
                                                    angle+=180;
                                                else if(coord2.y<coord1.y&&coord2.x<coord1.x)
                                                    angle=-angle;

                                                lumiereTemp=lumiere.intensite/2;
                                                if(lumiereTemp<0)
                                                    lumiereTemp=0;
                                               m_lumiereHero.AjouterOmbre((int)lumiereTemp,(int)angle,lumiere.hauteur);
                                            }

                                            if(lumiere.intensite>0)
                                            {
                                                m_lumiereHero.intensite+=lumiere.intensite;

                                                if(m_lumiereHero.intensite>255)
                                                   m_lumiereHero.intensite=255;
                                                if(m_lumiereHero.intensite<0)
                                                   m_lumiereHero.intensite=0;
                                            }
                                        }
                                    }

                                        if(m_decor[0][j][k].getMonstre()>=0&&m_decor[0][j][k].getMonstre()<m_monstre.size())
                                        if(m_monstre[m_decor[0][j][k].getMonstre()].getCaracteristique().vie>0)
                                        {
                                            //lumiere=m_lumiere;
                                            lumiere.intensite=m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite;
                                            lumiereTemp=0;

                                            if(lumiere.intensite>255)
                                                lumiere.intensite=255;

                                            // Je réduit la quantité de lumière reçue, en fonction de la distance, qui est calculée avec un simple pythagore ^^
                                           // lumiereTemp=(float)lumiereTile.intensite-(float)(gpl::sqrt((k*COTE_TILE-m*COTE_TILE)*(k*COTE_TILE-m*COTE_TILE)+(j*COTE_TILE-l*COTE_TILE)*(j*COTE_TILE-l*COTE_TILE)))/5;
                                            lumiereTemp=(float)lumiere.intensite-((((float)gpl::sqrt((m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().x-((float)m)*COTE_TILE)*(m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().x-((float)m)*COTE_TILE)+(m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().y-((float)l)*COTE_TILE)*(m_monstre[m_decor[0][j][k].getMonstre()].getCoordonneePixel().y-((float)l)*COTE_TILE))))*DIVISEUR_COTE_TILE)*60;
                                            lumiere.hauteur=m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().hauteur;

                                            if(lumiereTemp>0)
                                            lumiere.intensite=(int)lumiereTemp;
                                            else
                                            lumiere.intensite=0;

                                            lumiere.rouge=m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().rouge;
                                            lumiere.vert=m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().vert;
                                            lumiere.bleu=m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().bleu;

                                            lumiereTemp=m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite;

                                            if(lumiereTemp>255)
                                                lumiereTemp=255;

                                                // Je calcule la couleur de la lumière, par rapport à l'ancienne lumière et à la nouvelle, pour faire un fondu
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=(int)(lumiere.rouge*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=(int)(lumiere.vert*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);
                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=(int)(lumiere.bleu*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);

                                            if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge>255)
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=255;
                                            if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert>255)
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=255;
                                            if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu>255)
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=255;

                                            m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+=lumiere.intensite;

                                            if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite>255)
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=255;
                                            if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite<0)
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=0;
                                        }

                                        if(m_decor[1][j][k].getMonstre()>=0&&m_decor[1][j][k].getMonstre()<m_monstre.size())
                                        if(m_monstre[m_decor[1][j][k].getMonstre()].getCaracteristique().vie>0)
                                        {
                                            //lumiere=m_lumiere;
                                            lumiere.intensite=m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite;
                                            if(lumiere.intensite>255)
                                                lumiere.intensite=255;
                                            lumiereTemp=0;
                                            // Je réduit la quantité de lumière reçue, en fonction de la distance, qui est calculée avec un simple pythagore ^^
                                            lumiereTemp=lumiere.intensite-((((float)gpl::sqrt((m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().x-(m)*COTE_TILE)*(m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().x-(m)*COTE_TILE)+(m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().y-(l)*COTE_TILE)*(m_monstre[m_decor[1][j][k].getMonstre()].getCoordonneePixel().y-(l)*COTE_TILE))))*DIVISEUR_COTE_TILE)*60;
                                            lumiere.hauteur=m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().hauteur;

                                            if(lumiereTemp>0)
                                            lumiere.intensite=(int)lumiereTemp;
                                            else
                                            lumiere.intensite=0;

                                            if(lumiere.intensite>0)
                                            {
                                                lumiere.rouge=m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().rouge;
                                                lumiere.vert=m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().vert;
                                                lumiere.bleu=m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().bleu;
                                                    // Je calcule la couleur de la lumière, par rapport à l'ancienne lumière et à la nouvelle, pour faire un fondu
                                                lumiereTemp=m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite;

                                                if(lumiereTemp>255)
                                                    lumiereTemp=255;

                                                    // Je calcule la couleur de la lumière, par rapport à l'ancienne lumière et à la nouvelle, pour faire un fondu
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=(int)(lumiere.rouge*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=(int)(lumiere.vert*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);
                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=(int)(lumiere.bleu*lumiere.intensite+m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu*(int)lumiereTemp)/((int)lumiereTemp+lumiere.intensite);


                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].rouge=255;
                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].vert=255;
                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].bleu=255;

                                                m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite+=lumiere.intensite;

                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite>255)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=255;
                                                if(m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite<0)
                                                    m_tableauDesLampes[l-vueMin.y][m-vueMin.x].intensite=0;
                                            }
                                        }
                                    }
                                }
                    }
                }
            }
        }

        if(configuration.amelioration_lampes)
        for(int z=0;z<2;z++)
            for(int i=0;i<vueMax.y-vueMin.y;i++)
            for(int j=0;j<vueMax.x-vueMin.x;j++)
            {
                for(int y=-1;y<=1;y++)
                    for(int w=-1;w<=1;w++)
                        if(!((y==0&&w==0)))
                        {
                            if(i+y>0&&i+y+vueMin.y<m_decor[0].size()&&j+w>0&&j+w+vueMin.x<m_decor[0][0].size())
                            if(i+y>0&&i+y<30&&j+w>0&&j+w<30)
                            {
                                if((double)m_tableauDesLampes[i+y][j+w].intensite<(double)m_tableauDesLampes[i][j].intensite/1.5)
                                if(m_tableauDesLampes[i+y][j+w].intensite<32&&m_tableauDesLampes[i+y][j+w].intensite>=1)
                                {
                                    if(m_decor[1][i+vueMin.y][j+vueMin.x].getTileset()!= -1)
                                    {
                                    }
                                    else
                                    {
                                        bool erreur=false;
                                        if(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()>-1&&m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()<m_tileset.size())
                                        {
                                            if(m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='y'||m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='d')
                                                if(w>0)
                                                    erreur=true;
                                            if(m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='x'||m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='g')
                                                if(y>0)
                                                    erreur=true;
                                            if(m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='h')
                                                if(y>0||w>0)
                                                    erreur=true;
                                            if(m_tileset[m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTileset()].getOrientationDuTile(m_decor[1][i+vueMin.y+y][j+vueMin.x+w].getTile())=='b')
                                                if(y>0||w>0)
                                                    erreur=true;
                                        }

                                        if(!erreur)
                                        {
                                        m_tableauDesLampes[i+y][j+w].intensite=(int)(m_tableauDesLampes[i][j].intensite/1.5);
                                        m_tableauDesLampes[i+y][j+w].rouge=m_tableauDesLampes[i][j].rouge;
                                        m_tableauDesLampes[i+y][j+w].vert=m_tableauDesLampes[i][j].vert;
                                        m_tableauDesLampes[i+y][j+w].bleu=m_tableauDesLampes[i][j].bleu;
                                        if(m_tableauDesLampes[i+y][j+w].intensite>255)
                                            m_tableauDesLampes[i+y][j+w].intensite=255;
                                        }
                                    }
                                }
                            }
                        }

        }*/
}
void MoteurGraphique::AjouterCommandesMap(Map *map)
{
    /*coordonnee positionPartieDecor,vueMin,vueMax,positionHero;

	Sprite Sprite, spriteLumiereMask;

	 spriteLumiereMask.SetImage(lumiereMask);

	positionPartieDecor.x=0;
	positionPartieDecor.y=0;
	positionPartieDecor.w=128;
	positionPartieDecor.h=64;

	for(int i=0;i<m_tileset.size();i++)
		m_tileset[i].remiseAZeroDesSons();

    if(type==1)
    {
        coordonnee position;

        vueMin.x=hero->m_personnage.getCoordonnee().x-15;
        vueMin.y=hero->m_personnage.getCoordonnee().y-15;
        vueMax.x=hero->m_personnage.getCoordonnee().x+15;
        vueMax.y=hero->m_personnage.getCoordonnee().y+15;


        if(vueMin.x<0) { vueMin.x=0; }
        if(vueMin.y<0) { vueMin.y=0; }
        if(vueMax.x>m_decor[0][0].size()) { vueMax.x=m_decor[0][0].size(); }
        if(vueMax.y>m_decor[0].size()) { vueMax.y=m_decor[0].size(); }

        for(int couche=0;couche<2;couche++)
        {
            for(int j=hero->m_personnage.getCoordonnee().y-15;j<hero->m_personnage.getCoordonnee().y+15;j++)
            {
                for(int k=hero->m_personnage.getCoordonnee().x-15;k<hero->m_personnage.getCoordonnee().x+15;k++)
                {
                    position.x=(k-j-1+m_decor[0].size())*64;
                    position.y=(k+j)*32;
                    const sf::FloatRect& ViewRect = camera->GetRect();

                    if(j>=0&&j<m_decor[0].size()&&k>=0&&k<m_decor[0][0].size())
                    {
                        if(couche==1&&position.x+288>ViewRect.Left&&position.x<ViewRect.Right&&position.y+160>ViewRect.Top&&position.y<ViewRect.Bottom&&configuration.FonduLumiere)
                        {
                            spriteLumiereMask.SetX(position.x-128);
                            spriteLumiereMask.SetY(position.y-64);
                            int intensite=255-m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite;
                            if(intensite>255)
                                intensite=255;
                            if(intensite<0)
                                intensite=0;
                            if(intensite>0)
                            {
                                //EffectBlur.SetParameter("offset", 0.01);
                                spriteLumiereMask.SetColor(sf::Color(255,255,255,intensite));
                                ecran->Draw(spriteLumiereMask);
                            }

                        }

                        if(couche==1)
                        {
                            if(j>=0&&j<m_decor[0].size()&&k>=0&&k<m_decor[0][0].size())
                            if(configuration.Herbes)
                                if(m_decor[0][j][k].getHerbe()>=0&&m_decor[0][j][k].getHerbe()<m_herbe.size())
                                {
                                    positionPartieDecor=m_herbe[m_decor[0][j][k].getHerbe()].getPositionDuTile(m_decor[0][j][k].getNumeroHerbe());

                                    position.y-=32;


                                    Sprite.SetImage(*m_herbe[m_decor[0][j][k].getHerbe()].getImage());
                                    Sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));

                                    if(position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h<ViewRect.Bottom)
                                        {
                                            Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                            Sprite.SetY(position.y-positionPartieDecor.h+64);

                                            if(configuration.Lumiere)
                                            {
                                                Sprite.SetColor(sf::Color(
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].rouge)/255,
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].vert)/255,
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].bleu)/255,
                                                    255));
                                            }
                                            else
                                            {
                                                Sprite.SetColor(sf::Color(
                                                    255,
                                                    255,
                                                    255,
                                                    255));
                                            }

                                             ecran->Draw(Sprite);
                                        }
                                }

                        if(j>=0&&j<m_decor[0].size()&&k>=0&&k<m_decor[0][0].size())
                        {
                                if(m_monstreIllumine==m_decor[0][j][k].getMonstre()&&m_monstreIllumine!=-1||m_monstreIllumine==m_decor[1][j][k].getMonstre()&&m_monstreIllumine!=-1)
                                {
                                    LumiereOmbrage temp;
                                    if(configuration.Lumiere)
                                        temp=m_tableauDesLampes[j-vueMin.y][k-vueMin.x];

                                    temp.rouge=255;
                                    temp.vert=32;
                                    temp.bleu=32;
                                    temp.intensite=255;

                                    if(m_decor[0][j][k].getMonstre()>=0&&m_decor[0][j][k].getMonstre()<m_monstre.size())
                                        if(m_monstre[m_decor[0][j][k].getMonstre()].getCaracteristique().vie>0)
                                            m_monstre[m_decor[0][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&temp,&m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()]);
                                        else
                                            m_monstre[m_decor[0][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&m_tableauDesLampes[j-vueMin.y][k-vueMin.x],&m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()]);
                                    if(m_decor[1][j][k].getMonstre()>=0&&m_decor[1][j][k].getMonstre()<m_monstre.size())
                                        if(m_monstre[m_decor[1][j][k].getMonstre()].getCaracteristique().vie>0)
                                            m_monstre[m_decor[1][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&temp,&m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()]);
                                        else
                                            m_monstre[m_decor[1][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&m_tableauDesLampes[j-vueMin.y][k-vueMin.x],&m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()]);
                                    temp.detruire();
                                }
                                else
                                {
                                    LumiereOmbrage temp;
                                    if(configuration.Lumiere)
                                        temp=m_tableauDesLampes[j-vueMin.y][k-vueMin.x];



                                    if(m_decor[0][j][k].getMonstre()>=0&&m_decor[0][j][k].getMonstre()<m_monstre.size())
                                    {
                                        if(configuration.Lumiere)
                                        {
                                            temp.rouge=(temp.rouge*temp.intensite+m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().rouge*m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite)/(m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite+temp.intensite);
                                            temp.vert=(temp.vert*temp.intensite+m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().vert*m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite)/(m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite+temp.intensite);
                                            temp.bleu=(temp.bleu*temp.intensite+m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().bleu*m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite)/(m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite+temp.intensite);

                                            temp.intensite+=m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite;
                                            if(temp.intensite>255)
                                                temp.intensite=255;
                                            if(temp.intensite<0)
                                                temp.intensite=0;
                                            if(temp.rouge>255)
                                                temp.rouge=255;
                                            if(temp.vert<0)
                                                temp.vert=0;
                                            if(temp.bleu>255)
                                                temp.bleu=255;
                                        }
                                        m_monstre[m_decor[0][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&temp,&m_ModeleMonstre[m_monstre[m_decor[0][j][k].getMonstre()].getModele()]);
                                    }
                                    if(m_decor[1][j][k].getMonstre()>=0&&m_decor[1][j][k].getMonstre()<m_monstre.size())
                                    {
                                        if(configuration.Lumiere)
                                        {
                                            temp.rouge=(temp.rouge*temp.intensite+m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().rouge*m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite)/(m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite+temp.intensite);
                                            temp.vert=(temp.vert*temp.intensite+m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().vert*m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite)/(m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite+temp.intensite);
                                            temp.bleu=(temp.bleu*temp.intensite+m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().bleu*m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite)/(m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite+temp.intensite);

                                            temp.intensite+=m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()].getPorteeLumineuse().intensite;
                                            if(temp.intensite>255)
                                                temp.intensite=255;
                                            if(temp.intensite<0)
                                                temp.intensite=0;
                                            if(temp.rouge>255)
                                                temp.rouge=255;
                                            if(temp.vert<0)
                                                temp.vert=0;
                                            if(temp.bleu>255)
                                                temp.bleu=255;
                                        }
                                        m_monstre[m_decor[1][j][k].getMonstre()].Afficher(ecran,camera,position,getDimensions(),&temp,&m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()].getModele()]);
                                    }
                                }
                        }

                        if(hero->m_personnage.getCoordonnee().x==hero->m_personnage.getProchaineCase().x&&hero->m_personnage.getCoordonnee().y==hero->m_personnage.getProchaineCase().y)
                        {
                            if(hero->m_personnage.getCoordonnee().x==k&&hero->m_personnage.getCoordonnee().y==j)
                                hero->m_personnage.Afficher(ecran,camera,position,getDimensions(),&m_lumiereHero,&hero->m_modelePersonnage);
                        }
                        else
                        {
                            if(hero->m_personnage.getCoordonnee().x>hero->m_personnage.getProchaineCase().x&&hero->m_personnage.getCoordonnee().y>hero->m_personnage.getProchaineCase().y)
                            {
                                if(hero->m_personnage.getProchaineCase().x+1==k&&hero->m_personnage.getProchaineCase().y+1==j)
                                    hero->m_personnage.Afficher(ecran,camera,position,getDimensions(),&m_lumiereHero,&hero->m_modelePersonnage);
                            }
                            else
                                if(hero->m_personnage.getProchaineCase().x==k&&hero->m_personnage.getProchaineCase().y==j)
                                    hero->m_personnage.Afficher(ecran,camera,position,getDimensions(),&m_lumiereHero,&hero->m_modelePersonnage);
                        }






                        if(j>=0&&j<m_decor[0].size()&&k>=0&&k<m_decor[0][0].size())
                            if(configuration.Herbes)
                                if(m_decor[1][j][k].getHerbe()>=0&&m_decor[1][j][k].getHerbe()<m_herbe.size())
                                {
                                    position.x=(k-j-1+m_decor[1].size())*64;
                                    position.y=(k+j)*32;
                                    positionPartieDecor=m_herbe[m_decor[1][j][k].getHerbe()].getPositionDuTile(m_decor[1][j][k].getNumeroHerbe());

                                    position.y-=32;
                                    position.y+=positionPartieDecor.h/2;

                                    Sprite.SetImage(*m_herbe[m_decor[1][j][k].getHerbe()].getImage());
                                    Sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));

                                    if(position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h+64<ViewRect.Bottom)
                                        {
                                            Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                            Sprite.SetY(position.y-positionPartieDecor.h+64);

                                            if(configuration.Lumiere)
                                            {
                                                Sprite.SetColor(sf::Color(
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].rouge)/255,
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].vert)/255,
                                                    (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].bleu)/255,
                                                    255));
                                            }
                                            else
                                            {
                                                Sprite.SetColor(sf::Color(
                                                    255,
                                                    255,
                                                    255,
                                                    255));
                                            }

                                             ecran->Draw(Sprite);
                                        }
                                }
                    }


                        if(m_decor[couche][j][k].getTileset()>=0&&m_decor[couche][j][k].getTileset()<m_tileset.size())
                        {
                            positionPartieDecor=m_tileset[m_decor[couche][j][k].getTileset()].getPositionDuTile(m_decor[couche][j][k].getTile());
                            if(position.x+positionPartieDecor.h*2+positionPartieDecor.w>=ViewRect.Left&&position.x-positionPartieDecor.h*2-positionPartieDecor.w-64<ViewRect.Right&&position.y+positionPartieDecor.h+positionPartieDecor.w>=ViewRect.Top&&position.y-positionPartieDecor.h-32<ViewRect.Bottom+100&&m_tileset[m_decor[couche][j][k].getTileset()].getOmbreDuTile(m_decor[couche][j][k].getTile()&&configuration.Ombre&&m_tileset[m_decor[couche][j][k].getTileset()].getOmbreDuTile(m_decor[couche][j][k].getTile()))
                            ||position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h+64<ViewRect.Bottom)
                            {
                                Sprite.SetImage(*m_tileset[m_decor[couche][j][k].getTileset()].getImage());
                                Sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));

                                if(configuration.Ombre)
                                {
                                    if(m_tileset[m_decor[couche][j][k].getTileset()].getOmbreDuTile(m_decor[couche][j][k].getTile()))
                                    {
                                        for(int o=0;o<m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre.size();o++)
                                        {
                                            Sprite.SetColor(sf::Color(0,0,0,m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].intensite));

                                            Sprite.SetColor(sf::Color(0,0,0,m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].intensite));
                                            Sprite.SetX(position.x+64-positionPartieDecor.w/2+positionPartieDecor.w/2);
                                            Sprite.SetY(position.y-positionPartieDecor.h+64+positionPartieDecor.h-32);
                                           // Sprite.SetX(((m_positionPixel.x-m_positionPixel.y)*64/COTE_TILE+dimensionsMap.y*64)-64+(64-modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().w/2)+modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().w/2);
                                            //Sprite.SetY(((m_positionPixel.x+m_positionPixel.y)*64/COTE_TILE)/2+(64-modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().h)+modele->m_pose[m_etat][(int)(m_angle/45)][m_poseEnCours].getCoordonnee().h-32);
                                            Sprite.SetCenter((positionPartieDecor.w/2),(positionPartieDecor.h-32));
                                            Sprite.SetScale(1, m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].taille);
                                            Sprite.SetRotation(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle);
                                             if(m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle>90&&m_tableauDesLampes[j-vueMin.y][k-vueMin.x].m_ombre[o].angle<270)
                                                Sprite.FlipX(true);
                                            ecran->Draw(Sprite);
                                            Sprite.SetCenter(0,0);
                                            Sprite.SetScale(1, 1);
                                            Sprite.FlipX(false);
                                            Sprite.SetRotation(0);
                                        }
                                    }
                                }

                                positionHero.y=(int)((hero->m_personnage.getCoordonneePixel().x+hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE*32);
                                positionHero.x=(int)(((hero->m_personnage.getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE-1+m_decor[0].size())*64);

                                if(position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h+64<ViewRect.Bottom)
                                {
                                    if(couche==1)
                                    {
                                        Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                        Sprite.SetY(position.y-positionPartieDecor.h+64);
                                    }
                                    else
                                    {
                                        Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                        Sprite.SetY(position.y-positionPartieDecor.h/2+64);
                                    }

                                    int alpha=255;

                                    if(couche==1&&positionPartieDecor.h>256)
                                    {
                                        alpha=(int)((positionHero.y+32)-position.y)+64;
                                        //if(position.y>positionHero.y+96)
                                          //  alpha=32;

                                        if(alpha<64)
                                            alpha=64;
                                        if(alpha>255)
                                            alpha=255;
                                    }



                                    if(configuration.Lumiere&&configuration.FonduLumiere)
                                    {
                                        if(couche==1)
                                            Sprite.SetColor(sf::Color(
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].rouge)/255,
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].vert)/255,
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].bleu)/255,
                                            255));
                                        else
                                            Sprite.SetColor(sf::Color(
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].rouge),
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].vert),
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].bleu),
                                            alpha));
                                    }
                                    else if(configuration.Lumiere)
                                    {
                                        Sprite.SetColor(sf::Color(
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].rouge)/255,
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].vert)/255,
                                            (m_tableauDesLampes[j-vueMin.y][k-vueMin.x].intensite*m_tableauDesLampes[j-vueMin.y][k-vueMin.x].bleu)/255,
                                            alpha));
                                    }
                                    else
                                    {
                                        Sprite.SetColor(sf::Color(
                                            255,
                                            255,
                                            255,
                                            alpha));
                                    }

                                     ecran->Draw(Sprite);
                                }
                            }

                            positionHero.x=(hero->m_personnage.getCoordonnee().x-hero->m_personnage.getCoordonnee().y-1+m_decor[0].size())/5;
                            positionHero.y=(hero->m_personnage.getCoordonnee().x+hero->m_personnage.getCoordonnee().y)/5;

                            position.x=(k-j-1+m_decor[0].size())/5;
                            position.y=(k+j)/5;

                                if(m_tileset[m_decor[couche][j][k].getTileset()].getSonTile(m_decor[couche][j][k].getTile())>=0)
                                    m_tileset[m_decor[couche][j][k].getTileset()].jouerSon(m_tileset[m_decor[couche][j][k].getTileset()].getSonTile(m_decor[couche][j][k].getTile()),
                                    gpl::sqrt((hero->m_personnage.getCoordonnee().x-k)*(hero->m_personnage.getCoordonnee().x-k)+(hero->m_personnage.getCoordonnee().y-j)*(hero->m_personnage.getCoordonnee().y-j)),position,positionHero);
                        }
                    }
                    else
                    {
                        int w=j,z=k;

                        if(j>=m_decor[0].size())
                            w=m_decor[0].size()-1;
                        if(k>=m_decor[0][0].size())
                            z=m_decor[0][0].size()-1;
                        if(j<0)
                            w=0;
                        if(k<0)
                            z=0;

                        if(m_decor[couche][w][z].getTileset()>=0&&m_decor[couche][w][z].getTileset()<m_tileset.size())
                        {
                            if(couche==0)
                            {

                                positionPartieDecor=m_tileset[m_decor[couche][w][z].getTileset()].getPositionDuTile(m_decor[couche][w][z].getTile());
                                //positionPartieDecor.h/=2;
                                if(position.x+positionPartieDecor.w>=ViewRect.Left&&position.x<ViewRect.Right&&position.y+positionPartieDecor.h>=ViewRect.Top&&position.y-positionPartieDecor.h+64<ViewRect.Bottom)
                                {
                                    int alpha=255;
                                    if(couche==1)
                                    alpha=128;
                                    if(configuration.Lumiere)
                                    {
                                        Sprite.SetColor(sf::Color(
                                                (int)(((double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].intensite*(double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].rouge)*0.7/255),
                                                (int)(((double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].intensite*(double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].vert)*0.7/255),
                                                (int)(((double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].intensite*(double)m_tableauDesLampes[w-vueMin.y][z-vueMin.x].bleu)*0.7/255),
                                                alpha));
                                    }
                                    else
                                    {
                                        Sprite.SetColor(sf::Color(
                                                128,
                                                128,
                                                128,
                                                255));
                                    }
                                    Sprite.SetImage(*m_tileset[m_decor[couche][w][z].getTileset()].getImage());
                                    Sprite.SetSubRect(IntRect(positionPartieDecor.x, positionPartieDecor.y, positionPartieDecor.x+positionPartieDecor.w, positionPartieDecor.y+positionPartieDecor.h));
                                    if(couche==1)
                                    {
                                        Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                        Sprite.SetY(position.y-positionPartieDecor.h+64);
                                    }
                                    else
                                    {
                                        Sprite.SetX(position.x+64-positionPartieDecor.w/2);
                                        Sprite.SetY(position.y-positionPartieDecor.h/2+64);
                                    }
                                    ecran->Draw(Sprite);
                                }
                            }
                        }
                    }


                }
            }
        }

    }

	if(type==2)
	{
	    coordonneeDecimal position;
        vueMin.x=hero->m_personnage.getCoordonnee().x-15;
        vueMin.y=hero->m_personnage.getCoordonnee().y-15;
        vueMax.x=hero->m_personnage.getCoordonnee().x+16;
        vueMax.y=hero->m_personnage.getCoordonnee().y+15;

        Sprite.SetCenter(4*configuration.Resolution.x/800,4*configuration.Resolution.x/800);
        Sprite.SetRotation(45);

        Sprite.SetColor(sf::Color(255,255,255,128));

            for(int j=vueMin.y;j<vueMax.y;j++)
            {
                for(int k=vueMin.x;k<vueMax.x;k++)
                {
                    position.x=(float)(((k-vueMin.x)-(j-vueMin.y)-1+40)*6*(float)configuration.Resolution.x/800);
                    position.y=(float)(((k-vueMin.x)+(j-vueMin.y))*6*(float)configuration.Resolution.x/800);
                    if(position.x+465*configuration.Resolution.x/800>605*configuration.Resolution.x/800&&position.x+465*configuration.Resolution.x/800<800*configuration.Resolution.x/800&&position.y*configuration.Resolution.x/800>0&&position.y-80*configuration.Resolution.x/800<195*configuration.Resolution.x/800)
                    {
                        if(getTypeCase(k,j)==1)
                        {
                            Sprite.SetImage(carreBrun);
                            Sprite.SetX((float)(position.x+465*configuration.Resolution.x/800));
                            Sprite.SetY((float)(position.y-80*configuration.Resolution.x/800));
                            ecran->Draw(Sprite);
                        }

                        if(getTypeCase(k,j)==2)
                        {
                            Sprite.SetImage(carreRouge);

                            Sprite.SetX(position.x+465*configuration.Resolution.x/800);
                            Sprite.SetY(position.y-80*configuration.Resolution.x/800);
                            ecran->Draw(Sprite);
                        }

                        if(getTypeCase(k,j)==3)
                        {
                                Sprite.SetImage(carreVert);
                                Sprite.SetX(position.x+465*configuration.Resolution.x/800);
                                Sprite.SetY(position.y-80*configuration.Resolution.x/800);
                                ecran->Draw(Sprite);
                        }

                        if(hero->m_personnage.getCoordonnee().x==k&&hero->m_personnage.getCoordonnee().y==j)
                        {
                                Sprite.SetImage(carreBleu);
                                Sprite.SetX((float)(position.x+465*configuration.Resolution.x/800));
                                Sprite.SetY((float)(position.y-80*configuration.Resolution.x/800));
                                ecran->Draw(Sprite);
                        }
                    }
                }
            }
	}*/
}
