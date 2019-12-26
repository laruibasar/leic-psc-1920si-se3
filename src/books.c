/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */
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


