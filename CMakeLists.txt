# CMakeList.txt : CMake project for RTNW, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

SET(PROJECTNAME RTNW)
PROJECT(${PROJECTNAME})

set ( RTNW_CORE_HEADERS
	src/RTNW.h
	src/Vec3.h
	src/Ray.h
	src/Camera.h
	src/Color.h
	src/Hittable.h
	src/HittableList.h
	src/Material.h
	src/Sphere.h
	src/Utils.h
	src/MovingSphere.h		
	src/AABB.h			
	src/BVHNode.h		
	src/Texture.h			
	src/Perlin.h
	src/rtnw_stb_image.h
  "src/AARect.h" "src/Box.h" "src/ConstantMedium.h")

set ( RTNW_CORE_SOURCE
	src/Vec3.cpp
)

include_directories ( src )
#include_directories (external)

add_library ( rtnw_lib STATIC
	${RTNW_CORE_HEADERS}
	${RTNW_CORE_SOURCE}
)

set_target_properties(rtnw_lib PROPERTIES LINKER_LANGUAGE CXX)


add_executable (${PROJECTNAME} "src/main.cpp" )
set_target_properties(${PROJECTNAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries ( ${PROJECTNAME} rtnw_lib)

 