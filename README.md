# img-to-ascii

# To run the code

  - (for C)
    ```bash
    sudo apt-get install libopencv-dev
    ```
    ```bash
    g++ ascii_art_generator.c -o ascii_art_generator `pkg-config --cflags --libs opencv4`

    ./ascii_art_generator image.png ascii_density.csv
    ```
    
  - (for python)
     ```bash
     pip3 install pillow (for python)
     ```
