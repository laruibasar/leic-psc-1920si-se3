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
	
	printf("Teste funcao googleBooksSearchByAuthor\n");
	printf("Teste 1:\n");
	printf("Envio de uri: https://www.googleapis.com/books/v1/volumes\n");
	printf("Escrita para ficheiro teste2_t1.txt o resultado json\n");
	
	close_curl();
	return 0;
}
