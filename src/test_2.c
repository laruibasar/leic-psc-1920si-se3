/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <curl/curl.h>
#include <json-c/json.h>

#include "books.h"

int
main()
{
	init_curl();
	
	printf("Teste funcao httpGetJsonData\n");
	printf("Teste 1:\n");
	printf("Envio de uri: https://www.googleapis.com/books/v1/volumes\n");
	printf("Escrita para ficheiro teste2_t1.txt o resultado json\n");
	char *uri = "https://www.googleapis.com/books/v1/volumes";
	struct json_object *root = http_get_json_data(uri);
	if (root == NULL)
		printf("Erro na rececao e/ou tratemento json\n");
	
	json_object_to_file("teste2_t1.txt", root);

	printf("\n\nTeste 2:\n");
	printf("Envio de uri: https://www.googleapis.com/books/v1/volumes?q=inauthor:Doyle\n");
	printf("Escrita para ficheiro teste2_t2.txt o resultado\n");
	printf("Ficheiro deve ter formato JSON com linha 2:\t kind:\n");
	char *uri2 = "https://www.googleapis.com/books/v1/volumes?q=inauthor:Doyle";
	struct json_object *root2 = http_get_json_data(uri2);
	if (root2 == NULL)
		printf("Erro na rececao e/ou tratemento json\n");
	
	json_object_to_file("teste2_t2.txt", root2);

	printf("\n\nTeste 3:\n");
	printf("Envio de uri: https://www.homestudio.pt\n");
	printf("Deve reportar erro:\n");
	char *uri3 = "https://www.homestudio.pt";
	struct json_object *root3 = http_get_json_data(uri3);
	if (root3 == NULL) {
		printf("Erro na rececao e/ou tratemento json\n");
	} else {
		json_object_to_file("teste2_t3.txt", root3);
	}

	close_curl();
	return 0;
}
