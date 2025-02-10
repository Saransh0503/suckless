#!/bin/bash

# Script to install custom dwm, st, and dmenu configurations
# Author: Saransh

# Exit immediately if a command fails
set -e

# Colors for output
GREEN="\e[32m"
RED="\e[31m"
YELLOW="\e[33m"
RESET="\e[0m"

# Define paths to your custom repositories
DWM_REPO="$HOME/suckless/dwm"
ST_REPO="$HOME/suckless/st"
DMENU_REPO="$HOME/suckless/dmenu"

# Function to check for dependencies
check_dependencies() {
  echo -e "${YELLOW}Checking for required dependencies...${RESET}"
  dependencies=("git" "make" "gcc" "xorg" "xorg-xinit" "libx11" "libxft" "libxinerama" "brightnessctl" "pamixer" "flameshot" "python-pywal" "feh" )
  for dep in "${dependencies[@]}"; do
    if ! command -v "$dep" &> /dev/null; then
      echo -e "${RED}Error: $dep is not installed.${RESET}"
      echo -e "${YELLOW}Install it using your package manager and re-run the script.${RESET}"
      exit 1
    fi
  done
  echo -e "${GREEN}All required dependencies are installed.${RESET}"
}

# Function to build and install a repository
build_and_install() {
  local repo_path=$1
  local name=$2
  if [ -d "$repo_path" ]; then
    echo -e "${GREEN}Installing $name...${RESET}"
    cd "$repo_path"
    sudo make clean install
    echo -e "${GREEN}$name installed successfully.${RESET}"
  else
    echo -e "${RED}Error: $repo_path not found.${RESET}"
    exit 1
  fi
}

# Main script
main() {
  echo -e "${GREEN}Starting the installation process...${RESET}"

  # Check dependencies
  check_dependencies

  # Build and install dwm
  build_and_install "$DWM_REPO" "dwm"

  # Build and install st
  build_and_install "$ST_REPO" "st"

  # Build and install dmenu
  build_and_install "$DMENU_REPO" "dmenu"

  # Finishing up
  echo -e "${GREEN}All components installed successfully!${RESET}"
  echo -e "${YELLOW}Restart your X session or run 'startx' to use dwm.${RESET}"
}

# Execute the script
main
