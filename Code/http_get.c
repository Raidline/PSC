#include <stdio.h>
#include <curl/curl.h>

/* compilar o ficheiro  -> gcc http_get.c -o http_get -lcurl 
*/

int http_get(const char *uri, const char *filename);

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int main(){
	const char *uri = "http://imagem.band.com.br/f_198156.jpg";
	const char *filename = "figure.jpg";
	http_get(uri,filename);
	return 0;
}

int http_get(const char *uri, const char *filename){
	CURL *curl;
    CURLcode res;
	FILE *f;
	f = fopen(filename,"w");
	if (f == NULL){
		printf("Error opening file!\n");
		return 1;
	}
 
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, uri);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
		curl_easy_perform(curl);
		fclose(f);
		curl_easy_cleanup(curl);
		return 1;
	}
	curl_global_cleanup();
	return 0;
}
