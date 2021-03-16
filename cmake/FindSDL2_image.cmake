include(FindPackageHandleStandardArgs)

# Search for SDL2_image in ext/SDL2-2.0.14
find_path(SDL2_image_ROOT "include/SDL_image.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../ext/SDL2_image-2.0.5" NO_DEFAULT_PATH)
if(SDL2_image_ROOT)
    set(SDL2_image_INCLUDE_DIR "${SDL2_image_ROOT}/include")
    set(SDL2_image_LIBRARY "${SDL2_image_ROOT}/lib/x64/SDL2_image.lib")
    set(SDL2_image_DYNAMIC_LINK_LIBRARY "${SDL2_image_ROOT}/lib/x64/SDL2_image.dll")
endif()

mark_as_advanced(SDL2_image_ROOT)
find_package_handle_standard_args(SDL2_image DEFAULT_MSG SDL2_image_INCLUDE_DIR SDL2_image_LIBRARY SDL2_image_DYNAMIC_LINK_LIBRARY)