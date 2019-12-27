/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Luis Bandarra <luis.bandarra@homestudio.pt>
 * All rights reserved.
 */

#ifndef _BOOKS_H_
#define _BOOKS_H_

#include <curl/curl.h>
#include <json-c/json.h>

/*
 * Tipo de retorno possivel para utilizacao de URL
 */
typedef enum {
	URL_NO = 0,
	URL_THUMB,
	URL_PDF,
	URL_ALL
} BOOK_url;

/*
 * Tipo de dados que permite guardar a informacao sobre os livros
 * obtidos do servico Books API.
 */
typedef struct volume {
	char *identifier;
	char *title;
	char *ISBN;
	char *publish_date;
	int pdf_available;
} Volume;

/*
 * Tipo de dados que permite guardar a informacao sobre grupo de
 * volumes.
 */
typedef struct collection {
	size_t total;
	Volume *volumes;
} Collection;

/* global CURL para utilizacao na biblioteca */
extern CURL *curl;
extern CURLcode response;

/* definicao do uri da api, no futuro carregar de algum ficheiro */
extern char *api_url = "https://www.googleapis.com/books/v1/";

/*
 * Faz o inicializacao global da library curl, como indicado na especificacao.
 */
int 	init_curl(void);

/*
 * Faz a limpeza global da library curl, como indicado na documentacao.
 */
void 	close_curl(void);

/*
 * Realiza pedido HTTP GET ao uri especificado e armazena o resultado no 
 * fichero filename.
 * Devolve 0 e 1 para indicar sucesso e erro.
 */
int	http_get_to_file(const char *uri, const char *filename);

/*
 * Faz a escrita para o ficheiro, atraves de callback nas opcoes do curl
 */
size_t	write_to_file(char *ptr, size_t size, size_t nmemb, void *userdata);

/*
 * Realiza pedido HTTP GET ao uri especificado, do tipo application/json,
 * e retorna o ponteiro para struct json_object com o conteudo da resposta.
 * Devolve NULL em caso de erro e imprime no stderr a mensagem de erro.
 */
struct json_object*	http_get_json_data(const char *uri);

/*
 * Faz a escrita em memoria dos dados recebidos, atraves de callback.
 * Atencao que necessita de algum cuidado por nao ser garantido
 * pela libcurl dos dados serem enviados completos numa so pedido,
 * como indicado na documentacao da biblioteca.
 *
 * man 3 CURLOPT_WRITEFUNCTION
 * https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 */
size_t	write_to_buffer(char *ptr, size_t size, size_t nmemb, void *userdata);

/*
 * Realiza uma pesquisa por volumes com base no nome do author. No uri usado
 * deve constar a api key (key=apikey) e os query parameters na forma
 * q=query_paramenters.
 * No uri, os argumentos sao iniciados pelo char ? e separados pelo char &,
 * com os espacos codificados por %20 e aspas por %22.
 * Retorna o numero de volumes obtidos ou -1 no caso de erro. No caso de
 * sucesso, a funcao retorna atraves do argumento res, uma colecao de 
 * instancias do tipo Volume com o resultado da pesquisa.
 */
int 	google_books_search_by_author(const char *apikey, const char *author,
				 Collection *res);

/*
 * Retorna nos parametros pdf_url e thumb_url, os URLs onde se pode fazer o
 * download do PDF e da imagem do livro. O livro e identificado atraves do
 * parametro volumeID.
 * Retorna um inteiro que indica se e possivel obter os URLs.
 */
BOOK_url google_books_get_urls(const char *apikey, const char *volumeId,
			   char *thumb_url, size_t thumb_maxlen,
			   char *pdf_url, size_t pdf_maxlen);

#endif
