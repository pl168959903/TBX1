/*
* ________________________________________________________________________________________________________
* Copyright © 2016-2016 InvenSense Inc. Portions Copyright © 2014-2015 Movea. All rights reserved.
* This software, related documentation and any modifications thereto (collectively “Software”) is subject
* to InvenSense and its licensors' intellectual property rights under U.S. and international copyright and
* other intellectual property rights laws.
* InvenSense and its licensors retain all intellectual property and proprietary rights in and to the Software
* and any use, reproduction, disclosure or distribution of the Software without an express license
* agreement from InvenSense is strictly prohibited.
* ________________________________________________________________________________________________________
*/
#ifdef MEMS_20609

#include "driver/inv_mems_mpu_selftest.h"

#include "driver/inv_mems_defines.h"
#include "driver/inv_mems_base_driver.h"
#include "common/inv_mems_drv_hook.h"
#include "driver/inv_mems_transport.h"

////
extern int a_average[3];
extern int g_average[3];

/* register settings */
#define DEF_SELFTEST_GYRO_SENS          (32768 / 250)
/* wait time before collecting data */
#define MAX_PACKETS                     20
#define SELFTEST_WAIT_TIME              (MAX_PACKETS * 10)
#define DEF_ST_STABLE_TIME              20
#define DEF_GYRO_SCALE                  131
#define DEF_ST_PRECISION                1000
#define DEF_ST_ACCEL_FS_MG              2000UL
#define DEF_ST_SCALE                    32768
#define DEF_ST_TRY_TIMES                2
#define DEF_ST_ACCEL_RESULT_SHIFT       1
#define DEF_ST_SAMPLES                  200

#define DEF_ACCEL_ST_SHIFT_DELTA_MIN    500
#define DEF_ACCEL_ST_SHIFT_DELTA_MAX    1500
#define DEF_GYRO_CT_SHIFT_DELTA         500

#define SENSOR_UP_TIME	30
#define REG_UP_TIME		2

#define DEF_ST_ACCEL_FS_MG         2000UL
#define DEF_ACCEL_ST_SHIFT_DELTA   500
#define ACCEL_ST_AL_MIN ((DEF_ACCEL_ST_AL_MIN * DEF_ST_SCALE \
				 / DEF_ST_ACCEL_FS_MG) * DEF_ST_PRECISION)
#define ACCEL_ST_AL_MAX ((DEF_ACCEL_ST_AL_MAX * DEF_ST_SCALE \
				 / DEF_ST_ACCEL_FS_MG) * DEF_ST_PRECISION)

#define THREE_AXIS               3
#define DEF_ST_MPU6500_ACCEL_LPF        2
#define DEF_SELFTEST_SAMPLE_RATE        0
#define INV_MPU_SAMPLE_RATE_CHANGE_STABLE 50
#define DEF_SELFTEST_6500_ACCEL_FS      (0 << 3)
#define DEF_SELFTEST_GYRO_FS            (0 << 3)
#define DEF_ST_6500_STABLE_TIME         20
#define BIT_ACCEL_OUT           0x08
#define BITS_GYRO_OUT           0x70
#define THREE_AXIS               3
#define DEF_GYRO_WAIT_TIME              10

/* Gyro Offset Max Value (dps) */
#define DEF_GYRO_OFFSET_MAX             20
/* Gyro Self Test Absolute Limits ST_AL (dps) */
#define DEF_GYRO_ST_AL                  60
/* Accel Self Test Absolute Limits ST_AL (mg) */
#define DEF_ACCEL_ST_AL_MIN             225
#define DEF_ACCEL_ST_AL_MAX             675

struct recover_regs {
	uint8_t int_enable;		/* REG_INT_ENABLE */
	uint8_t fifo_en;		/* REG_FIFO_EN */
	uint8_t user_ctrl;		/* REG_USER_CTRL */
	uint8_t config;			/* REG_CONFIG */
	uint8_t gyro_config;	/* REG_GYRO_CONFIG */
	uint8_t accel_config;	/* REG_ACCEL_CONFIG */
	uint8_t accel_config_2;	/* REG_ACCEL_CONFIG_2 */
	uint8_t smplrt_div;		/* REG_SAMPLE_RATE_DIV */
	uint8_t lp_mode;		/* REG_LP_MODE_CTRL */
	uint8_t pwr_mgmt_1;		/* REG_PWR_MGMT_1 */
	uint8_t pwr_mgmt_2;		/* REG_PWR_MGMT_2 */
};

static struct recover_regs saved_regs;

