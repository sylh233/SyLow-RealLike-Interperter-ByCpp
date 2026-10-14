add_subdirectory(scanner)
add_subdirectory(parser)
# byd .cmake 文件没有作用域，还得是cmakelists

add_library(cbycpp_lib INTERFACE)
target_link_libraries(cbycpp_lib
  INTERFACE scanner
  INTERFACE parser
)
