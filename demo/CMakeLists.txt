# 指定cmake的版本
cmake_minimum_required(VERSION 2.6)

# 项目名称
set (TARGET_NAME echo_server)

# 包含头文件目录
set(WEBSOCKETPP_DIR /opensource/websocketpp-master)
set(BOOST_INCLUDE_DIR /usr/local/boost/include)
set(BOOST_INCLUDE_SUB_DIR /usr/local/boost/include/boost)
include_directories(. ${WEBSOCKETPP_DIR} ${BOOST_INCLUDE_DIR} ${BOOST_INCLUDE_SUB_DIR})

# 源文件目录
aux_source_directory(. DIR_SRCS)

# 用于连接的静态库
set(BOOST_LIB_DIR /usr/local/lib/)
set(LIBRARYS ${BOOST_LIB_DIR}/libboost_chrono.a ${BOOST_LIB_DIR}/libboost_system.a ${BOOST_LIB_DIR}/libboost_random.a)

# 编译可执行文件
add_executable(${TARGET_NAME} ${DIR_SRCS})

# 链接库
target_link_libraries(${TARGET_NAME} ${LIBRARYS} -lpthread -lrt -ldl)
