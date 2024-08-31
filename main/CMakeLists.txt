idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS ".")



target_link_options(${COMPONENT_LIB} PUBLIC -Wl,--allow-multiple-definition)
slint_target_sources(${COMPONENT_LIB} ../ui/mainwindow.slint)