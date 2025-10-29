# CMakeLists.txt
cmake_minimum_required(VERSION 4.00)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

project(LearnModules LANGUAGES CXX)

# 添加可执行文件（包含模块和主文件）
add_executable(LearnModules
# 模块接口
    MyAlgorithms.cppm
# 主文件
    LearnModules.cpp
)

target_sources(LearnModules PUBLIC
# FILE_SET任意取名
        FILE_SET public_modules
# 模块接口文件 需要使用 CXX_MODULES 类型
        TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/
        FILES MyAlgorithms.cppm
)

# 编译选项
target_compile_options(LearnModules PRIVATE
# 调试信息
    -glldb
    -fmodules
    -fimplicit-modules
# 手动指定 std.pcm 路径
    -fmodule-file=std=E:/clangC++/clangModules/C++23/std.pcm
)

# 链接 libc++
target_link_libraries(LearnModules PRIVATE c++)