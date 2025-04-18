#ifndef HEX_H
#define HEX_H

/* Converts a byte array to a null-terminated hex string */
char *convert_bytes_to_hex_string(const unsigned char *input, int input_size);

/* Converts a null-terminated hex string to a byte array */
unsigned char *convert_hex_string_to_bytes(const char *input);

#endif

