#define uint8_t char
typedef struct
{
   uint8_t Sns_cptFlag = 0; // flag of cargo capture,sent from stm
   // 0:no need to capture
   // 1:capture complete
   // 2:ready to capture,do it!
   // 3:lay it dowm!

   uint8_t Sns_algMode = 0; // mode of algorithm
   // 0:capture line
   // 1:avoid obstacle
   // 2:cargo recognition
   // 3:destination recognition

} SNS_OPENMV_DATA_O; // OpenMV of sense unit
// data transmit from stm

typedef struct
{
   uint8_t Sns_turnMode = 0; // determin turningradius level
   // several level of turningradius
   // 0 is straight, next value is inoperatable

   uint8_t Sns_turnDir = 0; // determin turning direction
   // 0:left
   // 1:right

} SNS_OPENMV_DATA_I;

SNS_OPENMV_DATA_I datai;
SNS_OPENMV_DATA_O datao;
#define OPENMV_DATA_I datai
#define OPENMV_DATA_O datao

void Sns_Openmv_SendData();    // send data
void Sns_Openmv_ReceiveData(); // receive data

void Sns_Openmv_SetCommand(); // set command to openmv