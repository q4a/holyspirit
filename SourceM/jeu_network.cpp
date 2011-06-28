#include "constantes.h"
#include "globale.h"
#include "jeu.h"

struct jeu_client
{
    jeu_client(Jeu *j,sf::TcpSocket *c) : jeu(j), client(c) {}

    Jeu *jeu;
    sf::TcpSocket *client;
};

sf::Packet& operator <<(sf::Packet& Packet, const Caracteristique& C)
{
    return Packet << C.vie << C.maxVie << C.niveau;
}
sf::Packet& operator >>(sf::Packet& Packet, Caracteristique& C)
{
    return Packet >> C.vie >> C.maxVie >> C.niveau;
}

sf::Packet& operator <<(sf::Packet& Packet, const coordonneeDecimal& C)
{
    return Packet << C.x << C.y << C.h << C.w;
}
sf::Packet& operator >>(sf::Packet& Packet, coordonneeDecimal& C)
{
    return Packet >> C.x >> C.y >> C.h >> C.w;
}

sf::Packet& operator <<(sf::Packet& Packet, const coordonnee& C)
{
    return Packet << C.x << C.y << C.h << C.w;
}
sf::Packet& operator >>(sf::Packet& Packet, coordonnee& C)
{
    return Packet >> C.x >> C.y >> C.h >> C.w;
}

sf::Packet& operator <<(sf::Packet& Packet,  Personnage& C)
{
    return Packet << C.getCaracteristique() << C.getCoordonneePixel() << C.getCoordonnee() << (sf::Int8)C.getEtat()  << (sf::Int8)C.m_entite_graphique.m_noAnimation << (sf::Int16)C.getAngle();
}
sf::Packet& operator >>(sf::Packet& Packet, Personnage& C)
{
    coordonnee pos2;
    coordonneeDecimal pos;
    sf::Int8 etat, pose;
    sf::Int16 angle;

    Caracteristique caract = C.getCaracteristique();

    Packet >> caract >> pos >> pos2 >> etat >> pose >> angle;
    C.setJustCoordonnee(pos2, pos);
    C.setEtat(etat,pose);
    C.setForcedAngle(angle);
    C.setCaracteristique(caract);

    return Packet;
}



