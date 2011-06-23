#include "constantes.h"
#include "globale.h"
#include "jeu.h"

sf::Packet& operator <<(sf::Packet& Packet, const Caracteristique& C)
{
    return Packet << C.nom << C.vitesse << C.vie << C.maxVie << C.niveau;
}
sf::Packet& operator >>(sf::Packet& Packet, Caracteristique& C)
{
    return Packet >> C.nom >> C.vitesse >> C.vie >> C.maxVie >> C.niveau;
}

sf::Packet& operator <<(sf::Packet& Packet, const coordonneeDecimal& C)
{
    return Packet << C.x << C.y << C.h << C.w;
}
sf::Packet& operator >>(sf::Packet& Packet, coordonneeDecimal& C)
{
    return Packet >> C.x >> C.y >> C.h >> C.w;
}

sf::Packet& operator <<(sf::Packet& Packet,  Personnage& C)
{
    return Packet << C.getCoordonneePixel() << (sf::Int8)C.getEtat() << (sf::Int16)C.getAngle() << (sf::Int8)C.m_entite_graphique.m_noAnimation;
}
sf::Packet& operator >>(sf::Packet& Packet, Personnage& C)
{
    coordonneeDecimal pos;
    sf::Int8 etat, anim;
    sf::Int16 angle;

    Packet >> pos >> etat >> angle;
    C.setCoordonneePixel2(pos);
    C.setEtat(etat);
    C.setForcedAngle(angle);

    return Packet >> C.m_entite_graphique.m_noAnimation;
}



void GererReseauClientTCP(void* UserData)
{
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
                     std::string cheminclasse;
                     Caracteristique caract;
                     packet>>caract>>cheminclasse;
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
            }
        }
    }
}
void GererReseauClientUDP(void* UserData)
{
    Jeu *jeu = static_cast<Jeu*>(UserData);

    sf::UdpSocket udp;
    udp.Bind(6667);

    sf::IpAddress ip;// = jeu->m_host->GetRemoteAddress();
    short unsigned int port;// = 6666;

    while(1)
   // if(jeu->m_contexte != jeu->m_mainMenu)
    {
        sf::Packet packet;
        if(udp.Receive(packet, ip, port) == sf::Socket::Done)
        {
             sf::Int8 type;
             packet>>type;

             if(type == P_PLAYERCARACT)
             {
                 sf::Int8 no;
                 packet>>no;

                 Caracteristique caract;
                 packet>>caract;

                 int i = 0;
                 for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                    p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                    if(i == no)
                    {
                        p->m_caracteristiques.vie = caract.vie;
                        p->m_personnage.setCaracteristique(p->m_caracteristiques);
                        packet>>p->m_personnage;
                    }
             }
        }
     }
}

void GererReseauHost(void* UserData)
{
    Jeu *jeu = static_cast<Jeu*>(UserData);

    sf::UdpSocket udp;
    udp.Bind(6668);

    while(1)
    {
      //  if(jeu->m_contexte != jeu->m_mainMenu)
        {
             int no = 0;
             for (std::list<sf::TcpSocket*>::iterator it = jeu->m_clients.begin(); it != jeu->m_clients.end(); ++it, ++no)
             {
                sf::TcpSocket& client = **it;

                sf::IpAddress ip = client.GetRemoteAddress();
                short unsigned int port = 6668;
                sf::Packet packet;
                if(udp.Receive(packet, ip, port) == sf::Socket::Done)
                {
                     sf::Int8 type;
                     packet>>type;

                     if(type == P_PLAYERCARACT)
                     {
                         Caracteristique caract;
                         packet>>caract;

                         Personnage *perso = NULL;

                         int i = 0;
                         for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
                            p != jeu->m_personnageClients.end() && i <= no; ++p, ++i)
                            if(i == no)
                            {
                                p->m_caracteristiques.vie = caract.vie;
                                p->m_personnage.setCaracteristique(p->m_caracteristiques);
                                packet>>p->m_personnage;
                                perso = &p->m_personnage;
                            }

                         int no2 = 1;

                         if(perso)
                         for (std::list<sf::TcpSocket*>::iterator it2 = jeu->m_clients.begin(); it2 != jeu->m_clients.end(); ++it2,++no2)
                         if(*it2 != *it)
                         {
                                sf::TcpSocket& client2 = **it2;
                                sf::Packet packet2;
                                packet2<<(sf::Int8)P_PLAYERCARACT<<(sf::Int8)(no+(no<no2))<<caract<<*perso;

                                sf::UdpSocket updS;
                                sf::IpAddress ip = client2.GetRemoteAddress();
                                updS.Send(packet, ip, 6667);
                         }
                     }
                }
             }
        }
    }
}



