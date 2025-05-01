# TODO: not very smart, but it works. Comprise all files extensions in one loop later

# script to apply tilt axes cpp code
#!/usr/bin/env bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 2 ]; then
    echo "Provide input folder containing .jpg and .txt files."
	echo "New files will be created in the same folder with '_description' appended to the filename."
    echo "Usage: $0 <input_folder> <theta>"
    exit 1
fi

# Get the input folder name
input_folder="$1"

# Check if the input folder exists
if [ ! -d "$input_folder" ]; then
	echo "Input folder does not exist."
	exit 1
fi

# Get theta from the user
theta="$2"

# get all .jpg and .txt files in the input folder
# loop through all .jpg and .JPG files
for jpg_file in "$input_folder"/*.jpg; do
    # check if the file exists
    if [ ! -f "$jpg_file" ]; then
	echo "No .jpg files found in the input folder."
	exit 1
    fi

    # get the base name of the file (without extension)
    base_name=$(basename "$jpg_file" .jpg)

    # check if the corresponding .txt file exists
    txt_file="$input_folder/$base_name.txt"

    if [ ! -f "$txt_file" ]; then
	echo "Corresponding .txt file for $jpg_file not found."
	continue
    fi

    # run the cpp code on the .txt file and save the output to the new file
    tilt_axes "$jpg_file" "$txt_file" "$theta"

    echo "Processed $jpg_file and $txt_file. Tilted by $theta degrees."
done
# loop through all .JPG files

for jpg_file in "$input_folder"/*.JPG; do
    # check if the file exists
    if [ ! -f "$jpg_file" ]; then
	echo "No .jpg files found in the input folder."
	exit 1
    fi

    # get the base name of the file (without extension)
    base_name=$(basename "$jpg_file" .JPG)

    # check if the corresponding .txt file exists
    txt_file="$input_folder/$base_name.txt"

    if [ ! -f "$txt_file" ]; then
	echo "Corresponding .txt file for $jpg_file not found."
	continue
    fi

    # run the cpp code on the .txt file and save the output to the new file
    tilt_axes "$jpg_file" "$txt_file" "$theta"

    echo "Processed $jpg_file and $txt_file. Tilted by $theta degrees."
done

# loop through all .png files
for jpg_file in "$input_folder"/*.png; do
    # check if the file exists
    if [ ! -f "$jpg_file" ]; then
	echo "No .jpg files found in the input folder."
	exit 1
    fi

    # get the base name of the file (without extension)
    base_name=$(basename "$jpg_file" .png)

    # check if the corresponding .txt file exists
    txt_file="$input_folder/$base_name.txt"

    if [ ! -f "$txt_file" ]; then
	echo "Corresponding .txt file for $jpg_file not found."
	continue
    fi

    # run the cpp code on the .txt file and save the output to the new file
    tilt_axes "$jpg_file" "$txt_file" "$theta"

    echo "Processed $jpg_file and $txt_file. Tilted by $theta degrees."
done

# loop through all .PNG files

for jpg_file in "$input_folder"/*.PNG; do
    # check if the file exists
    if [ ! -f "$jpg_file" ]; then
	echo "No .jpg files found in the input folder."
	exit 1
    fi

    # get the base name of the file (without extension)
    base_name=$(basename "$jpg_file" .PNG)

    # check if the corresponding .txt file exists
    txt_file="$input_folder/$base_name.txt"

    if [ ! -f "$txt_file" ]; then
	echo "Corresponding .txt file for $jpg_file not found."
	continue
    fi

    # run the cpp code on the .txt file and save the output to the new file
    tilt_axes "$jpg_file" "$txt_file" "$theta"

    echo "Processed $jpg_file and $txt_file. Tilted by $theta degrees."
done
