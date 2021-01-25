/**
 * @copyright(C) 2021 Andreas Bank, andreas.mikael.bank@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int hex2uint(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	} else {
		return -1;
	}
}

int main(int argc, char **argv) {
	int retval = 1;
	FILE *fp = NULL;
	const char *hex = NULL;
	size_t hex_len;
	uint8_t *bin = NULL;
	size_t bin_len;
	const char *filename = NULL;
	FILE *write_dest = stdout;
	size_t arg_idx = 1;
	int written;

	if (argc != 2 && argc != 4) {
		fprintf(stderr, "Wrong number of arguments\n");
		return 1;
	}

	if (strcmp(argv[arg_idx], "-f") == 0) {
		arg_idx++;
		filename = argv[arg_idx++];
	}

	hex_len = strlen(argv[arg_idx]);
	hex = argv[arg_idx++];

	/* Convert the hexadecimal string to binary */
	bin_len = hex_len / 2;
	bin = malloc(bin_len);
	for (size_t i = 0; i < bin_len; i++) {
		char u1 = hex2uint(hex[i*2]);
		char u2 = hex2uint(hex[i*2 + 1]);
		if (u1 < 0 || u1 > 15 || u2 < 0 || u2 > 15) {
			fprintf(stderr, "Invalid HEX characters in string "
				"('%c' or '%c'))\n", u1, u2);
			goto cleanup;
		}
		bin[i] = (uint8_t) ((u1 << 4) | u2);
	}

	if (filename != NULL) {
		if ((fp = fopen(filename, "wbx")) == NULL) {
			fprintf(stderr, "Failed to create file: %s\n",
				strerror(errno));
			goto cleanup;
		}
		write_dest = fp;
	}

	written = fwrite(bin, 1, bin_len, write_dest);
	if (written != bin_len) {
		fprintf(stderr, "Failed to write data to file: %s\n",
			strerror(errno));
		goto cleanup;
	}
	printf("\n");

	retval = 0;

#if TEST
	char *test_str = malloc(bin_len * 2 + 1);

	for (size_t i = 0; i < bin_len; i++) {
		sprintf(&test_str[i * 2], "%02x", bin[i]);
	}
	test_str[bin_len * 2] = '\0';

	printf("input string: '%s', test string: '%s'\n", hex, test_str);
	printf("Test result: %s\n",
		strcmp(hex, test_str) == 0 ? "SUCCESS" : "FAILURE");
#endif

cleanup:
	if (fp != NULL) {
		fclose(fp);
	}
	if (bin) {
		free(bin);
	}

	return retval;
}
