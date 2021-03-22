include(FindPackageHandleStandardArgs)

# Search for SDL2_ttf in ext/SDL2_ttf-2.0.15
find_path(SDL2_ttf_ROOT "include/SDL_ttf.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../ext/SDL2_ttf-2.0.15" NO_DEFAULT_PATH)
if(SDL2_ttf_ROOT)
    set(SDL2_ttf_INCLUDE_DIR "${SDL2_ttf_ROOT}/include")
    set(SDL2_ttf_LIBRARY "${SDL2_ttf_ROOT}/lib/x64/SDL2_ttf.lib")
    set(SDL2_ttf_DYNAMIC_LINK_LIBRARY "${SDL2_ttf_ROOT}/lib/x64/SDL2_ttf.dll")
endif()

mark_as_advanced(SDL2_ttf_ROOT)
find_package_handle_standard_args(SDL2_ttf DEFAULT_MSG SDL2_ttf_INCLUDE_DIR SDL2_ttf_LIBRARY SDL2_ttf_DYNAMIC_LINK_LIBRARY)