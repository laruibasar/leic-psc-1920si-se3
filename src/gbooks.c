/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include <json-c/json.h>

//#include "books.h"

void	usage();
void	set_html_header(FILE *fp);
void	set_html_body_total(FILE *fp, int total);
//void	set_html_body_volume(FILE *fp, Volume *volume);
void	set_html_footer(FILE *fp);

int
main(int argc, char *argv[])
{
	FILE *fp;
	char author[50];

	char *apikey = getenv("GBOOKS_API_KEY");
	if (apikey == NULL) {
		usage();
		return (-1);
	}

	if (argc != 2) {
		usage();
		return (-1);
	}
	strcpy((char *) author, argv[1]);
	printf("Author: %s\n", author);

	fp = fopen("test.html", "w");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open file to write.");
		return (-1);
	}

	set_html_header(fp);
	set_html_body_total(fp, 10);
	set_html_footer(fp);
	fclose(fp);
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
	fprintf(fp, "<tbody>\n");
	fprintf(fp, "<tr><td colspan=2>\n");
	fprintf(fp, "Total de volumes: %d\n", total);
	fprintf(fp, "</td></tr>\n");
}

void
set_html_footer(FILE *fp)
{
	fprintf(fp, "</tbody>\n");
	fprintf(fp, "</table>\n");
	fprintf(fp, "</body>\n");
	fprintf(fp, "</html>");
}