static const uint16_t mpu_st_tb[256] = {
	2620, 2646, 2672, 2699, 2726, 2753, 2781, 2808,
	2837, 2865, 2894, 2923, 2952, 2981, 3011, 3041,
	3072, 3102, 3133, 3165, 3196, 3228, 3261, 3293,
	3326, 3359, 3393, 3427, 3461, 3496, 3531, 3566,
	3602, 3638, 3674, 3711, 3748, 3786, 3823, 3862,
	3900, 3939, 3979, 4019, 4059, 4099, 4140, 4182,
	4224, 4266, 4308, 4352, 4395, 4439, 4483, 4528,
	4574, 4619, 4665, 4712, 4759, 4807, 4855, 4903,
	4953, 5002, 5052, 5103, 5154, 5205, 5257, 5310,
	5363, 5417, 5471, 5525, 5581, 5636, 5693, 5750,
	5807, 5865, 5924, 5983, 6043, 6104, 6165, 6226,
	6289, 6351, 6415, 6479, 6544, 6609, 6675, 6742,
	6810, 6878, 6946, 7016, 7086, 7157, 7229, 7301,
	7374, 7448, 7522, 7597, 7673, 7750, 7828, 7906,
	7985, 8065, 8145, 8227, 8309, 8392, 8476, 8561,
	8647, 8733, 8820, 8909, 8998, 9088, 9178, 9270,
	9363, 9457, 9551, 9647, 9743, 9841, 9939, 10038,
	10139, 10240, 10343, 10446, 10550, 10656, 10763, 10870,
	10979, 11089, 11200, 11312, 11425, 11539, 11654, 11771,
	11889, 12008, 12128, 12249, 12371, 12495, 12620, 12746,
	12874, 13002, 13132, 13264, 13396, 13530, 13666, 13802,
	13940, 14080, 14221, 14363, 14506, 14652, 14798, 14946,
	15096, 15247, 15399, 15553, 15709, 15866, 16024, 16184,
	16346, 16510, 16675, 16842, 17010, 17180, 17352, 17526,
	17701, 17878, 18057, 18237, 18420, 18604, 18790, 18978,
	19167, 19359, 19553, 19748, 19946, 20145, 20347, 20550,
	20756, 20963, 21173, 21385, 21598, 21814, 22033, 22253,
	22475, 22700, 22927, 23156, 23388, 23622, 23858, 24097,
	24338, 24581, 24827, 25075, 25326, 25579, 25835, 26093,
	26354, 26618, 26884, 27153, 27424, 27699, 27976, 28255,
	28538, 28823, 29112, 29403, 29697, 29994, 30294, 30597,
	30903, 31212, 31524, 31839, 32157, 32479, 32804
};

static int inv_save_setting(void)
{
	int result;

	result = inv_read_mems_reg_core(REG_PWR_MGMT_1, 1,
			&saved_regs.pwr_mgmt_1);
	if (result)
		return result;

	/* wake up */
	result = inv_write_single_mems_reg_core(REG_PWR_MGMT_1,
			(saved_regs.pwr_mgmt_1 & ~BIT_SLEEP));
	if (result)
		return result;

	result = inv_read_mems_reg_core(REG_INT_ENABLE, 1,
			&saved_regs.int_enable);
	if (result)
		return result;
	result = inv_read_mems_reg_core(REG_FIFO_EN, 1,
			&saved_regs.fifo_en);
	if (result)
		return result;
	result = inv_read_mems_reg_core(REG_USER_CTRL, 1,
			&saved_regs.user_ctrl);
	if (result)
		return result;
	result = inv_read_mems_reg_core(REG_CONFIG, 1,
			&saved_regs.config);
	if (result)
		return result;
	result = inv_read_mems_reg_core(REG_GYRO_CONFIG, 1,
			&saved_regs.gyro_config);
	if (result)
		return result;
	result = inv_read_mems_reg_core(REG_ACCEL_CONFIG, 1,
			&saved_regs.accel_config);
	if (result)
		return result;
	result = inv_read_mems_reg_core(REG_ACCEL_CONFIG_2, 1,
			&saved_regs.accel_config_2);
	if (result)
		return result;
	result = inv_read_mems_reg_core(REG_SAMPLE_RATE_DIV, 1,
			&saved_regs.smplrt_div);
	if (result)
		return result;
	result = inv_read_mems_reg_core(REG_PWR_MGMT_2, 1,
			&saved_regs.pwr_mgmt_2);
	if (result)
		return result;

	return result;
}

