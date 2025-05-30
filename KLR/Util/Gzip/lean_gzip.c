/*
Copyright (c) 2024 Amazon.com, Inc. or its affiliates. All Rights Reserved.
Released under Apache 2.0 license as described in the file LICENSE.
Authors: Paul Govereau, Sean McLaughlin
*/

#define _GNU_SOURCE // for vasprintf

#include <lean/lean.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <errno.h>
#include <string.h>

#include "lean_util.h"

LEAN_EXPORT lean_object* lean_gzip(b_lean_obj_arg input) {
  uint8_t *c_input = lean_sarray_cptr(input);
  size_t input_len = lean_sarray_size(input);

  // Calculate an upper bound for the compressed data size
  uLongf max_compressed_len = compressBound(input_len) + 32; // Add extra space for gzip header/footer

  // Allocate memory for the compressed data
  uint8_t* compressed_data = (uint8_t*)malloc(max_compressed_len);
  if (!compressed_data) {
    return io_err("Failed to allocate memory for compressed data");
  }

  // Initialize the z_stream structure for compression
  z_stream strm;
  memset(&strm, 0, sizeof(strm));

  // Initialize for gzip compression
  int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                         31, // 15 for max window size + 16 for gzip encoding
                         8,  // Default memory level
                         Z_DEFAULT_STRATEGY);

  if (ret != Z_OK) {
    free(compressed_data);
    return io_err("Failed to initialize deflate: %s", zError(ret));
  }

  // Set input data
  strm.next_in = c_input;
  strm.avail_in = input_len;

  // Set output buffer
  strm.next_out = compressed_data;
  strm.avail_out = max_compressed_len;

  // Compress all input in one call
  ret = deflate(&strm, Z_FINISH);

  // Check for errors
  if (ret != Z_STREAM_END) {
    deflateEnd(&strm);
    free(compressed_data);
    return io_err("Compression failed: %s", zError(ret));
  }

  // Get the actual compressed size
  size_t compressed_size = max_compressed_len - strm.avail_out;

  // Clean up the z_stream
  deflateEnd(&strm);

  // Create a Lean array to store the compressed data
  lean_object* result_array = lean_alloc_sarray(1, compressed_size, compressed_size);

  // Copy the compressed data to the Lean array
  memcpy(lean_sarray_cptr(result_array), compressed_data, compressed_size);

  // Free the temporary buffer
  free(compressed_data);

  return result_array;
}

LEAN_EXPORT lean_object* lean_gunzip(b_lean_obj_arg input) {
  uint8_t *c_input = lean_sarray_cptr(input);
  size_t input_len = lean_sarray_size(input);

  // Initial output buffer size - we'll grow this if needed
  size_t initial_output_size = input_len * 4; // Start with 4x the input size as an estimate
  uint8_t* decompressed_data = (uint8_t*)malloc(initial_output_size);
  if (!decompressed_data) {
    return io_err("Failed to allocate memory for decompressed data");
  }

  // Initialize the z_stream structure for decompression
  z_stream strm;
  memset(&strm, 0, sizeof(strm));

  // Initialize for gzip decompression
  int ret = inflateInit2(&strm,
                         31); // 15 for max window size + 16 for gzip encoding

  if (ret != Z_OK) {
    free(decompressed_data);
    return io_err("Failed to initialize inflate: %s", zError(ret));
  }

  // Set input data
  strm.next_in = c_input;
  strm.avail_in = input_len;

  // Set output buffer
  strm.next_out = decompressed_data;
  strm.avail_out = initial_output_size;

  // Decompress the data
  size_t total_out = 0;
  do {
    ret = inflate(&strm, Z_FINISH);

    // If the output buffer is full but there's still more data to decompress
    if (ret == Z_BUF_ERROR || (ret == Z_OK && strm.avail_out == 0)) {
      // Calculate how much we've decompressed so far
      total_out = initial_output_size - strm.avail_out;
      // Double the buffer size
      initial_output_size *= 2;
      uint8_t* new_buffer = (uint8_t*)realloc(decompressed_data, initial_output_size);

      if (!new_buffer) {
        free(decompressed_data);
        inflateEnd(&strm);
        return io_err("Failed to reallocate memory for decompressed data");
      }

      decompressed_data = new_buffer;

      // Update the stream to point to the new buffer position
      strm.next_out = decompressed_data + total_out;
      strm.avail_out = initial_output_size - total_out;

      // Continue decompression
      continue;
    }
    // Check for errors
    if (ret != Z_STREAM_END && ret != Z_OK) {
      free(decompressed_data);
      inflateEnd(&strm);
      return io_err("Decompression failed: %s", zError(ret));
    }
  } while (ret != Z_STREAM_END);

  // Get the actual decompressed size
  size_t decompressed_size = initial_output_size - strm.avail_out;

  // Clean up the z_stream
  inflateEnd(&strm);

  // Create a Lean array to store the decompressed data
  lean_object* result_array = lean_alloc_sarray(1, decompressed_size, decompressed_size);

  // Copy the decompressed data to the Lean array
  memcpy(lean_sarray_cptr(result_array), decompressed_data, decompressed_size);

  // Free the temporary buffer
  free(decompressed_data);

  return result_array;
}
