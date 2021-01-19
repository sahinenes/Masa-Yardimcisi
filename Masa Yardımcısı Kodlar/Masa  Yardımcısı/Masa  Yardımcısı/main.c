/*
 * Masa  Yardımcısı.c
 *
 * Created: 10.01.2021 16:51:10
 * Author : Enes
 
 
 
 https://www.avrfreaks.net/projects/ds1302-timekeeping-chip-library ds1302 library
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include "lcd1602.h"
#include "ds1302.h"
#include "DHT11sensor v1.0.h"
#include "Keypad.h"
#include <util/delay.h>
#include <stdlib.h>




#define Delay_ns(__ns) \
if((unsigned long) (F_CPU/1000000000.0 * __ns) != F_CPU/1000000000.0 * __ns)\
__builtin_avr_delay_cycles((unsigned long) ( F_CPU/1000000000.0 * __ns)+1);\
else __builtin_avr_delay_cycles((unsigned long) ( F_CPU/1000000000.0 * __ns))

struct ymdhms_type mytime;

uint8_t saat_hazir;///saat kontrol

char str[50];




int8_t DHTreturnCode; //siccaklik sensoru
uint8_t alarm_kur;
uint8_t alarm_durum;
uint8_t say_;
uint8_t alarm_cal;
char saat1_;
char saat2_;
char dakika1_;
char dakika2_;




 unsigned char alarm_saat_1;
 unsigned char alarm_saat_2;
 unsigned char alarm_dakika_1;
 unsigned char alarm_dakika_2;
 
 unsigned int alarm_saat_int_1;
 unsigned int alarm_saat_int_2;
 unsigned int alarm_dakika_int_1;
 unsigned int alarm_dakika_int_2;
 
volatile char key;




int main(void)
{
	
	DDRB|=(1<<PORTB4);
	PORTB&=~(1<<PORTB4);
	
	
	///keypad
		
		key = Key_None;	
		alarm_durum=0;
		alarm_kur=0;
		say_=0;
		saat1_=0;
		saat2_=0;
		dakika1_=0;
		dakika2_=0;
		alarm_cal=0;
		
		KP_Setup();
		
	////
	


	///lcd
	
	lcd1602_init();
	lcd1602_clear();
    ///

   
	
  
	///saat
	
	if (ds1302_init())
	{
		//saat hazır
		saat_hazir=1;


	}
	else
	{
		//saat hazır değil bekle
		saat_hazir=0;
		
	}

   ////
   
    while (1) 
    {
		if (saat_hazir==1)//saat hazır ise
		{
			


		


		 
			
			if (alarm_durum!=1)
			{
				DHTreturnCode = DHT11ReadData(); ///dht11 oku
			
		
			
		
			if (ds1302_get_time(&mytime))
			{
				//zaman var
				
				lcd1602_clear();
			    Delay_ns(1500); 
				
				lcd1602_goto_xy(0,0);
				itoa(mytime.hour1,str,10);
				lcd1602_send_string(str);
				itoa(mytime.hour2,str,10);
				lcd1602_send_string(str);
				lcd1602_send_string(":");
				itoa(mytime.minute1,str,10);
				lcd1602_send_string(str);
				itoa(mytime.minute2,str,10);
				lcd1602_send_string(str);
				lcd1602_send_string("   ");
			
				itoa(mytime.day,str,10);
				lcd1602_send_string(str);
				lcd1602_send_string("/");
				itoa(mytime.month,str,10);
				lcd1602_send_string(str);
				lcd1602_send_string("/");
				itoa(mytime.year,str,10);
				lcd1602_send_string(str);
				

				
			
				
			}
			else
			{
				ds1302_reset_time();
				//zaman yok
				mytime.day=10;
				mytime.month=1;
				mytime.year=21;
				mytime.hour1=1;
				mytime.hour2=3;
				mytime.minute1=5;
				mytime.minute2=9;
				mytime.second=0;
				ds1302_set_time(&mytime);
			}
                
				////sicaklik ve nem goster
			
					lcd1602_goto_xy(0,1);
					itoa(DHT11DisplayTemperature(),str,10);
					lcd1602_send_string(str);
					lcd1602_send_string(" C ");
						
					itoa(DHT11DisplayHumidity(),str,10);
					lcd1602_send_string(str);
					lcd1602_send_string(" % ");
				
			
				

				/////
			}
				
				///Keypad
				         key=KP_GetKey();
				


				      
						if(key=='A'&&alarm_durum!=2)
						{  //alarm kuruluyor
							lcd1602_clear();
							alarm_durum=1;
							alarm_kur=1;
							say_=1;
						}
						if(key=='B')
						{   //alarm var
							lcd1602_clear();
							alarm_durum=2;
							alarm_kur=0;
						}
						if(key=='C')
						{   //alarm yok
							lcd1602_clear();
							alarm_durum=0;
							alarm_kur=0;
							alarm_cal=0;
						}
						
				        if (alarm_durum==0)
				        {  
					        lcd1602_goto_xy(10,1);
					        lcd1602_send_string("YOK");
				        }
				        if (alarm_durum==1)
				        {  
					        lcd1602_goto_xy(0,1);
					        lcd1602_send_string("KURULUYOR");
				        }
						if (alarm_durum==2)
						{
							
 
		                    alarm_saat_1=(unsigned char)mytime.hour1;
							alarm_saat_2=(unsigned char)mytime.hour2;
							alarm_dakika_1=(unsigned char)mytime.minute1;
							alarm_dakika_2=(unsigned char)mytime.minute2;
							
							alarm_saat_int_1=alarm_saat_1+48;
							alarm_saat_int_2=alarm_saat_2+48;
							alarm_dakika_int_1=alarm_dakika_1+48;
							alarm_dakika_int_2=alarm_dakika_2+48;
							
							
							if (alarm_saat_int_1==saat1_&&alarm_saat_int_2==saat2_&&alarm_dakika_int_1==dakika1_&&alarm_dakika_int_2==dakika2_)
							{
								alarm_cal=1;
								

							}
							else
							{
								lcd1602_goto_xy(12,1);
								lcd1602_send_string(":");
								lcd1602_goto_xy(10,1);
								lcd1602_send_char(saat1_);
								lcd1602_goto_xy(11,1);
								lcd1602_send_char(saat2_);
								lcd1602_goto_xy(13,1);
								lcd1602_send_char(dakika1_);
								lcd1602_goto_xy(14,1);
								lcd1602_send_char(dakika2_);
								lcd1602_goto_xy(14,1);
							}
							
							if (alarm_cal==1)
							{
								lcd1602_goto_xy(10,1);
								lcd1602_send_string("UYAN");
								PORTB|=(1<<PORTB4); //buzzer çal
								_delay_ms(1000);
								PORTB&=~(1<<PORTB4);
								_delay_ms(1000);
								
									
							}
							if (alarm_cal==0)
							{
							}
							{
								PORTB&=~(1<<PORTB4);
							}
							
							/*
						    unsigned char c=(unsigned char)mytime.hour1;
							unsigned int e=c+48;
							unsigned char d=saat1_;
							
							*/

						
							
						}
						
						
						if(alarm_kur==1 && key!='A' && key!='B' && key!='C'&& key!='D'&& key!='E'&& key!='F')
						{
							
							lcd1602_goto_xy(10,0);
							lcd1602_send_string(":");
							lcd1602_goto_xy(8,0);
							lcd1602_send_char(saat1_);
							lcd1602_goto_xy(9,0);
							lcd1602_send_char(saat2_);
							lcd1602_goto_xy(11,0);
							lcd1602_send_char(dakika1_);
							lcd1602_goto_xy(12,0);
							lcd1602_send_char(dakika2_);
							
							
							if (say_==1 && key != Key_None)
							{
								saat1_=(char)key;
								_delay_ms(500);
								key=Key_None;
								say_++;
								
							}
							if (say_==2&& key != Key_None && saat1_!=0)
							{
								saat2_=(char)key;
								_delay_ms(500);
								key=Key_None;
								say_++;
	
							}
							if (say_==3&& key != Key_None &&saat2_!=0)
							{
								dakika1_=(char)key;
								_delay_ms(500);
								key=Key_None;
								say_++;

							}
							if (say_==4&& key != Key_None && dakika1_!=0)
							{
								dakika2_=(char)key;
								key=Key_None;
								say_=0;
							}
							

							
						}
				
				
				
				////
				
				

				 

				

				
				
				  
			
		}
		
    }
}


