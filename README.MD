# ROS2 Workspace Build & Setup Guide

## 📦 Build the Workspace

Run the following command in your workspace root (e.g., `~/ros2_ws`):

```bash
colcon build --symlink-install
```

---

## 🔧 Build a Specific Package

To build only one package:

```bash
colcon build --packages-select your_package_name
```

---

## 🌱 Source the Environment

Before using ROS2, you need to source the environment:

### 1. Source ROS2 installation and workspace

```bash
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
```

---

## ⚙️ Add to `.bashrc` (Recommended)

To avoid sourcing every time, add these lines to your `~/.bashrc`:

```bash
gedit ~/.bashrc
```

Append the following:

```bash
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
```

Then apply changes:

```bash
source ~/.bashrc
```

---
