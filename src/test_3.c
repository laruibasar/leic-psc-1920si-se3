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

#include <json-c/json.h>

#include "books.h"

int
main()
{
	init_curl();
	
	char *apikey = "AIzaSyAdM5Km30Z3x7I9vvn15akWjLd9lBnwVpU";
	char *author1 = "Camilo Castelo Branco";
	char *author2 = "Alexandre Herculano";

	Collection *col = NULL;

	printf("Teste funcao googleBooksSearchByAuthor\n");
	printf("Teste 1:\n");
	printf("Retorno teste1 nr: %d\n",
		google_books_search_by_author(apikey, author1, col));
	
	printf("Teste 2:\n");
	printf("Retorno nr: %d\n",
		google_books_search_by_author(apikey, author2, col));

	close_curl();
	return 0;
}
