/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <curl/curl.h>

#include "books.h"

int
http_get_to_file(const char *uri, const char *filename)
{
	FILE *fp;
	curl = curl_easy_init();
	/* para caso avancemos muito na execucao, nao retornar sem limpar */
	int result = 0;

	if (curl == NULL) {
		printf("Failed to setup connection");
		curl_easy_cleanup(curl);
		return 1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, uri);
	/* we disable SSL verification for simplicity */
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

	/* Define our callback to get called when there's data to be written */
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_file);

	/* Set the filename to pass to the callback */
	fp = fopen(filename, "w");
	if (fp != NULL) {
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		response = curl_easy_perform(curl);
		if (response != CURLE_OK) {
			fprintf(stderr, "Failed to connect: %s\n",
				curl_easy_strerror(response));
			result = 1;
		}
	} else {
		fprintf(stderr, "Failed to open file %s \terror: %s", filename,
			strerror(errno));
		result = 1;
	}

	fclose(fp);
	curl_easy_cleanup(curl);

	return result;
}

size_t
write_to_file(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	return fwrite(ptr, size, nmemb, (FILE *) userdata);
}

