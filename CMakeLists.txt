cmake_minimum_required(VERSION 3.12)
project(
  demo
  VERSION 0.0.1
  LANGUAGES C CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 全局包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 全局包含库目录(指定库的搜索路径)
link_directories(${CMAKE_SOURCE_DIR}/lib)

# 源文件
file(GLOB chess ${CMAKE_SOURCE_DIR}/src/chess/*.c)
file(GLOB guessNumber ${CMAKE_SOURCE_DIR}/src/guessNumber/*.c)
file(GLOB studentSystem ${CMAKE_SOURCE_DIR}/src/studentSystem/*.c)
file(GLOB swap ${CMAKE_SOURCE_DIR}/src/swap/*.c)
file(GLOB func_pointer ${CMAKE_SOURCE_DIR}/src/func_pointer/*.c)
file(GLOB heap ${CMAKE_SOURCE_DIR}/src/heap/*.c)
file(GLOB sort ${CMAKE_SOURCE_DIR}/src/sort/*.c)
file(GLOB linked_list ${CMAKE_SOURCE_DIR}/src/linked_list/*.c)
file(GLOB vector ${CMAKE_SOURCE_DIR}/src/vector/*.c)
file(GLOB test ${CMAKE_SOURCE_DIR}/src/test/*.c*)
file(GLOB open_file_and_folder ${CMAKE_SOURCE_DIR}/src/open_file_and_folder/*.cpp)
file(GLOB c_merge ${CMAKE_SOURCE_DIR}/src/c_merge/*.cpp)
file(GLOB get_encode_bytes ${CMAKE_SOURCE_DIR}/src/get_encode_bytes/*.c)
file(GLOB find_string_index ${CMAKE_SOURCE_DIR}/src/find_string_index/*.c)
file(GLOB worktime ${CMAKE_SOURCE_DIR}/src/worktime/*.c)
file(GLOB midi_player ${CMAKE_SOURCE_DIR}/src/midi_player/*.cpp)
file(GLOB midi_extract ${CMAKE_SOURCE_DIR}/src/midi_player/midi_extract.cpp)
file(GLOB firework ${CMAKE_SOURCE_DIR}/src/firework/*.c*)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/midi_player/rtmidi)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/midi_player/midifile)


# 可执行文件
# add_executable(chess ${chess})

# add_executable(guessNumber ${guessNumber})

# add_executable(studentSystem ${studentSystem})

# add_executable(swap ${swap})

# add_executable(func_pointer ${func_pointer})

# add_executable(heap ${heap})

# add_executable(sort ${sort})

# add_executable(linked_list ${linked_list})

# add_executable(vector ${vector})

# add_executable(test ${test})

# add_executable(c_merge ${c_merge})

# add_executable(open_file_and_folder ${open_file_and_folder})

# add_executable(get_encode_bytes ${get_encode_bytes})

# add_executable(find_string_index ${find_string_index})

# add_executable(worktime ${worktime})

add_executable(midi_player ${midi_player})

add_executable(firework ${firework})



# 链接头文件
target_include_directories(firework PRIVATE ${CMAKE_SOURCE_DIR}/include/easyx)

# 链接库目录
target_link_directories(firework PRIVATE ${CMAKE_SOURCE_DIR}/lib/lib64/easyx)
target_link_directories(firework PRIVATE ${CMAKE_SOURCE_DIR}/lib/lib64/glfw)
target_link_directories(midi_player PRIVATE ${CMAKE_SOURCE_DIR}/lib/lib64/glfw)

# 链接库
target_link_libraries(midi_player PRIVATE rtmidi midifile)
target_link_libraries(firework PRIVATE glfw3)
