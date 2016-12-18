#include <stdio.h>
#include <jansson.h>
#include <curl/curl.h>
#include <string.h>

json_t * http_get_json_data(const char *uri);
char * request(const char *uri);
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

int main(){
	const char *uri = "https://www.googleapis.com/plus/v1/people/+DeWittClinton";
	json_t * root;
	root = http_get_json_data(uri);
}

json_t * http_get_json_data(const char *uri){ // função que descodifica os dados contidos na string usando o json
	char *coded;
	json_t *root;
	json_error_t error;
	coded = request(uri);
	if(!coded) return NULL;
	root = json_loads(coded,0,&error);
	if(!root){
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return NULL;
	}
    return root;
}

char * request(const char *uri){ // função que faz o pedido http usando a livraria curl
	char *toReturn; // string que contém os dados do pedido http que posteriormente será usado pelo json
	CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, uri);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, toReturn);
		res = curl_easy_perform(curl);
		if(res != 0){
			fprintf(stderr, "error: unable to request data from %s:\n", uri);
			fprintf(stderr, "%s\n", curl_easy_strerror(res));
			return NULL;
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return toReturn;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){ // função callback do curl ( guarda na string o valor do pedido http )
  char *data = (char *)stream;
  memcpy(data, ptr, size * nmemb);
  *data += '\0';
  return size * nmemb;
}