void GererReseauClientTCP(void* UserData)
{
    sf::Context context;
    Jeu *jeu = static_cast<Jeu*>(UserData);

    while(jeu->m_host)
    {
        //if(jeu->m_contexte != jeu->m_mainMenu)
        {
            sf::Packet packet;
            if (jeu->m_host->Receive(packet) == sf::Socket::Done)
            {
                sf::Int8 type;
                packet>>type;
                if(type == P_NEWPLAYER)
                {
                    std::string cheminclasse, nom;
                    Caracteristique caract;
                    packet>>caract.nom>>caract>>cheminclasse;
                    jeu->m_personnageClients.push_back(Hero ());
                    jeu->m_personnageClients.back().m_caracteristiques = caract;
                    jeu->m_personnageClients.back().m_personnage.setCaracteristique(caract);
                    jeu->m_personnageClients.back().m_personnage.m_heroic = true;

                    jeu->GlobalMutex.Lock();

                    jeu->m_personnageClients.back().m_classe.Charger(cheminclasse,std::vector<int> (),caract);
                    jeu->m_personnageClients.back().ChargerModele();

                    jeu->hero.m_amis.push_back(&jeu->m_personnageClients.back().m_personnage);
                    std::cout<<"Joueur " << caract.nom << std::endl;

                    jeu->GlobalMutex.Unlock();
                }
                else if(type == P_DELETEPLAYER)
                {
                    sf::Int8 no;
                    packet>>no;

                    jeu->DeletePersonnageClient((int)no);
                }
                else if(type == P_PLAYERSKIN)
                {
                    sf::Int8 no;
                    packet>>no;

                    int i = 0;
                    for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                            p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                        if(i == no)
                        {
                            sf::Int8 cas;
                            packet>>cas;
                            p->m_cas = cas;
                            for (int k=0; k<NOMBRE_MORCEAU_PERSONNAGE; ++k)
                                packet>>p->m_cheminModeleNouveau[k]>>p->m_pasEquipe[k];

                            jeu->GlobalMutex.Lock();
                            p->ChargerGraphics();
                            jeu->GlobalMutex.Unlock();
                        }
                }
                else if(type == P_CHANGEMAP)
                {
                    std::string prochaineMap;
                    coordonnee coordonneePerso;
                    packet>>prochaineMap>>coordonneePerso;


                    jeu->GlobalMutex.Lock();

                    jeu->m_chargement->setC_Chargement(prochaineMap,coordonneePerso);
                    jeu->m_contexte = jeu->m_chargement;

                    jeu->GlobalMutex.Unlock();
                }
                else if(type == P_DEGATS)
                {
                    sf::Uint16 degats;
                    sf::Int8  typeD;
                    sf::Uint16 temps;
                    packet>>degats>>typeD>>temps;
                    jeu->hero.m_personnage.InfligerDegats(degats, typeD, temps);
                }
                else if(type == P_KILLMONSTRE && jeu->map)
                {
                    sf::Int16 no;
                    sf::Int16 angle;
                    sf::Uint16 degats;
                    packet>>no>>angle>>degats;

                    if(jeu->map->m_loaded)
                        jeu->map->KillMonstre(jeu->map->getEntiteMonstre(no),angle,degats,jeu);
                }
            }
        }
    }
}
void GererReseauClientUDP(void* UserData)
{
    Jeu *jeu = static_cast<Jeu*>(UserData);

    sf::IpAddress ip;// = jeu->m_host->GetRemoteAddress();
    short unsigned int port;// = 6666;

    while(jeu->m_host)
        // if(jeu->m_contexte != jeu->m_mainMenu)
    {
        sf::Packet packet;
        if(jeu->m_udp.Receive(packet, ip, port) == sf::Socket::Done)
        {
            sf::Int8 type;
            packet>>type;

            if(type == P_PLAYERCARACT)
            {
                sf::Int8 no;
                packet>>no;

                int i = 0;
                for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                        p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                    if(i == no)
                    {
                        packet>>p->m_personnage;
                        p->m_caracteristiques.vie = p->m_personnage.getCaracteristique().vie;
                    }
            }
            else if(type == P_INFOSMONSTRE && jeu->map)
            {
                do
                {
                    sf::Int16 no;
                    packet>>no;

                    if(jeu->map->m_loaded)
                    {
                        int x = (int)((jeu->map->getEntiteMonstre(no))->getCoordonneePixel().x/COTE_TILE + 0.5);
                        int y = (int)((jeu->map->getEntiteMonstre(no))->getCoordonneePixel().y/COTE_TILE + 0.5);
                        packet>>*(jeu->map->getEntiteMonstre(no));
                        jeu->map->DeplacerMonstreCase(no,x,y);
                    }

                    if(!packet.EndOfPacket())
                        packet>>type;
                    else
                        type = -1;

                }while(type == P_INFOSMONSTRE);
            }
        }
    }
}

void GererReseauHostTCP(void* UserData)
{
    sf::Context context;
    Jeu *jeu = static_cast<Jeu*>(UserData);

    while(jeu->m_runTCPHost)
    {
        //  if(jeu->m_contexte != jeu->m_mainMenu)
        {
            if (jeu->m_selector.Wait(1000))
            {
                if (jeu->m_selector.IsReady(jeu->m_listener))
                {
                    sf::TcpSocket* client = new sf::TcpSocket;
                    if (jeu->m_listener.Accept(*client) == sf::Socket::Done)
                        jeu->AddClient(client);
                    else
                        delete client;
                }
                else
                {
                    int no = 0;
                    for (std::list<sf::TcpSocket*>::iterator it = jeu->m_clients.begin(); it != jeu->m_clients.end(); ++it, ++no)
                    {
                        sf::TcpSocket& client = **it;
                        if (jeu->m_selector.IsReady(client))
                        {
                            sf::Packet packet;
                            if (client.Receive(packet) == sf::Socket::Done)
                                jeu->CheckPacket(packet, no, it);
                        }
                    }
                }
            }
        }
    }
}

