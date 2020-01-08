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

void
create_volume(json_object *info, Volume *vol)
{
	char *volume_id = NULL,
	     *title = NULL,
	     *publish_date = NULL,
	     *isbn = NULL,
	     *thumbnail = NULL,
	     *pdf_link = NULL;
	int number_isbn = 0;
	size_t total_authors = 0;

	BOOK_url urls = URL_NO;
	struct json_object *id, *volume_info, *access_info;

	if (json_object_object_get_ex(info, "id", &id)) {
		json_strncpy(&volume_id, json_object_get_string(id),
				json_object_get_string_len(id));
	}
	vol->volume_id = volume_id;

	if (json_object_object_get_ex(info, "volumeInfo", &volume_info)) {
		struct json_object *js_title,
				   *js_authors,
				   *js_publish_date,
				   *js_industry,
				   *js_image;

		if (json_object_object_get_ex(volume_info, "title",
					&js_title)) {
			json_strncpy(&title,
				json_object_get_string(js_title),
				json_object_get_string_len(js_title));
		}
		
		if (json_object_object_get_ex(volume_info, "authors",
					&js_authors)) {
			size_t length = json_object_array_length(js_authors);
			char **author = NULL;
			if ((author = malloc(sizeof (char) * length)) == NULL) {
				fprintf(stderr,
					"Failed to allocate space to string\n");
				author = NULL;
			}

			for (size_t i = 0; i < length; i++) {
				struct json_object *js_author = 
					json_object_array_get_idx(js_authors, i);
				char *str = NULL;
				json_strncpy(&str, 
					json_object_get_string(js_author),
					json_object_get_string_len(js_author));
				author[i] = str;
			}

			total_authors = length;
			vol->author = author;
		} else 
			vol->author = NULL;

		if (json_object_object_get_ex(volume_info,
				"publishedDate", &js_publish_date)) {
			json_strncpy(&publish_date,
				json_object_get_string(js_publish_date),
				json_object_get_string_len(js_publish_date));
		}
		
		if (json_object_object_get_ex(volume_info,
				"industryIdentifiers", &js_industry)) {
			size_t length = json_object_array_length(js_industry);
			for (size_t i = 0; i < length; i++) {
				struct json_object *js_isbn =
					json_object_array_get_idx(js_industry, i);
				
				struct json_object *js_isbn_type, *js_isbn_nr;
				if (json_object_object_get_ex(
							js_isbn,
							"type",
							&js_isbn_type)) {
					const char *type = 
						json_object_get_string(js_isbn_type);
					
					if (strstr(type, "ISBN") != NULL) {
						json_strncpy(&isbn, type,
							json_object_get_string_len(js_isbn_type));
						
						json_object_object_get_ex(
								js_isbn,
								"identifier",
								&js_isbn_nr);
						number_isbn = 
							json_object_get_int(js_isbn_nr);
					} else
						isbn = "OTHER";
				}
			}
		}
		
		if (json_object_object_get_ex(volume_info, "imageLinks",
					&js_image)) {
			struct json_object *js_thumbnail;

			if (json_object_object_get_ex(js_image, "thumbnail",
					&js_thumbnail)) {
				size_t length = json_object_get_string_len(js_thumbnail);
				length = (length < THUMB_MAX_SIZE) ? length : THUMB_MAX_SIZE;
				json_strncpy(&thumbnail,
					json_object_get_string(js_thumbnail),
					length);
				urls = URL_THUMB;
			}
		}
	}
	vol->title = title;
	vol->total_authors = total_authors;
	vol->publish_date = publish_date;
	vol->isbn = isbn;
	vol->number_isbn = number_isbn;
	vol->thumbnail = thumbnail;
	
	if (json_object_object_get_ex(info, "accessInfo", &access_info)) {
		struct json_object *js_pdf;

		if (json_object_object_get_ex(access_info, "pdf", &js_pdf)) {
			struct json_object *js_link;
			if (json_object_object_get_ex(js_pdf, "downloadLink",
					&js_link)
			|| json_object_object_get_ex(js_pdf,"acsTokenLink",
						&js_link)) {
				size_t length = json_object_get_string_len(js_link);
				length = (length < PDF_MAX_SIZE) ? length : PDF_MAX_SIZE;
				json_strncpy(&pdf_link, 
					json_object_get_string(js_link),
					length);
				urls += URL_PDF;
			}
		}
	}
	vol->pdf_link = pdf_link;
	vol->url_available = urls;
}

size_t
json_strncpy(char **dst, const char *src, size_t length)
{
	char *str;
	if ((str = malloc(sizeof (char) * (length + 1))) == NULL) {
		fprintf(stderr, "Failed to allocate space to string");
		return 0;
	}
	strncpy(str, src, length + 1);
	*dst = str;
	return length + 1;
}