void Jeu::GererMultijoueur()
{
    if(configuration->hote)
    {
        if (m_selector.Wait(2))
        {
             if (m_selector.IsReady(m_listener))
             {
                 sf::TcpSocket* client = new sf::TcpSocket;
                 if (m_listener.Accept(*client) == sf::Socket::Done)
                 {
                     m_clients.push_back(client);

                     std::cout<<"Un joueur arrive !"<<std::endl;

                     m_selector.Add(*client);


                     sf::Packet packet2;
                     packet2<<(sf::Int8)P_NEWPLAYER<<hero.m_caracteristiques<<hero.m_cheminClasse;
                     client->Send(packet2);

                     for (std::list<Hero>::iterator p = m_personnageClients.begin();
                            p != m_personnageClients.end(); ++p)
                    {
                        sf::Packet packet3;
                        packet3<<(sf::Int8)P_NEWPLAYER<<p->m_caracteristiques<<p->m_cheminClasse;
                        client->Send(packet3);
                    }
                 }
             }
             else
             {
                 int no = 0;
                 for (std::list<sf::TcpSocket*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it, ++no)
                 {
                     sf::TcpSocket& client = **it;
                     if (m_selector.IsReady(client))
                     {
                         sf::Packet packet;
                         if (client.Receive(packet) == sf::Socket::Done)
                         {
                             sf::Int8 type;
                             packet>>type;
                             if(type == P_NEWPLAYER)
                             {
                                 Caracteristique caract;
                                 std::string cheminclasse;
                                 packet>>caract>>cheminclasse;
                                 std::cout<<caract.nom<<" s'est connecte a la partie"<<std::endl;
                                 m_personnageClients.push_back(Hero ());
                                 m_personnageClients.back().m_caracteristiques = caract;
                                 m_personnageClients.back().m_personnage.setCaracteristique(caract);
                                 m_personnageClients.back().m_personnage.m_heroic = true;

                                 m_personnageClients.back().m_classe.Charger(cheminclasse,std::vector<int> (),caract);
                                 m_personnageClients.back().ChargerModele();

                                 hero.m_amis.push_back(&m_personnageClients.back().m_personnage);

                                 sf::Packet packet2;
                                 packet2<<(sf::Int8)P_NEWPLAYER<<caract<<cheminclasse;
                                 for (std::list<sf::TcpSocket*>::iterator it2 = m_clients.begin(); it2 != m_clients.end(); ++it2)
                                 if(&*it2 != &*it)
                                 {
                                        sf::TcpSocket& client2 = **it2;
                                        client2.Send(packet2);
                                 }
                             }
                         }
                     }
                 }
             }
         }

         int no = 0;
         for (std::list<sf::TcpSocket*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it, ++no)
         {
            sf::TcpSocket& client = **it;

            sf::Packet ping;
            ping<<(sf::Int8)(-1);
            if(client.Send(ping) != sf::TcpSocket::Done)
            {
                 m_clients.erase(it);

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

            sf::Packet packet2;
            sf::UdpSocket upd;
            packet2<<(sf::Int8)P_PLAYERCARACT<<(sf::Int8)0<<hero.m_caracteristiques<<hero.m_personnage;
            sf::IpAddress ip = client.GetRemoteAddress();
            short unsigned int port = 6667;
            upd.Send(packet2, ip, port);
         }
    }
    else
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
        }

        sf::Packet packet;
        sf::UdpSocket upd;
        packet<<(sf::Int8)P_PLAYERCARACT<<hero.m_caracteristiques<<hero.m_personnage;
        sf::IpAddress ip = m_host->GetRemoteAddress();
        short unsigned int port = 6668;
        upd.Send(packet, ip, port);
    }
}

void Jeu::LaunchServer()
{
    if(m_thread_host)
        CloseServer();
    m_thread_host = new sf::Thread(&GererReseauHost, this);
    m_thread_host->Launch();
}

void Jeu::CloseServer()
{
    if(m_thread_host)
    {
        m_thread_host->Terminate();
        delete m_thread_host;
    }
    m_thread_host = NULL;
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

    m_thread_clientTCP = new sf::Thread(&GererReseauClientTCP, this);
    m_thread_clientTCP->Launch();

    m_thread_clientUDP = new sf::Thread(&GererReseauClientUDP, this);
    m_thread_clientUDP->Launch();

    return true;
}

void Jeu::Disconnect()
{
    if(m_thread_clientTCP)
    {
        m_thread_clientTCP->Terminate();
        delete m_thread_clientTCP;
    }
    if(m_thread_clientUDP)
    {
        m_thread_clientUDP->Terminate();
        delete m_thread_clientUDP;
    }

    if(m_host)
        delete m_host;

    m_host = NULL;
    m_thread_clientTCP = NULL;
    m_thread_clientUDP = NULL;
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
            for(int j = 0 ; j < hero.m_amis.size() ; ++j)
                if(hero.m_amis[j] == &p->m_personnage)
                    hero.m_amis.erase(hero.m_amis.begin() + j);
            m_personnageClients.erase(p);
            GlobalMutex.Unlock();

            return true;
        }
    return false;
}

