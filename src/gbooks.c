/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "books.h"

void	usage();
void	set_html_header(FILE *fp);
void	set_html_body_total(FILE *fp, int total);
void	set_html_body_volume(FILE *fp, Volume *volume);
void	set_html_footer(FILE *fp);
void	swap_whitespace(char *str);

int
main(int argc, char *argv[])
{
	FILE *fp;
	char *prefix = "book_";
	char *file_type = ".html";
	/* 
	 * The filename format is: books_authorname.html
	 * The lenght will allways be >11
	 */
	size_t file_length = 11; 
	int books;

	/* per libbooks and curl */
	init_curl();

	Collection collection;
	collection.total = 0;
	collection.volumes = NULL;

	char *apikey = getenv("GBOOKS_API_KEY");
	if (apikey == NULL) {
		usage();
		return (-1);
	}

	if (argc != 2) {
		usage();
		return (-1);
	}

	size_t size = strlen(argv[1]) + 1;
	char *author = malloc(size);
	if (author == NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return (-1);
	}
	strcpy(author, argv[1]);
	printf("Author: %s\n", author);

	/* create filename and prepare to open file handler */
	file_length += size;
	char *filename = malloc(file_length);
	if (filename == NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return (-1);
	}
	strcpy(filename, prefix);
	strcat(filename, author);
	strcat(filename, file_type);
	printf("Filename: %s\n", filename);
	
	fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open file to write.");
		return (-1);
	}
	
	books = google_books_search_by_author(apikey, author, &collection);
	if (books < 0) {
		printf("Error in getting information\nPlease try again.");
		return (-1);
	}

	set_html_header(fp);
	set_html_body_total(fp, books);
	Volume *volume = collection.volumes;
	/* current limitation of only max of 10 books */
	books = (books > 10) ? 10 : books;
	while (books-- > 0)
		set_html_body_volume(fp, volume++);

	set_html_footer(fp);
	fclose(fp);
	free(filename);
	free(author);

	/* curl cleanup */
	close_curl();
	
	return 0;
}

void
usage()
{
	printf("Usage: gbooks autor\n");
	printf("\t\ta variavel de ambiente GBOOKS_API_KEY.\n");
	printf("autor\t\tnecessario para fazer a pesquisa. Pode ter espacos, \n");
	printf("\t\tmas nesse caso devem colocar aspas para limitar a pesquisa.\n");
	printf("\t\tDeve definir a variavel ambiente GBOOKS_API_KEY para um\n");
	printf("\t\tvalor, exemplo: AIzaSyAdM5Km30Z3x7I9vvn15akWjLd9lBnwVpU\n");
}

void
swap_whitespace(char *str)
{
	char *ch = str;
	while (*ch != '\0')
		if (*ch == ' ')
			*ch = '_';
}

void set_html_header(FILE *fp)
{
	fprintf(fp, "<!DOCTYPE html>\n");
	fprintf(fp, "<html lang=\"pt\">\n");
	fprintf(fp, "<header>\n");
	fprintf(fp, "<meta charset=\"utf-8\">\n");
	fprintf(fp, "<title>Apresentacao de volumes</title>\n");
	fprintf(fp, "</header>\n");
	fprintf(fp, "<body>\n");
}

void
set_html_body_total(FILE *fp, int total)
{
	fprintf(fp, "<table>\n");
	fprintf(fp, "<thead>\n");
	fprintf(fp, "<tr><td colspan=2>\n");
	fprintf(fp, "Total: %d (So apresenta max 10)\n", total);
	fprintf(fp, "</td></tr>\n");
	fprintf(fp, "</thead><tbody>\n");

}

void
set_html_body_volume(FILE *fp, Volume *volume)
{
	if (volume->thumbnail != NULL)
		fprintf(fp, "<tr><td rowspan=\"6\"><img src=\"%s\"></td></tr>",
				volume->thumbnail);

	fprintf(fp, "<tr><td>Id: %s</td></tr>", volume->volume_id);
	fprintf(fp, "<tr><td>Titulo: %s</td></tr>", volume->title);
	fprintf(fp, "<tr><td>Autores:");
	for (int i = 0; i < volume->total_authors; i++)
		fprintf(fp, " %s ", volume->author[i]);

	fprintf(fp, "</td></tr>");
	fprintf(fp, "<tr><td>Publicacao: %s</td></tr>",
			volume->publish_date);

	if (volume->url_available == URL_PDF ||
	    volume->url_available == URL_ALL)
		fprintf(fp, "<tr><td>PDF: <a href=\"%s\">Link PDF</a></td></tr>", volume->pdf_link);
	else
		fprintf(fp, "<tr><td>PDF: sem link disponivel</td></tr>");
}

void
set_html_footer(FILE *fp)
{
	fprintf(fp, "</tbody>\n");
	fprintf(fp, "</table>\n");
	fprintf(fp, "</body>\n");
	fprintf(fp, "</html>");
}
