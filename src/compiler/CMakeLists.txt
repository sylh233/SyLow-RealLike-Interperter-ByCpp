aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/scanner scanner_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/parser parser_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/exception exception_src)

set(inc_dir ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(private_inc_dir ${CMAKE_CURRENT_SOURCE_DIR}/private_inc)

add_library(compiler)
target_include_directories(compiler
  PUBLIC ${inc_dir}
  PRIVATE ${private_inc_dir}
)
target_sources(compiler
  PRIVATE ${scanner_src}
  PRIVATE ${parser_src}
  PRIVATE ${exception_src}
)
target_compile_features(compiler PRIVATE cxx_std_17)
include(3rd_icu/icu.cmake)
target_link_libraries(compiler PUBLIC ICU::uc ICU::i18n ICU::data) # ICU::io)
