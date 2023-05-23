ARG UBUNTU_DISTRO=jammy
#rolling is alternative
ARG ROS_DISTRO=humble 
FROM ubuntu:${UBUNTU_DISTRO}

# Set up install, set tzdata
ARG UBUNTU_DISTRO
ARG ROS_DISTRO

# Get ROS key
RUN apt update && apt install -y curl gnupg2 lsb-release wget && \
    curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg

# Install apt deps 
RUN echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu ${UBUNTU_DISTRO} main" | tee /etc/apt/sources.list.d/ros2.list > /dev/null
RUN apt update && DEBIAN_FRONTEND=noninteractive apt install -y \
  ros-${ROS_DISTRO}-desktop \
  ros-${ROS_DISTRO}-rmw-cyclonedds-cpp \
  python3-colcon-common-extensions

# Install H.264 specific dependencies
RUN DEBIAN_FRONTEND=noninteractive apt install -y \
    libavdevice-dev \
    libavformat-dev \
    libavcodec-dev \ 
    libavutil-dev \
    libswscale-dev \
    libx264-dev \ 
    # Dependency below only needed for the demo, but not really needed for H.264
    ros-${ROS_DISTRO}-camera-calibration-parsers

# Create H.264 FogROS2 worspace and build it
ENV ROS_WS=/home/root/fog_ws
RUN mkdir -p ${ROS_WS}/src
WORKDIR ${ROS_WS}/src
COPY .  ${ROS_WS}/src/
WORKDIR ${ROS_WS}

RUN . /opt/ros/${ROS_DISTRO}/setup.sh && \
      colcon build
