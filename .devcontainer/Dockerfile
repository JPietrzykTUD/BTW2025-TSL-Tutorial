FROM mcr.microsoft.com/devcontainers/base:ubuntu

RUN apt-get update && apt-get install -y \
    curl vim htop \
    python3 python3-pip python3-venv \
    && apt-get clean && rm -rf /var/lib/apt/lists/*
