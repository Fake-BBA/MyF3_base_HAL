#include "sensor.h"
#include "fliter.h"
#include "Dri_MPU6050.h"

struct Sensor sensor;

/*低通滤波参数*/
#define GYRO_LPF_CUTOFF_FREQ  80
#define ACCEL_LPF_CUTOFF_FREQ 30
static lpf2pData accLpf[3];		//加速度计低通滤波变量
static lpf2pData gyroLpf[3];	//陀螺仪低通滤波变量



BiasObj	gyroBiasRunning;
static Axis3f  gyroBias;

uint8 SensorInit()
{
	for (uint8 i = 0; i < 3; i++)// 初始化加速计和陀螺二阶低通滤波
	{
		lpf2pInit(&gyroLpf[i], 1000, GYRO_LPF_CUTOFF_FREQ);
		lpf2pInit(&accLpf[i],  1000, ACCEL_LPF_CUTOFF_FREQ);
	}
	
	return 1;
}

/**
 * 计算陀螺方差
 */
static bool processGyroBias(int16_t gx, int16_t gy, int16_t gz, Axis3f *gyroBiasOut)
{
	sensorsAddBiasValue(&gyroBiasRunning, gx, gy, gz);

	if (!gyroBiasRunning.isBiasValueFound)
	{
		sensorsFindBiasValue(&gyroBiasRunning);
		if (gyroBiasRunning.isBiasValueFound)
		{
			ledseqRun(SYS_LED, seq_calibrated);
		}
	}

	gyroBiasOut->x = gyroBiasRunning.bias.x;
	gyroBiasOut->y = gyroBiasRunning.bias.y;
	gyroBiasOut->z = gyroBiasRunning.bias.z;

	return gyroBiasRunning.isBiasValueFound;
}

/*处理加速计和陀螺仪数据*/
void processAccGyroMeasurements(struct Sensor *p_Sensor)
{
	int16_t ax = p_Sensor->mpu6050.acc.x.data;
	int16_t ay = p_Sensor->mpu6050.acc.y.data;
	int16_t az = p_Sensor->mpu6050.acc.z.data;
	int16_t gy = p_Sensor->mpu6050.gyro.x.data;
	int16_t gx = p_Sensor->mpu6050.gyro.y.data;
	int16_t gz = p_Sensor->mpu6050.gyro.z.data;

	static bool gyroBiasFound=false;
	gyroBiasFound = processGyroBias(gx, gy, gz, &gyroBias);	//计算陀螺方差
	if (gyroBiasFound)
	{
		//processAccScale(ax, ay, az);	/*计算accScale*/
	}
	
	sensors.gyro.x = -(gx - gyroBias.x) * SENSORS_DEG_PER_LSB_CFG;	/*单位 °/s */
	sensors.gyro.y =  (gy - gyroBias.y) * SENSORS_DEG_PER_LSB_CFG;
	sensors.gyro.z =  (gz - gyroBias.z) * SENSORS_DEG_PER_LSB_CFG;
	applyAxis3fLpf((lpf2pData*)(&gyroLpf), &sensors.gyro);

	sensors.acc.x = -(ax) * SENSORS_G_PER_LSB_CFG * accScale;	/*单位 g(9.8m/s^2)*/
	sensors.acc.y =  (ay) * SENSORS_G_PER_LSB_CFG * accScale;	/*重力加速度缩放因子accScale 根据样本计算得出*/
	sensors.acc.z =  (az) * SENSORS_G_PER_LSB_CFG * accScale;
	applyAxis3fLpf((lpf2pData*)(&accLpf), &sensors.acc);
}

void SensorThread()
{
	Read_MPU6050();	//读取MPU6050数据到sensor结构体
	
	//处理MPU6050原始数据
	processAccGyroMeasurements(&sensor);
	
	
}