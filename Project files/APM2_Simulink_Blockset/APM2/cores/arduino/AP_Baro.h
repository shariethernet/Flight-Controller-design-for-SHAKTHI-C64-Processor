/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#ifndef __AP_BARO_H__
#define __AP_BARO_H__

#include <AP_Param.h>
#include <Filter.h>
#include <DerivativeFilter.h>
#include "AP_PeriodicProcess.h"

class AP_Baro
{
    public:
	bool healthy;
	AP_Baro() {}
	virtual bool    init(AP_PeriodicProcess *scheduler)=0;
	virtual uint8_t read() = 0;
	virtual float get_pressure() = 0;
	virtual float get_temperature() = 0;
	
	virtual int32_t get_raw_pressure() = 0;
	virtual int32_t get_raw_temp() = 0;

    // calibrate the barometer. This must be called on startup if the
    // altitude/climb_rate/acceleration interfaces are ever used
    // the callback is a delay() like routine
    void calibrate(void (*callback)(unsigned long t));

    // get current altitude in meters relative to altitude at the time
    // of the last calibrate() call
    float get_altitude(void);

    // return how many pressure samples were used to obtain
    // the last pressure reading
    uint8_t get_pressure_samples(void) { return _pressure_samples; }

    // get current climb rate in meters/s. A positive number means
    // going up
    float get_climb_rate(void);

    // the ground values are only valid after calibration
    float get_ground_temperature(void) { return _ground_temperature.get(); }
    float get_ground_pressure(void) { return _ground_pressure.get(); }

	static const struct AP_Param::GroupInfo var_info[];

protected:
    uint32_t _last_update;
    uint8_t _pressure_samples;

private:
    AP_Float    _ground_temperature;
    AP_Float    _ground_pressure;
    float       _altitude;
    float       _climb_rate;
    uint32_t    _last_climb_rate_t;
    uint32_t    _last_altitude_t;
    DerivativeFilterFloat_Size7 _climb_rate_filter;
};

#include "AP_Baro_MS5611.h"
#include "AP_Baro_BMP085.h"
#include "AP_Baro_BMP085_hil.h"

#endif // __AP_BARO_H__
