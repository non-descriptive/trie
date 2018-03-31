cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

set(PROJ_MAIN Trie)

project(${PROJ_MAIN}
    LANGUAGES CXX)

include_directories(
    include
    )

set(SOURCE
    src/Trie.cpp
    main.cpp
    )


add_executable(${PROJ_MAIN} ${SOURCE})


add_subdirectory(tests)
