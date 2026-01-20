add_executable(conchu)

target_sources(conchu
    PRIVATE
      main.cpp
      cli.cpp
      abstract_registry.cpp
      docker_hub_registry.cpp
      container_image.cpp
)
