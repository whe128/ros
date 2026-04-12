cmake_minimum_required(VERSION 3.8)
project(custom_msg_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(custom_msgs REQUIRED)


# ====== C++ node ======
add_executable(custom_msg_pub_node src/custom_msg_pub_node.cpp)
add_executable(custom_msg_sub_node src/custom_msg_sub_node.cpp)

ament_target_dependencies(custom_msg_pub_node rclcpp custom_msgs)
ament_target_dependencies(custom_msg_sub_node rclcpp custom_msgs)

install(TARGETS
    custom_msg_pub_node
    custom_msg_sub_node
    DESTINATION lib/${PROJECT_NAME}
)

# ====== Python node ======
install(PROGRAMS
    src/custom_msg_pub_node.py
    src/custom_msg_sub_node.py
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