static int inv_recover_setting(void)
{
	int result;
	/* Stop sensors */
	result = inv_write_single_mems_reg_core(REG_PWR_MGMT_2,
			BIT_PWR_ACCEL_STBY | BIT_PWR_GYRO_STBY);
	if (result)
		return result;

	/* Restore sensor configurations */
	result = inv_write_single_mems_reg_core(REG_INT_ENABLE,
			saved_regs.int_enable);
	if (result)
		return result;
	result = inv_write_single_mems_reg_core(REG_FIFO_EN,
			saved_regs.fifo_en);
	if (result)
		return result;
	result = inv_write_single_mems_reg_core(REG_USER_CTRL,
			saved_regs.user_ctrl);
	if (result)
		return result;
	result = inv_write_single_mems_reg_core(REG_CONFIG,
			saved_regs.config);
	if (result)
		return result;
	result = inv_write_single_mems_reg_core(REG_GYRO_CONFIG,
			saved_regs.gyro_config);
	if (result)
		return result;
	result = inv_write_single_mems_reg_core(REG_ACCEL_CONFIG,
			saved_regs.accel_config);
	if (result)
		return result;
	result = inv_write_single_mems_reg_core(REG_ACCEL_CONFIG_2,
			saved_regs.accel_config_2);
	if (result)
		return result;
	result = inv_write_single_mems_reg_core(REG_SAMPLE_RATE_DIV,
			saved_regs.smplrt_div);
	if (result)
		return result;
	result = inv_write_single_mems_reg_core(REG_PWR_MGMT_1,
			saved_regs.pwr_mgmt_1);
	if (result)
		return result;

	result = inv_write_single_mems_reg_core(REG_PWR_MGMT_2,
			saved_regs.pwr_mgmt_2);
	if (result)
		return result;


	return result;
}

int inv_switch_engine(int en, uint32_t mask)
{
	uint8_t data, mgmt_1;
	int result;

	if (BIT_PWR_GYRO_STBY == mask) {
		result = inv_read_mems_reg_core(REG_PWR_MGMT_1, 1, &mgmt_1);
		if (result)
			return result;
		mgmt_1 &= ~BIT_CLK_MASK;
	}

	if ((BIT_PWR_GYRO_STBY == mask) && (!en)) {
		result = inv_write_single_mems_reg_core(REG_PWR_MGMT_1, mgmt_1);
		if (result)
			return result;
	}

	result = inv_read_mems_reg_core(REG_PWR_MGMT_2, 1, &data);
	if (result)
		return result;
	if (en)
		data &= (~mask);
	else
		data |= mask;
	result = inv_write_single_mems_reg_core(REG_PWR_MGMT_2, data);
	if (result)
		return result;

	if ((BIT_PWR_GYRO_STBY == mask) && en) {
		/* only gyro on needs sensor up time */
		inv_sleep(SENSOR_UP_TIME);
		/* after gyro is on & stable, switch internal clock to PLL */
		mgmt_1 |= BIT_CLK_PLL;
		result = inv_write_single_mems_reg_core(REG_PWR_MGMT_1, mgmt_1);
		if (result)
			return result;
	}
	if ((BIT_PWR_ACCEL_STBY == mask) && en)
		inv_sleep(REG_UP_TIME);

	return 0;
}

int inv_set_offset_reg(int reg, int val)
{
	int result;
	uint8_t d;

	d = ((val >> 8) & 0xff);
	result = inv_write_single_mems_reg_core(reg, d);
	if (result)
		return result;

	d = (val & 0xff);
	result = inv_write_single_mems_reg_core(reg + 1, d);

	return result;
}

/**
* inv_check_gyro_self_test() - check gyro self test. this function
*                                   returns zero as success. A non-zero return
*                                   value indicates failure in self test.
*  @*reg_avg: average value of normal test.
*  @*st_avg:  average value of self test
*/
int inv_check_gyro_self_test(int *reg_avg, int *st_avg) {
	uint8_t regs[3];
	int ret_val, result;
	int otp_value_zero = 0;
	int st_shift_prod[3], st_shift_cust[3], i;

	ret_val = 0;
	result = inv_read_mems_reg_core(REG_6500_XG_ST_DATA, 3, regs);
	if (result)
		return result;

	for (i = 0; i < 3; i++) {
		if (regs[i] != 0) {
			st_shift_prod[i] = mpu_st_tb[regs[i] - 1];
		} else {
			st_shift_prod[i] = 0;
			otp_value_zero = 1;
		}
	}

	for (i = 0; i < 3; i++) {
		st_shift_cust[i] = st_avg[i] - reg_avg[i];
		if (!otp_value_zero) {
			/* Self Test Pass/Fail Criteria A */
			if (st_shift_cust[i] < DEF_GYRO_CT_SHIFT_DELTA
						* st_shift_prod[i])
					ret_val = 1;
		} else {
			/* Self Test Pass/Fail Criteria B */
			if (st_shift_cust[i] < DEF_GYRO_ST_AL *
						DEF_SELFTEST_GYRO_SENS *
						DEF_ST_PRECISION)
				ret_val = 1;
		}
	}

	if (ret_val == 0) {
		/* Self Test Pass/Fail Criteria C */
		for (i = 0; i < 3; i++)
			if (abs(reg_avg[i]) > DEF_GYRO_OFFSET_MAX *
						DEF_SELFTEST_GYRO_SENS *
						DEF_ST_PRECISION)
				ret_val = 1;
	}

	return ret_val;
}

