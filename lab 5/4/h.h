#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//  THE FOLLOWING 3 LINES ARE REQUIRED TO GET ACCESS TO  M_PI  UNDER Linux
#if     defined(__linux__)
#define __USE_XOPEN
#endif

#include <math.h>

//  written by Chris.McDonald@uwa.edu.au
//
//  compile:  cc -std=c99 -Wall -Werror -o haversine haversine.c -lm
//  run:      ./haversine -31.977537 115.816509 -31.982831 115.818898

#define EARTH_RADIUS_IN_METRES          6372797

#define	MIN_LAT				-90.0
#define	MAX_LAT				 90.0
#define	MIN_LON				-180.0
#define	MAX_LON				 180.0

double degrees_to_radians(double);
double radians_to_degrees(double);
bool valid_location(double , double );
double haversine(double lat1, double lon1, double lat2, double lon2);

