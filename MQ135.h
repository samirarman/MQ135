/**************************************************************************/
/*!
@file     MQ135.h
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3

First version of an Arduino Library for the MQ135 gas sensor
TODO: Review the correction factor calculation. This currently relies on
the datasheet but the information there seems to be wrong.

@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/
#ifndef MQ135_H
#define MQ135_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/// The load resistance on the board
// Changing default load resistance to reflect
// cheap board configuration
// #define RLOAD 10.0
# define RLOAD 10.0
/// Calibration resistance at atmospheric CO2 level
#define RZERO 33552408.
/// Parameters for calculating ppm of CO2 from sensor resistance
#define PARA 5.071587
#define PARB 0.343698

/// Atmospheric CO2 level for calibration purposes
#define ATMOCO2 415.24

class MQ135 {
 private:
  uint8_t _pin;

 public:
  MQ135(uint8_t pin);
  double getCorrectionFactor(double t, double h);
  double getResistance();
  double getCorrectedResistance(double t, double h);
  double getPPM();
  double getCorrectedPPM(double t, double h);
  double getRZero();
  double getCorrectedRZero(double t, double h);
  double getRsOverRZero();
};
#endif
