#include "Copter.h"
#include <GCS_MAVLink/GCS_MAVLink.h>
#define Y 5000
#define X -10000
#define Z 300
#ifdef USERHOOK_INIT
void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
}
#endif

#ifdef USERHOOK_FASTLOOP
void Copter::userhook_FastLoop()
{
    static Location target_loc;
    wp_nav->get_wp_destination(target_loc);
    static int flag = 0;
    target_loc.set_alt_cm(Z, Location::AltFrame::ABOVE_HOME); //установка предельной высоты
    static Vector3f tmp;
    static int f = 0;
    static int i = 0;

    if (copter.current_loc.alt >= Z)
    {
        if (!f)
        {
            gcs().send_text(MAV_SEVERITY_CRITICAL, "up_complete");
            target_loc.lat = copter.current_loc.lat + Y; //установка новой точки
            target_loc.lng = copter.current_loc.lng + X;
            tmp.y = target_loc.lat; //установка точек для перелёта
            tmp.x = target_loc.lng;
            tmp.z = target_loc.alt;

            copter.pos_control->set_pos_target(tmp);
            f = 1;
        }
        //temp = copter.pos_control->get_distance_to_target();
      if (i >= (int)(sqrt(Y * Y + X * X) / 400 * 100))
        {
            gcs().send_text(MAV_SEVERITY_CRITICAL, "start_land");
            set_mode(LAND, MODE_REASON_THROTTLE_LAND_ESCAPE);
            flag = 1;
        }
        ++i;
    }

    if (!wp_nav->set_wp_destination(target_loc)) {
        // failure to set destination can only be because of missing terrain data
        //Log_Write_Error(ERROR_SUBSYSTEM_NAVIGATION, ERROR_CODE_FAILED_TO_SET_DESTINATION);
        // failure is propagated to GCS with NAK
        return;
    }

    mode_guided.auto_yaw.set_mode(AUTO_YAW_HOLD);

    // clear i term when we're taking off
    mode_guided.set_throttle_takeoff();
 
    if (flag == 0)
    {
        set_mode(GUIDED, MODE_REASON_MISSION_END);
    }
    
    motors_output();

    return;
}
#endif

#ifdef USERHOOK_50HZLOOP
void Copter::userhook_50Hz()
{
    // put your 50Hz code here
}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void Copter::userhook_MediumLoop()
{
    // put your 10Hz code here
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
    // put your 1Hz code here
}
#endif

#ifdef USERHOOK_AUXSWITCH
void Copter::userhook_auxSwitch1(uint8_t ch_flag)
{
    // put your aux switch #1 handler here (CHx_OPT = 47)
}

void Copter::userhook_auxSwitch2(uint8_t ch_flag)
{
    // put your aux switch #2 handler here (CHx_OPT = 48)
}

void Copter::userhook_auxSwitch3(uint8_t ch_flag)
{
    // put your aux switch #3 handler here (CHx_OPT = 49)
}
#endif
