cmake_minimum_required(VERSION 3.16)
project(SDL_Projects C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Keep generated executables in the existing build folder at repository root.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

find_package(PkgConfig QUIET)

set(SDL3_TARGET "")
if(PkgConfig_FOUND)
    pkg_check_modules(SDL3 QUIET IMPORTED_TARGET sdl3)
    if(TARGET PkgConfig::SDL3)
        set(SDL3_TARGET PkgConfig::SDL3)
    endif()
endif()

if(NOT SDL3_TARGET)
    find_path(SDL3_INCLUDE_DIR SDL3/SDL.h)
    find_library(SDL3_LIBRARY NAMES SDL3 REQUIRED)

    add_library(SDL3::SDL3 UNKNOWN IMPORTED)
    set_target_properties(SDL3::SDL3 PROPERTIES IMPORTED_LOCATION "${SDL3_LIBRARY}")
    if(SDL3_INCLUDE_DIR)
        set_target_properties(SDL3::SDL3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INCLUDE_DIR}")
    endif()
    set(SDL3_TARGET SDL3::SDL3)
endif()

set(SDL3_IMAGE_TARGET "")
if(PkgConfig_FOUND)
    pkg_check_modules(SDL3_IMAGE QUIET IMPORTED_TARGET sdl3_image)
    if(TARGET PkgConfig::SDL3_IMAGE)
        set(SDL3_IMAGE_TARGET PkgConfig::SDL3_IMAGE)
    endif()
endif()

if(NOT SDL3_IMAGE_TARGET)
    find_path(SDL3_IMAGE_INCLUDE_DIR SDL3_image/SDL_image.h)
    find_library(SDL3_IMAGE_LIBRARY NAMES SDL3_image)

    if(SDL3_IMAGE_LIBRARY)
        add_library(SDL3_image::SDL3_image UNKNOWN IMPORTED)
        set_target_properties(SDL3_image::SDL3_image PROPERTIES IMPORTED_LOCATION "${SDL3_IMAGE_LIBRARY}")
        if(SDL3_IMAGE_INCLUDE_DIR)
            set_target_properties(SDL3_image::SDL3_image PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SDL3_IMAGE_INCLUDE_DIR}")
        endif()
        set(SDL3_IMAGE_TARGET SDL3_image::SDL3_image)
    endif()
endif()

add_executable(3141 3141/3141.c)
target_link_libraries(3141 PRIVATE ${SDL3_TARGET})

add_executable(fluid fluid/fluid.c)
target_link_libraries(fluid PRIVATE ${SDL3_TARGET})

add_executable(random_walk random_walk/random_walk.c)
target_link_libraries(random_walk PRIVATE ${SDL3_TARGET} m)

add_executable(doom doom/doom.c doom/kengine.c)
target_link_libraries(doom PRIVATE ${SDL3_TARGET} m)

add_executable(digital_clock digital_clock/main.c digital_clock/clock.c)
target_link_libraries(digital_clock PRIVATE ${SDL3_TARGET})

add_executable(client ping_ponk/client.c)
add_executable(server ping_ponk/server.c)

if(NOT SDL3_IMAGE_TARGET)
    message(FATAL_ERROR "SDL3_image was not found. It is required for targets: space_sim and pingo.")
endif()

add_executable(space_sim space_sim/space_sim.c space_sim/body.c)
target_link_libraries(space_sim PRIVATE ${SDL3_TARGET} ${SDL3_IMAGE_TARGET} m)

add_executable(pingo ping_ponk/pingo.c)
target_link_libraries(pingo PRIVATE ${SDL3_TARGET} ${SDL3_IMAGE_TARGET} m)

add_custom_command(TARGET pingo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/ping_ponk/assets"
            "$<TARGET_FILE_DIR:pingo>/assets"
)