void GererReseauHostUDP(void* UserData)
{
    sf::Context context;
    jeu_client *o = static_cast<jeu_client*>(UserData);

    Jeu *jeu = o->jeu;
    sf::TcpSocket *client = o->client;

    sf::UdpSocket *udp = &jeu->m_clients_udp.back()->socket;
    bool *run = &jeu->m_clients_udp.back()->running;

    udp->Bind(6668);
    udp->SetBlocking(false);

    jeu->m_ready = true;

    while(*run)
    {
        int no = 0;
        for (std::list<sf::TcpSocket*>::iterator it = jeu->m_clients.begin(); it != jeu->m_clients.end(); ++it, ++no)
            if(*it == client)
            {
                it = jeu->m_clients.end();
                break;
            }

        sf::IpAddress ip = client->GetRemoteAddress();
        short unsigned int port = 6668;
        sf::Packet packet;

        if(udp->Receive(packet, ip, port) == sf::Socket::Done)
        {
            sf::Int8 type;
            packet>>type;

            if(type == P_PLAYERCARACT)
            {
                Personnage *perso = NULL;
                int i = 0;
                for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                        p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                    if(i == no)
                    {
                        packet>>p->m_personnage;
                        perso = &p->m_personnage;

                        p->m_caracteristiques.vie =  p->m_personnage.getCaracteristique().vie;
                    }

                int no2 = 1;

                if(perso)
                    for (std::list<sf::TcpSocket*>::iterator it2 = jeu->m_clients.begin(); it2 != jeu->m_clients.end(); ++it2,++no2)
                        if(*it2 != client)
                        {
                            sf::TcpSocket& client2 = **it2;
                            sf::Packet packet2;
                            packet2<<(sf::Int8)P_PLAYERCARACT<<(sf::Int8)(no+(no<no2))<<*perso;

                            sf::UdpSocket updS;
                            sf::IpAddress ip2 = client2.GetRemoteAddress();
                            updS.Send(packet, ip2, 6667);
                        }
            }
        }
    }
}



void Jeu::GererMultijoueur()
{
    if(configuration->hote)
    {
        int no = 0;
        for (std::list<sf::TcpSocket*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it, ++no)
        {
            sf::TcpSocket& client = **it;

            sf::Packet ping;
            ping<<(sf::Int8)(-1);
            if(client.Send(ping) != sf::TcpSocket::Done)
            {
                m_selector.Remove(*(*it));
                m_clients.erase(it);
                //delete *&*it;

                int i = 0;

                for (std::list<ClientUDP*>::iterator p = m_clients_udp.begin() ;
                     p != m_clients_udp.end()&& i <= no; ++p, ++i)
                    if(i == no)
                    {
                        (*p)->running = false;
                        (*p)->socket.Unbind();

                        (*p)->thread->Wait();
                        delete (*p)->thread;

                        delete *p;
                        m_clients_udp.erase(p);
                    }

                if(!DeletePersonnageClient(no))
                    std::cout<<"Un joueur est partit"<<std::endl;
                int no2 = 1;
                for (std::list<sf::TcpSocket*>::iterator it2 = m_clients.begin(); it2 != m_clients.end(); ++it2,++no2)
                {
                    sf::Packet packet2;
                    packet2<<(sf::Int8)P_DELETEPLAYER<<(sf::Int8)(no+(no>no2));
                    sf::TcpSocket& client2 = **it2;
                    client2.Send(packet2);
                }

                it = m_clients.begin();
                no = 0;
            }
            else
            {
                sf::Packet packet2;
                sf::UdpSocket upd;
                packet2<<(sf::Int8)P_PLAYERCARACT<<(sf::Int8)0<<hero.m_personnage;
                sf::IpAddress ip = client.GetRemoteAddress();
                short unsigned int port = 6667;
                upd.Send(packet2, ip, port);
            }
        }
    }
    else if(m_host)
    {
        sf::Packet ping;
        ping<<(sf::Int8)(-1);
        if(!(m_host->Send(ping) == sf::TcpSocket::Done))
        {
            m_mainMenu->m_save  = true;
            m_mainMenu->m_reset = true;
            m_contexte = m_mainMenu;
            std::cout<<"L'hôte à quitté la partie"<<std::endl;

            Disconnect();
            CloseServer();
        }
        else
        {
            sf::Packet packet;
            sf::UdpSocket upd;
            packet<<(sf::Int8)P_PLAYERCARACT<<hero.m_personnage;
            sf::IpAddress ip = m_host->GetRemoteAddress();
            short unsigned int port = 6668;
            upd.Send(packet, ip, port);
        }
    }
}

