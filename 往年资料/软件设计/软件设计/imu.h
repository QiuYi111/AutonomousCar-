#define uint8_t char
typedef struct
{
   uint8_t Imu_angle[3] = {};
   // roll,yaw,pitch
   uint8_t Imu_acc[3] = {};
   // ax,ay,az
   uint8_t Imu_omega[3] = {};
   // wx,wy,wz
} SNS_IMU;
SNS_IMU imu1;
#define IMU imu1

void Imu_GetInf();
// get information
// 3 sub-fun may included