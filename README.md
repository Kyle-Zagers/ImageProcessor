# TGA Image Processor

A C++ library and application for processing TGA (Targa) image files with various image manipulation operations. This project provides a comprehensive set of image processing functions including blending modes, color channel manipulation, and geometric transformations.

### Features:

- File I/O: Read and write TGA image files with proper header handling
- Blending Modes: Multiply, subtract, overlay, and screen blend operations
- Color Channel Operations: Extract individual RGB channels, add/scale color values
- Image Transformations: Flip images vertically, combine multiple images
- Utility Functions: Color clamping and normalization for precise pixel manipulation

### Key Components:

- TGA_Header: Manages TGA file header structure with proper byte packing
- TGA_File: Core class handling image data and processing operations
- Support for 24-bit RGB TGA images
- Memory-safe vector-based pixel data storage
- Build System: Uses a Makefile-based build system with dependency tracking and proper C++ compilation flags.

### Use Cases:

- Batch image processing
- Color correction and enhancement
- Image composition and blending
- Educational tool for understanding image file formats and pixel manipulation