void Jeu::LaunchServer()
{
    CloseServer();
    m_runTCPHost = true;
    m_thread_hostTCP = new sf::Thread(&GererReseauHostTCP, this);
    m_thread_hostTCP->Launch();
}

void Jeu::CloseServer()
{
    if(m_thread_hostTCP)
    {
        m_runTCPHost = false;
        m_listener.Close();
        m_thread_hostTCP->Wait();

      //  m_thread_host->Wait();
        delete m_thread_hostTCP;
    }
    m_thread_hostTCP = NULL;


    for (std::list<ClientUDP*>::iterator p = m_clients_udp.begin();
            p != m_clients_udp.end(); ++p)
        {
            (*p)->running = false;
            (*p)->socket.Unbind();
            (*p)->thread->Wait();

            delete (*p)->thread;
            delete (*p);
        }

    m_clients_udp.clear();

    m_personnageClients.clear();

    for (std::list<sf::TcpSocket*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
        delete *it;
    m_clients.clear();
}

bool Jeu::Connect(sf::IpAddress ServerAddress)
{
    m_host = new sf::TcpSocket();

    if (m_host->Connect(ServerAddress, 6666, 3000) != sf::TcpSocket::Done)
    {
        delete m_host;
        m_host = NULL;
        return false;
    }

    m_udp.Bind(6667);

    m_thread_clientTCP = new sf::Thread(&GererReseauClientTCP, this);
    m_thread_clientTCP->Launch();

    m_thread_clientUDP = new sf::Thread(&GererReseauClientUDP, this);
    m_thread_clientUDP->Launch();

    return true;
}

void Jeu::Disconnect()
{
    std::cout<<"Vous avez ete deconnecte"<<std::endl;

   // GlobalMutex.Lock();
    if(m_host)
    m_host->Disconnect();
    if(m_host)
        delete m_host;
    m_host = NULL;
    m_udp.Unbind();
   // GlobalMutex.Unlock();

    if(m_thread_clientTCP)
    {
        //m_thread_clientTCP->Terminate();
        m_thread_clientTCP->Wait();
        delete m_thread_clientTCP;
    }

    if(m_thread_clientUDP)
    {
        //m_thread_clientUDP->Terminate();
        m_thread_clientUDP->Wait();
        delete m_thread_clientUDP;
    }

    m_thread_clientTCP = NULL;
    m_thread_clientUDP = NULL;
}


void Jeu::AddClient(sf::TcpSocket* client)
{
    sf::Packet packet;

    m_clients.push_back(client);

    jeu_client temp(this, client);

    m_ready = false;

    m_clients_udp.push_back(new ClientUDP ());
    m_clients_udp.back()->running = true;
    m_clients_udp.back()->thread = new sf::Thread(&GererReseauHostUDP, &temp);
    m_clients_udp.back()->thread->Launch();

    //m_thread_hostUDP.push_back(new sf::Thread(&GererReseauHostUDP, &temp));
    //m_thread_hostUDP.back()->Launch();

    while(!m_ready)
    std::cout<<m_ready<<std::endl;

    std::cout<<"Un joueur arrive !"<<std::endl;

    m_selector.Add(*client);

    packet<<(sf::Int8)P_NEWPLAYER<<hero.m_caracteristiques.nom<<hero.m_caracteristiques<<hero.m_cheminClasse;
    client->Send(packet);

    packet.Clear();
    packet<<(sf::Int8)P_PLAYERSKIN<<(sf::Int8)0<<(sf::Int8)hero.m_cas;
    for (int i=0; i<NOMBRE_MORCEAU_PERSONNAGE; ++i)
        packet<<hero.m_cheminModele[i]<<hero.m_pasEquipe[i];
    client->Send(packet);

    int no2 = 1;
    for (std::list<Hero>::iterator p = m_personnageClients.begin();
            p != m_personnageClients.end(); ++p, ++no2)
    {
        packet.Clear();
        packet<<(sf::Int8)P_NEWPLAYER<<p->m_caracteristiques.nom<<p->m_caracteristiques<<p->m_cheminClasse;
        client->Send(packet);

        packet.Clear();
        packet<<(sf::Int8)P_PLAYERSKIN<<(sf::Int8)no2<<(sf::Int8)p->m_cas;
        for (int i=0; i<NOMBRE_MORCEAU_PERSONNAGE; ++i)
            packet<<p->m_cheminModele[i]<<p->m_pasEquipe[i];
        client->Send(packet);
    }
}

void Jeu::CheckPacket(sf::Packet &packet, int no, std::list<sf::TcpSocket*>::iterator it)
{
    sf::Int8 type;
    packet>>type;

    if(type == P_NEWPLAYER)
    {
        Caracteristique caract;
        std::string cheminclasse;
        packet>>caract.nom>>caract>>cheminclasse;
        std::cout<<caract.nom<<" s'est connecte a la partie"<<std::endl;
        m_personnageClients.push_back(Hero ());
        m_personnageClients.back().m_caracteristiques = caract;
        m_personnageClients.back().m_personnage.setCaracteristique(caract);
        m_personnageClients.back().m_personnage.m_heroic = true;

        m_personnageClients.back().m_classe.Charger(cheminclasse,std::vector<int> (),caract);
        m_personnageClients.back().ChargerModele();

        hero.m_amis.push_back(&m_personnageClients.back().m_personnage);

        sf::Packet packet2;
        packet2<<(sf::Int8)P_NEWPLAYER<<caract.nom<<caract<<cheminclasse;
        for (std::list<sf::TcpSocket*>::iterator it2 = m_clients.begin(); it2 != m_clients.end(); ++it2)
            if(&*it2 != &*it)
            {
                sf::TcpSocket& client2 = **it2;
                client2.Send(packet2);
            }

        packet.Clear();
        packet<<(sf::Int8)P_CHANGEMAP<<map->getNom()<<hero.m_personnage.getCoordonnee();
        (*it)->Send(packet);
    }
    else if(type == P_PLAYERSKIN)
    {
        int i = 0;
        for (std::list<Hero>::iterator p = m_personnageClients.begin();
                p != m_personnageClients.end() && i <= no; ++p, ++i)
            if(i == no)
            {
                sf::Int8 cas;
                packet>>cas;
                p->m_cas = cas;
                for (int k=0; k<NOMBRE_MORCEAU_PERSONNAGE; ++k)
                    packet>>p->m_cheminModeleNouveau[k]>>p->m_pasEquipe[k];
                p->ChargerGraphics();
            }
    }
    else if(type == P_CHANGEMAP)
    {
        std::string prochaineMap;
        coordonnee coordonneePerso;
        packet>>prochaineMap>>coordonneePerso;
        SendChangeMap(prochaineMap, coordonneePerso);


        GlobalMutex.Lock();

        m_chargement->setC_Chargement(prochaineMap,coordonneePerso);
        m_contexte = m_chargement;

        GlobalMutex.Unlock();
    }
    else if(type == P_DEGATS && map && map->m_loaded)
    {
        sf::Int16  no2;
        sf::Uint16 degats;
        sf::Int8  typeD;
        sf::Uint16 temps;
        packet>>no2>>degats>>typeD>>temps;

        int i = 0;
        for (std::list<Hero>::iterator p = m_personnageClients.begin();
                p != m_personnageClients.end() && i <= no; ++p, ++i)
            if(i == no)
                map->InfligerDegats((int)no2,&p->m_personnage,(float)degats,(int)typeD,this,(float)temps);
    }
}

bool Jeu::DeletePersonnageClient(int no)
{
    int i = 0;
    for (std::list<Hero>::iterator p = m_personnageClients.begin();
            p != m_personnageClients.end() && i <= no; ++p, ++i)
        if(i == no)
        {
            std::cout<<p->m_caracteristiques.nom<<" est partit"<<std::endl;

            GlobalMutex.Lock();
            for(unsigned j = 0 ; j < hero.m_amis.size() ; ++j)
                if(hero.m_amis[j] == &p->m_personnage)
                    hero.m_amis.erase(hero.m_amis.begin() + j);
            m_personnageClients.erase(p);
            GlobalMutex.Unlock();

            return true;
        }
    return false;
}

void Jeu::SendSkin()
{
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_PLAYERSKIN;

        if(configuration->hote)
            packet<<(sf::Int8)0;

        packet<<(sf::Int8)hero.m_cas;

        for (int i=0; i<NOMBRE_MORCEAU_PERSONNAGE; ++i)
            packet<<hero.m_cheminModele[i]<<hero.m_pasEquipe[i];

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.Send(packet);
            }
        }
        else
            m_host->Send(packet);
    }
}

