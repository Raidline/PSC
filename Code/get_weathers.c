#include <stdio.h>
#include <stdlib.h>
#include "weathers.h"

Weathers * get_weathers(Location * location, Date * date, int days){
	Weathers * days_weathers = malloc(sizeof(Weathers)*days);
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
	int i = 0;
	while(days_weathers[i].weather != NULL){
		free_hourly_weather(days_weathers[i].weather);
		++i;
	}
	free(days_weathers);
}