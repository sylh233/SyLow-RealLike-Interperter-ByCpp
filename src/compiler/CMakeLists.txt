aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/scanner scanner_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/parser parser_src)

set(inc_dir ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(private_inc_dir ${CMAKE_CURRENT_SOURCE_DIR}/private_inc)

add_library(scanner)
target_include_directories(scanner
  PUBLIC ${inc_dir}
  PRIVATE ${private_inc_dir}
)
target_sources(scanner
  PRIVATE ${scanner_src}
)
target_compile_features(scanner PRIVATE cxx_std_17)

add_library(parser)
target_include_directories(parser
  PUBLIC ${inc_dir}
  PRIVATE ${private_inc_dir}
)
target_sources(parser
  PRIVATE ${parser_src}
)
target_compile_features(parser PRIVATE cxx_std_17)
