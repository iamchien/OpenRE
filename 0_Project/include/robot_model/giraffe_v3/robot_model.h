/***********************************************************************************************************************
* Copyright (c) Hands Free Team. All rights reserved.
* Contact:  QQ Exchange Group -- 521037187
*
* LICENSING TERMS:
* The Hands Free is licensed generally under a permissive 3-clause BSD license.
* Contributions are requiredto be made under the same license.
*
* History:
* <author>      <time>      <version>      <desc>
*
* Description:
***********************************************************************************************************************/

#ifndef ROBOT_MODEL_H
#define ROBOT_MODEL_H

#include "robot_abstract.h"

class RobotModel : public RobotParameters
{
public:
    RobotModel() : RobotParameters()
    {

        strcpy(robot_info.robot_name, "giraffe_v3");
        strcpy(robot_info.robot_description ,  "this is a differential2 robot of handsfree");

        system_para.battery_series = 6; //series number of batteries
        system_para.battery_voltage_alarm_ = 20.4; //low voltage alarm(V)

        motor_para.driver_type = MotorDriver_TDE124;
        motor_para.motor_enable_num = 0;
        motor_para.simulation_model = 0;
        motor_para.pid_t = 0.02;
        motor_para.encoder_num  = 0;
        motor_para.pwm_max = 0;
        motor_para.pwm_dead_zone = 0;
        motor_para.speed_low_filter = 0;
        motor_para.protect_current = 0;  // 200A means disable current  protect
        motor_para.static_damping_coefficient = 0;  //0~0.3;  default = 0.05
        motor_para.pid =  {0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f};

        chassis_para.type = DIFFERENTIAL2;
        chassis_para.wheel_radius = 0.085;
        chassis_para.body_radius = 0.1706;
        chassis_para.speed_low_filter = 0.4;
        chassis_para.imu_fusion_enalbe = 0;
        chassis_para.control_enable = 1;
        chassis_para.max_speed_limit.x = 1.2;

        head_para.type = HFANALOG;
        head_para.speed_low_filter = 0.3;
        head_para.range.pitch = 50 * degree_to_radian;
        head_para.range.yaw = 70 * degree_to_radian;
        head_para.offset.pitch = 0 *degree_to_radian;
        head_para.offset.yaw = 0 * degree_to_radian;
        head_para.id.pitch = 7;
        head_para.id.yaw = 8;
        head_para.control_enable = 1;

        arm_para.type = DOBOT2;
        arm_para.speed_low_filter = 1;
        arm_para.dof = 4;
        arm_para.control_enable = 1;
    }
};

#endif
