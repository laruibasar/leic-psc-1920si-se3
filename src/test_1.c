/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <curl/curl.h>

#include "books.h"

int
main()
{
	init_curl();
	
	printf("Teste funcao httpGetToFile\n");
	printf("Teste 1:\n");
	printf("Envio de uri: https://www.googleapis.com/books/v1/volumes\n");
	printf("Escrita para ficheiro teste1_t1.txt\n");
	printf("Ficheiro deve ter formato JSON com linha 2:\t error:\n");
	char *uri = "https://www.googleapis.com/books/v1/volumes";
	char *file = "teste1_t1.txt";
	if (http_get_to_file(uri, file) == 0) 
		printf("Sucesso");
	else 
		printf("Falha");
	
	printf("\n\nTeste 2:\n");
	printf("Envio de uri: https://www.googleapis.com/books/v1/volumes?q=inauthor:Doyle\n");
	printf("Escrita para ficheiro teste1_t2.txt\n");
	printf("Ficheiro deve ter formato JSON com linha 2:\t kind:\n");
	char *uri2 = "https://www.googleapis.com/books/v1/volumes?q=inauthor:Doyle";
	char *file2 = "teste1_t2.txt";
	if (http_get_to_file(uri2, file2) == 0) 
		printf("Sucesso");
	else 
		printf("Falha");
	
	printf("\n\nTeste 3:\n");
	printf("Envio de uri: https://www.homestudio.pt\n");
	printf("Escrita para ficheiro teste1_t3.txt\n");
	printf("Deve reportar erro:\n");
	char *uri3 = "https://www.homestudio.pt";
	char *file3 = "teste1_t3.txt";
	if (http_get_to_file(uri3, file3) == 0) 
		printf("Sucesso");
	else 
		printf("Falha");
	
	close_curl();
	return 0;
}
