# cmake自带FindICU.cmake，只需设置LIB_ROOT让它找库文件等即可
set(ICU_ROOT "D:/msys64/ucrt64/")
# 否则可用CMAKE_MODULE_PATH指定自己实现的FindLIB的位置
# 或设置LIB_ROOT/DIR去找LIBConfig.cmake
find_package(ICU GLOBAL REQUIRED COMPONENTS uc i18n data) # io)

