/*
 * UNG's Not GNU
 *
 * Copyright (c) 2011-2017, Jakob Kaivo <jkk@ung.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#define _XOPEN_SOURCE 700
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
		fprintf(stderr, "asa: Couldn't open %s: %s\n", path ? path : "stdin", strerror(errno));
		return 1;
	}

	while (!feof(f)) {
		char *line = NULL;
		size_t n = 0;

		if (getline(&line, &n, f) == -1) {
			if (ferror(f)) {
				fprintf(stderr, "asa: Couldn't open %s: %s\n", path ? path : "stdin", strerror(errno));
				return 1;
			}
			if (line) {
				free(line);
			}
			continue;
		}

		switch(line[0]) {
		case '0':
			putchar('\n');
			break;

		case '1':
			putchar('\f');
			break;

		case '+':
			putchar('\r');
			break;

		case ' ':
		default:
			break;
		}

		fputs(line + 1, stdout);
		free(line);
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
