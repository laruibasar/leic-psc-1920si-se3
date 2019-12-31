/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */
#include <stdio.h>
#include <dlfcn.h>

#include <json-c/json.h>

//#include "books.h"

void	set_html_header(FILE *fp);
void	set_html_body_total(FILE *fp, int total);
//void	set_html_body_volume(FILE *fp, Volume *volume);
void	set_html_footer(FILE *fp);

int
main()
{
	FILE *fp;

	fp = fopen("test.html", "w");
	set_html_header(fp);
	set_html_body_total(fp, 10);
	set_html_footer(fp);
	fclose(fp);
	return 0;
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
