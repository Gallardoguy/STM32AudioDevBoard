/*
 * codec.c
 *
 *  Created on: Nov 29, 2024
 *      Author: Nezar
 */
#include "codec.h"



HAL_StatusTypeDef sgtl5000_read_register(uint16_t reg_address, uint16_t *data) {
    uint8_t reg_buffer[2]; // Buffer for the 16-bit register address
    uint8_t data_buffer[2]; // Buffer for the received 16-bit data

    // Split the 16-bit register address into two bytes (MSB first)
    reg_buffer[0] = (reg_address >> 8) & 0xFF; // High byte of register address
    reg_buffer[1] = reg_address & 0xFF;        // Low byte of register address

    // Step 1: Send the register address
    if (HAL_I2C_Master_Transmit(&I2C_HANDLE, ADDRESS, reg_buffer, 2, HAL_MAX_DELAY) != HAL_OK) {
        return HAL_ERROR; // Address write failed
    }

    // Step 2: Read the data from the register
    if (HAL_I2C_Master_Receive(&I2C_HANDLE, ADDRESS | 0x01, data_buffer, 2, HAL_MAX_DELAY) != HAL_OK) {
        return HAL_ERROR; // Data read failed
    }

    // Combine the two bytes into a 16-bit value (MSB first)
    *data = (data_buffer[0] << 8) | data_buffer[1];

    return HAL_OK;
}

HAL_StatusTypeDef sgtl5000_write_register(uint16_t reg_address, uint16_t data) {
    uint8_t buffer[4]; // 2 bytes for the address + 2 bytes for the data

    // Split the 16-bit register address into two bytes (MSB first)
    buffer[0] = (reg_address >> 8) & 0xFF; // High byte of register address
    buffer[1] = reg_address & 0xFF;        // Low byte of register address

    // Split the 16-bit data into two bytes (MSB first)
    buffer[2] = (data >> 8) & 0xFF; // High byte of data
    buffer[3] = data & 0xFF;        // Low byte of data

    // Transmit the buffer over I2C
    return HAL_I2C_Master_Transmit(&I2C_HANDLE, ADDRESS, buffer, 4, HAL_MAX_DELAY);
}


void Codec_Init() {
	uint16_t data;
	uint16_t mask;
	sgtl5000_read_register(0x0000, &data);
	if ((data >> 8) != 0xA0) {
		while(1){

		}//error
	}//device ID is good

//I followed the suggested configuration sequence in the SGTL5000 Datasheet
	sgtl5000_write_register(CHIP_ANA_POWER, 0x4260);
	sgtl5000_write_register(CHIP_LINREG_CTRL, 0x006C);
	sgtl5000_write_register(CHIP_REF_CTRL, 0x001F);
	sgtl5000_write_register(CHIP_LINE_OUT_CTRL, 0x0322);
	sgtl5000_write_register(CHIP_REF_CTRL, 0x004F);
	sgtl5000_write_register(CHIP_SHORT_CTRL, 0x1106);
	sgtl5000_write_register(CHIP_ANA_CTRL, 0x0133);
	sgtl5000_write_register(CHIP_ANA_POWER, 0x6AFF);
	HAL_Delay(10);
	sgtl5000_read_register(CHIP_ANA_POWER, &data);
	mask = data & ~(0x4040);
	sgtl5000_write_register(CHIP_ANA_POWER, mask);
//	sgtl5000_write_register(CHIP_ANA_TEST2, 0x1000);


	sgtl5000_write_register(CHIP_DIG_POWER, 0x0073);


	sgtl5000_read_register(CHIP_CLK_CTRL, &data);
	mask = data | 0x000C;
	sgtl5000_write_register(CHIP_CLK_CTRL, mask);

	sgtl5000_read_register(CHIP_I2S_CTRL, &data);
	mask = data | 0x0130;
	sgtl5000_write_register(CHIP_I2S_CTRL, mask);

	sgtl5000_read_register(CHIP_SSS_CTRL, &data);
	mask = data | 0x0070;
	mask &= ~(0x0003);
	sgtl5000_write_register(CHIP_SSS_CTRL, mask);

	sgtl5000_read_register(CHIP_ANA_CTRL, &data);
	mask = data & ~(1 << 6);
	mask |= 1 << 2;
	sgtl5000_write_register(CHIP_ANA_CTRL, mask);

	sgtl5000_read_register(DAP_CONTROL, &data);
	mask = data & ~(0x0011);
	mask |= 0x0001;
	sgtl5000_write_register(DAP_CONTROL, mask);

	sgtl5000_write_register(CHIP_ANA_ADC_CTRL, 0x0000);

	sgtl5000_read_register(CHIP_ANA_CTRL, &data);
	mask = data & ~(1 << 4);
	sgtl5000_write_register(CHIP_ANA_CTRL, mask);


	sgtl5000_write_register(CHIP_DAC_VOL, 0x3C3C);


	sgtl5000_read_register(CHIP_ADCDAC_CTRL, &data);
	mask = data & ~(3 << 2);
	sgtl5000_write_register(CHIP_ADCDAC_CTRL, mask);

	sgtl5000_read_register(CHIP_ANA_CTRL, &data);
	mask = data & ~(0x0001);
	sgtl5000_write_register(CHIP_ANA_CTRL, mask);

}

void toggleBypass() { // toggles the bit that reroutes audio from ADC to headphone out creating a toggleable analog bypass
	uint16_t data, mask;
	sgtl5000_read_register(CHIP_ANA_CTRL, &data);
	mask = data ^ (1<<6);
	sgtl5000_write_register(CHIP_ANA_CTRL, mask);
}
