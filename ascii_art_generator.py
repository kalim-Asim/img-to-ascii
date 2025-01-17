import csv
from PIL import Image, ImageFont, ImageDraw

# Step 1: Create the ASCII Palette
def calculate_ascii_densities():
    font = ImageFont.load_default()
    densities = []
    for i in range(32, 127):  # ASCII printable characters
        char = chr(i)
        img = Image.new("L", (10, 10), 255)
        draw = ImageDraw.Draw(img)
        draw.text((0, 0), char, font=font, fill=0)
        density = sum(pixel < 128 for pixel in img.getdata())
        densities.append((density, i, char))
    densities.sort()  # Sort by density
    with open("ascii_density.csv", "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Density", "ASCII Number", "Character"])
        writer.writerows(densities)

# Step 2: Generate ASCII Art from Image
def generate_ascii_art(input_image, output_file, width=80):
    # Load image
    img = Image.open(input_image).convert("L")
    
    # Resize image
    aspect_ratio = img.height / img.width
    new_height = int(aspect_ratio * width)
    img = img.resize((width, new_height), Image.Resampling.BOX)
    
    # Load ASCII densities
    with open("ascii_density.csv", "r") as f:
        reader = csv.reader(f)
        next(reader)
        ascii_map = [row for row in reader]
    densities = [int(row[0]) for row in ascii_map]
    chars = [row[2] for row in ascii_map]
    
    # Map pixels to ASCII
    ascii_art = ""
    for pixel in list(img.getdata()):
        idx = int(pixel / 255 * (len(densities) - 1))
        ascii_art += chars[idx]
    ascii_art = "\n".join([ascii_art[i:i+width] for i in range(0, len(ascii_art), width)])
    
    # Save ASCII Art
    with open(output_file, "w") as f:
        f.write(ascii_art)

# Run Steps
calculate_ascii_densities()
generate_ascii_art("your image location", "ascii_art.txt")

