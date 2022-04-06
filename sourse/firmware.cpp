#include "mbed.h"
#include "BME280.h"
#include "serial_mac_api.h"
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
int main()
{
struct data
{
char Warning;
char Temperature[MAX_DIGITS + sizeof(char)];
char Pressure[MAX_DIGITS + sizeof(char)];
char Humidity[MAX_DIGITS + sizeof(char)];
} data;
while(1) {
std::to_chars(data.Temperature, data.Temperature + MAX_DIGITS,
sensor.getTemperature());
std::to_chars(data.Pressure, data.Pressure + MAX_DIGITS, sensor.getPressure());
std::to_chars(data.Humidity, data.Humidity + MAX_DIGITS, sensor.getHumidity());
//pc.printf("%s\n", data.Temperature);
//pc.printf("%s\n", data.Pressure);
//pc.printf("%s\n", data.Humidity);
//pc.printf("\n\n");
//string comand = "at+send=lora:1:";
pc.baud(115200);
dev.baud(115200);
//for(int i = 0; i < comand.length(); i++)
//{
// dev.putc(comand[i]);
//}
if (sensor.getTemperature() < 18 || sensor.getTemperature() > 24 || sensor.getHumidity() <
40 || sensor.getHumidity() > 60)
{
pc.attach(&pc_recv, Serial::RxIrq);
dev.attach(&dev_recv, Serial::RxIrq);
data.Warning = '1';
wait(18000);
sleep();
}
else
{
pc.attach(&pc_recv, Serial::RxIrq);
dev.attach(&dev_recv, Serial::RxIrq);
data.Warning = '0';
wait(108000);
sleep();
}
}
}
