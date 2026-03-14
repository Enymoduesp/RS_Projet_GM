cmake_minimum_required(VERSION 4.0)
project(PROJET C)

set(CMAKE_C_STANDARD 11)

add_executable(PROJET
        include/friendship.h
        include/elmlist.h
        include/list.h
        include/io.h
        src/elmlist.c
        src/friendship.c
        src/person.c
        src/main.c
        src/list.c
        src/io.c
        include/person.h
        include/graph.h
        src/graph.c
        include/set.h
        src/set.c
)
