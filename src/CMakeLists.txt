add_executable(conchu)

target_sources(conchu
    PRIVATE
      abstract_registry.cpp
      cli.cpp
      config.cpp
      container.cpp
      container_engine.cpp
      container_image.cpp
      docker_hub_registry.cpp
      main.cpp
      mode_oneshot.cpp
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
