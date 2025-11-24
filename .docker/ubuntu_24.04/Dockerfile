# Use Ubuntu 24.04 as the base image
FROM ubuntu:24.04

# Set DEBIAN_FRONTEND to noninteractive to avoid prompts
ENV DEBIAN_FRONTEND=noninteractive

# Update package lists and install required tools
RUN apt-get update && \
    apt-get install -y \
    git \
    cmake \
    ninja-build \
    gcc-14 \
    g++-14 \
    && rm -rf /var/lib/apt/lists/*

# Configure alternatives for gcc and g++
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 100

# Switch to the non-root user
USER ubuntu

# Set the working directory
WORKDIR /work

# Verify the versions of the installed tools
RUN echo "Verifying tool versions:" && \
    gcc --version && \
    g++ --version && \
    cmake --version && \
    ninja --version
