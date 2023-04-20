idf_component_register( ... )
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++2b)

idf_component_register(SRCS "hello_world_main.cpp"
                    INCLUDE_DIRS "")
