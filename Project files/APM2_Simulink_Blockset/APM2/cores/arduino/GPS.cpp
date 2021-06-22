// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: t -*-

#include <FastSerial.h>

#define GPS_DEBUGGING 0

#if GPS_DEBUGGING
#include <FastSerial.h>
# define Debug(fmt, args...)  do {Serial.printf("%s:%d: " fmt "\n", __FUNCTION__, __LINE__ , ##args); delay(0); } while(0)
#else
# define Debug(fmt, args...)
#endif

#include <AP_Common.h>
#include <AP_Math.h>
#include "GPS.h"
#define ARDUINO 100
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

void
GPS::update(void)
{
    bool	result;
	uint32_t tnow;

    // call the GPS driver to process incoming data
    result = read();

	tnow = millis();

    // if we did not get a message, and the idle timer has expired, re-init
    if (!result) {
        if ((tnow - _idleTimer) > idleTimeout) {
			Debug("gps read timeout %lu %lu", (unsigned long)tnow, (unsigned long)_idleTimer);
            _status = NO_GPS;

            init(_nav_setting);
            // reset the idle timer
            _idleTimer = tnow;
        }
    } else {
        // we got a message, update our status correspondingly
        _status = fix ? GPS_OK : NO_FIX;

        valid_read = true;
        new_data = true;

        // reset the idle timer
        _idleTimer = tnow;

		if (_status == GPS_OK) {
			// update our acceleration
			float deltat = 1.0e-3 * (_idleTimer - last_fix_time);
			float deltav = 1.0e-2 * ((float)ground_speed - (float)_last_ground_speed);
			float gps_heading = ToRad(ground_course * 0.01);
			float gps_speed   = ground_speed * 0.01;
			float sin_heading, cos_heading;

			cos_heading = cos(gps_heading);
			sin_heading = sin(gps_heading);

			last_fix_time = _idleTimer;
			_last_ground_speed = ground_speed;

			_velocity_north = gps_speed * cos_heading;
			_velocity_east  = gps_speed * sin_heading;

			if (deltat > 2.0 || deltat == 0) {
				// we didn't get a fix for 2 seconds - set
				// acceleration to zero, as the estimate will be too
				// far out
				_acceleration = 0;
				_acceleration_north = 0;
				_acceleration_east = 0;
			} else {
				// calculate a mildly smoothed acceleration value
				_acceleration = (0.7 * _acceleration) + (0.3 * (deltav/deltat));

				// calculate the components, to save time in the AHRS code
				_acceleration_north = _acceleration * cos_heading;
				_acceleration_east  = _acceleration * sin_heading;
			}
		}
    }
}

void
GPS::setHIL(uint32_t _time, float _latitude, float _longitude, float _altitude,
            float _ground_speed, float _ground_course, float _speed_3d, uint8_t _num_sats)
{
}

// XXX this is probably the wrong way to do it, too
void
GPS::_error(const char *msg)
{
    Serial.println(msg);
}

///
/// write a block of configuration data to a GPS
///
void GPS::_write_progstr_block(Stream *_fs, const prog_char *pstr, uint8_t size)
{
	while (size--) {
		_fs->write(pgm_read_byte(pstr++));
	}
}
