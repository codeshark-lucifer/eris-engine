cmake_minimum_required(VERSION 3.20)
project(ErisEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}   -static -static-libgcc")
endif()

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)  # optional: ensure static build
set(SDL_STATIC ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_STATIC_LIB ON CACHE BOOL "" FORCE)
set(ASSIMP_USE_STATIC_CRT ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(ASSIMP_USE_SYSTEM_ZLIB OFF CACHE BOOL "" FORCE)

add_subdirectory(external/stb)
add_subdirectory(external/glad)
add_subdirectory(external/SDL)
add_subdirectory(external/assimp)

file(GLOB_RECURSE FILE_SRC
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(engine ${FILE_SRC})

target_link_libraries(engine PRIVATE
    assimp::assimp
    SDL3::SDL3-static
    glad
    stb
)

target_include_directories(engine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/external/assimp/include
)

if(WIN32)
    target_link_libraries(engine PRIVATE
        winmm
        imm32
        setupapi
        version
        ole32
        oleaut32
        uuid
        advapi32
        shell32
        user32
        gdi32
    )
endif()
