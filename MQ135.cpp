

/**************************************************************************/
/*!
@file     MQ135.cpp
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3

First version of an Arduino Library for the MQ135 gas sensor
TODO: Review the correction factor calculation. This currently relies on
the datasheet but the information there seems to be wrong.

@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/

#include "MQ135.h"

/**************************************************************************/
/*!
@brief  Default constructor

@param[in] pin  The analog input pin for the readout of the sensor
*/
/**************************************************************************/

MQ135::MQ135(uint8_t pin) {
  _pin = pin;
}


/**************************************************************************/
/*!
@brief  Get the correction factor to correct for temperature and humidity

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The calculated correction factor
*/
/**************************************************************************/
double MQ135::getCorrectionFactor(double t, double h) {
  
  double low = -0.000006*pow(t,3)+0.0007*pow(t,2)-0.0291*t+1.3684;
  double high = -0.0000032*pow(t,3)+0.0005*pow(t,2)-0.0253*t+1.2373;
  double disp = (high - low) / (83. - 33.);
  return low + (h - 33.) * disp;
}

/**************************************************************************/
/*!
@brief  Get the resistance of the sensor, ie. the measurement value

@return The sensor resistance in kOhm
*/
/**************************************************************************/
double MQ135::getResistance() {
  int val = analogRead(_pin);
  return ((1023./(double)val)  - 1.)*RLOAD;
}

/**************************************************************************/
/*!
@brief  Get the resistance of the sensor, ie. the measurement value corrected
        for temp/hum

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The corrected sensor resistance kOhm
*/
/**************************************************************************/
double MQ135::getCorrectedResistance(double t, double h) {
  return getResistance()/getCorrectionFactor(t, h);
}

/**************************************************************************/
/*!
@brief  Get the ppm of CO2 sensed (assuming only CO2 in the air)

@return The ppm of CO2 in the air
*/
/**************************************************************************/
double MQ135::getPPM() {
  return PARA * pow(getResistance()/RZERO, PARB);
}

/**************************************************************************/
/*!
@brief  Get the ppm of CO2 sensed (assuming only CO2 in the air), corrected
        for temp/hum

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The ppm of CO2 in the air
*/
/**************************************************************************/
double MQ135::getCorrectedPPM(double t, double h) {
  return PARA * pow((getCorrectedResistance(t, h)/RZERO), PARB);
}

/**************************************************************************/
/*!
@brief  Get the resistance RZero of the sensor for calibration purposes

@return The sensor resistance RZero in kOhm
*/
/**************************************************************************/
double MQ135::getRZero() {
  return getResistance() * pow((PARA/ATMOCO2), (1./PARB));
}

/**************************************************************************/
/*!
@brief  Get the corrected resistance RZero of the sensor for calibration
        purposes

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The corrected sensor resistance RZero in kOhm
*/
/**************************************************************************/
double MQ135::getCorrectedRZero(double t, double h) {
  return getCorrectedResistance(t, h) * pow(PARA/ATMOCO2, (1./PARB));
}
