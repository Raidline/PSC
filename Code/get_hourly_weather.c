#include <stdio.h>
#include <jansson.h>
#include <string.h>

#define API_KEY "660f829a6b5911945807f2117204a3ed"
#define URI "https://api.forecast.io/forecast/"API_KEY"/"

typedef struct {
	float temperature;
	float wind;
	float humidity;
	float cloud;
} Weather;

typedef struct {
	const char *name;
	float latitude;
	float longitude;
} Location;

typedef struct {
	unsigned int year;
	unsigned short month;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
	short deviation_hour;
	unsigned short deviation_minute;
} Date;

Weather * get_hourly_weather(Location *, Date *);
void free_hourly_weather(Weather *);
json_t * http_get_json_data(const char *uri);	

/*int main(){
	const char * name = "Lisboa";
	Location *location = &(Location){
		.name = name,
		.latitude = 38.722252,
		.longitude = -9.139337
	};
	Date *date = &(Date){
		2016,
		10,
		24,
		00,
		00,
		00,
		00,
		00
	};
	Weather * weather;
	weather = get_hourly_weather(location,date);
	free_hourly_weather(weather);
	return 0;
}*/

Weather * get_hourly_weather(Location * location, Date * date){
	char * time_example = "2016-10-24T10:10:10-0400"; 
	char * latitude;
	char * longitude;
	char * time;
	char * aux;
	char *uri;
	latitude = (char *)malloc(10);
	longitude = (char *)malloc(10);
	time = (char *)malloc(25);
	*time = '\0';
	aux = (char *)malloc(4);
	uri = (char *)malloc(150);
	sprintf(latitude,"%2.8f,",location->latitude);
	sprintf(longitude,"%2.8f,",location->longitude);
	sprintf(aux, "%04u-", date->year);
	strcat(time, aux);
	sprintf(aux, "%02u-", date->month);
	strcat(time, aux);
	sprintf(aux, "%02uT", date->day);
	strcat(time, aux);
	sprintf(aux, "%02u:", date->hour);
	strcat(time, aux);
	sprintf(aux, "%02u:", date->minute);
	strcat(time, aux);
	sprintf(aux, "%02u", date->second);
	strcat(time, aux);
	if(date->deviation_hour >= 0)
		sprintf(aux, "+%02d", date->deviation_hour);
	else sprintf(aux, "%02d", date->deviation_hour);
	strcat(time, aux);
	sprintf(aux, "%02u", date->deviation_minute);
	strcat(time, aux);
	
	strcpy(uri,URI);
	strcat(uri,latitude);
	strcat(uri, longitude);
	strcat(uri, time);
	json_t * root;
	root = http_get_json_data(uri);
	free(aux);
	free(latitude);
	free(longitude);
	free(uri);
	free(time);
	if(!json_is_object(root)){
		fprintf(stderr, "root is not an object");
		json_decref(root);
		return NULL;
	}
	json_t * hourly = json_object_get(root, "hourly");
	if(!json_is_object(hourly)){
		fprintf(stderr, "hourly is not an object");
		json_decref(root);
		return NULL;
	}
	json_t * data = json_object_get(hourly, "data");
	if(!json_is_array(data)){
		fprintf(stderr, "data is not an array");
		json_decref(root);
		return NULL;
	}
	
	Weather * weather = malloc(sizeof(Weather)*24);
	json_t * hour_data, *temp, *wind, *humidity, *cloud;
	
	for(int i = 0 ; i < json_array_size(data) ; ++i){
		hour_data = json_array_get(data,i);
		if(!json_is_object(hour_data)){
			fprintf(stderr, "hourly_data is not an object");
			json_decref(root);
			return NULL;
		}
		
		
		temp = json_object_get(hour_data,"temperature");
		wind = json_object_get(hour_data,"windSpeed");
		humidity = json_object_get(hour_data,"humidity");
		cloud = json_object_get(hour_data,"cloudCover");
		
		weather[i].temperature = (float)json_real_value(temp);
		weather[i].wind = (float)json_real_value(wind);
		weather[i].humidity = (float)json_real_value(humidity);
		weather[i].cloud = (float)json_real_value(cloud);
	}
	return weather;
}

void free_hourly_weather(Weather * weather){
	if(weather != NULL){
		free(weather);
	}
}