/**
* inv_check_accel_self_test() - check 6500 accel self test. this function
*                                   returns zero as success. A non-zero return
*                                   value indicates failure in self test.
*  @*reg_avg: average value of normal test.
*  @*st_avg:  average value of self test
*/
int inv_check_accel_self_test(int *reg_avg, int *st_avg) {
	int ret_val, result;
	int st_shift_prod[3], st_shift_cust[3], st_shift_ratio[3], i;
	uint8_t regs[3];
	int otp_value_zero = 0;

	ret_val = 0;
	result = inv_read_mems_reg_core(REG_6500_XA_ST_DATA, 3, regs);
	if (result)
		return result;

	for (i = 0; i < 3; i++) {
		if (regs[i] != 0) {
			st_shift_prod[i] = mpu_st_tb[regs[i] - 1];
		} else {
			st_shift_prod[i] = 0;
			otp_value_zero = 1;
		}
	}

	if (!otp_value_zero) {
		/* Self Test Pass/Fail Criteria A */
		for (i = 0; i < 3; i++) {
			st_shift_cust[i] = st_avg[i] - reg_avg[i];
			st_shift_ratio[i] = abs(st_shift_cust[i] /
					st_shift_prod[i] - DEF_ST_PRECISION);
			if (st_shift_ratio[i] > DEF_ACCEL_ST_SHIFT_DELTA)
				ret_val = 1;
		}
	} else {
		/* Self Test Pass/Fail Criteria B */
		for (i = 0; i < 3; i++) {
			st_shift_cust[i] = abs(st_avg[i] - reg_avg[i]);
			if (st_shift_cust[i] < ACCEL_ST_AL_MIN ||
					st_shift_cust[i] > ACCEL_ST_AL_MAX)
				ret_val = 1;
		}
	}

	return ret_val;
}

/*
 *  inv_do_test() - do the actual test of self testing
 */
