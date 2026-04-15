cmake_minimum_required(VERSION 3.8)
project(cv_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

# ====== C++ node ======
add_executable(cv_image_node src/cv_image_node.cpp)
add_executable(cv_hsv_node src/cv_hsv_node.cpp)
add_executable(cv_follow_node src/cv_follow_node.cpp)
ament_target_dependencies(cv_image_node rclcpp cv_bridge OpenCV sensor_msgs)
ament_target_dependencies(cv_hsv_node rclcpp cv_bridge OpenCV sensor_msgs)
ament_target_dependencies(cv_follow_node rclcpp cv_bridge OpenCV sensor_msgs geometry_msgs)

install(TARGETS
    cv_image_node
    cv_hsv_node
    cv_follow_node
    DESTINATION lib/${PROJECT_NAME}
)

# ====== Python node ======
install(PROGRAMS
    src/cv_image_node.py
    src/cv_hsv_node.py
    src/cv_follow_node.py
    DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
