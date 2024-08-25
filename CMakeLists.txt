cmake_minimum_required(VERSION 3.28)
project(PlantsVsZombies CXX)

# 使用 C++11 标准
set(CMAKE_CXX_STANDARD 11)

# 包含 EasyX 的头文件目录
include_directories(./EasyX/include ./Header)

# 链接 EasyX 的库文件目录
link_directories(./EasyX/lib64)

# 链接 EasyX 库文件
link_libraries(EasyX)

# 添加源文件并生成可执行文件
add_executable(PlantsVsZombies main.cpp ./Source/tools.cpp)
