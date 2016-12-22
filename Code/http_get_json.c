#include <stdio.h>
#include <jansson.h>
#include <curl/curl.h>
#include <string.h>

#define BUFFER_SIZE (32 * 1024)
#define API_KEY "660f829a6b5911945807f2117204a3ed"
#define FILE_NAME "debug.txt"

json_t * http_get_json_data(const char *uri);
char * request(const char *uri);
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

struct write_request{ // foi necessario criar uma estrutura para que sempre que fosse chamada a função de callback do curl
    char *data;       // o conteúdo da API seria escrito no sitio correto da String ???
    int pos;
};

int main(){
	const char *uri = "https://api.forecast.io/forecast/"API_KEY"/38.722252,-9.139337";
	json_t * root;
	root = http_get_json_data(uri);
	return 0;
}

json_t * http_get_json_data(const char *uri){ // função que descodifica os dados contidos na string usando o json
	static char *coded = NULL;
	json_t *root;
	json_error_t error;
	coded = request(uri);
	if(!coded) return NULL;
	root = json_loads(coded,0,&error);
	free(coded);  // libertar a memoria alocada dinamicamente
	if(!root){
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return NULL;
	}
    return root;
}

char * request(const char *uri){ // função que faz o pedido http usando a livraria curl
	static char *coded = NULL; // string que contém os dados do pedido http que posteriormente será usado pelo json
	coded = (char *)malloc(BUFFER_SIZE);
	if(!coded) return NULL;
	CURL *curl;
    CURLcode res;
    struct write_request write_req = {
		.data = coded,
		.pos = 0
	};
    FILE *f;
	f = fopen(FILE_NAME,"w");  // O FICHEIRO SÓ SERVE PRA FAZER DEBUG ( VERIFICAR SE OS DADOS TÃO CORRETOS )
	if (f == NULL){
		printf("Error opening file!\n");
		return NULL;
	}
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, uri);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_req);
		res = curl_easy_perform(curl);
		if(res != 0){
			fprintf(stderr, "error: unable to request data from %s:\n", uri);
			fprintf(stderr, "%s\n", curl_easy_strerror(res));
			return NULL;
		}
		curl_easy_cleanup(curl);
	}
	fputs(coded,f);
	fclose(f);
	curl_global_cleanup();
	coded[write_req.pos] = '\0';  // os dados do curl não vem com a string terminada por '\0'
	return coded;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){ // função callback do curl ( guarda na estrutura os dados do pedido http )
  struct write_request * write_req = (struct write_request*) stream;
  if(write_req->pos + size * nmemb >= BUFFER_SIZE-1){
	fprintf(stderr,"error : buffer size too small\n");
	return 0;
  }
  memcpy(write_req->data + write_req->pos, ptr, size * nmemb);
  write_req->pos += size *nmemb;
  return size * nmemb;
}