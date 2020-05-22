# LZW-compressor
A compressor utilizing the LZW algorithm which mimics the UNIX CLI 'compress' tool.

File are compressed by reading an input file byte by byte.

Usage:

$ make
$ ./uvcompress < path/to/inputfile > output.Z

output.Z will contain the compressed file.
