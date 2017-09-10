cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall")

project(th3d)

#add_definitions(-DDEBUG)

file(GLOB_RECURSE th3d_SOURCES "src/*.cpp")
file(GLOB_RECURSE th3d_HEADERS "src/*.hpp")

file(COPY resources DESTINATION .)

if (MSVC)
	# according to the organization in the file systems
	macro(GroupSources curdir)

		file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
		${PROJECT_SOURCE_DIR}/${curdir}/*)

		foreach(child ${children})
			if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
				GroupSources(${curdir}/${child})
			else()
				string(REPLACE "/" "\\" groupname ${curdir})
				source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()

	GroupSources(src)
endif(MSVC)

find_package(PNG REQUIRED)
include_directories(${PNG_PNG_INCLUDE_DIR})
set(LIBS ${LIBS} ${PNG_LIBRARY})

find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
set(LIBS ${LIBS} ${LUA_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLEW_LIBRARIES})

find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLFW_LIBRARIES})

find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIR})
set(LIBS ${LIBS} ${ASSIMP_LIBRARY_RELEASE})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

find_package(PNGXX REQUIRED)
include_directories(${PNGXX_INCLUDE_DIRS})

add_executable(th3d ${th3d_SOURCES} ${th3d_HEADERS})
target_link_libraries(th3d ${LIBS})
