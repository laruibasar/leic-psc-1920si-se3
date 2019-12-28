/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */
#include <string.h>
#include <curl/curl.h>

#include "books.h"

CURL *curl;
CURLcode response;

int
init_curl()
{
	return curl_global_init(CURL_GLOBAL_ALL);
}

void
close_curl()
{
	curl_global_cleanup();
}

char *
string_encode(char *str)
{
	/* um ponteiro para iterar pela origen */
	char *pstr = str;
	
	/* a nova cadeia e o ponteiro para iterar */
	char *new;
	if ((new = (char*) malloc(URI_MAX_SIZE * sizeof(char))) == NULL)
		return NULL;

	char *pnew = new;

	while (*pstr) {
		if (*pstr == '"') {
			*pnew++ = '%';
			*pnew++ = '2';
			*pnew++ = '2';
		} else if (*pstr == ' ') {
			*pnew++ = '%';
			*pnew++ = '2';
			*pnew++ = '0';
		} else 
			*pnew++ = *pstr;

		pstr++;
	}
	*pnew = '\0';

	return new;
}

size_t
set_query_string(const char *url, const char *apikey, const char *separator,
		const char *param, const char *value, char* uri)
{
	strcat(uri, url);
	strcat(uri, param);
	strcat(uri, value);
	strcat(uri, separator);
	strcat(uri, "key=");
	strcat(uri, apikey);

	return strnlen(uri, URI_MAX_SIZE);
}
