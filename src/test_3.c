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

void	print_volume(Volume *volume);

int
main()
{
	init_curl();
	
	char *apikey = "AIzaSyAdM5Km30Z3x7I9vvn15akWjLd9lBnwVpU";
	char *author1 = "Camilo Castelo Branco";
	char *author2 = "Alexandre Herculano";
	char *author3 = "\"Alexandre Herculano\"";

	Collection col1;
	col1.total = 0;
	col1.volumes = NULL;

	printf("Teste funcao googleBooksSearchByAuthor\n");
	printf("Teste 1:\n");
	printf("Retorno teste1 nr: %d\n",
		google_books_search_by_author(apikey, author1, &col1));
	printf("Tamanho: %zu\n", col1.total);
	Volume *vol = col1.volumes;
	for (int i = 0; i < col1.total; i++) {
		print_volume(vol++);
	}
	
	Collection col2;
	col2.total = 0;
	col2.volumes = NULL;
	printf("Teste 2:\n");
	printf("Retorno nr: %d\n",
		google_books_search_by_author(apikey, author2, &col2));
	Volume *vol2 = col2.volumes;
	for (int i = 0; i < col2.total; i++) {
		print_volume(vol2++);
	}
	
	Collection col3;
	col3.total = 0;
	col3.volumes = NULL;
	printf("Teste 3:\n");
	printf("Retorno nr: %d\n",
		google_books_search_by_author(apikey, author3, &col3));
	Volume *vol3 = col3.volumes;
	for (int i = 0; i < col3.total; i++) {
		print_volume(vol3++);
	}
	
	close_curl();
	return 0;
}

void
print_volume(Volume *volume)
{
	printf("\n\tVolume: %s\n", volume->volume_id); 
	printf("Titulo: %s\n", volume->title);
	printf("Autor(es): "); 
	for (int i = 0; i < volume->total_authors; i++) 
		printf("%s ", volume->author[i]);

	printf("\nData publicacao: %s\n", volume->publish_date);
	printf("%s : %d\n", volume->isbn, volume->number_isbn);
	printf("Thumbnail: %s\n", volume->thumbnail);
	printf("PDF: %s\n", volume->pdf_link);
}
