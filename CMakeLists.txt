# CMakeLists.txt
#
# Projet : Sensor driver + HAL (host) + capteur simulé
# Objectif :
# - Compiler une bibliothèque "driver" portable
# - Compiler une bibliothèque "HAL host" (fake bus, time, log)
# - Compiler un exécutable de démo (demo)
#
# Build (macOS) :
#   mkdir -p build
#   cmake -S . -B build
#   cmake --build build
#   ./build/demo

cmake_minimum_required(VERSION 3.16)

# Nom du projet + langage
project(sensor_driver_template C)

# Standard C (tu peux passer à 17 si tu veux)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Options de warnings (pratique pour un projet propre)
# Note : pas de -Werror pour éviter de bloquer les débutants.
add_compile_options(-Wall -Wextra -Wpedantic)

# ---------------------------------------------------------------------------
# Bibliothèque "sensor_driver"
# Contient le driver capteur (portable)
# ---------------------------------------------------------------------------
add_library(sensor_driver STATIC
    src/sensor/sensor.c
)

# Inclure les headers publics (include/)
target_include_directories(sensor_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---------------------------------------------------------------------------
# Bibliothèque "hal_host"
# Contient les implémentations host (macOS/PC) :
# - bus simulé
# - time fake
# - log stdio
# ---------------------------------------------------------------------------
add_library(hal_host STATIC
    src/hal/hal_bus_fake.c
    src/hal/hal_time_fake.c
    src/hal/hal_log_stdio.c
)

# Même dossier d'headers
target_include_directories(hal_host PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---------------------------------------------------------------------------
# Exécutable de démonstration
# ---------------------------------------------------------------------------
add_executable(demo
    examples/demo_main.c
)

# Linker les libs au programme demo
target_link_libraries(demo PRIVATE
    sensor_driver
    hal_host
)

# ---------------------------------------------------------------------------
# (Optionnel) Tests
# Tu pourras activer ça quand test_sensor.c sera prêt.
# ---------------------------------------------------------------------------
option(ENABLE_TESTS "Build unit tests" ON)

if(ENABLE_TESTS)
    enable_testing()

    # Exemple : un exécutable "tests" qui contient tes tests
    # (Tu peux renommer en sensor_tests si tu veux)
    add_executable(sensor_tests
        tests/test_sensor.c
    )

    target_link_libraries(sensor_tests PRIVATE
        sensor_driver
        hal_host
    )

    add_test(NAME sensor_tests COMMAND sensor_tests)
endif()
