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
	struct json_object *json, *total_items;
	
	/* Construir o uri de pesquisa, com base na apikey e author */
	/* utilizado calloc, mesmo com overhead da inicializacao
	 */
	if ((uri = (char *) calloc(URI_MAX_SIZE, sizeof (char))) == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return -1;
	}

	/* exemplo:
	 * https://www.googleapis.com/books/v1/volumes
	 * ?q=inauthor:"author"&key=apikey
	 */
	set_query_string(API_URL, apikey, "&", "?q=inauthor:", author, uri);

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
	 * resultantes da pesquisa, o que sugere fazer o free depois.
	 */
	free(encoded_uri);
	if (json == NULL) {
		json_object_put(json);
		return -1;
	}
	/* 
	 * afetar o resultado do numero de livros e construir a colecao
	 * caso esta nao seja vazia
	 */
	if (json_object_object_get_ex(json, "totalItems", &total_items)) {
		books = json_object_get_int64(total_items);
	}

	/* 
	 * Como temos livros, tem de percorrer o array de resultados.
	 */
	if (books > 0) {
		Volume *volumes[books];
		struct json_object* items;
		if (json_object_object_get_ex(json, "items", &items)) {
			json_object_to_file("items.txt", items);

			size_t length = json_object_array_length(items);
			for (size_t i = 0; i < length; i++) {
				struct json_object *item = 
					json_object_array_get_idx(items, i);
				Volume vol;
				create_volume(item, &vol);
				volumes[i] = &vol;
			}
			/* 
			 * Afetamos a collection com os valores to tamanho
			 * e o apontador para o endereco do array de ponteiros
			 * dos volumes
			 */
			res->total = length;
			res->volumes = (Volume *) volumes; 
		}
		printf("\n\tVolume: %s\n", volumes[0]->volume_id); 
		printf("Titulo: %s\n", volumes[0]->title);
		printf("Autor(es): "); 
		for (int i = 0; i < volumes[0]->total_authors; i++) {
			char *str = volumes[0]->author[i];
			printf("%p ", str);
		}
		printf("\nData publicacao: %s\n", volumes[0]->publish_date);
		printf("%s : %d\n", volumes[0]->isbn, volumes[0]->number_isbn);
		printf("Thumbnail: %s\n", volumes[0]->thumbnail);
		printf("PDF: %s\n", volumes[0]->pdf_link);
	}
json_object_put(json);

	return books;
}