void Jeu::SendChangeMap(const std::string &prochaineMap,const coordonnee &coordonneePerso)
{
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_CHANGEMAP<<prochaineMap<<coordonneePerso;

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.Send(packet);
            }
        }
        else
            m_host->Send(packet);
    }
}

void Jeu::SendInfosMonstre(sf::Packet &packet)
{
    if(configuration->multi && configuration->hote)
    {
        for (std::list<sf::TcpSocket*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
        {
            sf::TcpSocket& client = **it;

            sf::UdpSocket upd;
            sf::IpAddress ip = client.GetRemoteAddress();
            short unsigned int port = 6667;
            upd.Send(packet, ip, port);
        }
    }
}

void Jeu::SendDegats(Hero *hero2, float degats, int type, float temps)
{
    if(configuration->multi && configuration->hote)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_DEGATS<<(sf::Uint16)degats<<(sf::Int8)type<<(sf::Uint16)temps;

        int no = 0;
        for (std::list<Hero>::iterator p = m_personnageClients.begin();
                p != m_personnageClients.end() && &*p != hero2; ++p, ++no) {}

        int no2 = 0;
        for (std::list<sf::TcpSocket*>::iterator it = m_clients.begin(); it != m_clients.end() && no2 < no; ++it, ++no2)
            if(no == no2 + 1)
            {
                sf::TcpSocket& client = **it;
                client.Send(packet);
            }
    }
}

void Jeu::SendDegats(int no, float degats, int type, float temps)
{
    if(configuration->multi && !configuration->hote)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_DEGATS<<(sf::Int16)no<<(sf::Uint16)degats<<(sf::Int8)type<<(sf::Uint16)temps;
        m_host->Send(packet);
    }
}

void Jeu::SendKillMonstre(int no, int angle, float degats)
{
    if(configuration->multi && configuration->hote)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_KILLMONSTRE<<(sf::Int16)no<<(sf::Int16)angle<<(sf::Uint16)degats;

        for (std::list<sf::TcpSocket*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
        {
            sf::TcpSocket& client = **it;
            client.Send(packet);
        }
    }
}


