#include <stdio.h>
#include <stdlib.h>

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

typedef struct {
	Weather * weather;
}Weathers;

Weather * get_hourly_weather(Location *, Date *);
Weathers * get_weathers(Location *, Date *, int);
void free_weathers(Weathers *);

int main(){
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
	Weathers * weathers = get_weathers(location,date,3);
	free_weathers(weathers);
	return 0;
}

Weathers * get_weathers(Location * location, Date * date, int days){
	Weathers * days_weathers = malloc(sizeof(Weathers));
	days_weathers->weather = malloc(sizeof(*days_weathers->weather)*days);
	if(days == 0) return NULL;
	for(int i = 0 ; i < days ; ++i){
		days_weathers[i].weather = get_hourly_weather(location,date);
		if(date->day + 1 > 30){
			if(date->month + 1 > 12){
				date->year += 1;
				date->month = 1;
			}
			else {
				date->month += 1;
			}
			date->day = 1;
		}
		else date->day += 1;
	}
	return days_weathers;
}

void free_weathers(Weathers * days_weathers){
	if(days_weathers->weather != NULL){
		free(days_weathers->weather);
		free(days_weathers);
	}
}