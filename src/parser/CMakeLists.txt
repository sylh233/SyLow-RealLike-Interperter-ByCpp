aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/. src_list)
set(inc_dir ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(private_inc_dir ${CMAKE_CURRENT_SOURCE_DIR}/private_inc)
file(GLOB inc_list ${inc_dir}/*.hpp)

add_library(parser)
target_include_directories(parser
  PUBLIC ${inc_dir}
  PRIVATE ${private_inc_dir}
)
target_sources(parser
  PRIVATE ${src_list}
  PRIVATE ${inc_list}
)
target_compile_features(parser PRIVATE cxx_std_17)
