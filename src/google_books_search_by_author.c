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
	int books = 0, max_results = 10;
	char *uri, *encoded_uri;
	struct json_object *json, *total_items;
	
	/* Construir o uri de pesquisa, com base na apikey e author */
	/* utilizado calloc, mesmo com overhead da inicializacao
	 */
	if ((uri = (char *) calloc(URI_MAX_SIZE, sizeof (char))) == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return -1;
	}

	strcat(uri, API_URL);
	strcat(uri, "volumes?q=inauthor:\"");
	strcat(uri, author);
	strcat(uri, "\"&key=");
	strcat(uri, apikey);

	/* encode da string */
	encoded_uri = string_encode(uri);
	free(uri);

	if (encoded_uri == NULL)
		return -1;

	/* executar a pesquisa e guardar o resultado */
	json = http_get_json_data(encoded_uri);
	/*
	 * A melhorar e permitir a possibilidade de reaproveitar a encoded_uri
	 * para fazer novos pedidos, de modo a receber todos os livros
	 * resultantes da pesquisa
	 */
	free(encoded_uri);
	
	if (json == NULL)
		return -1;

	/* 
	 * afetar o resultado do numero de livros e construir a colecao
	 * caso esta nao seja vazia
	 */
	if (json_object_object_get_ex(json, "totalItems", &total_items)) {
		books = json_object_get_int64(total_items);
		free(total_items);
	}

	if (books > 0) {
		struct json_object* items;
		if (json_object_object_get_ex(json, "items", &items)) {
			printf("\nmuitos books: %d\n", max_results);
			json_object_to_file("items.txt", items);
		}
		free(items);
	}
	free(json);

	return books;
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
