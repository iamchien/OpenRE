#ifndef BOARD_ABSTRACT_H
#define BOARD_ABSTRACT_H

#ifdef __cplusplus
extern "C" {
#endif
#include "bsplib.h"
#ifdef __cplusplus
}
#endif

#include "string.h"
#include "queue.h"
#include <math.h>

//device type
enum DeviceType : unsigned char
{
    USART_DEBUG,
    USART_PC,
    USART_RADIO,
    USART_GPS,
    USART_SBUS,
    USART_DIGITAL_SERVO,
    USART_IMU,
    IIC_IMU,
    IIC_AT24CXX,
    IIC_OLED,
    SPI_IMU,
    SPI_NRF24L01,
    SPI_LCD,
    CAN_IMU,
    CAN_PAN_AND_TILT,
    MOTOR1,
    MOTOR2,
    MOTOR3,
    MOTOR4,
    LAST_DEVICE_FLAG
};

class BoardAbstract
{
public:
    BoardAbstract();

public:
    float cpu_temperature;
    float cpu_usage;
    float battery_voltage;
    float system_time;   //system working time (unit:us), start after power-on
    float power_remain;
    uint8_t date_year , date_month , date_day , date_week;
    uint8_t time_hour , time_min , time_sec , time_ampm;
    uint8_t system_init; //state of system: 0-->not initialize  1-->initialized
    uint16_t cnt_1ms , cnt_2ms ,  cnt_5ms , cnt_10ms , cnt_20ms , cnt_50ms , cnt_100ms, cnt_500ms, cnt_1000ms, cnt_2000ms;
    uint32_t chipUniqueID[3];
    uint16_t flashSize;    //Unit: KB
    //the device id 's high 4 bit means which channel , low 4 bit means the channel mapping interface
    // 0x00 means this device does not support
    uint8_t device_type[LAST_DEVICE_FLAG];

public:
    /*****system support functions*********************************************************************************************/
    //operation: 0 off , 1 on , 2 toggle
    virtual void setLedState(uint8_t led_id, uint8_t operation) = 0;
    //operation: 0 off , 1 on , 2 toggle
    virtual void setBeepState(uint8_t operation) = 0 ;
    /****hardwareinterface -- motor********************************************************************************************/
    //mode = 1 (enable pwm1 pwm2)  ; mode = 2 (pwm , IOA , IOB)
    virtual void motorInterfaceInit(uint8_t mode , uint8_t motor_id , float motor_pwm_t) = 0;
    virtual void motorEnable(uint8_t mode , uint8_t motor_id ) = 0;
    virtual void motorDisable(uint8_t mode , uint8_t motor_id ) = 0;
    virtual void motorSetPWM(uint8_t mode , uint8_t motor_id , int pwm_value) = 0;
    virtual float getMotorEncoderCNT(uint8_t motor_id) {return 0;}
    virtual float getMotorCurrent(uint8_t motor_id) {return 0;}
    /*****hardwareinterface -- digital servo**************************************************************************************/
    virtual void axServoInterfaceInit(void) = 0;
    virtual void axServoTxModel(void) = 0;
    virtual void axServoRxModel(void) = 0;
    /******hardwareinterface -- pwmoutput**************************************************************************************/
    //mode=0 high frequency control motor mode
    //mode=1 for electric modulation analog servo mode
    virtual void pwmInterfaceInit(uint8_t channel_x , uint8_t mode) = 0;
    virtual void setPWMInterfaceValue(uint8_t channel_x , uint16_t pwm_value) = 0;
    virtual uint16_t readPWMInterfaceValue(uint8_t channel_x) = 0;
    virtual float getADCInterfaceValue(uint8_t channel_x) = 0;

public:
    void boardBasicInit(void);
    void boardBasicCall(void);
    void softwareReset(void)
    {
        //feed_dog_enable = 0;
        HF_IWDG_Init();
        delay_ms(1000);
    }
    float getClock(void)
    {
        return HF_Get_System_Time();
    }
    void debugInterfaceInit(void)
    {
        usartDeviceInit(USART_DEBUG , 921600);  //debug USART init
    }
    void debugPutChar(uint8_t tx_byte)
    {
        usartDeviceWriteByte(USART_DEBUG , tx_byte);
    }
    //model : 0 off , 1 on , 2 toggle , 3 -- 10hz , 4 -- 2hz , 5 -- 0.5hz
    void setBeepModel(uint8_t model){ beep_model = model;}
    uint8_t getBeepModel(void){return beep_model;}
    //tweet n * 10ms by timer
    void setBeepTweet(uint16_t nx10ms){ beep_tweet_nx10ms = nx10ms;}
    //operation: 0 off , 1 click , 2 double-click, 3 press
    uint8_t getKeyState(uint8_t key_id)
    {
        if(key_id <= 5)
        {
            unsigned char state = key_state[key_id -1];
            if(state == 1 || state == 2 || state == 3) //if click or double-click or long press
            {
                key_state[key_id -1] = 0;
            }

            return state;
        }
        else return 0;
    }
    void setBatteryPara(unsigned char series_ , float voltage_alarm_)
    {
        battery_series_ = series_;
        battery_voltage_alarm_ = voltage_alarm_;
    }

