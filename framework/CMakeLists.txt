# ---------------------------------------------------------------------------
# Framework library
#   Collects all framework source files (geometry, materials, rendering)
#   into a static library. The raytracer, example app, and tests all
#   link against this library to reuse the same code.
# ---------------------------------------------------------------------------

file(GLOB FRAMEWORK_SOURCE *.cpp)
file(GLOB FRAMEWORK_INLINE *.inl)
file(GLOB FRAMEWORK_HEADER *.hpp)

# Group inline files separately in IDEs like Visual Studio for easier navigation
source_group("Inline Files" FILES ${FRAMEWORK_INLINE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

# Build the framework as a static library
add_library(${FRAMEWORK_NAME} STATIC glew.c
  ${FRAMEWORK_SOURCE}
  ${FRAMEWORK_INLINE}
  ${FRAMEWORK_HEADER}
 )

# Link the framework against the graphics libraries it depends on
target_link_libraries(${FRAMEWORK_NAME}
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
)