int inv_do_test(int self_test_flag,
		int *gyro_result, int *accel_result)
{
	int result, i, j, packet_size;
	uint8_t data[BYTES_PER_SENSOR * 2], d;
	int fifo_count, packet_count, ind, s;

	packet_size = BYTES_PER_SENSOR * 2;
	result = inv_write_single_mems_reg_core(REG_INT_ENABLE, 0);
	if (result)
		return result;
	/* disable the sensor output to FIFO */
	result = inv_write_single_mems_reg_core(REG_FIFO_EN, 0);
	if (result)
		return result;
	/* disable fifo reading */
	result = inv_write_single_mems_reg_core(REG_USER_CTRL, 0);
	if (result)
		return result;
	/* clear FIFO */
	result = inv_write_single_mems_reg_core(REG_USER_CTRL, BIT_FIFO_RST);
	if (result)
		return result;
	/* setup parameters */
	result = inv_write_single_mems_reg_core(REG_CONFIG, INV_FILTER_98HZ);
	if (result)
		return result;

	/* config accel LPF register for MPU6500 */
	result = inv_write_single_mems_reg_core(REG_6500_ACCEL_CONFIG2,
					DEF_ST_MPU6500_ACCEL_LPF);
	if (result)
		return result;

	result = inv_write_single_mems_reg_core(REG_SAMPLE_RATE_DIV,
			DEF_SELFTEST_SAMPLE_RATE);
	if (result)
		return result;
	/* wait for the sampling rate change to stabilize */
	inv_sleep(INV_MPU_SAMPLE_RATE_CHANGE_STABLE);
	result = inv_write_single_mems_reg_core(REG_GYRO_CONFIG,
		self_test_flag | DEF_SELFTEST_GYRO_FS);
	if (result)
		return result;

	d = DEF_SELFTEST_6500_ACCEL_FS;
	d |= self_test_flag;
	result = inv_write_single_mems_reg_core(REG_ACCEL_CONFIG, d);
	if (result)
		return result;

	/* wait for the output to get stable */
	inv_sleep(DEF_ST_6500_STABLE_TIME);

	/* enable FIFO reading */
	result = inv_write_single_mems_reg_core(REG_USER_CTRL, BIT_FIFO_EN);
	if (result)
		return result;
	/* enable sensor output to FIFO */
	d = BITS_GYRO_OUT | BIT_ACCEL_OUT;
	for (i = 0; i < THREE_AXIS; i++) {
		gyro_result[i] = 0;
		accel_result[i] = 0;
	}
	s = 0;
	while (s < 200) {
		/* clear FIFO */
		result = inv_write_single_mems_reg_core(REG_USER_CTRL, BIT_FIFO_RST);
		if (result)
			return result;
		/* enable FIFO reading */
		result = inv_write_single_mems_reg_core(REG_USER_CTRL, BIT_FIFO_EN);
		if (result)
			return result;

		result = inv_write_single_mems_reg_core(REG_FIFO_EN, d);
		if (result)
			return result;
		inv_sleep(DEF_GYRO_WAIT_TIME);
		/* result = inv_write_single_mems_reg_core(REG_FIFO_EN, 0);
		if (result)
			return result; */

		result = inv_read_mems_reg_core(REG_FIFO_COUNT_H,
					FIFO_COUNT_BYTE, data);
		if (result)
			return result;
		fifo_count = (data[0] << 8) | (data[1]);
		packet_count = fifo_count / packet_size;
		i = 0;
		while ((i < packet_count) && (s < 200)) {
			short vals[3];
			result = inv_read_mems_reg_core(REG_FIFO_R_W,
				packet_size, data);
			if (result)
				return result;
			ind = 0;

			for (j = 0; j < THREE_AXIS; j++) {
				vals[j] = (short)((data[ind + 2 * j] << 8) | (data[ind + 2 * j + 1]));
				accel_result[j] += vals[j];
			}
			ind += BYTES_PER_SENSOR;

			for (j = 0; j < THREE_AXIS; j++) {
				vals[j] = (short)((data[ind + 2 * j] << 8) | (data[ind + 2 * j + 1]));
				gyro_result[j] += vals[j];
			}
			s++;
			i++;
		}
	}

	for (j = 0; j < THREE_AXIS; j++) {
		accel_result[j] = accel_result[j] / s;
		accel_result[j] *= DEF_ST_PRECISION;
	}
	for (j = 0; j < THREE_AXIS; j++) {
		gyro_result[j] = gyro_result[j] / s;
		gyro_result[j] *= DEF_ST_PRECISION;
	}

	return 0;
}


int inv_power_up_self_test(void)
{
	int result;

	/* make sure no interrupts */
	result = inv_write_single_mems_reg_core(REG_INT_ENABLE, 0);
	if (result)
		return result;

	if (result)
		return result;
	result = inv_switch_engine(1, BIT_PWR_ACCEL_STBY);
	if (result)
		return result;
	result = inv_switch_engine(1, BIT_PWR_GYRO_STBY);
	if (result)
		return result;

	return 0;
}

/*
 *  inv_mems_run_selftest() - main function to do hardware self test
 */
int inv_mems_run_selftest(void)
{
	int result;
	int gyro_bias_st[THREE_AXIS], gyro_bias_regular[THREE_AXIS];
	int accel_bias_st[THREE_AXIS], accel_bias_regular[THREE_AXIS];
	int test_times, i;
	char accel_result, gyro_result;

	result = inv_save_setting();
	if (result)
		return result;

	result = inv_power_up_self_test();
	if (result)
		return result;
	accel_result = 0;
	gyro_result = 0;
	test_times = DEF_ST_TRY_TIMES;
	while (test_times > 0) {
		result = inv_do_test(0, gyro_bias_regular,
			accel_bias_regular);
		if (result)
			test_times--;
		else
			test_times = 0;
	}
	if (result)
		goto test_fail;

	for (i = 0; i < 3; i++) {
		g_average[i] = gyro_bias_regular[i] / DEF_ST_PRECISION;
		a_average[i] = accel_bias_regular[i] / DEF_ST_PRECISION;
	}

	test_times = DEF_ST_TRY_TIMES;
	while (test_times > 0) {
		result = inv_do_test(BITS_SELF_TEST_EN, gyro_bias_st,
					accel_bias_st);
		if (result)
			test_times--;
		else
			break;
	}
	if (result)
		goto test_fail;

	accel_result = !inv_check_accel_self_test(accel_bias_regular, accel_bias_st);
	gyro_result = !inv_check_gyro_self_test(gyro_bias_regular, gyro_bias_st);

test_fail:
	inv_recover_setting();
	return (accel_result << DEF_ST_ACCEL_RESULT_SHIFT) | gyro_result;
}

#endif // MEMS_20609
