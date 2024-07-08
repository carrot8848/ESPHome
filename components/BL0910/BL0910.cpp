#include "BL0910.h"
#include "esphome/core/log.h"

namespace esphome {
namespace BL0910 {

static const char *const TAG = "BL0910";
uint8_t USR_WRPROT_Witable[6]={0xCA, 0x9E, 0x55, 0x55, 0x00, 0xB7};  //用户寄存器可操作指令
uint8_t USR_WRPROT_Onlyread[6]={0xCA, 0x9E, 0x00, 0x00, 0x00, 0x61};  //用户寄存器只读指令

static const uint8_t BL0910_READ_COMMAND = 0x35;  // 读操作命令
static const uint8_t BL0910_WRITE_COMMAND = 0xCA;  // 写操作命令

// BL0910寄存器地址
static const uint8_t BL0910_I_1_RMS = 0x0C;  //电流 1 通道有效值寄存器地址
static const uint8_t BL0910_I_2_RMS = 0x0D;  //电流 2 通道有效值寄存器地址
static const uint8_t BL0910_I_3_RMS = 0x0E;  //电流 3 通道有效值寄存器地址
static const uint8_t BL0910_I_4_RMS = 0x0F;  //电流 4 通道有效值寄存器地址
static const uint8_t BL0910_I_5_RMS = 0x10;  //电流 5 通道有效值寄存器地址
static const uint8_t BL0910_I_6_RMS = 0x11;  //电流 6 通道有效值寄存器地址
static const uint8_t BL0910_I_7_RMS = 0x12;  //电流 7 通道有效值寄存器地址
static const uint8_t BL0910_I_8_RMS = 0x13;  //电流 8 通道有效值寄存器地址
static const uint8_t BL0910_I_9_RMS = 0x14;  //电流 9 通道有效值寄存器地址
static const uint8_t BL0910_I_10_RMS = 0x15;  //电流 10 通道有效值寄存器地址
static const uint8_t BL0910_V_RMS = 0x16;     //电压通道有效值寄存器
static const uint8_t BL0910_WATT_1 = 0X22;    //通道 1 有功功率寄存器
static const uint8_t BL0910_WATT_2 = 0X23;    //通道 2 有功功率寄存器
static const uint8_t BL0910_WATT_3 = 0X24;    //通道 3 有功功率寄存器
static const uint8_t BL0910_WATT_4 = 0X25;    //通道 4 有功功率寄存器
static const uint8_t BL0910_WATT_5 = 0X26;    //通道 5 有功功率寄存器
static const uint8_t BL0910_WATT_6 = 0X27;    //通道 6 有功功率寄存器
static const uint8_t BL0910_WATT_7 = 0X28;    //通道 7 有功功率寄存器
static const uint8_t BL0910_WATT_8 = 0X29;    //通道 8 有功功率寄存器
static const uint8_t BL0910_WATT_9 = 0X2A;    //通道 9 有功功率寄存器
static const uint8_t BL0910_WATT_10 = 0X2B;    //通道 10 有功功率寄存器
static const uint8_t BL0910_WATT_SUM = 0X2C;       //总有功功率寄存器
static const uint8_t BL0910_CF_1_CNT = 0X2F;  //通道 1 有功脉冲计数，无符号
static const uint8_t BL0910_CF_2_CNT = 0X30;  //通道 2 有功脉冲计数，无符号
static const uint8_t BL0910_CF_3_CNT = 0X31;  //通道 3 有功脉冲计数，无符号
static const uint8_t BL0910_CF_4_CNT = 0X32;  //通道 4 有功脉冲计数，无符号
static const uint8_t BL0910_CF_5_CNT = 0X33;  //通道 5 有功脉冲计数，无符号
static const uint8_t BL0910_CF_6_CNT = 0X34;  //通道 6 有功脉冲计数，无符号
static const uint8_t BL0910_CF_7_CNT = 0X35;  //通道 7 有功脉冲计数，无符号
static const uint8_t BL0910_CF_8_CNT = 0X36;  //通道 8 有功脉冲计数，无符号
static const uint8_t BL0910_CF_9_CNT = 0X37;  //通道 9 有功脉冲计数，无符号
static const uint8_t BL0910_CF_10_CNT = 0X38;  //通道 10 有功脉冲计数，无符号
static const uint8_t BL0910_CF_SUM_CNT = 0X39;     //总有功脉冲计数，无符号
static const uint8_t BL0910_FREQUENCY = 0X4E;     //线电压频率周期，无符号
static const uint8_t BL0910_TEMPERATURE = 0X5E;     //内部温度值，无符号  
//校正寄存器
static const uint8_t BL0910_RMSGN_1 = 0x6C;  //有效值增益调整寄存器地址,通道1
static const uint8_t BL0910_RMSGN_2 = 0x6D;  //有效值增益调整寄存器地址,通道2
static const uint8_t BL0910_RMSGN_3 = 0x6E;  //有效值增益调整寄存器地址,通道3
static const uint8_t BL0910_RMSGN_4 = 0x6F;  //有效值增益调整寄存器地址,通道4
static const uint8_t BL0910_RMSGN_5 = 0x70;  //有效值增益调整寄存器地址,通道5
static const uint8_t BL0910_RMSGN_6 = 0x71;  //有效值增益调整寄存器地址,通道6
static const uint8_t BL0910_RMSGN_7 = 0x72;  //有效值增益调整寄存器地址,通道7
static const uint8_t BL0910_RMSGN_8 = 0x73;  //有效值增益调整寄存器地址,通道8
static const uint8_t BL0910_RMSGN_9 = 0x74;  //有效值增益调整寄存器地址,通道9
static const uint8_t BL0910_RMSGN_10 = 0x75;  //有效值增益调整寄存器地址,通道10
static const uint8_t BL0910_RMSOS_1 = 0x77;  //有效值偏置校正寄存器地址,通道1
static const uint8_t BL0910_RMSOS_2 = 0x78;  //有效值偏置校正寄存器地址,通道2
static const uint8_t BL0910_RMSOS_3 = 0x79;  //有效值偏置校正寄存器地址,通道3
static const uint8_t BL0910_RMSOS_4 = 0x7A;  //有效值偏置校正寄存器地址,通道4
static const uint8_t BL0910_RMSOS_5 = 0x7B;  //有效值偏置校正寄存器地址,通道5
static const uint8_t BL0910_RMSOS_6 = 0x7C;  //有效值偏置校正寄存器地址,通道6  
static const uint8_t BL0910_RMSOS_7 = 0x7D;  //有效值偏置校正寄存器地址,通道7 
static const uint8_t BL0910_RMSOS_8 = 0x7E;  //有效值偏置校正寄存器地址,通道8 
static const uint8_t BL0910_RMSOS_9 = 0x7F;  //有效值偏置校正寄存器地址,通道9 
static const uint8_t BL0910_RMSOS_10 = 0x80;  //有效值偏置校正寄存器地址,通道10 
static const uint8_t BL0910_WATTGN_1 = 0xB6; //有功功率增益调整寄存器,通道1
static const uint8_t BL0910_WATTGN_2 = 0xB7; //有功功率增益调整寄存器,通道2
static const uint8_t BL0910_WATTGN_3 = 0xB8; //有功功率增益调整寄存器,通道3
static const uint8_t BL0910_WATTGN_4 = 0xB9; //有功功率增益调整寄存器,通道4
static const uint8_t BL0910_WATTGN_5 = 0xBA; //有功功率增益调整寄存器,通道5
static const uint8_t BL0910_WATTGN_6 = 0xBB; //有功功率增益调整寄存器,通道6  
static const uint8_t BL0910_WATTGN_7 = 0xBC; //有功功率增益调整寄存器,通道7 
static const uint8_t BL0910_WATTGN_8 = 0xBD; //有功功率增益调整寄存器,通道8 
static const uint8_t BL0910_WATTGN_9 = 0xBE; //有功功率增益调整寄存器,通道9 
static const uint8_t BL0910_WATTGN_10 = 0xBF; //有功功率增益调整寄存器,通道10 
  
void BL0910::loop() {
//
}

void BL0910::setup() {
//此处可校正BL0910电参数。    
while (this->available())
this->flush();
this-> write_array(USR_WRPROT_Witable,sizeof(USR_WRPROT_Witable));//用户寄存器可操作指令
  //电参数校正（参数1：寄存器地址；参数2：校正前值；参数3：校正后值、电参数值与寄存器值的转换系数)
Bias_correction(BL0910_RMSOS_1, 0.04200, 0);//电流有效值偏置校正，通道1
Bias_correction(BL0910_RMSOS_2, 0.01400, 0);//电流有效值偏置校正，通道2
Bias_correction(BL0910_RMSOS_3, 0.01200, 0);//电流有效值偏置校正，通道3
Bias_correction(BL0910_RMSOS_4, 0.01000, 0);//电流有效值偏置校正，通道4
Bias_correction(BL0910_RMSOS_5, 0.00900, 0);//电流有效值偏置校正，通道5
Bias_correction(BL0910_RMSOS_6, 0.00900, 0);//电流有效值偏置校正，通道6
Bias_correction(BL0910_RMSOS_7, 0.00900, 0);//电流有效值偏置校正，通道7
Bias_correction(BL0910_RMSOS_8, 0.01000, 0);//电流有效值偏置校正，通道8
Bias_correction(BL0910_RMSOS_9, 0.01000, 0);//电流有效值偏置校正，通道9
Bias_correction(BL0910_RMSOS_10, 0.01000, 0);//电流有效值偏置校正，通道10
gain_correction(BL0910_RMSGN_1, 0.239, 0.210, BL0910_ki);//电流有效值增益调整，通道1
gain_correction(BL0910_RMSGN_2, 0.239, 0.210, BL0910_ki);//电流有效值增益调整，通道2
gain_correction(BL0910_RMSGN_3, 0.239, 0.210, BL0910_ki);//电流有效值增益调整，通道3
gain_correction(BL0910_RMSGN_4, 0.239, 0.210, BL0910_ki);//电流有效值增益调整，通道4
gain_correction(BL0910_RMSGN_5, 0.239, 0.210, BL0910_ki);//电流有效值增益调整，通道5
gain_correction(BL0910_RMSGN_6, 0.239, 0.210, BL0910_ki);//电流有效值增益调整，通道6
gain_correction(BL0910_RMSGN_7, 0.239, 0.210, BL0910_ki);//电流有效值增益调整，通道7
gain_correction(BL0910_RMSGN_8, 0.239, 0.210, BL0910_ki);//电流有效值增益调整，通道8
gain_correction(BL0910_RMSGN_9, 0.239, 0.210, BL0910_ki);//电流有效值增益调整，通道9
gain_correction(BL0910_RMSGN_10, 0.239, 0.210, BL0910_ki);//电流有效值增益调整，通道10
//gain_correction(BL0910_WATTGN_1, 15.13427, 14.5, BL0910_Kp);//功率增益调整，通道1
//gain_correction(BL0910_WATTGN_2, 15.23937, 14.5, BL0910_Kp);//功率增益调整，通道2
//gain_correction(BL0910_WATTGN_3, 15.44956, 14.5, BL0910_Kp);//功率增益调整，通道3
//gain_correction(BL0910_WATTGN_4, 16.57646, 14.5, BL0910_Kp);//功率增益调整，通道4
//gain_correction(BL0910_WATTGN_5, 15.27440, 14.5, BL0910_Kp);//功率增益调整，通道5
//gain_correction(BL0910_WATTGN_6, 31.75744, 14.5, BL0910_Kp);//功率增益调整，通道6
//gain_correction(BL0910_WATTGN_7, 31.75744, 14.5, BL0910_Kp);//功率增益调整，通道7
//gain_correction(BL0910_WATTGN_8, 31.75744, 14.5, BL0910_Kp);//功率增益调整，通道8
//gain_correction(BL0910_WATTGN_9, 31.75744, 14.5, BL0910_Kp);//功率增益调整，通道9
//gain_correction(BL0910_WATTGN_10, 31.75744, 14.5, BL0910_Kp);//功率增益调整，通道10
this-> write_array(USR_WRPROT_Onlyread,sizeof(USR_WRPROT_Onlyread));//用户寄存器只读指令
}


void BL0910::update() {
  while (this->available())
  this->flush();
  read_data(BL0910_FREQUENCY, frequency_reference_, frequency_sensor_ ); //频率
  read_data(BL0910_TEMPERATURE, temperature_reference_, temperature_sensor_); //温度
  read_data(BL0910_V_RMS, voltage_reference_, voltage_sensor_); //电压
  read_data(BL0910_I_1_RMS, current_reference_, current_sensor_1_ ); //电流,通道1
  read_data(BL0910_I_2_RMS, current_reference_, current_sensor_2_ ); //电流,通道2
  read_data(BL0910_I_3_RMS, current_reference_, current_sensor_3_ ); //电流,通道3
  read_data(BL0910_I_4_RMS, current_reference_, current_sensor_4_ ); //电流,通道4
  read_data(BL0910_I_5_RMS, current_reference_, current_sensor_5_ ); //电流,通道5
  read_data(BL0910_I_6_RMS, current_reference_, current_sensor_6_ ); //电流,通道6
  read_data(BL0910_I_7_RMS, current_reference_, current_sensor_7_ ); //电流,通道7
  read_data(BL0910_I_8_RMS, current_reference_, current_sensor_8_ ); //电流,通道8
  read_data(BL0910_I_9_RMS, current_reference_, current_sensor_9_ ); //电流,通道9
  read_data(BL0910_I_10_RMS, current_reference_, current_sensor_10_ ); //电流,通道10
  read_data(BL0910_WATT_1, power_reference_, power_sensor_1_ );//功率。通道1 
  read_data(BL0910_WATT_2, power_reference_, power_sensor_2_ );//功率。通道2 
  read_data(BL0910_WATT_3, power_reference_, power_sensor_3_ );//功率。通道3 
  read_data(BL0910_WATT_4, power_reference_, power_sensor_4_ );//功率。通道4 
  read_data(BL0910_WATT_5, power_reference_, power_sensor_5_ );//功率。通道5 
  read_data(BL0910_WATT_6, power_reference_, power_sensor_6_ );//功率。通道6
  read_data(BL0910_WATT_7, power_reference_, power_sensor_7_ );//功率。通道7 
  read_data(BL0910_WATT_8, power_reference_, power_sensor_8_ );//功率。通道8 
  read_data(BL0910_WATT_9, power_reference_, power_sensor_9_ );//功率。通道9 
  read_data(BL0910_WATT_10, power_reference_, power_sensor_10_ );//功率。通道10   
  read_data(BL0910_WATT_SUM, sum_power_reference_, power_sensor_sum_ );//功率。通道总和 
  read_data(BL0910_CF_1_CNT, energy_reference_, energy_sensor_1_ ); //电量。通道1
  read_data(BL0910_CF_2_CNT, energy_reference_, energy_sensor_2_ ); //电量。通道2
  read_data(BL0910_CF_3_CNT, energy_reference_, energy_sensor_3_ ); //电量。通道3
  read_data(BL0910_CF_4_CNT, energy_reference_, energy_sensor_4_ ); //电量。通道4
  read_data(BL0910_CF_5_CNT, energy_reference_, energy_sensor_5_ ); //电量。通道5
  read_data(BL0910_CF_6_CNT, energy_reference_, energy_sensor_6_ ); //电量。通道6
  read_data(BL0910_CF_7_CNT, energy_reference_, energy_sensor_7_ ); //电量。通道7
  read_data(BL0910_CF_8_CNT, energy_reference_, energy_sensor_8_ ); //电量。通道8
  read_data(BL0910_CF_9_CNT, energy_reference_, energy_sensor_9_ ); //电量。通道9
  read_data(BL0910_CF_10_CNT, energy_reference_, energy_sensor_10_ ); //电量。通道10
  read_data(BL0910_CF_SUM_CNT, sum_energy_reference_, energy_sensor_sum_ ); //电量。通道总和
}

