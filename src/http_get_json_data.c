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

struct MemoryStruct {
	char *memory;
	size_t size;
};

struct json_object *
http_get_json_data(const char *uri)
{
	struct MemoryStruct chunck;
	enum json_tokener_error error;
	struct json_object *json = NULL;
	curl = curl_easy_init();

	if (curl == NULL) {
		fprintf(stderr, "Failed to setup connection");
		return NULL;
	}

	chunck.memory = malloc(1); /* aumentara conforme os dados recebidos */
	chunck.size = 0; /* sem dados atualmente */
	if (chunck.memory == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return NULL;
	}

	curl_easy_setopt(curl, CURLOPT_URL, uri);
	/* Define our callback to get called when there's data to be written */
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wmemory);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunck);

	/* we disable SSL verification for simplicity */
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

	response = curl_easy_perform(curl);
	if (response != CURLE_OK) {
		fprintf(stderr, "Failed to connect: %s\n",
			curl_easy_strerror(response));
		return NULL;
	} else {
		/* agora temos o chunck.memory com os dados recebidos
		 * da api com a dimensao chunck.size
		 */
		json = json_tokener_parse_verbose(chunck.memory, &error);
		if (error != json_tokener_success) {
			fprintf(stderr, "Failed to parse JSON: %s\n",
				json_tokener_error_desc(error));
			return NULL;
		}
	}

	free(chunck.memory);
	curl_easy_cleanup(curl);
	
	return json;
}

size_t
wmemory(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t realsize = size * nmemb;
	/* cria um ponteiro para os dados atuais ja recebidos */
	struct MemoryStruct *mem = (struct MemoryStruct *)userdata;
	
	/* define um ponteiro para uma zona de maior que permite alojar os
	 * dados ja recebidos, e os que vao ser guardados neste novo conjunto 
	 * recebidos.
	 */
	char *ch = realloc(mem->memory, mem->size + realsize + 1);
	if (ch == NULL) {
		/* out of memory */
		fprintf(stderr, "Not enough memory for data received.\n");
		return 0;
	}

	/* afecta o novo espaco no struct e copia o adiciona os dados
	 * recebidos
	 */
	mem->memory = ch;
	memcpy(&(mem->memory[mem->size]), ptr, realsize);

	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

