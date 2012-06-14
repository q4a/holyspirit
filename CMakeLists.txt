Project (Holyspirit)
cmake_minimum_required (VERSION 2.6)

add_definitions(-DUNICODE -D_UNICODE)

set(BINDIR  "games"                  CACHE STRING "Directory from CMAKE_INSTALL_PREFIX where game executable will be installed.")
set(DATADIR "share/games/holyspirit" CACHE STRING "Directory from CMAKE_INSTALL_PREFIX where game data files will be installed.")

add_definitions(-DDATA_INSTALL_DIR="${CMAKE_INSTALL_PREFIX}/${DATADIR}")

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
	Source/Contextes/c_bless.cpp
	Source/Contextes/c_changementMap.cpp
	Source/Contextes/c_craft.cpp
	Source/Contextes/c_demarrage.cpp
	Source/Contextes/c_documents.cpp
	Source/Contextes/c_inventaire.cpp
	Source/Contextes/c_jeu.cpp
	Source/Contextes/c_mainMenu.cpp
	Source/Contextes/c_menuInGame.cpp
	Source/Contextes/c_miracles.cpp
	Source/Contextes/c_potales.cpp
	Source/Contextes/c_quetes.cpp
	Source/Contextes/contexte.cpp
	
	Source/Entites/case.cpp
	Source/Entites/classe.cpp
	Source/Entites/hero.cpp
	Source/Entites/liste_case.cpp
	Source/Entites/miracle.cpp
	Source/Entites/monstre.cpp
	Source/Entites/objet.cpp
	Source/Entites/personnage.cpp
	Source/Entites/quete.cpp
	Source/Entites/script.cpp
	
	Source/Map/Climate.cpp
	Source/Map/decor.cpp
	Source/Map/map.cpp
	Source/Map/map_charger.cpp
	Source/Map/map_miracles.cpp
	Source/Map/map_scripts.cpp
	Source/Map/tile.cpp
	Source/Map/tileset.cpp
	
	Source/Moteurs/console.cpp
	Source/Moteurs/Entite_graphique.cpp
	Source/Moteurs/eventManager.cpp
	Source/Moteurs/label.cpp
	Source/Moteurs/light.cpp
	Source/Moteurs/lightManager.cpp
	Source/Moteurs/modeleMoteurParticule.cpp
	Source/Moteurs/moteurGraphique.cpp
	Source/Moteurs/moteurParticule.cpp
	Source/Moteurs/moteurSons.cpp
	Source/Moteurs/typo.cpp
	
	Source/Network/network.cpp
	Source/Network/receive_client.cpp
	Source/Network/receive_host.cpp
	Source/Network/send.cpp
	
	Source/sfml.cpp
	Source/configuration.cpp
	Source/datFile.cpp
	Source/jeu.cpp
	Source/main.cpp
	Source/menu.cpp
	Source/video.cpp)

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
find_package(SFML 2.0 REQUIRED system window graphics network audio)
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	list(APPEND LINKS
${GNUWIN_LIBRARY} ${SFML_AUDIO_LIBRARY} ${SFML_NETWORK_LIBRARY} ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY})
endif()

#If game doesn't build try to put next IF statement before SFML detection
if (NOT WIN32 AND SFML_STATIC_LIBRARIES)

#This can be used if your distro can't find needed libraries
#find_package(OpenGL REQUIRED)		${OPENGL_gl_LIBRARY}
#find_package(Freetype REQUIRED)	${FREETYPE_LIBRARY}
#find_package(GLEW REQUIRED)		${GLEW_LIBRARY}
#find_package(JPEG REQUIRED)		${JPEG_LIBRARY}
#find_package(X11 REQUIRED)			${X11_LIBRARIES}
#find_package(OpenAL REQUIRED)		${OPENAL_LIBRARY}
#find_package(Sndfile REQUIRED)		${SNDFILE_LIBRARY}

list(APPEND LINKS
#if needed uncomment those
#pthread
#X11
#GL
	Xrandr
	freetype
	GLEW
	jpeg
	sndfile
	openal
	)
endif()


target_link_libraries(Holyspirit ${LINKS})

set(HOLYSPIRIT_EXECUTABLE_PATH ${CMAKE_INSTALL_PREFIX}/${BINDIR}/Holyspirit)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/distribution/Holyspirit.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/Holyspirit.desktop")


# installing to the proper places
install(PROGRAMS
  ${CMAKE_CURRENT_BINARY_DIR}/Holyspirit
  ${CMAKE_CURRENT_BINARY_DIR}/LauncherHolyspirit
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${BINDIR})
install(DIRECTORY
  "${CMAKE_CURRENT_SOURCE_DIR}/Data"
  "${CMAKE_CURRENT_SOURCE_DIR}/Launcher/Data"
  "${CMAKE_CURRENT_SOURCE_DIR}/Launcher/Languages"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${DATADIR})
install(FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/Liste_des_references_des_touches.txt"
  "${CMAKE_CURRENT_SOURCE_DIR}/configuration.conf"
  "${CMAKE_CURRENT_SOURCE_DIR}/credits.txt"
  "${CMAKE_CURRENT_SOURCE_DIR}/holyspirit.ini"
  "${CMAKE_CURRENT_SOURCE_DIR}/key_mapping.conf"
  "${CMAKE_CURRENT_SOURCE_DIR}/liste_resolution.conf"
  "${CMAKE_CURRENT_SOURCE_DIR}/Launcher/liste_langages.conf"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${DATADIR})
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/Holyspirit.desktop"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
install(FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/icone.ico"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps
  RENAME Holyspirit.ico)
