#include "Dri_MPU6050.h"
#include "sensor.h"

uint8_t MPU6050_Init1[3] = {0x00, //0x6A ����IIC�ӿ�
							0x01,	//0x6B ��������X����Ϊʱ�ӣ�����MPU6050�������¶ȴ�����
							0x00};	//0x6C 
uint8_t MPU6050_Init2[2] = {0x80,	//0x37 ��ʾ�͵�ƽΪ�жϵ���Ч�ź�
							0x01};	//0x38	����׼������ж�
uint8_t MPU6050_Init3[1] = {0x00};	//0x23 ��ʾ���ٶȼƣ������ǡ��¶ȼƵȵ�ֵ����д��FIFO
uint8_t MPU6050_Init4[4] = {0x00,	//0x19 ��ʾ�����ǲ����� ���ò�������: 1000 / (1 + 0x13) = 50Hz,���ò�������: 1000 / (1 + 0) = 1000Hz
							0x02,	//0x1A ���õ�ͨ�˲�Ϊ98HZ
							0x18,	//0x1B �����ǲ��Լ죬��������Ϊ 2000 ��/s
							0x18};	//0x1C ���ٶȼƲ��Լ죬��������Ϊ16g

uint8_t MPU6050_Data[14];																							//����Ϊ�˼����ѹ�ƣ�ADʱ������20ms����ϵͳƵ�ʶ�Ϊ10Hz

							//MPU6050�ж��ź�
void Init_EXIT_GPIO()
{
	__HAL_RCC_GPIOC_CLK_ENABLE();	//ʹ��GPIOC ʱ��
	GPIO_InitTypeDef GPIO_InitStruct;
	
	
	/* ����KEY2 GPIO:��������ģʽ */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;	//�������ж�GPIO_MODE_IT_FALLING���½���
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;		//GPIO����
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
    /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	
}


void Init_MPU6050()
{
	/*
	*@param  DevAddress Target device address: The device 7 bits address value
	*         in datasheet must be shift at right before call interface@param  MemAddress Internal memory address
	* @param  MemAddSize Size of internal memory address Ŀ�������ڲ��Ĵ�С������ʼ��ַ
	* @param  pData Pointer to data buffer
	* @param  Size Amount of data to be sent
	* @param  Timeout Timeout duration
	* @retval HAL status
	*/
	HAL_I2C_Mem_Write(&hi2c1,ADDR_MPU6050_Write,0x6A,I2C_MEMADD_SIZE_8BIT,MPU6050_Init1,3,100);
	HAL_I2C_Mem_Write(&hi2c1,ADDR_MPU6050_Write,0x37,I2C_MEMADD_SIZE_8BIT,MPU6050_Init2,2,100);
	HAL_I2C_Mem_Write(&hi2c1,ADDR_MPU6050_Write,0x23,I2C_MEMADD_SIZE_8BIT,MPU6050_Init3,1,100);	
	HAL_I2C_Mem_Write(&hi2c1,ADDR_MPU6050_Write,0x19,I2C_MEMADD_SIZE_8BIT,MPU6050_Init4,4,100);
	
	Init_EXIT_GPIO();
}

uint8 Read_MPU6050()
{
	HAL_I2C_Mem_Read_DMA(&hi2c1,ADDR_MPU6050_Read,0x3B,I2C_MEMADD_SIZE_8BIT,MPU6050_Data,14);
	
	sensor.mpu6050.acc.x.H_L[1]=MPU6050_Data[0];
	sensor.mpu6050.acc.x.H_L[0]=MPU6050_Data[1];
	sensor.mpu6050.acc.y.H_L[1]=MPU6050_Data[2];
	sensor.mpu6050.acc.y.H_L[0]=MPU6050_Data[3];
	sensor.mpu6050.acc.z.H_L[1]=MPU6050_Data[4];
	sensor.mpu6050.acc.z.H_L[0]=MPU6050_Data[5];
	
	sensor.mpu6050.thermometer.Temp.H_L[1]=MPU6050_Data[6];
	sensor.mpu6050.thermometer.Temp.H_L[0]=MPU6050_Data[7];
	
	sensor.mpu6050.gyro.x.H_L[1]=MPU6050_Data[8];
	sensor.mpu6050.gyro.x.H_L[0]=MPU6050_Data[9];
	sensor.mpu6050.gyro.y.H_L[1]=MPU6050_Data[10];
	sensor.mpu6050.gyro.y.H_L[0]=MPU6050_Data[11];
	sensor.mpu6050.gyro.z.H_L[1]=MPU6050_Data[12];
	sensor.mpu6050.gyro.z.H_L[0]=MPU6050_Data[13];
}


