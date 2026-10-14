cmake_minimum_required(VERSION 4.1)

# 该项目可作为新的cmake组织方法范例
project(CompilerByCpp
  LANGUAGES CXX C
  VERSION 0.1.0
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# cpp标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(apps)

