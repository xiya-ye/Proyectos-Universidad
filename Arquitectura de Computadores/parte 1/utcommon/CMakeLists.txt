set(COMMON_SRC_FILES 
  "${CMAKE_SOURCE_DIR}/common/src/material.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/sphere.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/cylinder.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/scene.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/config.cpp"
)

set(CURRENT_DIR_SRC_FILES 
  "${CMAKE_CURRENT_SOURCE_DIR}/test_vector.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_ray.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_color.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_material.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_sampler.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_object.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_sphere.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_cylinder.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_scene.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_camera.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_ppm_writer.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_config.cpp"
)

add_unit_test_target(
  TARGET_NAME utest-common
  SOURCE_FILES ${COMMON_SRC_FILES} ${CURRENT_DIR_SRC_FILES}
  LIBRARY_FILTER common
  COVERAGE_DIR coverage-common
  LIBRARY_TO_LINK common
  INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/aos/include
)