# img-to-ascii

# To run the code
  - sudo apt-get install libopencv-dev (for c)
  -  pip3 install pillow (for python)
  -  in C
     - g++ ascii_art_generator.c -o ascii_art_generator `pkg-config --cflags --libs opencv4`
     - ./ascii_art_generator image.png ascii_density.csv
