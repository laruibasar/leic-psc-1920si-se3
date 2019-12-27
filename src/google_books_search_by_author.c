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

int
google_books_search_by_author(const char *apikey, const char *author,
				Collection *res)
{
	int books = 0;
	char *uri, *encoded_uri;
	struct json_object *json, *total_items, *items;
	
	/* Construir o uri de pesquisa, com base na apikey e author */
	uri = malloc(URI_MAX_SIZE * sizeof(char));
	if (uri == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return -1;
	}

	strcat(uri, API_URL);
	strcat(uri, "volumes?q=inauthor:\"");
	strcat(uri, author);
	strcat(uri, "\"&key=");
	strcat(uri, apikey);

	/* encode da string */
	encoded_uri = malloc(URI_MAX_SIZE * sizeof(char));
	if (encoded_uri == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return -1;
	}
	string_encode(encoded_uri, uri);

	/* executar a pesquisa e guardar o resultado */
	json = http_get_json_data(encoded_uri);
	if (json == NULL)
		return -1;

	/* 
	 * afetar o resultado do numero de livros e construir a colecao
	 * caso esta nao seja vazia
	 */
	if (json_object_object_get_ex(json, "totalItems", &total_items)) {
		books = json_object_get_int64(total_items);
	}

	if (books > 0) {
		if (json_object_object_get_ex(json, "items", &items))
			printf("\nmuitos books\n");
	}

	/* magic envolved */
	free(encoded_uri);
	free(uri);
	free(items);
	free(total_items);
	free(json);

	return books;
}

void
string_encode(char *dst, char *src)
{
	char c = src[0];
	int j = 0;
	for (int i = 0; c != 0 && i < URI_MAX_SIZE; c = src[++i], ++j) {
		switch (c) {
			case '"':
				dst[j] = '%';
				dst[++j] = '2';
				dst[++j] = '2';
				break;
			case ' ':
				dst[j] = '%';
				dst[++j] = '2';
				dst[++j] = '0';
				break;
			default:
				dst[j] = c;
				break;
		}
	}
	dst[j] = 0;
}
