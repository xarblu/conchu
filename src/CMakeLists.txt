add_executable(conchu)

target_sources(conchu
    PRIVATE
      main.cpp
      cli.cpp
      abstract_registry.cpp
      docker_hub_registry.cpp
      container_image.cpp
      container_engine.cpp
      container.cpp
      tag.cpp
      tag_filter.cpp
)

target_include_directories(conchu
    PRIVATE
      ${httplib_INCLUDE_DIRS}
      ${nlohmann_json_INCLUDE_DIRS}
)

target_link_libraries(conchu
    PRIVATE
      httplib::httplib
      nlohmann_json::nlohmann_json 
)
