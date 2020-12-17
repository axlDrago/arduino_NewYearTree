#include <IRremote.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

int pinIR = 2, pinRele = 4, pinIndicatorTimeOut = 9;

IRrecv irrecv(pinIR);
decode_results results;
unsigned long previousMillis = 0;
const int long interval = 60000 * 15;
bool onTimeout = false;

void setup()
{
    Serial.begin(9600);
    irrecv.enableIRIn();

    pinMode(pinIndicatorTimeOut, OUTPUT);
    pinMode(pinRele, OUTPUT);
}

void loop()
{

    if (irrecv.decode(&results))
    {
        Serial.println(results.value); //Посмотреть консоль

        if (results.value == 50139885 or results.value == 83573445)
        {
            digitalWrite(pinRele, !digitalRead(pinRele));
        }

        if (results.value == 50188845 or results.value == 83622405)
        {
            previousMillis = millis();
            onTimeout = !onTimeout;
            digitalWrite(pinIndicatorTimeOut, !digitalRead(pinIndicatorTimeOut));
            if(digitalRead(pinIndicatorTimeOut)) digitalWrite(pinRele, true);
        }

        Serial.println(results.value);
        irrecv.resume(); // Receive the next value
    }

    timeout();
}

void timeout()
{
    if (millis() - previousMillis >= interval && onTimeout)
    {
        digitalWrite(pinRele, false);
        digitalWrite(pinIndicatorTimeOut, false);
        onTimeout = false;
    }
}
