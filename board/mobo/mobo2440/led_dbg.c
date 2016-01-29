#define GPBCON_T  (*( volatile unsigned int *)0x56000010)

#define LED_A_ON     (1<<10)
#define LED_A_OFF    ~(3<<10)
#define LED_B_ON     (1<<12)
#define LED_B_OFF    ~(3<<12)
#define LED_C_ON     (1<<14)
#define LED_C_OFF    ~(3<<14)
#define LED_D_ON     (1<<16)
#define LED_D_OFF    ~(3<<16)

/** 设置CSB226板的0、1、2三个指示灯的开关状态
 * csb226_set_led: - switch LEDs on or off
 * @param led:   LED to switch (0,1,2)
 * @param state: switch on (1) or off (0)
 */
static void fnst2440_set_led(int led, int state)
{
      switch(led) {
             case 0:
            	 if (state==1) {
            	    GPBCON_T |= LED_A_ON ;
                  } else if (state==0) {
                	  GPBCON_T &= LED_A_OFF ;
                  }
                  break;
             case 1:
            	 if (state==1) {
            		 GPBCON_T |= LED_B_ON ;
                 } else if (state==0) {
                	 GPBCON_T &= LED_B_OFF ;
                 }
                  break;
             case 2:
            	 if (state==1) {
            		 GPBCON_T |= LED_C_ON ;
                  } else if (state==0) {
                	  GPBCON_T &= LED_C_OFF ;
                  }
                  break;
             case 3:
            	 if (state==1) {
            		 GPBCON_T |= LED_D_ON ;
                  } else if (state==0) {
                	  GPBCON_T &= LED_D_OFF ;
                  }
                  break;
             default:
            	 break;
      }

      return;
}
/** 显示启动进度函数，在比较重要的阶段，设置三个灯为亮的状态（1, 5, 15）*/
void show_boot_progress (int status)
{
      switch(status) {
            case  1: fnst2440_set_led(0,1); break;
            case  2: fnst2440_set_led(0,0); break;

            case  3: fnst2440_set_led(1,1); break;
            case  4: fnst2440_set_led(1,0); break;

            case  5: fnst2440_set_led(2,1); break;
            case  6: fnst2440_set_led(2,0); break;

            case  7: fnst2440_set_led(3,1); break;
            case  8: fnst2440_set_led(3,0); break;
            default:
            	break;

      }
      return;
}
