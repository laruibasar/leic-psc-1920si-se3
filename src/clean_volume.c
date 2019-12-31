/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */
#include <stdlib.h>

#include "books.h"

void
clean_volume(Volume *vol)
{
	free_elem(vol->pdf_link);
	free_elem(vol->thumbnail);
	free_elem(vol->isbn);
	free_elem(vol->publish_date);
	for (int i = 0; i < vol->total_authors; i++) {
		char *ptr = vol->author[i];
		free(&ptr);
	}
	free_elem(vol->title);
	free_elem(vol->volume_id);
}	

void
free_elem(void *elem)
{
	if (elem == NULL) {
		fprintf(stderr, "Nothing to clean");
		return;
	}
	free((char *) elem);
}
