include(FindPackageHandleStandardArgs)

# Check if "main" was specified as a component
set(SDL2_use_main FALSE)
foreach(SDL2_component ${SDL2_FIND_COMPONENTS})
    if(SDL2_component STREQUAL "main")
        set(SDL2_use_main TRUE)
    else()
        message(WARNING "Unrecognized component \"${_SDL2_component}\"")
    endif()
endforeach()

# Search for SDL2 in ext/SDL2-2.0.14
find_path(SDL2_ROOT "include/SDL.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../ext/SDL2-2.0.14" NO_DEFAULT_PATH)
if(SDL2_ROOT)
    set(SDL2_INCLUDE_DIR "${SDL2_ROOT}/include")
    set(SDL2_LIBRARY "${SDL2_ROOT}/lib/x64/SDL2.lib")
    set(SDL2_DYNAMIC_LINK_LIBRARY "${SDL2_ROOT}/lib/x64/SDL2.dll")
    if(SDL2_use_main)
        list(APPEND SDL2_LIBRARY "${SDL2_ROOT}/lib/x64/SDL2main.lib")
    endif()
endif()

mark_as_advanced(SDL2_ROOT)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_INCLUDE_DIR SDL2_LIBRARY SDL2_DYNAMIC_LINK_LIBRARY)