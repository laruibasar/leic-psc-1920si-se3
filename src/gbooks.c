/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */
#include <stdio.h>
#include <dlfcn.h>

#include <json-c/json.h>

#include "books.h"

int
main()
{
	init_curl();
	
	close_curl();
	return 0;
}
