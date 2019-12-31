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
		volume_id = json_to_string(json_object_get_string(id),
					json_object_get_string_len(id));
	}
	vol->volume_id = volume_id;

	if (json_object_object_get_ex(info, "volumeInfo", &volume_info)) {
		struct json_object *js_title, *js_author, *js_publish_date,
			*js_industry, *js_image;
		if (json_object_object_get_ex(volume_info,
				"title", &js_title)) {
			title = json_to_string(
					json_object_get_string(js_title),
					json_object_get_string_len(js_title));
		}
		
		if (json_object_object_get_ex(volume_info,
				"authors", &js_author)) {
			size_t length = json_object_array_length(js_author);
			char *author[length];
			for (size_t i = 0; i < length; i++) {
				struct json_object *js_authors = 
					json_object_array_get_idx(js_author, i);
				char *str = json_to_string(
						json_object_get_string(js_authors),
						json_object_get_string_len(js_authors));
				author[i] = str;
			}
			total_authors = length;
			vol->author = author;
		} else 
			vol->author = NULL;

		if (json_object_object_get_ex(volume_info,
				"publishedDate", &js_publish_date)) {
			publish_date = json_to_string(
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
						isbn = json_to_string(type,
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
		
		if (json_object_object_get_ex(volume_info,
				"imageLinks", &js_image)) {
			struct json_object *js_thumbnail;
			if (json_object_object_get_ex(
					js_image, 
					"thumbnail",
					&js_thumbnail)) {
				int len = json_object_get_string_len(js_thumbnail);
				thumbnail = json_to_string(
						json_object_get_string(js_thumbnail),
						(len > THUMB_MAX_SIZE)
							? THUMB_MAX_SIZE
							: len);
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
			if (json_object_object_get_ex(
					js_pdf,
					"downloadLink",
					&js_link)
			|| json_object_object_get_ex(
						js_pdf,
						"acsTokenLink",
						&js_link)) {
				int len = json_object_get_string_len(js_link);
				pdf_link = json_to_string(
						json_object_get_string(js_link),
						(len > PDF_MAX_SIZE)
							? PDF_MAX_SIZE
							: len);
				urls += URL_PDF;
			}
		}
	}
	vol->pdf_link = pdf_link;
	vol->url_available = urls;
}

char *
json_to_string(const char *src, size_t size)
{
	char *str;
	if ((str = calloc(size + 1, sizeof (char))) == NULL) {
		fprintf(stderr, "Failed to allocate space for string");
		return (NULL);
	}
	strlcpy(str, src, size + 1);
	return str;
}
