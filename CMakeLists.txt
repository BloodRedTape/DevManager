cmake_minimum_required(VERSION 3.10)

project(Sample)

set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(StraitXPackages/Base)
add_subdirectory(StraitXPackages/ImGui)
add_subdirectory(thirdparty/json)

set(SAMPLE_SOURCES
    "./sources/main.cpp"
    "./sources/application.cpp"
    "./sources/views/project_types_view.cpp"
    "sources/views/search_paths_view.cpp"
     "sources/serialization.cpp" )

set(SAMPLE_INCLUDE
    "./sources"
)

add_executable(Sample ${SAMPLE_SOURCES})
target_link_libraries(Sample StraitXBase StraitXImGui nlohmann_json)
target_include_directories(Sample
    PUBLIC ${SAMPLE_INCLUDE}
)