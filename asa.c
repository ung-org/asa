/*
 * UNG's Not GNU
 *
 * Copyright (c) 2011-2019, Jakob Kaivo <jkk@ung.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define _POSIX_C_SOURCE 2
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int asa(const char *path)
{
	FILE *f = stdin;
	if (path && strcmp(path, "-")) {
		f = fopen(path, "rb");
	}

	if (f == NULL) {
		fprintf(stderr, "asa: %s: %s\n", path, strerror(errno));
		return 1;
	}

	int c;
	int newline = -1;
	while ((c = fgetc(f)) != EOF) {
		if (newline != 0) {
			switch (c) {
			case '0':
				c = '\n';
				break;

			case '1':
				c = '\f';
				break;

			case '+':
				c = '\r';
				newline = 0;
				break;

			case ' ':
			default:
				c = ' ';
				break;
			}

			if (newline == 1) {
				putchar('\n');
			}

			if (c != ' ') {
				putchar(c);
			}
			newline = 0;
		} else if (c == '\n') {
			newline = 1;
		} else {
			newline = 0;
			putchar(c);
		}
	}

	if (newline == 1) {
		putchar('\n');
	}

	if (f != stdin) {
		fclose(f);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	while (getopt(argc, argv, "") != -1) {
		return 1;
	}

	int r = 0;

	do {
		r |= asa(argv[optind++]);
	} while (optind < argc);

	return r;
}
