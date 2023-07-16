find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package (PythonInterp 3 REQUIRED)
find_package (PythonLibs 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories("${EIGEN3_INCLUDE_DIR}")
add_executable(${PROJECT_NAME}.x quill.cpp)
message(${PYTHON_LIBRARIES} )
add_library(inp inp.cpp)
add_library(geo geo.cpp)
add_library(int int.cpp)
add_library(scf scf.cpp)
target_link_libraries(${PROJECT_NAME}.x ${EIGEN3_LIBRARIES} ${PYTHON_LIBRARIES} inp geo int scf)

# These are the new install commands:
install(TARGETS ${PROJECT_NAME}.x DESTINATION bin)
install(TARGETS inp geo int scf DESTINATION lib)

