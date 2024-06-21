cmake_minimum_required(VERSION 3.5)

project(watermelon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(watermelon main.cpp
    include/tokenization.h
    src/tokenization.cpp
    include/language.h

    include/parser.h
    src/parser.cpp
    src/language.cpp
    include/patternmatching.h
    src/patternmatching.cpp
)

include(GNUInstallDirs)
install(TARGETS watermelon
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
