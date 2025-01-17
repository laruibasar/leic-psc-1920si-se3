/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */
#include <stdlib.h>
#include <string.h>

#include <json-c/json.h>

#include "books.h"

BOOK_url
google_books_get_urls(const char *apikey, const char *volumeId,
			   char *thumb_url, size_t thumb_maxlen,
			   char *pdf_url, size_t pdf_maxlen)
{
	BOOK_url book_url = URL_NO;
	char *uri, *encoded_uri;
	struct json_object *json;
	Volume vol;
	
	/* Construir o uri de pesquisa, com base na apikey e author */
	/* utilizado calloc, mesmo com overhead da inicializacao
	 */
	if ((uri = (char *) calloc(URI_MAX_SIZE, sizeof (char))) == NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return 0;
	}
	
	/* exemplo:
	 * https://www.googleapis.com/books/v1/volumes/volumeId&key=apikey
	 */
	set_query_string(API_URL, apikey, "?", "/", volumeId, uri);

	/* encode da string */
	encoded_uri = string_encode(uri);
	free(uri);
	if (encoded_uri == NULL)
		return URL_NO;

	/* executar a pesquisa e guardar o resultado */
	json = http_get_json_data(encoded_uri);
	free(encoded_uri);
	if (json == NULL) {
		json_object_put(json);
		return URL_NO;
	}

	create_volume(json, &vol);
	json_object_put(json);

	strncpy(thumb_url, vol.thumbnail, thumb_maxlen);
	strncpy(pdf_url, vol.pdf_link, pdf_maxlen);
	book_url = vol.url_available;

	clean_volume(&vol);
	
	return book_url;
}
