#include "sht.h"
#include <xc.h>
#include <delays.h>

#define SHT_DATA_PIN     PORTAbits.RA0
#define SHT_DATA         LATAbits.LATA0       /* Data tx/rx pin */
#define SHT_DATA_DDR     TRISAbits.TRISA0         /* Data direction register */
#define SHT_SCK          LATAbits.LATA1     	/* Clk pin */
#define SHT_SCK_DDR      TRISAbits.TRISA1       /* Clk direction register */
#define SHT_DATA_CNF     ANCON0bits.PCFG0
#define SHT_SCK_CNF      ANCON0bits.PCFG1

/**
 * Generates a transmission start
 *       _____         ________
 * DATA:      |_______|
 *           ___     ___
 * SCK : ___|   |___|   |______
 */
static void Sht11Start(void);

/* Reads a byte form the Sensibus and gives an acknowledge in case of "ack=1" */
static uint8_t Sht11Read(uint8_t ack);

/* Writes a byte on the Sensibus and checks the acknowledge */
static uint8_t Sht11Write(uint8_t value);


/* Makes a measurement (humidity/temperature) with checksum */
static uint8_t Sht11MeasureParam(uint16_t *p_value, uint8_t *p_checksum, uint8_t mode);

void Sht11Init() {
    SHT_DATA_CNF = 1;
    SHT_SCK_CNF = 1;
    SHT_DATA = 1;
    SHT_SCK_DDR = 0;
    SHT_DATA_DDR = 0;
    SHT_SCK = 0;
}

uint8_t Sht11Measure(ShtData* data) {
    uint8_t error = 0;
    // Get measures
    error += Sht11MeasureParam(&data->temperature.i, &data->tempChk,
            SHT_MEASURE_TEMP);
    error += Sht11MeasureParam(&data->humidity.i, &data->humiChk,
            SHT_MEASURE_HUMI);
    return error;
}

static uint8_t Sht11Write(uint8_t value) {
    uint8_t i, error = 0;
    //shift bit for masking
    for (i = 0x80; i > 0; i /= 2) {
        if (i & value) {
            SHT_DATA = 1; //masking value with i , write to SENSI-BUS
        } else {
            SHT_DATA = 0;
        }
        Delay10TCYx(10); //observe setup time
        SHT_SCK = 1; //clk for SENSI-BUS
        Delay10TCYx(10); //pulswith approx. 5 us
        SHT_SCK = 0;
        Delay10TCYx(10); //observe hold time
    }
    SHT_DATA = 1; //release SHT_DATA-line
    Delay10TCYx(10); //observe setup time
    SHT_SCK = 1; //clk #9 for ack
    Delay10TCYx(10);
    // To read SHT ack(low), change pin direction
    SHT_DATA_DDR = 1;
    // Read pin
    error = SHT_DATA_PIN; //check ack (SHT_DATA will be pulled down by SHT11)
    SHT_SCK = 0;
    return error; //error=1 in case of no acknowledge
}

static uint8_t Sht11Read(uint8_t ack) {
    uint8_t i, val = 0;
    SHT_DATA_DDR = 1;
    SHT_DATA = 1;
    for (i = 0x80; i > 0; i /= 2) //shift bit for masking
    {
        Delay10TCYx(10);
        SHT_SCK = 1; //clk for SENSI-BUS
        if (SHT_DATA_PIN) {
            val = (val | i); //read bit
        }
        Delay10TCYx(10);
        SHT_SCK = 0;
    }

    if (ack) {
        SHT_DATA_DDR = 0;
        SHT_DATA = 0; //in case of "ack==1" pull down SHT_DATA-Line
    }
    Delay10TCYx(10); //observe setup time
    SHT_SCK = 1; //clk #9 for ack
    Delay10TCYx(10); //pulswith approx. 5 us
    SHT_SCK = 0;
    Delay10TCYx(10); //observe hold time
    return val;
}

//----------------------------------------------------------------------------------

static void Sht11Start(void) {
    SHT_DATA = 1;
    SHT_SCK = 0; //Initial state
    Delay10TCYx(10);
    SHT_SCK = 1;
    Delay10TCYx(10);
    SHT_DATA = 0;
    Delay10TCYx(10);
    SHT_SCK = 0;
    Delay10TCYx(10);
    SHT_SCK = 1;
    Delay10TCYx(10);
    SHT_DATA = 1;
    Delay10TCYx(10);
    SHT_SCK = 0;
}

static uint8_t Sht11MeasureParam(uint16_t *p_value, uint8_t *p_checksum, uint8_t mode) {
    uint8_t error = 0;
    SHT_DATA_DDR = 0;
    SHT_DATA = 1;
    Sht11Start(); //transmission start
    switch (mode) { //send command to sensor
        case SHT_MEASURE_TEMP: error += Sht11Write(SHT_MEASURE_TEMP);
            break;
        case SHT_MEASURE_HUMI: error += Sht11Write(SHT_MEASURE_HUMI);
            break;
        default: break;
    }
    Delay10TCY();
    SHT_DATA_DDR = 1;
    while (SHT_DATA_PIN == 1);
    // Read two bytes response
    *(p_value) = Sht11Read(SHT_ACK); //read the first byte (MSB)
    *(p_value) = *(p_value) << 8;
    *(p_value) += Sht11Read(SHT_ACK); //read the second byte (LSB)
    *p_checksum = Sht11Read(SHT_NACK); //read checksum

    return error;
}