  //校验和。SUM 字节为（Addr+Data_L+Data_M+Data_H）&0xFF 取反
uint8_t BL0910_checksum(const uint8_t address, const DataPacket *data) {
  return (address + data->l + data->m + data->h) ^ 0xFF; 
}
  
//读取数据
void BL0910::read_data(const uint8_t address, const float reference, sensor::Sensor *sensor_) {
  DataPacket buffer;
  ube24_t data_u24;
  sbe24_t data_s24;
  float value=0;
  //读取功率
  if (reference == power_reference_){
    this-> write_byte(BL0910_READ_COMMAND);
    this-> write_byte(address);
    if (this-> read_array((uint8_t *) &buffer, sizeof(buffer)-1))  {      
      if (BL0910_checksum(address,&buffer)==buffer.checksum) {
        data_s24.l = buffer.l;
        data_s24.m = buffer.m;
        data_s24.h = buffer.h;
        value = (float) to_int32_t(data_s24) * reference;
        if (sensor_ != nullptr) {
          sensor_->publish_state(value);
        }      
      }
    } else {
      ESP_LOGW(TAG, "Junk on wire. Throwing away partial power message");
      while (read() >= 0)
        ;
    } 
  }

  //读取 总功率
  if (reference == sum_power_reference_){
    this-> write_byte(BL0910_READ_COMMAND);
    this-> write_byte(address);
    if (this-> read_array((uint8_t *) &buffer, sizeof(buffer)-1))  {      
      if (BL0910_checksum(address,&buffer)==buffer.checksum) {
        data_s24.l = buffer.l;
        data_s24.m = buffer.m;
        data_s24.h = buffer.h;
        value = (float) to_int32_t(data_s24) * reference;
        if (sensor_ != nullptr) {
          sensor_->publish_state(value);
        }      
      }
    } else {
      ESP_LOGW(TAG, "Junk on wire. Throwing away partial power message");
      while (read() >= 0)
        ;
    } 
  }

  //读取电压、电流、电量、总电量
  if (reference == voltage_reference_ || reference == current_reference_ || reference == energy_reference_ || reference == sum_energy_reference_) {
    this-> write_byte(BL0910_READ_COMMAND);
    this-> write_byte(address);
    if (this-> read_array((uint8_t *) &buffer, sizeof(buffer)-1))  {    
    //ESP_LOGW(TAG, "checksum= 0x%02X", BL0910_checksum(BL0910_V_RMS,&buffer));      
      if (BL0910_checksum(address,&buffer)==buffer.checksum) {
        data_u24.l = buffer.l;
        data_u24.m = buffer.m;
        data_u24.h = buffer.h;
        value = (float) to_uint32_t(data_u24) * reference;
        if (sensor_ != nullptr) {
          sensor_->publish_state(value);
        }      
      }
    } else {
      ESP_LOGW(TAG, "Junk on wire. Throwing away partial message");
      while (read() >= 0)
        ;
    }     
  }  
    //读取频率
  if (reference == frequency_reference_) {
    this-> write_byte(BL0910_READ_COMMAND);
    this-> write_byte(address);
    if (this-> read_array((uint8_t *) &buffer, sizeof(buffer)-1))  {    
      if (BL0910_checksum(address,&buffer)==buffer.checksum) {
        data_u24.l = buffer.l;
        data_u24.m = buffer.m;
        data_u24.h = buffer.h;
        value = reference / (float) to_uint32_t(data_u24);
        if (sensor_ != nullptr) {
          sensor_->publish_state(value);
        }      
      }
    } else {
      ESP_LOGW(TAG, "Junk on wire. Throwing frequency message");
      while (read() >= 0)
        ;
    }     
  }
    //读取芯片温度
  if (reference == temperature_reference_) {
    this-> write_byte(BL0910_READ_COMMAND);
    this-> write_byte(address);
    if (this-> read_array((uint8_t *) &buffer, sizeof(buffer)-1))  {    
      if (BL0910_checksum(address,&buffer)==buffer.checksum) {
        data_s24.l = buffer.l;
        data_s24.m = buffer.m;
        data_s24.h = buffer.h;
        value = (float) to_int32_t(data_s24);
        value = (value - 64) *12.5/59-40;  
        if (sensor_ != nullptr) {
          sensor_->publish_state(value);
        }      
      }
    } else {
      ESP_LOGW(TAG, "Junk on wire. Throwing frequency message");
      while (read() >= 0)
        ;
    }     
  }  
}  
//有效值偏置校正
void BL0910::Bias_correction(const uint8_t address, const float measurements, const float Correction) {
  DataPacket data;  
  float ki = 12875*1*(5.1+5.1)*1000/2000/1.097; //电流系数
  float I_RMS0 = measurements * ki;
  float I_RMS = Correction * ki;
  int32_t value = (I_RMS*I_RMS-I_RMS0*I_RMS0) / 256; 
  data.l = value << 24 >>24;
  data.m = value << 16 >>24;
  if (value < 0) {
    data.h = (value << 8 >>24)|0b10000000;    
  }  
  data.address = BL0910_checksum(address, &data);
  ESP_LOGW(TAG, "RMSOS:%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",BL0910_WRITE_COMMAND,address,data.l ,data.m, data.h,data.address );
  this-> write_byte(BL0910_WRITE_COMMAND);
  this-> write_byte(address);
  this-> write_byte(data.l);
  this-> write_byte(data.m);
  this-> write_byte(data.h);
  this-> write_byte(data.address);
}  
//增益调整
void BL0910::gain_correction(const uint8_t address, const float measurements, const float Correction, const float coefficient) {
  DataPacket data;  
  float I_RMS0 = measurements * coefficient;
  float I_RMS = Correction * coefficient;
  float rms_gn = int((I_RMS/I_RMS0-1) * 65536);
  int16_t value;
  if (rms_gn <= -32767){
    value = -32767;
  } else {
    value = int(rms_gn);  
  }  
  data.h = 0xFF;
  data.m = value >> 8;
  data.l = value << 8 >>8;  
  data.address = BL0910_checksum(address, &data);
  //ESP_LOGW(TAG, "RMSOS:%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",BL0910_WRITE_COMMAND,address,data.l ,data.m, data.h,data.address );
  this-> write_byte(BL0910_WRITE_COMMAND);
  this-> write_byte(address);
  this-> write_byte(data.l);
  this-> write_byte(data.m);
  this-> write_byte(data.h);
  this-> write_byte(data.address);
}    

  //void BL0910::dump_config() {  // NOLINT(readability-function-cognitive-complexity)
  //ESP_LOGCONFIG(TAG, "BL0910:");
  //LOG_SENSOR("", "Voltage", this->voltage_sensor_);
//}

uint32_t BL0910::to_uint32_t(ube24_t input) { return input.h << 16 | input.m << 8 | input.l; }

int32_t BL0910::to_int32_t(sbe24_t input) { return input.h << 16 | input.m << 8 | input.l; }

}  // namespace BL0910
}  // namespace esphome
