cmake_minimum_required(VERSION 3.5)

project(pie LANGUAGES C)

# Recherche la bibliothèque GD
find_package(gd)

add_executable(pie main.c)

# Lie la bibliothèque GD
target_link_libraries(pie gd m)

# Installe l'exécutable
install(TARGETS pie DESTINATION ${CMAKE_INSTALL_BINDIR})

