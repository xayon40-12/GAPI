cmake_minimum_required(VERSION 3.7)
project(GAPI)

#CMAKE_SOURCE_DIR correspond a l'emplacement du fichier CMakeLists.txt (donc plus ou moins le dossier du projet), donc si on l'utilise avec la forme ${CMAKE_SOURCE_DIR} ca donne le chemin absolu vers le dossier
#CMAKE_MODULE_PATH correspond a où sont les .cmake
set(RESSOURCES_PATH /Users/nathan/Programmation/ressources)
set(CMAKE_MODULE_PATH ${RESSOURCES_PATH}/CMake)
#dans le fichier FindSDL2.cmake vers le debut le "SET(SDL2_SEARCH_PATHS ... ${SDL2_PATH})" cherche un peut partout dans macOS et linux et permet a windows de definir où chercher en donnant une valeur à SDL2_PATH, ce que je fait a la ligne suivante
#set(SDL2_PATH ${CMAKE_SOURCE_DIR}/SDL2)#j'ai aussi mit la SDL dans le dossier principale par simplicité mais tu peut juste lui dire où elle est dans ton ordi si tu prefère
#set(SDL2_IMAGE_PATH ${CMAKE_SOURCE_DIR}/SDL2_image/x86_64-w64-mingw32)#ici pareil pour SDLimage (attention il faut donner le nom du dossier qui contient les dossier headers et lib)

find_package(GAPI)
#find_package(SDL2)#ici je lui dit de charger le fichier FindSDL2.cmake
#find_package(SDL2_image)#ici je lui dit de charger le fichier FindSDL2_image.cmake  (en gros c'est find_package(nomLib) et ca cherche "Find+nomLib+.cmake")
#find_package(OPENGL)
#find_package(OPENCL)
#les .cmake vont créer plein de racoursis crossplateform pour nous que l'on utilise ensuite
include_directories(${GAPI_INCLUDE_DIR})#ici je lui dit ou sont les headers avec les valeurs fourni par les .cmake charger juste avant (tu trouve les nom dans les .cmake au niveau du "FIND_PATH(SDL2_INCLUDE_DIR SDL.h ...)" c'est le nom juste après FIND_PATH)

set(CMAKE_CXX_STANDARD 11)#la je ne suis pas sur mais je pense que ca active C++11

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
endif()

set(SOURCE_FILES OpenGL/Camera.cpp SDL/Colour.cpp SDL/Event.cpp OpenGL/Frame.cpp OpenGL/FrameBuffer.cpp OpenGL/Frustum.cpp OpenGL/GLWindow.cpp SDL/Keyboard.cpp SDL/Mouse.cpp OpenGL/Object.cpp OpenGL/Shader.cpp Tools/Sound.cpp OpenGL/Texture.cpp Tools/Thread.cpp Tools/Vec.cpp SDL/Window.cpp Tools/Tools.hpp SDL/SDL.hpp OpenGL/OpenGL.hpp OpenCL/OpenCL.cpp OpenCL/OpenCL.hpp GAPI.hpp OpenCL/OpenCL_GL.cpp OpenCL/OpenCL_GL.hpp)#ici on met tout les cpp qu'il faut compiler (ca ajoute normalement tout seul si tu creer les fichier directe dans CLion mais si tu en rajoute a la main ca va etre la qu'il faut les mettre je pense, les un après les autres séparés par un espace)
add_library(GAPI STATIC ${SOURCE_FILES})

target_link_libraries(GAPI ${GAPI_LIBRARY})#ici je lui dis à quel
