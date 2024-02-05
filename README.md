# Compression and decompresion with Huffman coding and the LZW algorithm
#### Dec. 2023


## About
This repository includes an example program implementing compression and decompression using Huffman coding and the 
Lempel-Ziv-Welch algorithm.

### Please check PROSE.md for a (lengthy) run through the program and its benchmarking results.


## Program Overview

The program contains:

- Two algorithms (LZW and Huffman) for compressing and decompressing files
- A file handler to manage opening, reading and writing binary to/from files
- An encompassing compression handling class that incorporates all the above elements into simple to use functions.


<div style="display: flex">
<img style="margin: auto" src="docs/img/Overview.png" alt="UML overview of the program" width="1300"/>
</div>
<div style="text-align: center;">
UML Overview of the program (public methods only)
</div>

## File Structure

```text
- docs
    - drawio
        - Activity diagram Huffman.drawio
        - Alphabetically ordered single frequencies.drawio
        - Overview.drawio
    - img
        - Activity diagram Huffman.png
        - Alphabetically ordered single frequencies.png
        - EOL windows only appearing in compressed data.png
        - EOL_compact.png
        - huff_decoding.png
        - huff_encoding.png
        - Overview.png
        - Tree for programming.png
    - presentation
        - compression_v4.pptx
- include
    - compression_handler.h
    - file_handler.h
    - huff.h
    - lzw.h
- src
    - compression_handler.cpp
    - file_hanlder.cpp
    - huff.cpp
    - lzw.cpp
- test_txt
    - (..)
- CMakeLists.txt
- main.cpp
- PROSE.md
- README.md
```

