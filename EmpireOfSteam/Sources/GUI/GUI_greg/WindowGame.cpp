#include "WindowGame.hpp"
#include <iostream>

using namespace std;
using namespace sf;

WindowGame::WindowGame() : GUIWindow(1024,768,"Empire of Steam")
{
    m_tab_resources = new TabBar(0,0,512,384);

    m_tab_food     = new Tab(20 ,16,92,48, "Nourriture");
    m_tab_material = new Tab(114,16,92,48, "Matériaux");
    m_tab_science  = new Tab(208,16,92,48, "Science");
    m_tab_army     = new Tab(302,16,92,48, "Armées");
    m_tab_religion = new Tab(396,16,92,48, "Religion");

    m_tab_food      ->m_label.SetCharacterSize(14);
    m_tab_food      ->m_label.SetColor(sf::Color(224, 224, 224));
    m_tab_material  ->m_label.SetCharacterSize(14);
    m_tab_material  ->m_label.SetColor(sf::Color(224, 224, 224));
    m_tab_science   ->m_label.SetCharacterSize(14);
    m_tab_science   ->m_label.SetColor(sf::Color(224, 224, 224));
    m_tab_army      ->m_label.SetCharacterSize(14);
    m_tab_army      ->m_label.SetColor(sf::Color(224, 224, 224));
    m_tab_religion  ->m_label.SetCharacterSize(14);
    m_tab_religion  ->m_label.SetColor(sf::Color(224, 224, 224));

    m_panel_food        = new Widget(0,0,512,384);
    m_panel_material    = new Widget(0,0,512,384);
    m_panel_science     = new Widget(0,0,512,384);
    m_panel_army        = new Widget(0,0,512,384);
    m_panel_religion    = new Widget(0,0,512,384);

    m_label         = new Label(100,25,"Test");
    m_label->SetColor(sf::Color(255, 0, 0));
    m_label->SetCharacterSize(18);

    m_panel_ressources          = new GUIImage(                 "pictures/GUI/relief.png");
    m_panel_map                 = new GUIImage(512  , 0     ,   "pictures/GUI/relief.png");
    m_panel_bejeweled           = new GUIImage(0    , 384   ,   "pictures/GUI/reliefBejeweld.png");
    m_panel_city                = new GUIImage(512  , 384   ,   "pictures/GUI/relief.png");

    m_cadran_bejeweled          = new GUIImage("pictures/GUI/laitonBejeweld.png");
    m_cadran_map                = new GUIImage("pictures/GUI/laitonMap.png");
    m_cadran_resources          = new GUIImage("pictures/GUI/vitrageRessources.png");
    m_map                       = new GUIImage(16,12,480,360,"pictures/GUI/map.png");

    m_panel_army_deco           = new GUIImage(0,0,512,384,"pictures/GUI/armyPanel.png");
    m_compteur                  = new GUIImage(0,0,512,384,"pictures/GUI/compteur.png");

    m_bejeweled                 = new Bejeweled(94,32,320,320);

    AddWidget(m_panel_ressources);
    AddWidget(m_panel_map);
    AddWidget(m_panel_bejeweled);
    AddWidget(m_panel_city);

    m_panel_ressources->AddWidget(m_tab_resources);

    m_tab_resources->AddTab(m_tab_food      , m_panel_food);
    m_tab_resources->AddTab(m_tab_material  , m_panel_material);
    m_tab_resources->AddTab(m_tab_science   , m_panel_science);
    m_tab_resources->AddTab(m_tab_army      , m_panel_army);
    m_tab_resources->AddTab(m_tab_religion  , m_panel_religion);

    m_panel_army->AddWidget(m_panel_army_deco);
    m_panel_army->AddWidget(m_compteur);

    m_panel_ressources->AddWidget(m_cadran_resources);

    m_panel_bejeweled->AddWidget(m_bejeweled);
    m_panel_bejeweled->AddWidget(m_cadran_bejeweled);

    m_panel_map->AddWidget(m_map);
    m_panel_map->AddWidget(m_cadran_map);
}

WindowGame::~WindowGame()
{
    delete m_tab_food;
    delete m_tab_material;
    delete m_tab_science;
    delete m_tab_army;
    delete m_tab_religion;

    delete m_tab_resources;

    delete m_panel_army_deco;
    delete m_compteur;

    delete m_panel_food;
    delete m_panel_material;
    delete m_panel_science;
    delete m_panel_army;
    delete m_panel_religion;

    delete m_panel_ressources;
    delete m_panel_map;
    delete m_panel_bejeweled;
    delete m_panel_city;

    delete m_cadran_bejeweled;
    delete m_cadran_map;
    delete m_cadran_resources;
    delete m_map;

    delete m_label;

    delete m_bejeweled;
}

void WindowGame::Run()
{
    while(m_window.IsOpened())
    {
        mainEventManager->Update(m_window);
        Update();

        m_window.ShowMouseCursor(mainEventManager->GetShowCursor());

        if(mainEventManager->GetEvent(EventKey, sf::Key::Escape))
            m_window.Close();

        Show();
    }
}


