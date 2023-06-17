#include "../new_common.h"
#include "../new_pins.h"
#include "../new_cfg.h"
// Commands register, execution API and cmd tokenizer
#include "../cmnds/cmd_public.h"
#include "../logging/logging.h"
#include "drv_i2c_local.h"

void DRV_I2C_CHT8305_readTemperatureHumidity(int dev_adr, int busID, int *temp, int *humidity)
{
	//addLogAdv(LOG_INFO, LOG_FEATURE_I2C,"DRV_I2C_TC74_readTemperature: called for addr %i\n", dev_adr);
	// I2C_ReadBytes Soft 0x40 0x00 4 20000
	
	byte buffer[4];

	DRV_I2C_Begin(dev_adr,busID);
	DRV_I2C_ReadBytesAdv(0x00,buffer,4,20000);
	DRV_I2C_Close();

	*temp = (int)(1650.*((buffer[0]*256. + buffer[1]*1.)/65535.)-400.);
	*humidity = (int)(1000.*((buffer[2]*256. + buffer[3]*1.)/65535.));

	addLogAdv(LOG_INFO, LOG_FEATURE_I2C,"CHT8305 reads (%i %i; %i %i) - Temp: %f, Humidity %f", buffer[0],buffer[1],buffer[2],buffer[3], (*temp)*0.1, (*humidity)*0.1);

}
void DRV_I2C_CHT8305_RunDevice(i2cDevice_t *dev)
{
	i2cDevice_CHT8305_t *cht8305;

	cht8305 = (i2cDevice_CHT8305_t*)dev;

	int temp;
	int humidity;
	DRV_I2C_CHT8305_readTemperatureHumidity(cht8305->base.addr, cht8305->base.busType, &temp, &humidity);

	CHANNEL_Set(cht8305->targetChannelTemp, temp, 0);
	CHANNEL_Set(cht8305->targetChannelHum,  humidity, 0);
}