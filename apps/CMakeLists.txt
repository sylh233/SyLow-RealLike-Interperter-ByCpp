set(HOME ${CMAKE_CURRENT_SOURCE_DIR}) # 通用的设置路径
set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} src_list)
set(inc_dir ${CMAKE_CURRENT_SOURCE_DIR}/inc)
file(GLOB inc_list ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.hpp)

add_executable(cbycpp)
target_include_directories(cbycpp PRIVATE ${inc_dir})
target_sources(cbycpp
  PRIVATE ${src_list}
  PRIVATE ${inc_list}
)
target_link_libraries(cbycpp PRIVATE cbycpp_lib)
target_compile_features(cbycpp PRIVATE cxx_std_17)
