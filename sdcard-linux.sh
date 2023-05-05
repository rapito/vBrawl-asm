#!/bin/bash

# Save the current directory.
CURRENT_DIR="$(pwd)"

# Change to the directory containing the script.
cd "$(dirname "$0")"

VOLUME_NAME="BrawlbackSDCard"
MOUNT_DRIVE="/mnt/$VOLUME_NAME"
SD_CARD1_PATH="./sdcard1"
SD_CARD2_PATH="./sdcard2"

# Define the relative path of the folder to copy to the SD card.
SD_CARD_FOLDER="./SDCard"

# Define the size of the SD card image.
SD_CARD_SIZE="3g"

# Define an array of the SD card paths.
SD_CARDS=("$SD_CARD1_PATH" "$SD_CARD2_PATH")

for sd_card_path in "${SD_CARDS[@]}"; do
  # Check if the SD card image already exists.
  if [ -f "$sd_card_path.raw" ]; then
    echo "Mounting existing SD card: $sd_card_path"
  else
    echo "Creating new SD card: $sd_card_path"
#    sudo dd if=/dev/zero of="$sd_card_path.raw" bs=1M count="$SD_CARD_SIZE"
    sudo dd if=/dev/zero of="$sd_card_path.raw" bs=1 count=0 seek="$SD_CARD_SIZE"
    sudo mkfs.fat -n "$VOLUME_NAME" -F32 "$sd_card_path.raw"
  fi

  # Mount the SD card image.
  echo "Mounting SD card: $sd_card_path.raw"
  sudo mkdir -p "$MOUNT_DRIVE"
  sudo mount -o loop "$sd_card_path.raw" "$MOUNT_DRIVE"

  # Copy the contents of the specified folder to the SD card.
  echo "Copying files to SDCard: $SD_CARD_FOLDER => $MOUNT_DRIVE"
  sudo cp -R "$SD_CARD_FOLDER/*" "$MOUNT_DRIVE"

  # Unmount the SD card image.
  echo "Unmounting SD card: $MOUNT_DRIVE"
  sudo umount "$MOUNT_DRIVE"

done

# Restore the current directory.
cd "$CURRENT_DIR"
