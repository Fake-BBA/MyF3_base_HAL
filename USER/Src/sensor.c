#include "sensor.h"
#include "fliter.h"
#include "Dri_MPU6050.h"
#include "math.h"
#include "Dri_MPU6050.h"

struct Sensor sensor;

/*��ͨ�˲�����*/
#define GYRO_LPF_CUTOFF_FREQ  80
#define ACCEL_LPF_CUTOFF_FREQ 30
static lpf2pData accLpf[3];		//���ٶȼƵ�ͨ�˲�����
static lpf2pData gyroLpf[3];	//�����ǵ�ͨ�˲�����


//��������ƫ��ֵ
BiasObj	gyroBiasRunning;	//����������ƫ��ֵ
static Axis3f  gyroBias;	//�����ҵ���ƫ��ֵ

//��������������
static bool gyroBiasFound = false;
static float accScaleSum = 0;
static float accScale = 1;

uint8 SensorInit()
{
	for (uint8 i = 0; i < 3; i++)// ��ʼ�����ټƺ����ݶ��׵�ͨ�˲�
	{
		lpf2pInit(&gyroLpf[i], 1000, GYRO_LPF_CUTOFF_FREQ);
		lpf2pInit(&accLpf[i],  1000, ACCEL_LPF_CUTOFF_FREQ);
	}
	
	return 1;
}


/*���׵�ͨ�˲�*/
static void applyAxis3fLpf(lpf2pData *data, union Union_TFloat* in)
{
	for (uint8 i = 0; i < 3; i++) 
	{
		in->axis[i] = lpf2pApply(&data[i], in->axis[i]);
	}
}

/**
 * �������������������ٶ���������
 */
static bool processAccScale(int16_t ax, int16_t ay, int16_t az)
{
	static bool accBiasFound = false;
	static uint32_t accScaleSumCount = 0;

	
	if (!accBiasFound)	//���δ�ҵ�������������
	{
		//powf x��y����
		accScaleSum += sqrtf(powf(ax * SENSORS_G_PER_LSB_CFG, 2) + powf(ay * SENSORS_G_PER_LSB_CFG, 2) + powf(az * SENSORS_G_PER_LSB_CFG, 2));
		accScaleSumCount++;

		if (accScaleSumCount == SENSORS_ACC_SCALE_SAMPLES)
		{
			accScale = accScaleSum / SENSORS_ACC_SCALE_SAMPLES;
			accBiasFound = true;
		}
	}

	return accBiasFound;
}

/*���㷽���ƽ��ֵ*/
static void sensorsCalculateVarianceAndMean(BiasObj* bias, Axis3f* varOut, Axis3f* meanOut)
{
	uint32 i;
	int64_t sum[3] = {0};
	int64_t sumsq[3] = {0};

	for (i = 0; i < SENSORS_NBR_OF_BIAS_SAMPLES; i++)
	{
		sum[0] += bias->buffer[i].x;
		sum[1] += bias->buffer[i].y;
		sum[2] += bias->buffer[i].z;
		sumsq[0] += bias->buffer[i].x * bias->buffer[i].x;
		sumsq[1] += bias->buffer[i].y * bias->buffer[i].y;
		sumsq[2] += bias->buffer[i].z * bias->buffer[i].z;
	}

	varOut->x = (sumsq[0] - ((int64_t)sum[0] * sum[0]) / SENSORS_NBR_OF_BIAS_SAMPLES);
	varOut->y = (sumsq[1] - ((int64_t)sum[1] * sum[1]) / SENSORS_NBR_OF_BIAS_SAMPLES);
	varOut->z = (sumsq[2] - ((int64_t)sum[2] * sum[2]) / SENSORS_NBR_OF_BIAS_SAMPLES);

	meanOut->x = (float)sum[0] / SENSORS_NBR_OF_BIAS_SAMPLES;
	meanOut->y = (float)sum[1] / SENSORS_NBR_OF_BIAS_SAMPLES;
	meanOut->z = (float)sum[2] / SENSORS_NBR_OF_BIAS_SAMPLES;
}

/*����������ƫ��ֵ*/
static bool sensorsFindBiasValue(BiasObj* bias)
{
	bool foundbias = false;

	if (bias->isBufferFilled)	//�������buff�Ѿ����ˣ��Ϳ�ʼ����ƫ��ֵ
	{
		Axis3f variance;
		Axis3f mean;

		sensorsCalculateVarianceAndMean(bias, &variance, &mean);	//���㷽���ƽ��ֵ

		if (variance.x < GYRO_VARIANCE_BASE && variance.y < GYRO_VARIANCE_BASE && variance.z < GYRO_VARIANCE_BASE)
		{
			bias->bias.x = mean.x;
			bias->bias.y = mean.y;
			bias->bias.z = mean.z;
			foundbias = true;
			bias->isBiasValueFound= true;
		}else
			bias->isBufferFilled=false;
	}
	return foundbias;
}

