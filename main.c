#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "i2c.h"
#define ClearBit(reg, bit)       reg &= (~(1<<(bit)))
#define SetBit(reg, bit)         reg |= (1<<(bit))  
#define RTC_adr_write 0b11010000
#define RTC_adr_read  0b11010001

uint16_t displayData = 0;
uint64_t millis = 0, t_dot = 0, t_time = 0;
unsigned char hour, min, sec,
    day, wday, month, year;
   // temp, t1, t2;

ISR(TIMER0_COMPA_vect)
{
    millis++;
    TCNT0 = 0;
}

void initDisplay(){
    //Set DDRs
    SetBit(DDRC, PORTC0); //A
    SetBit(DDRC, PORTC1); //B
    SetBit(DDRC, PORTC2); //C
    SetBit(DDRC, PORTC3); //D
    SetBit(DDRB, PORTB6); //E
    SetBit(DDRB, PORTB7); //F
    SetBit(DDRD, PORTD5); //G
    SetBit(DDRD, PORTD6); //H
    SetBit(DDRB, PORTB5); //1
    SetBit(DDRB, PORTB4); //2
    SetBit(DDRB, PORTB0); //3
    SetBit(DDRD, PORTD7); //4
    //Set PORTs
    SetBit(PORTC, PORTC0); //A
    SetBit(PORTC, PORTC1); //B
    SetBit(PORTC, PORTC2); //C
    SetBit(PORTC, PORTC3); //D
    SetBit(PORTB, PORTB6); //E
    SetBit(PORTB, PORTB7); //F
    SetBit(PORTD, PORTD5); //G
    SetBit(PORTD, PORTD6); //H
    SetBit(PORTB, PORTB5); //1
    SetBit(PORTB, PORTB4); //2
    SetBit(PORTB, PORTB0); //3
    SetBit(PORTD, PORTD7); //4
}

void updateDigit(int n){
    ClearBit(PORTC, PORTC0);
    switch(n){
        case 0:
            ClearBit(PORTC, PORTC0); //A
            ClearBit(PORTC, PORTC1); //B
            ClearBit(PORTC, PORTC2); //C
            SetBit(PORTC, PORTC3); //D
            ClearBit(PORTB, PORTB6); //E
            ClearBit(PORTB, PORTB7); //F
            ClearBit(PORTD, PORTD6); //H
            break;
        case 1:
            SetBit(PORTC, PORTC0); //A
            SetBit(PORTC, PORTC1); //B
            SetBit(PORTC, PORTC2); //C
            SetBit(PORTC, PORTC3); //D
            SetBit(PORTB, PORTB6); //E
            ClearBit(PORTB, PORTB7); //F
            ClearBit(PORTD, PORTD6); //H
            break;
        case 2:
            ClearBit(PORTC, PORTC0); //A
            ClearBit(PORTC, PORTC1); //B
            SetBit(PORTC, PORTC2); //C
            ClearBit(PORTC, PORTC3); //D
            ClearBit(PORTB, PORTB6); //E
            ClearBit(PORTB, PORTB7); //F
            SetBit(PORTD, PORTD6); //H
            break;
        case 3:
            ClearBit(PORTC, PORTC0); //A
            SetBit(PORTC, PORTC1); //B
            SetBit(PORTC, PORTC2); //C
            ClearBit(PORTC, PORTC3); //D
            ClearBit(PORTB, PORTB6); //E
            ClearBit(PORTB, PORTB7); //F
            ClearBit(PORTD, PORTD6); //H
            break;
        case 4:
            SetBit(PORTC, PORTC0); //A
            SetBit(PORTC, PORTC1); //B
            ClearBit(PORTC, PORTC2); //C
            ClearBit(PORTC, PORTC3); //D
            SetBit(PORTB, PORTB6); //E
            ClearBit(PORTB, PORTB7); //F
            ClearBit(PORTD, PORTD6); //H
            break;
        case 5:
            ClearBit(PORTC, PORTC0); //A
            SetBit(PORTC, PORTC1); //B
            ClearBit(PORTC, PORTC2); //C
            ClearBit(PORTC, PORTC3); //D
            ClearBit(PORTB, PORTB6); //E
            SetBit(PORTB, PORTB7); //F
            ClearBit(PORTD, PORTD6); //H
            break;
        case 6:
            ClearBit(PORTC, PORTC0); //A
            ClearBit(PORTC, PORTC1); //B
            ClearBit(PORTC, PORTC2); //C
            ClearBit(PORTC, PORTC3); //D
            ClearBit(PORTB, PORTB6); //E
            SetBit(PORTB, PORTB7); //F
            ClearBit(PORTD, PORTD6); //H
            break;
        case 7:
            SetBit(PORTC, PORTC0); //A
            SetBit(PORTC, PORTC1); //B
            SetBit(PORTC, PORTC2); //C
            SetBit(PORTC, PORTC3); //D
            ClearBit(PORTB, PORTB6); //E
            ClearBit(PORTB, PORTB7); //F
            ClearBit(PORTD, PORTD6); //H
            break;
        case 8:
            ClearBit(PORTC, PORTC0); //A
            ClearBit(PORTC, PORTC1); //B
            ClearBit(PORTC, PORTC2); //C
            ClearBit(PORTC, PORTC3); //D
            ClearBit(PORTB, PORTB6); //E
            ClearBit(PORTB, PORTB7); //F
            ClearBit(PORTD, PORTD6); //H
            break;
        case 9:
            ClearBit(PORTC, PORTC0); //A
            SetBit(PORTC, PORTC1); //B
            ClearBit(PORTC, PORTC2); //C
            ClearBit(PORTC, PORTC3); //D
            ClearBit(PORTB, PORTB6); //E
            ClearBit(PORTB, PORTB7); //F
            ClearBit(PORTD, PORTD6); //H
            break;
        default:
            SetBit(PORTC, PORTC0); //A
            SetBit(PORTC, PORTC1); //B
            SetBit(PORTC, PORTC2); //C
            SetBit(PORTC, PORTC3); //D
            SetBit(PORTB, PORTB6); //E
            SetBit(PORTB, PORTB7); //F
            SetBit(PORTD, PORTD5); //G
            SetBit(PORTD, PORTD6); //H
            break;
    }
}

