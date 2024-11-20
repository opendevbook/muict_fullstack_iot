#ifndef SENSER_H
#define SENSOR_H

#include "WeatherStation.h"
#include "TempHumid.h"

Weatherstation wn90lp(144, "weather", &Serial);
TempHumid soilnode1(2, "soiltemp", &Serial);

/*
 * sensors/  sensor.h
 */
void initData()
{
    wn90lp.init();
    wn90lp.reportValues();
    soilnode1.init();
}

void getDataSensor()
{
}
#endif