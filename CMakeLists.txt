cmake_minimum_required (VERSION 3.8)

project ("GreedySnake")

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(Qt5 REQUIRED Widgets)
set(CMAKE_AUTOMOC ON)

set(src_dir .)
foreach(dir ${src_dir})
	string(TOUPPER "${dir}" dir_u)			
	file(GLOB ${dir_u}_UI_FILES ${dir}/*.ui)
	file(GLOB ${dir_u}_H_FILES ${dir}/*.h)
	file(GLOB ${dir_u}_CXX_FILES ${dir}/*.cpp)	
	list(APPEND UI_FILES ${${dir_u}_UI_FILES})		
	list(APPEND SRCS_FILES ${${dir_u}_CXX_FILES})
	list(APPEND HEADER_FILES ${${dir_u}_H_FILES})
endforeach(dir ${src_dir})

if(UI_FILES)
	qt5_wrap_ui( UIC_SRCS ${UI_FILES}) 
endif()

add_executable (${PROJECT_NAME} 
	${SRCS_FILES}
	${HEADER_FILES}
	#${UIC_SRCS}  # 添加此选项会生成 xxx_autogen 工程 
	${UI_FILES} # 和${UIC_SRCS} 二选一
	${RCC_SRCS})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)