void updateDisplay(){
    const float t = 3;
    static int f = 0;

    updateDigit(displayData/1000%10);
    ClearBit(PORTB, PORTB5);
    _delay_ms(t);
    SetBit(PORTB, PORTB5);

    updateDigit(displayData/100%10);
    ClearBit(PORTB, PORTB4);
    _delay_ms(t);
    SetBit(PORTB, PORTB4);

    if(millis - t_dot > 1250){
        if(f){
            f = 0;
        } else {
            f = 1;
        }
        t_dot = millis;
    }
    if(f){
        ClearBit(PORTD, PORTD5);
        ClearBit(PORTD, PORTD6);
        _delay_ms(t/2.0);
        SetBit(PORTB, PORTB7);
        SetBit(PORTD, PORTD5);
    } else {
        SetBit(PORTB, PORTB7);
        SetBit(PORTD, PORTD5);
        _delay_ms(t/2.0);
    }


    updateDigit(displayData/10%10);
    ClearBit(PORTB, PORTB0);
    _delay_ms(t);
    SetBit(PORTB, PORTB0);

    updateDigit(displayData%10);
    ClearBit(PORTD, PORTD7);
    _delay_ms(t);
    SetBit(PORTD, PORTD7);
}

unsigned char bcd (unsigned char data){
	unsigned char bc;
	bc=((((data&(1<<6))|(data&(1<<5))|(data&(1<<4)))*0x0A)>>4)+((data&(1<<3))|(data&(1<<2))|(data&(1<<1))|(data&0x01));
	return bc;
}

unsigned char bin(unsigned char dec){
	char bcd;
	char n, dig, num, count;
	num = dec;
	count = 0;
	bcd = 0;
	for (n=0; n<4; n++) {
		dig = num%10;
		num = num/10;
		bcd = (dig<<count)|bcd;
		count += 4;
	}
	return bcd;
}

void initI2C(void){
 
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(RTC_adr_write);   // передача адреса устройства, режим записи
    i2c_send_byte(0x0E);            // передача адреса памяти 
    i2c_send_byte(0b00100000);      // запустить преобразование температуры и выход на 1 Гц
    i2c_send_byte(0b00001000);      // разрешить выход 32 кГц
    i2c_stop_cond();                // остановка i2c
 
}

void RTC_read_time(void){
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(RTC_adr_write);   // передача адреса устройства, режим записи
    i2c_send_byte(0x00);            // передача адреса памяти 
    i2c_stop_cond();                // остановка i2c
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(RTC_adr_read);    // передача адреса устройства, режим чтения
    sec = bcd(i2c_get_byte(0));     // чтение секунд, ACK
    min = bcd(i2c_get_byte(0));     // чтение минут, ACK
    hour = bcd(i2c_get_byte(0));    // чтение часов, ACK
    wday = bcd(i2c_get_byte(0));    // чтение день недели, ACK
    day = bcd(i2c_get_byte(0));     // чтение число, ACK
    month = bcd(i2c_get_byte(0));   // чтение месяц, ACK
    year = bcd(i2c_get_byte(1));    // чтение год, NACK
    i2c_stop_cond();                // остановка i2c
}

void RTC_write_time(unsigned char hour1,unsigned char min1, unsigned char sec1){
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(RTC_adr_write);   // передача адреса устройства, режим записи
    i2c_send_byte(0x00);            // передача адреса памяти 
    i2c_send_byte(bin(sec1));       // 0x00 секунды (целесообразно ли задавать еще и секунды?)
    i2c_send_byte(bin(min1));       // 0x01 минуты
    i2c_send_byte(bin(hour1));      // 0x02 часы
    i2c_stop_cond();                // остановка i2c
}

void RTC_write_date(unsigned char wday, unsigned char day, unsigned char month, unsigned char year){
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(RTC_adr_write);   // передача адреса устройства, режим записи
    i2c_send_byte(0x03);        // передача адреса памяти 
    i2c_send_byte(bin(wday));       // 0x03 день недели (воскресенье - 1, пн 2, вт 3, ср 4, чт 5, пт 6, сб 7)
    i2c_send_byte(bin(day));        // 0x04 день месяц
    i2c_send_byte(bin(month));      // 0x05 месяц
    i2c_send_byte(bin(year));       // 0x06 год
    i2c_stop_cond();                // остановка i2c
}

void updateTime(){
    //displayData = millis/1000;
    if(millis - t_time > 1000){
        RTC_read_time();
        displayData = 100*hour + min;
        t_time = millis;
    }
}

int main(void)
{
    initDisplay();
    initI2C();
    RTC_read_time();
    TIMSK0 |= (1<<OCIE0A);
    OCR0A = 125;
    TCCR0B = (1<<CS01)|(1<<CS00);
    sei();
    while (1) 
    {
        updateDisplay();
        updateTime();
    }
}

