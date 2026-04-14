# ROS2 Workspace Build & Setup Guide

### 📦 Build

Run the following command in your workspace root (e.g., `~/ros2_ws`):

```bash
colcon build --symlink-install
```

To build only one package:

```bash
colcon build --packages-select your_package_name --symlink-install
```

### 🌱 Source the Environment

Before using ROS2, you need to source the environment (ros installation and workspace):

```bash
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
```

### ⚙️ Add to `.bashrc` (Recommended)

To avoid sourcing every time, add these lines to your `~/.bashrc`:

```bash
gedit ~/.bashrc
```

### 📦 Create package

```bash
ros2 pkg create package_name --build-type ament_cmake --dependencies rclcpp rclpy std_msgs
ros2 pkg create --build-type ament_python package_name --dependencies rclpy std_msgs
```

### ⚙️ common command

```bash
# check the rqt graph
rqt_graph

# check the rqt tf tree
ros2 run rqt_tf_tree rqt_tf_tree

# dynamically change the config, avoid 
ros2 run rqt_reconfigure rqt_reconfigure

# let the py file can run
chmod +x xx.py

# check the msg format
ros2 interface show msg
```
