FROM archlinux:latest

RUN pacman --noconfirm -Syu && \
    pacman --noconfirm -S \
    git base-devel go \
    vim sudo bash-completion \
    wget curl zsh && \
    rm /var/cache/pacman/pkg/*

# Create a non-root user for using yay safely
RUN useradd -m -s /bin/bash dev && \
    echo "dev ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# Switch to the new user
USER dev
WORKDIR /home/dev

# Install yay
RUN git clone https://aur.archlinux.org/yay-bin.git /home/dev/yay-bin && \
    cd /home/dev/yay-bin && \
    makepkg -si --noconfirm && \
    rm -rf /home/dev/yay-bin

# Set PATH (optional, but ensures yay is found)
ENV PATH="/home/dev/.local/bin:$PATH"

# Install an AUR package globally
RUN yay -Sy --noconfirm intel-sde

WORKDIR /home/dev/