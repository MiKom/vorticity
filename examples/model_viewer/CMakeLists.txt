add_executable(model_viewer WIN32 model_viewer.cpp)
target_link_libraries(model_viewer vorticity)

set_target_properties(model_viewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
