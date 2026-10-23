add_subdirectory(compiler)

# byd .cmake 文件没有作用域，还得是cmakelists

add_library(cbycpp_lib INTERFACE)
target_link_libraries(cbycpp_lib
  INTERFACE compiler
  #这里的依赖是前面的依赖后面的，不要把在源文件定义的函数放在前面
)

# INTERFACE "$<LINK_GROUP:RESCAN,compiler,parser>"
