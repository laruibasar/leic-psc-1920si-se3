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

#include <curl/curl.h>
#include <json-c/json.h>

#include "books.h"

struct BufferResponse {
	char *buffer;
	size_t size;
};

struct json_object *
http_get_json_data(const char *uri)
{
	struct BufferResponse res;
	enum json_tokener_error error;
	struct json_object *json = NULL;
	struct curl_slist *header = NULL;
	curl = curl_easy_init();

	if (curl == NULL) {
		fprintf(stderr, "Failed to setup connection");
		return NULL;
	}

	header = curl_slist_append(header, "Content-Type: application/json");
	
	/* aumentara conforme os dados recebidos */
	res.buffer = malloc(1); 
	/* sem dados atualmente */
	res.size = 0; 
	if (res.buffer == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return NULL;
	}

	curl_easy_setopt(curl, CURLOPT_URL, uri);
	/* Define our callback to get called when there's data to be written */
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_buffer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&res);

	/* we disable SSL verification for simplicity */
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

	response = curl_easy_perform(curl);
	if (response != CURLE_OK) {
		fprintf(stderr, "Failed to connect: %s\n",
			curl_easy_strerror(response));
		return NULL;
	} else {
		/* agora temos a resposta em res com os dados recebidos
		 * da api e a sua dimensao 
		 */
		json = json_tokener_parse_verbose(res.buffer, &error);
		if (error != json_tokener_success) {
			fprintf(stderr, "Failed to parse JSON: %s\n",
				json_tokener_error_desc(error));
			return NULL;
		}
	}

	free(res.buffer);
	curl_easy_cleanup(curl);
	curl_slist_free_all(header);
	
	return json;
}

size_t
write_to_buffer(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t realsize = size * nmemb;
	/* Cria um ponteiro para os dados atuais
	 * por receber ou ja recebidos
	 */
	struct BufferResponse *mem = (struct BufferResponse *)userdata;
	
	/* Define um ponteiro para uma zona de maior espaco que permite alojar
	 * os dados ja recebidos, e os que vao ser guardados neste novo
	 * conjunto recebidos.
	 */
	char *ch = realloc(mem->buffer, mem->size + realsize + 1);
	if (ch == NULL) {
		/* Out of memory */
		fprintf(stderr, "Not enough memory for data received.\n");
		return 0;
	}

	/* Afecta o novo espaco no struct e copia o adiciona os dados
	 * recebidos
	 */
	mem->buffer = ch;
	memcpy(&(mem->buffer[mem->size]), ptr, realsize);

	mem->size += realsize;
	mem->buffer[mem->size] = 0;

	return realsize;
}

