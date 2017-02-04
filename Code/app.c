#include <stdio.h>
#include <string.h>
#include "weathers.h"

int main(int argc, char * argv[]){
	/*char * lat = strtok(argv[1],",");
	char * lon = strtok(NULL,",");
	Location location = {
		NULL,
		(float)atof(lat),
		(float)atof(lon)
	};
	char * y, * m, * d;
	y = strtok(argv[2],"-");
	m = strtok(NULL,"-");
	d = strtok(NULL,"-");
	Date date = {
		(unsigned int)atoi(y),
		(unsigned short)atoi(m),
		(unsigned short)atoi(d),
		0,
		0,
		0,
		0,
		0
	};
	unsigned int days = atoi(argv[3]);*/
	Location location = {
		NULL,
		37.019355,
		-7.93044
	};
	Date date = {
		2016,
		7,
		2,
		0,
		0,
		0,
		0,
		0
	};
	int days = 10;
	float scale=0;
	char temp = 'u';
	switch (temp){
		case 't':
			scale = 1.0;
			break;
		case 'u':
		case 'n':
			scale = 5.0;
			break;
		case 'v':
			scale = 0.5;
			break;
		default:
			return 1;
	}
	Weathers * weathers = get_weathers(&location,&date,days);
	float max = get_max_value(weathers,temp);
	float min = get_min_value(weathers,temp);
	for(float c=max ; c >= min ; c -= scale){
		printf("%03.1f| ",c);
		if(abs(c) < 10.0) printf(" ");
		for(int i = 0 ; i < days ; ++i){
			float aux = 0.0;
			float med = avg(weathers[i].weather,temp);
			for(int j=0 ; j < 24 ; ++j){
				switch (temp){
					case 't':
						aux = weathers[i].weather[j].temperature-32.0;
						aux *= 5/9.0;
						break;
					case 'v':
						aux = weathers[i].weather[j].wind * 1.6093 * 1000.0;
						aux /= 3600.0;
						break;
					case 'u':
						aux = weathers[i].weather[j].humidity * 100;
						break;
					case 'n':
						aux = weathers[i].weather[j].cloud * 100;
						break;
				}
				if(equals(med,c,scale)){
					printf("* ");
					break;
				}
				if(equals(aux,c,scale)){
					printf(": ");
					break;
				}
				if((j==23))
					printf("  ");
			}
		}
		printf("\n");
	}
	printf("    --");
	for(int i = 1 ; i <= days ; ++i){
		printf("--");
	}
	printf("\n");
	printf("      ");
	for(int i = 1; i <= days ; ++i){
		printf("%d ",i);
	}
	printf("\n");
	free_weathers(weathers);
	return 0;
}

float get_max_value(Weathers * weathers, char temp){
	float max = -999.0;
	float aux;
	for(int i=0 ; weathers[i].weather != NULL ; ++i){
		for(int j=0 ; j < 24 ; ++j){
			switch (temp){
				case 't':
					aux = weathers[i].weather[j].temperature-32.0;
					aux *= 5/9.0;
					break;
				case 'v':
					aux = weathers[i].weather[j].wind * 1.6093 * 1000.0;
					aux /= 3600.0;
					break;
				case 'u':
					aux = weathers[i].weather[j].humidity * 100;
					break;
				case 'n':
					aux = weathers[i].weather[j].cloud * 100;
					break;
			}
			if((aux > max) && (aux != 0.0))
				max = aux;
		}
	}
	return max;
}

float get_min_value(Weathers * weathers, char temp){
	float min = 999.0;
	float aux;
	for(int i=0 ; weathers[i].weather != NULL ; ++i){
		for(int j=0 ; j < 24 ; ++j){
			switch (temp){
				case 't':
					aux = weathers[i].weather[j].temperature-32.0;
					aux *= 5/9.0;
					break;
				case 'v':
					aux = weathers[i].weather[j].wind * 1.6093 * 1000.0;
					aux /= 3600.0;
					break;
				case 'u':
					aux = weathers[i].weather[j].humidity * 100;
					break;
				case 'n':
					aux = weathers[i].weather[j].cloud * 100;
					break;
			}
			if((aux < min) && (aux != 0.0))
				min = aux;
		}
	}
	return min;
}

int equals(float val, float cmp, float scale){
	if((val-cmp > -scale/2) && (val-cmp <= scale/2)) return 1;
	return 0;
}

float avg(Weather * weather, char temp){
	float c=0.0;
	float sum=0, aux;
	for(int j=0 ; j < 24 ; ++j){
		switch (temp){
				case 't':
					aux = weather[j].temperature-32.0;
					aux *= 5/9.0;
					if ( aux != 0.0){
						sum += aux;
						c++;
					}
					break;
				case 'v':
					aux = weather[j].wind * 1.6093 * 1000.0;
					aux /= 3600.0;
					if (aux != 0.0){
						sum += aux;
						c++;
					}
					break;
				case 'u':
					aux = weather[j].humidity * 100.0;
					if ( aux != 0.0){
						sum += aux;
						c++;
					}
					break;
				case 'n':
					aux = weather[j].cloud * 100.0;
					if (aux != 0.0){
						sum += aux;
						c++;
					}
					break;
			}
	}
	return (sum/c);
}