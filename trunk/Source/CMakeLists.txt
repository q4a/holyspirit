Project (Holyspirit)
cmake_minimum_required (VERSION 2.6)

# define a macro that helps defining an option
macro(set_option var default type docstring)
	if(NOT DEFINED ${var})
		set(${var} ${default})
	endif()
	set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

include_directories(
	${SFML_INCLUDE}
	${GNUWIN_INCLUDE_DIR})

# Sources

list(APPEND GAME_SOURCES
	Contextes/c_bless.cpp
	Contextes/c_changementMap.cpp
	Contextes/c_craft.cpp
	Contextes/c_demarrage.cpp
	Contextes/c_documents.cpp
	Contextes/c_inventaire.cpp
	Contextes/c_jeu.cpp
	Contextes/c_mainMenu.cpp
	Contextes/c_menuInGame.cpp
	Contextes/c_miracles.cpp
	Contextes/c_potales.cpp
	Contextes/c_quetes.cpp
	Contextes/contexte.cpp
	
	Entites/case.cpp
	Entites/classe.cpp
	Entites/hero.cpp
	Entites/liste_case.cpp
	Entites/miracle.cpp
	Entites/monstre.cpp
	Entites/objet.cpp
	Entites/personnage.cpp
	Entites/quete.cpp
	Entites/script.cpp
	
	Map/Climate.cpp
	Map/decor.cpp
	Map/map.cpp
	Map/map_charger.cpp
	Map/map_miracles.cpp
	Map/map_scripts.cpp
	Map/tile.cpp
	Map/tileset.cpp
	
	Moteurs/console.cpp
	Moteurs/Entite_graphique.cpp
	Moteurs/eventManager.cpp
	Moteurs/label.cpp
	Moteurs/light.cpp
	Moteurs/lightManager.cpp
	Moteurs/modeleMoteurParticule.cpp
	Moteurs/moteurGraphique.cpp
	Moteurs/moteurParticule.cpp
	Moteurs/moteurSons.cpp
	Moteurs/typo.cpp
	
	Network/network.cpp
	Network/receive_client.cpp
	Network/receive_host.cpp
	Network/send.cpp
	
	sfml.cpp
	configuration.cpp
	datFile.cpp
	jeu.cpp
	main.cpp
	menu.cpp
	video.cpp)

# Add icon to executable for Windows systems
if (WIN32)
list(APPEND GAME_SOURCES 
	ressource.rc)
find_package(GNUWIN REQUIRED)
endif()

add_Executable (Holyspirit ${GAME_SOURCES})

# add an option for choosing the build type of SFML(shared or static)
set_option(SFML_STATIC_LIBRARIES TRUE BOOL "TRUE to build SFML as static libraries, FALSE to build it as shared libraries")

# Detect and add SFML
find_package(SFML 1.6 REQUIRED system window graphics network audio)
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	target_link_libraries(Holyspirit ${GNUWIN_LIBRARY} ${SFML_AUDIO_LIBRARY} ${SFML_NETWORK_LIBRARY} ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY})
endif()
