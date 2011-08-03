add_executable(rotating_boxes WIN32 rotating_boxes.cpp)
target_link_libraries(rotating_boxes vorticity)

set_target_properties(rotating_boxes PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")