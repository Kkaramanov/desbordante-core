set(NAME pp.phm)
desbordante_add_lib(NAME)

target_sources(${NAME} PRIVATE phm.cpp phm_parser.cpp)

target_link_libraries(
    ${NAME}
    PUBLIC ${DESBORDANTE_PREFIX}::algos Boost::headers
    PRIVATE ${DESBORDANTE_PREFIX}::config
            ${DESBORDANTE_PREFIX}::util
            spdlog::spdlog_header_only
)