/*
 * LS7366R.cpp
 *
 *  Created on: Aug 8, 2022
 *      Author: tyEngineering
 */


#include "LS7366R.h"

#define time_out 10


void initEncoder(GPIO_TypeDef* GPIOtype, uint16_t GPIO_pin, SPI_HandleTypeDef hspi_) {
	const char MDR0 = 0b10001000;
	const char MDR1 = 0b00000011;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); 		// Start Communication
	HAL_SPI_Transmit(&hspi_, (uint8_t *)&MDR0, 1, time_out);	// Write to MDR0 w/ 0x88
	HAL_SPI_Transmit(&hspi_, (uint8_t *)&MDR1, 1, time_out);	// Write to MDR1 w/ 0x03
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);		// End Communication
}

void clearEncoderCount(GPIO_TypeDef* GPIOtype, uint16_t GPIO_pin, SPI_HandleTypeDef hspi_) {
	const char write_byte = 0b10011000;
	const char zero_byte = 0b00000000;
	const char load_byte = 0b11100000;
	HAL_GPIO_WritePin(GPIOtype, GPIO_pin, GPIO_PIN_RESET); 			// Start Communication
	HAL_SPI_Transmit(&hspi_, (uint8_t *)&write_byte, 1, time_out);
	HAL_SPI_Transmit(&hspi_, (uint8_t *)&zero_byte, 1, time_out);
	HAL_SPI_Transmit(&hspi_, (uint8_t *)&zero_byte, 1, time_out);
	HAL_SPI_Transmit(&hspi_, (uint8_t *)&zero_byte, 1, time_out);
	HAL_SPI_Transmit(&hspi_, (uint8_t *)&zero_byte, 1, time_out);
	HAL_GPIO_WritePin(GPIOtype, GPIO_pin, GPIO_PIN_SET); 				// end Communication
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOtype, GPIO_pin, GPIO_PIN_RESET); 			// Start Communication
	HAL_SPI_Transmit(&hspi_, (uint8_t *)&load_byte, 1, time_out);			// Load data to IC
	HAL_GPIO_WritePin(GPIOtype, GPIO_pin, GPIO_PIN_SET); 				// End Communication

}

long readEncoder(GPIO_TypeDef* GPIOtype, uint16_t GPIO_pin, SPI_HandleTypeDef hspi_) {
	// Temporary Value
	  long count_value = 0;
	  const char start_byte = 0b01100000;
	  const char collect_byte = 0b00000000;
	  uint8_t temp;
	  // Read encoder
	  HAL_GPIO_WritePin(GPIOtype, GPIO_pin, GPIO_PIN_RESET);		// Begin SPI conversation
	  HAL_SPI_Transmit(&hspi_, (uint8_t *)&start_byte, 1, time_out);//(0x60);                     // Request count

	  // Collect 4 bytes to construct the long encoder value
	  HAL_SPI_TransmitReceive(&hspi_, (uint8_t *)&collect_byte, (uint8_t *)&temp, 1, time_out);	// 1st Byte
	  count_value = temp;
	  HAL_SPI_TransmitReceive(&hspi_, (uint8_t *)&collect_byte, (uint8_t *)&temp, 1, time_out);	// 2nd Byte
	  count_value = (count_value << 8) + temp;
	  HAL_SPI_TransmitReceive(&hspi_, (uint8_t *)&collect_byte, (uint8_t *)&temp, 1, time_out);	// 3rd Byte
	  count_value = (count_value << 8) + temp;
	  HAL_SPI_TransmitReceive(&hspi_, (uint8_t *)&collect_byte, (uint8_t *)&temp, 1, time_out);	// 4th Byte
	  count_value = (count_value << 8) + temp;

	  HAL_GPIO_WritePin(GPIOtype, GPIO_pin, GPIO_PIN_SET);		// Terminate SPI conversation

	  return count_value;
}