/**
 * �����������ѭ�������������һ����ֵ�������������滻�ɵ�ֵ
 */
static void sensorsAddBiasValue(BiasObj* bias, int16_t x, int16_t y, int16_t z)
{
	bias->bufHead->x = x;
	bias->bufHead->y = y;
	bias->bufHead->z = z;
	bias->bufHead++;

	if (bias->bufHead >= &bias->buffer[SENSORS_NBR_OF_BIAS_SAMPLES])
	{
		bias->bufHead = bias->buffer;
		bias->isBufferFilled = true;
	}
}

/**
 * �������ݷ���
 */
static bool processGyroBias(int16_t gx, int16_t gy, int16_t gz, Axis3f *gyroBiasOut)
{
	sensorsAddBiasValue(&gyroBiasRunning, gx, gy, gz);	//�����������ѭ�������������һ����ֵ

	if (!gyroBiasRunning.isBiasValueFound)	//���ƫ��ֵû���ҵ�
	{
		sensorsFindBiasValue(&gyroBiasRunning);
		if (gyroBiasRunning.isBiasValueFound)	
		{
			//�����δ�ҵ���������
			;//ledseqRun(SYS_LED, seq_calibrated);
		}
	}

	gyroBiasOut->x = gyroBiasRunning.bias.x;
	gyroBiasOut->y = gyroBiasRunning.bias.y;
	gyroBiasOut->z = gyroBiasRunning.bias.z;

	return gyroBiasRunning.isBiasValueFound;
}

/*������ټƺ�����������*/
void processAccGyroMeasurements(struct Sensor *p_Sensor)
{
	int16_t ax = p_Sensor->mpu6050.acc.x.data;
	int16_t ay = p_Sensor->mpu6050.acc.y.data;
	int16_t az = p_Sensor->mpu6050.acc.z.data;
	int16_t gy = p_Sensor->mpu6050.gyro.x.data;
	int16_t gx = p_Sensor->mpu6050.gyro.y.data;
	int16_t gz = p_Sensor->mpu6050.gyro.z.data;

	static bool gyroBiasFound=false;
	gyroBiasFound = processGyroBias(gx, gy, gz, &gyroBias);	//�������ݷ���
	if (gyroBiasFound)	//���������ƫ��ֵ�ɹ��ҵ������Ҽ��ٶȵ�������������
	{
		processAccScale(ax, ay, az);	/*����accScale*/ 
	}
	
	p_Sensor->mpu6050.gyro.axisTFloat_DEG.axisTF.x = -(gx - gyroBias.x) * SENSORS_DEG_PER_LSB_CFG;	/*��λ ��/s */
	p_Sensor->mpu6050.gyro.axisTFloat_DEG.axisTF.y =  (gy - gyroBias.y) * SENSORS_DEG_PER_LSB_CFG;
	p_Sensor->mpu6050.gyro.axisTFloat_DEG.axisTF.z =  (gz - gyroBias.z) * SENSORS_DEG_PER_LSB_CFG;
	applyAxis3fLpf((lpf2pData*)(&gyroLpf), &p_Sensor->mpu6050.gyro.axisTFloat_DEG);

	p_Sensor->mpu6050.acc.axisTFloat_G.axisTF.x = -(ax) * SENSORS_G_PER_LSB_CFG * accScale;	/*��λ g(9.8m/s^2)*/
	p_Sensor->mpu6050.acc.axisTFloat_G.axisTF.y =  (ay) * SENSORS_G_PER_LSB_CFG * accScale;	/*�������ٶ���������accScale ������������ó�*/
	p_Sensor->mpu6050.acc.axisTFloat_G.axisTF.z =  (az) * SENSORS_G_PER_LSB_CFG * accScale;
	applyAxis3fLpf((lpf2pData*)(&accLpf), &p_Sensor->mpu6050.acc.axisTFloat_G);
}

void SensorThread()
{
	Read_MPU6050();	//��ȡMPU6050���ݵ�sensor�ṹ��
	
	//����MPU6050ԭʼ����
	processAccGyroMeasurements(&sensor);
	
	
}