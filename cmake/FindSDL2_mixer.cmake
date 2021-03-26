include(FindPackageHandleStandardArgs)

# Search for SDL2_mixer in ext/SDL2_mixer-2.0.4
find_path(SDL2_mixer_ROOT "include/SDL_mixer.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../ext/SDL2_mixer-2.0.4" NO_DEFAULT_PATH)
if(SDL2_mixer_ROOT)
    set(SDL2_mixer_INCLUDE_DIR "${SDL2_mixer_ROOT}/include")
    set(SDL2_mixer_LIBRARY "${SDL2_mixer_ROOT}/lib/x64/SDL2_mixer.lib")
    set(SDL2_mixer_DYNAMIC_LINK_LIBRARY "${SDL2_mixer_ROOT}/lib/x64/SDL2_mixer.dll")
endif()

mark_as_advanced(SDL2_mixer_ROOT)
find_package_handle_standard_args(SDL2_mixer DEFAULT_MSG SDL2_mixer_INCLUDE_DIR SDL2_mixer_LIBRARY SDL2_mixer_DYNAMIC_LINK_LIBRARY)