    /******device -- for usart device or interface***********************************************************************************/
    Queue* getUsartQueue(uint8_t channel);
    void usartDeviceInit(DeviceType usart_device_type ,  uint32_t baudrate ); // if baudrate==0 means default baudrate
    void usartDeviceWriteByte(DeviceType usart_device_type , uint8_t reg_data);
    Queue* usartDeviceReadData(DeviceType usart_device_type);

    /********device -- for iic device or interface***********************************************************************************/
    void iicDeviceInit(DeviceType iic_device_type , I2C_Sensor_Type sensor_type);
    void iicDeviceWriteByte(DeviceType iic_device_type , uint8_t equipment_address,
                            uint8_t reg_address , uint8_t reg_data);
    uint8_t iicDeviceReadByte(DeviceType iic_device_type , uint8_t equipment_address,
                              uint8_t reg_address);
    uint8_t iicDeviceWriteBuf(DeviceType iic_device_type , uint8_t equipment_address,
                              uint8_t reg_address , uint8_t* pt_char , uint8_t size);
    uint8_t iicDeviceReadBuf(DeviceType iic_device_type , uint8_t equipment_address,
                             uint8_t reg_address , uint8_t* pt_char , uint8_t size);

    /******device -- for spi device or interface*************************************************************************************/
    void spiDeviceInit(DeviceType spi_device_type);
    uint8_t spiDeviceReadWriteByte(DeviceType spi_device_type , uint8_t byte);

    /******device -- for can device or interface************************************************************************************/
    void canDeviceInit(DeviceType can_device_type);
    void canDeviceSendMessage(DeviceType can_device_type , uint8_t sender_id, uint8_t receiver_id,
                              uint8_t *txbuf, uint8_t length);
    /***********************************************************************************************************************/

    /******device -- for io device or interface************************************************************************************/
    virtual void ioDeviceInit(DeviceType io_device_type) = 0;
    virtual float getIODeviceData(DeviceType io_device_type) = 0;

    /******common***********************************************************************************************************/
    uint8_t getByteHighFourBit(uint8_t data){ return (data&0xf0)>>4;}
    uint8_t getByteLowFourBit(uint8_t data){ return data&0x0f;}

protected:
    unsigned char battery_series_;
    float battery_voltage_;
    float battery_voltage_alarm_ ;
    float battery_proportion_ ;

    float cpu_temperature_;
    float cpu_temperature_alarm_;

    uint8_t beep_model;
    int16_t beep_tweet_nx10ms;
    uint8_t beep_toggle_cnt_ ;

    uint16_t board_call_5ms , board_call_20ms , board_call_1s , board_call_2s;
    uint8_t key_state[5];
    Queue usart1_queue ,  usart2_queue , usart3_queue , usart4_queue , usart5_queue , usart6_queue;

private:
    virtual float getBatteryVoltage(void) = 0;
    virtual void getCPUInfo(void) = 0;
    virtual void adcInit(void) = 0;
    virtual void timerInit(void) = 0;
    virtual void ledInit(void) = 0;
    virtual void keyInit(void) = 0;
    virtual void keyStateRenew(void) = 0;  //100HZ
    virtual void beepInit(void) = 0;

    float voltage_to_capacity(float voltage) //lithium cell capacity = 0~1 voltage = 3 ~ 4.2
    {
        if(voltage >=4.2) return 1;
        if(voltage <=3 && voltage >=1.5) return 0;
        if(voltage < 1.5 ) return -1;
        float A = 1.0136017772655;
        float B = -38.6694358844531;
        float C = 3.84067378865132;
        float capacity = A/(1+pow((voltage/C),B));
        return capacity;
    }
    float capacity_to_voltage(float capacity) //lithium cell capacity = 0~1 voltage = 3 ~ 4.2
    {
        if(capacity <= 0.01) return 2.5;
        if(capacity <= 0.05) return 3.5;
        if(capacity >= 1) return 4.2;
        float a = 3.40486290747938;
        float b = 2.98683529266958;
        float c = -8.49035538952372;
        float d = 10.418086187689;
        float e = -4.1298884360942;
        float voltage = a +capacity*(b+capacity*(c+capacity*(d+capacity*e)));
        return voltage;
    }

    void beepStateRenew(void); //100HZ
    //Initialize the measurement systemm
    void systemClockInit(void);
    void updateLocalTime(void);
    float getCPUUsage(void);
    float getCPUTemperature(void);

private:
    uint8_t feed_dog_enable;
};

#endif // BOARD_ABSTRACT_H
