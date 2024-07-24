typedef struct
{
   int motor_setSpd = 0; // motor's setspeed
   // same to void set_wheelx_speed(int set_speed);

   int Motor_ecdSpd = 0; // motor's encoder speed
   // same to int wheel1_speed();

   int Motor_spdDif = 0; // diffenrence of above two

   int Motor_dir = 0; // motor spin direction
                      // 0:stop
                      // 1:forward
                      // 2:backward

   void Motor_Movement()
   {
      // if (Motor_dir == 1)
      // Fun1();
   } // call in every period
   // 3 branches in it

   void Motor_Forward();
   void Motor_Backward();
   void Motor_Stop();
   // branches

   void Op_Motor_Init(); // same to wheelx_init

   void Motor_SetSpeed(int set_value);
   // set speed and drive motor

} OP_MOTOR; // Motor of operation unit

OP_MOTOR motor1;
OP_MOTOR motor2;
#define MOTOR_R motor1;
#define MOTOR_L motor2;
