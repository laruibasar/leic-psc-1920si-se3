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
	char *volumeID_1 = "G8MaDgAAQBAJ";
//	char *volumeID_2 = "3gEaDgAAQBAJ";
	char *thumbs = NULL, *pdf = NULL;

	printf("Teste funcao googleBooksGetUrls\n");
	printf("Teste 1:\n");
	printf("Retorno teste1 nr: %d\n",
		google_books_get_urls(apikey, volumeID_1,
			thumbs, THUMB_MAX_SIZE, pdf, PDF_MAX_SIZE));
	
	close_curl();
	return 0;
}
