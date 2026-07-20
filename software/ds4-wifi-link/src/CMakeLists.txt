add_subdirectory(implementation)
target_include_directories(implementation PUBLIC include)

find_package(SDL3 CONFIG REQUIRED)

add_executable(ds4-wl main.cpp)

target_link_libraries(ds4-wl PRIVATE SDL3::SDL3)
target_link_libraries(ds4-wl PRIVATE implementation)

