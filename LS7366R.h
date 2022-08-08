/*
 * LS7366R.h
 *
 *  Created on: Aug 8, 2022
 *      Author: tyEngineering
 */

#ifndef LS7366R_H_
#define LS7366R_H_

#include "main.h"


void initEncoder(GPIO_TypeDef* GPIOtype, uint16_t GPIO_pin, SPI_HandleTypeDef hspi_);

void clearEncoderCount(GPIO_TypeDef* GPIOtype, uint16_t GPIO_pin, SPI_HandleTypeDef hspi_);

long readEncoder(GPIO_TypeDef* GPIOtype, uint16_t GPIO_pin, SPI_HandleTypeDef hspi_);



#endif /* LS7366R_H_ */
