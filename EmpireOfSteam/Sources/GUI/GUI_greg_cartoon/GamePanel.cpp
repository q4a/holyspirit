#include "GamePanel.hpp"

GamePanel::GamePanel()
{
    m_tab_resources = new TabBar(0,0,512,384);

    m_tab_food     = new Tab(12 ,48,118,35, "");
    m_tab_material = new Tab(12 ,83,118,35, "");
    m_tab_science  = new Tab(12 ,118,118,35, "");
    m_tab_army     = new Tab(12 ,153,118,35, "");
    m_tab_religion = new Tab(12 ,188,118,35, "");

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

    m_panel_ressources          = new GUIImage(                 "pictures/GUI/reliefRessources.png");
    m_panel_map                 = new GUIImage(512  , 0     ,   "pictures/GUI/reliefMap.png");
    m_panel_bejeweled           = new GUIImage(0    , 384   ,   "pictures/GUI/reliefBejeweld.png");
    m_panel_city                = new GUIImage(512  , 384   ,   "pictures/GUI/reliefCity.png");

    m_cadran_bejeweled          = new GUIImage(0,-384,"pictures/GUI/laitonBejeweld.png");
    m_cadran_map                = new GUIImage("pictures/GUI/laitonMap.png");
    m_cadran_resources          = new GUIImage("pictures/GUI/ressourcesBackground.png");
    m_map                       = new GUIImage(18,16,480,360,"pictures/GUI/map.png");

    m_panel_army_deco           = new GUIImage(150,32,335,251,"pictures/GUI/armyPanel.png");

    m_bejeweled                 = new Bejeweled(114,52,256,256);

    AddWidget(m_panel_ressources);
    AddWidget(m_panel_map);
    AddWidget(m_panel_bejeweled);
    AddWidget(m_panel_city);


    m_panel_ressources->AddWidget(m_cadran_resources);

    m_animated_tab_army_off = new GUIAnimatedImage (0,0,118,35,7,"pictures/GUI/ongletArmeeOff/OngletAnimation-.png");
    m_animated_tab_army_off->SetRate(0.5);

    m_animated_tab_army_on  = new GUIAnimatedImage (0,0,118,35,7,"pictures/GUI/ongletArmeeOn/OngletAnimation-.png");
    m_animated_tab_army_on->SetRate(0.5);

    m_animated_tab_religion_off = new GUIAnimatedImage (0,0,118,35,8,"pictures/GUI/ongletReligionOff/OngletAnimation-.png");
    m_animated_tab_religion_off->SetRate(0.5);

    m_animated_tab_religion_on  = new GUIAnimatedImage (0,0,118,35,8,"pictures/GUI/ongletReligionOn/OngletAnimation-.png");
    m_animated_tab_religion_on->SetRate(0.5);

    m_animated_tab_science_off = new GUIAnimatedImage (0,0,118,35,8,"pictures/GUI/ongletScienceOff/OngletAnimation-.png");
    m_animated_tab_science_off->SetRate(0.5);

    m_animated_tab_science_on  = new GUIAnimatedImage (0,0,118,35,8,"pictures/GUI/ongletScienceOn/OngletAnimation-.png");
    m_animated_tab_science_on->SetRate(0.5);

    m_animated_tab_food_off = new GUIAnimatedImage (0,0,118,35,9,"pictures/GUI/ongletNourritureOff/OngletAnimation-.png");
    m_animated_tab_food_off->SetRate(0.5);

    m_animated_tab_food_on  = new GUIAnimatedImage (0,0,118,35,9,"pictures/GUI/ongletNourritureOn/OngletAnimation-.png");
    m_animated_tab_food_on->SetRate(0.5);

    m_animated_tab_material_off = new GUIAnimatedImage (0,0,118,35,7,"pictures/GUI/ongletMaterielOff/OngletAnimation-.png");
    m_animated_tab_material_off->SetRate(0.5);

    m_animated_tab_material_on  = new GUIAnimatedImage (0,0,118,35,7,"pictures/GUI/ongletMaterielOn/OngletAnimation-.png");
    m_animated_tab_material_on->SetRate(0.5);

    m_tab_army->SetImage(m_animated_tab_army_off);
    m_tab_army->SetImage(m_animated_tab_army_off,Button_hover);
    m_tab_army->SetImage(m_animated_tab_army_on ,Button_clicked);

    m_tab_religion->SetImage(m_animated_tab_religion_off);
    m_tab_religion->SetImage(m_animated_tab_religion_off,Button_hover);
    m_tab_religion->SetImage(m_animated_tab_religion_on ,Button_clicked);

    m_tab_science->SetImage(m_animated_tab_science_off);
    m_tab_science->SetImage(m_animated_tab_science_off,Button_hover);
    m_tab_science->SetImage(m_animated_tab_science_on ,Button_clicked);

    m_tab_food->SetImage(m_animated_tab_food_off);
    m_tab_food->SetImage(m_animated_tab_food_off,Button_hover);
    m_tab_food->SetImage(m_animated_tab_food_on ,Button_clicked);

    m_tab_material->SetImage(m_animated_tab_material_off);
    m_tab_material->SetImage(m_animated_tab_material_off,Button_hover);
    m_tab_material->SetImage(m_animated_tab_material_on ,Button_clicked);

    m_tab_resources->AddTab(m_tab_food      , m_panel_food);
    m_tab_resources->AddTab(m_tab_material  , m_panel_material);
    m_tab_resources->AddTab(m_tab_science   , m_panel_science);
    m_tab_resources->AddTab(m_tab_army      , m_panel_army);
    m_tab_resources->AddTab(m_tab_religion  , m_panel_religion);

    m_panel_army    ->AddWidget(m_panel_army_deco);

    m_fish_ressources   = new GUIProgressBar(230,64,221,36,"Poisson");
    m_fish_ressources   ->m_label.SetColor(sf::Color(255, 255, 255));
    m_fish_ressources   ->SetImage("pictures/GUI/jaugeContour.png",Img_Background);
    m_fish_ressources   ->SetImage("pictures/GUI/jaugeFond.png",Img_Bar);
    m_fish_ressources   ->SetValue(0);

    m_weath_ressources   = new GUIProgressBar(230,104,221,36,"Blé");
    m_weath_ressources   ->m_label.SetColor(sf::Color(255, 255, 255));
    m_weath_ressources   ->SetImage("pictures/GUI/jaugeContour.png",Img_Background);
    m_weath_ressources   ->SetImage("pictures/GUI/jaugeFond.png",Img_Bar);
    m_weath_ressources   ->SetValue(32);

    m_meat_ressources   = new GUIProgressBar(230,144,221,36,"Viande");
    m_meat_ressources   ->m_label.SetColor(sf::Color(255, 255, 255));
    m_meat_ressources   ->SetImage("pictures/GUI/jaugeContour.png",Img_Background);
    m_meat_ressources   ->SetImage("pictures/GUI/jaugeFond.png",Img_Bar);
    m_meat_ressources   ->SetValue(0);

    m_panel_food    ->AddWidget(m_fish_ressources);
    m_panel_food    ->AddWidget(m_weath_ressources);
    m_panel_food    ->AddWidget(m_meat_ressources);


    m_rock_ressources   = new GUIProgressBar(230,64,221,36,"Pierre");
    m_rock_ressources   ->m_label.SetColor(sf::Color(255, 255, 255));
    m_rock_ressources   ->SetImage("pictures/GUI/jaugeContour.png",Img_Background);
    m_rock_ressources   ->SetImage("pictures/GUI/jaugeFond.png",Img_Bar);
    m_rock_ressources   ->SetValue(50);

    m_wood_ressources   = new GUIProgressBar(230,104,221,36,"Bois");
    m_wood_ressources   ->m_label.SetColor(sf::Color(255, 255, 255));
    m_wood_ressources   ->SetImage("pictures/GUI/jaugeContour.png",Img_Background);
    m_wood_ressources   ->SetImage("pictures/GUI/jaugeFond.png",Img_Bar);
    m_wood_ressources   ->SetValue(0);

    m_panel_material    ->AddWidget(m_rock_ressources);
    m_panel_material    ->AddWidget(m_wood_ressources);

    m_panel_ressources->AddWidget(m_tab_resources);

    m_panel_bejeweled->AddWidget(m_bejeweled);
    m_panel_bejeweled->AddWidget(m_cadran_bejeweled);

    m_panel_map->AddWidget(m_map);
    m_panel_map->AddWidget(m_cadran_map);
}

