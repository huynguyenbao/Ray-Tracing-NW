# CMakeList.txt : CMake project for RT1W, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

SET(PROJECTNAME RT1W)
PROJECT(${PROJECTNAME})

set ( RT1W_CORE_HEADERS
	src/RT1W.h
	src/Vec3.h
	src/Ray.h
	src/Camera.h
	src/Color.h
	src/Hittable.h
	src/HittableList.h
	src/Material.h
	src/Sphere.h
	src/Utils.h
 )

set ( RT1W_CORE_SOURCE
	src/Vec3.cpp
	src/Ray.cpp
	src/Camera.cpp
	src/Color.cpp
	src/Hittable.cpp
	src/HittableList.cpp
	src/Material.cpp
	src/Sphere.cpp
)

include_directories ( src )

add_library ( rt1w_lib STATIC
	${RT1W_CORE_SOURCE}
	${RT1W_CORE_HEADERS}
)

set_target_properties(rt1w_lib PROPERTIES LINKER_LANGUAGE CXX)


add_executable (${PROJECTNAME} "src/main.cpp" )
set_target_properties(${PROJECTNAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries ( ${PROJECTNAME} rt1w_lib)


# Add source to this project's executable.
# add_executable (RT1W "main.cpp"  "Vec3.h" "Color.h"  "Ray.h" "Hittable.h" "Sphere.h" "HittableList.h" "RT1W.h" "Camera.h" "Utils.h" "Material.h")

# TODO: Add tests and install targets if needed.
