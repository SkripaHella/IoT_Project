#include "mbed.h"
#include "BME280.h"
#include <arm_acle.h>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <charconv>

#define MAX_DIGITS 10
RawSerial pc(USBTX, USBRX);
RawSerial dev(D8, D2);
BME280 sensor(I2C_SDA, I2C_SCL);
void dev_recv()
{
    while(dev.readable()) {
        pc.putc(dev.getc());
    }
}

void pc_recv()
{
    while(pc.readable()) {
        dev.putc(pc.getc());
    }
}

//void gotoSleep(void)
//{
// //включение часов управления PWR
// RCC->APB1ENR |= (RCC_APB1ENR_PWREN);

// //установление бита SLEEPDEEP в регистре управления системой Cortex
// SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; // регистр управления системой SCB
// выбирается обычный сон или глубокий сон (как выбрано в данном случае)
// //выбор режима ожидания
// PWR->CR |= PWR_CR_PDDS; // переход к регистру питания периферийного устройства или переход к регистру управления и установка 1 в бит PWR_CR_PDDS
// //снятие флага пробуждения, очищение
// PWR->CR |= PWR_CR_CWUF; // если флаг WUF будет иметь значение 1, то в режим сна устройство не перейдет. Сам по себе WUF в режиме чтения. Поэтому используем CWUF и через него меняем

// //включение запроса пина для пробуждения Ожидание Прерывания
// PWR->CSR |= (PWR_CSR_EWUP); // устанавливаем в 1 бит сброса флага пробуждения.
// в даташите нет этого этапа, но он отчасти нужен, так как используем пробуждение - переходим в регистр состояния управления питанием и включаем пробуждение

// //ожидание Прерывания
// __wfi();
//}


void print_f(char temperature[], char pressure[], char humidity[])
{
    pc.printf("%i\n", sensor.getTemperature());
    pc.printf("--------------\n");
    for (int i = 0; i < 2; i++)
    {
        pc.printf("%c", temperature[i]);
    }
    pc.printf("\n");
    pc.printf("--------------\n");
    pc.printf("--------------\n");

    pc.printf("%i\n", sensor.getPressure());
    pc.printf("--------------\n");
    for (int i = 0; i < 4; i++)
    {
        pc.printf("%c", pressure[i]);
    }
    pc.printf("\n");
    pc.printf("--------------\n");
    pc.printf("--------------\n");

    pc.printf("%i\n", sensor.getHumidity());
    pc.printf("--------------\n");
    for (int i = 0; i < 2; i++)
    {
        pc.printf("%c", humidity[i]);
    }
    pc.printf("\n");
    pc.printf("--------------\n");
    pc.printf("--------------\n");
}

//char inttochar(int param)
//{
//}

int main()
{
    int q=0;
    while(1) {
        char command_WAKE_UP[2] = {'a', 't'};
        for(int i = 0; i < strlen(command_WAKE_UP); i++)
        {
            dev.putc(command_WAKE_UP[i]);
            pc.printf("%c", command_WAKE_UP[i]);
        }
        dev.putc('\n');
        dev.putc('\r');
        wait(3);
        pc.baud(115200);
        dev.baud(115200);
        char command_JOIN[9] = {'a', 't', '+', 'j', 'o', 'i', 'n', '\n', '\r'};
        if(q==3) // 10
        {
            for(int i = 0; i < 9; i++)
            {
                dev.putc(command_JOIN[i]);
                pc.printf("%c", command_JOIN[i]);
            }
            q=0;
            wait(10);
        }
        q++;
        //print_f(data.Temperature, data.Pressure, data.Humidity);
        char command_SEND[23] = {'a', 't', '+', 's', 'e', 'n', 'd', '=', 'l', 'o', 'r', 'a', ':', '1', ':'};
        // типа метод по преобразованию инта температуры в символы
        char* temp;
        int k = sensor.getTemperature();
        pc.printf("\n%d\n", sensor.getTemperature());
        temp = (char *)malloc(10 * sizeof(char));
        int v = 0; //количество цифр в числе n
        //разбиваем на отдельные символы число n
        while (k > 9)
        {
            temp[v++] = (k % 10) + '0';
            k = k / 10;
        }
        temp[v++] = k + '0';
        temp[v] = '\0';
        char t;
        //инвертируем массив символов
        for (int i = 0; i < v / 2; i++)
        {
            t = temp[i];
            temp[i] = temp[v - 1 - i];
            temp[v - 1 - i] = t;
        }
        v = 0;
        for(int i = 0; i < strlen(temp);i++)
        {
            command_SEND[i+15] += temp[i];
        }
        free(temp);

        // типа метод по преобразованию инта давления в символы
        //char* pres;
        //int k2 = sensor.getPressure();
        //pres = (char *)malloc(10 * sizeof(char));
        //int v2 = 0; //количество цифр в числе n
        //разбиваем на отдельные символы число n
        //while (k2 > 9)
        //{
        //    pres[v2++] = (k2 % 10) + '0';
        //    k2 = k2 / 10;
        //}
        //pres[v2++] = k2 + '0';
        //pres[v2] = '\0';
        //char t2;
        //инвертируем массив символов
        //for (int i = 0; i < v2 / 2; i++)
        //{
        //    t2 = pres[i];
        //    pres[i] = pres[v2 - 1 - i];
        //    pres[v2 - 1 - i] = t2;
        //}
        //v2 = 0;
        //for(int i = 0; i < strlen(pres);i++)
        //{
        //    command_SEND[i+15+strlen(temp)] += pres[i];
        //}
        //free(pres);

        // типа метод по преобразованию инта влажности в символы
        //char* hum;
        //int k3 = sensor.getHumidity();
        //hum = (char *)malloc(10 * sizeof(char));
        //int v3 = 0; //количество цифр в числе n
        //разбиваем на отдельные символы число n
        //while (k3 > 9)
        //{
        //    hum[v3++] = (k3 % 10) + '0';
        //    k3 = k3 / 10;
        //}
        //hum[v3++] = k3 + '0';
        //hum[v3] = '\0';
        //char t3;
        //инвертируем массив символов
        //for (int i = 0; i < v3 / 2; i++)
        //{
        //    t2 = hum[i];
        //    hum[i] = hum[v3 - 1 - i];
        //    hum[v3 - 1 - i] = t3;
        //}
        //v3 = 0;
        //for(int i = 0; i < strlen(hum);i++)
        //{
        //    command_SEND[i+15+strlen(temp)+strlen(pres)] += hum[i];
        //}
        //free(hum);

        pc.baud(115200);
        dev.baud(115200);
        for(int i = 0; i < strlen(command_SEND); i++)
        {
            dev.putc(command_SEND[i]);
            pc.printf("%c", command_SEND[i]);
        }
        dev.putc('\n');
        dev.putc('\r');
        wait(7);
        pc.attach(&pc_recv, Serial::RxIrq);
        dev.attach(&dev_recv, Serial::RxIrq);

        char command_SLEEP[28] = {'a', 't', '+', 's', 'e', 't', '_', 'c', 'o', 'n', 'f', 'i', 'g', 
        '=', 'd', 'e', 'v', 'i', 'c', 'e', ':', 's', 'l', 'e', 'e', 'p', ':', '1'};
        for(int i = 0; i < strlen(command_SLEEP); i++)
        {
            dev.putc(command_SLEEP[i]);
            pc.printf("%c", command_SLEEP[i]);
        }
        dev.putc('\n');
        dev.putc('\r');
        //data.Warning = '1';
        ThisThread::sleep_for(5000); // или 10000 для демонстрации (а если для реальной работы, то 2700000, то есть сон 45 минут)
    }
}


