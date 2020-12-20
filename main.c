#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "i2c.h"
#define ClearBit(reg, bit)       reg &= (~(1<<(bit)))
#define SetBit(reg, bit)         reg |= (1<<(bit))  
#define RTC_adr_write 0b11010000
#define RTC_adr_read  0b11010001

uint8_t buttonCurrentState = 0, longPush = 0, shortPush = 0;
uint16_t displayData = 0;
uint64_t millis = 0;
uint32_t t_dot = 0, t_time = 0, t_but = 0, t_push = 0, t_temp = 0;
unsigned char hour, min, sec;
   // day, wday, month, year;

ISR(TIMER0_COMPA_vect){
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
        case 10:
            SetBit(PORTC, PORTC0); //A
            SetBit(PORTC, PORTC1); //B
            ClearBit(PORTC, PORTC2); //C
            ClearBit(PORTC, PORTC3); //D
            ClearBit(PORTB, PORTB6); //E
            ClearBit(PORTB, PORTB7); //F
            SetBit(PORTD, PORTD6); //H
            break;
        case 11:
            ClearBit(PORTC, PORTC0); //A
            ClearBit(PORTC, PORTC1); //B
            ClearBit(PORTC, PORTC2); //C
            SetBit(PORTC, PORTC3); //D
            ClearBit(PORTB, PORTB6); //E
            SetBit(PORTB, PORTB7); //F
            SetBit(PORTD, PORTD6); //H
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

void updateDisplay(float t1, float t2){
    static int f = 0;

    updateDigit(displayData/1000%10);
    ClearBit(PORTB, PORTB5);
    _delay_ms(t1);
    SetBit(PORTB, PORTB5);

    updateDigit(displayData/100%10);
    ClearBit(PORTB, PORTB4);
    _delay_ms(t1);
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
        _delay_ms((t1+t2)/4.0);
        SetBit(PORTB, PORTB7);
        SetBit(PORTD, PORTD5);
    } else {
        SetBit(PORTB, PORTB7);
        SetBit(PORTD, PORTD5);
        _delay_ms((t1+t2)/4.0);
    }


    updateDigit(displayData/10%10);
    ClearBit(PORTB, PORTB0);
    _delay_ms(t2);
    SetBit(PORTB, PORTB0);

    updateDigit(displayData%10);
    ClearBit(PORTD, PORTD7);
    _delay_ms(t2);
    SetBit(PORTD, PORTD7);
}

void updateDisplayTemp(int tmp){
    static int f = 0;

    updateDigit(tmp/10);
    ClearBit(PORTB, PORTB5);
    _delay_ms(3);
    SetBit(PORTB, PORTB5);

    updateDigit(tmp%10);
    ClearBit(PORTB, PORTB4);
    _delay_ms(3);
    SetBit(PORTB, PORTB4);

    updateDigit(10);
    ClearBit(PORTB, PORTB0);
    _delay_ms(3);
    SetBit(PORTB, PORTB0);

    updateDigit(11);
    ClearBit(PORTD, PORTD7);
    _delay_ms(3);
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
    hour = bcd(i2c_get_byte(1));    // чтение часов, ACK
    //wday = bcd(i2c_get_byte(0));    // чтение день недели, ACK
    //day = bcd(i2c_get_byte(0));     // чтение число, ACK
    //month = bcd(i2c_get_byte(0));   // чтение месяц, ACK
    //year = bcd(i2c_get_byte(1));    // чтение год, NACK
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

int RTC_read_temper(void){
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(RTC_adr_write);   // передача адреса устройства, режим записи
    i2c_send_byte(0x11);            // передача адреса памяти 
    i2c_stop_cond();                // остановка i2c
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(RTC_adr_read);    // передача адреса устройства, режим чтения
    uint8_t t1 = i2c_get_byte(0);           // чтение MSB температуры
    uint8_t t2 = i2c_get_byte(1);           // чтение LSB температуры
    i2c_stop_cond();                // остановка i2c
    t2=(t2/128);                    // сдвигаем на 6 - точность 0,25 (2 бита)
                                    // сдвигаем на 7 - точность 0,5 (1 бит)
    t2=t2*5;
    if(t2 > 0.5) return t1 + 1;
    return t1;
}

void updateTime(){
    //displayData = millis/1000;
    if(millis - t_time > 350){
        RTC_read_time();
        displayData = 100*hour + min;
        t_time = millis;
    }
}

void updateButton(){
    if(millis - t_but > 5){
        if(!(longPush||shortPush)){
            if(!buttonCurrentState && (PIND&(1<<PIND3))){
                buttonCurrentState = 1;
                t_push = millis;
            }
            if(buttonCurrentState && !(PIND&(1<<PIND3))){
                buttonCurrentState = 0;\
                if(millis - t_push > 2500) longPush = 1; else 
                if(millis - t_push > 50) shortPush = 1;
            }
        }
        t_but = millis;
    }
}

uint8_t getShortPush(){
    if(shortPush){
        shortPush = 0;
        longPush = 0;
        return 1;
    } else return 0;
}

uint8_t getLongPush(){
    if(longPush){
        shortPush = 0;
        longPush = 0;
        return 1;
    } else return 0;
}

int main(void){
    initDisplay();
    initI2C();
    RTC_read_time();
    TIMSK0 |= (1<<OCIE0A);
    OCR0A = 125;
    TCCR0B = (1<<CS01)|(1<<CS00);
    sei();
    while(1){
        updateDisplay(3, 3);
        updateTime();
        updateButton();
        if(getShortPush()){
            t_temp = millis;
            while(millis - t_temp < 10000) updateDisplayTemp(RTC_read_temper()-8);
        }
        if(getLongPush()){
            while (!getLongPush()){
                updateDisplay(3, 0.5);
                updateButton();
                displayData = 100*hour + min;
                if(getShortPush()) {
                    hour++;
                    hour %= 24;
                    RTC_write_time(hour, min, 0);
                }
            }
            while (!getLongPush()){
                updateDisplay(0.5, 3);
                updateButton();
                displayData = 100*hour + min;
                if(getShortPush()) {
                    min++;
                    min %= 60;
                    RTC_write_time(hour, min, 0);
                }
            }
        }
    }
}