GamePanel::~GamePanel()
{
    delete m_fish_ressources;
    delete m_rock_ressources;

    delete m_animated_tab_army_off;
    delete m_animated_tab_army_on;
    delete m_animated_tab_religion_off;
    delete m_animated_tab_religion_on;
    delete m_animated_tab_science_off;
    delete m_animated_tab_science_on;
    delete m_animated_tab_food_off;
    delete m_animated_tab_food_on;
    delete m_animated_tab_material_off;
    delete m_animated_tab_material_on;

    delete m_tab_food;
    delete m_tab_material;
    delete m_tab_science;
    delete m_tab_army;
    delete m_tab_religion;

    delete m_tab_resources;

    delete m_panel_army_deco;

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

void GamePanel::Update()
{
    Widget::Update();

    switch (m_bejeweled->GetLastJeweld())
    {
        case 3:
            m_rock_ressources->SetValue(m_rock_ressources->GetValue() + 1);
            break;
        case 4:
            m_fish_ressources->SetValue(m_fish_ressources->GetValue() + 1);
            break;
        case 5:
            m_weath_ressources->SetValue(m_weath_ressources->GetValue() + 1);
            break;
        case 6:
            m_meat_ressources->SetValue(m_meat_ressources->GetValue() + 1);
            break;
        case 7:
            m_wood_ressources->SetValue(m_wood_ressources->GetValue() + 1);
            break;
        default:
            break;
    }
}

