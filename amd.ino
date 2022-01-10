#include <LiquidCrystal.h>

const int lcdRSPin = 12;
const int lcdRWPin = 11;
const int lcdEnPin = 10;
const int lcdD4Pin = 5;
const int lcdD5Pin = 4;
const int lcdD6Pin = 3;
const int lcdD7Pin = 2;

const int buttonPin = 7;

const int photoresistorPin = A0;

const int resetTime = 5000;
const int timeCode = 1500;
const int photoresistorThreshold = 800;

unsigned long timeNow;
String decodedMessage;

LiquidCrystal lcd(
    lcdRSPin,
    lcdRWPin,
    lcdEnPin,
    lcdD4Pin,
    lcdD5Pin,
    lcdD6Pin,
    lcdD7Pin);

char morseToChar(String);
char readButton();
char readPhotoresistor();

void setup()
{
    Serial.begin(9600);
    pinMode(photoresistorPin, INPUT);
    pinMode(buttonPin, INPUT);
    lcd.begin(16, 2);
    lcd.print("System Ready!");
}

void loop()
{
    bool readingButton = false;
    bool readingPhotoresistor = false;
    String previousDecodedMessage = "";
    String decodedMessage = "";
    String code;
    do
    {
        code = "";
        if (readingPhotoresistor || (!readingPhotoresistor && !readingButton && digitalRead(buttonPin) == HIGH))
        {
            if (analogRead(photoresistorPin) > photoresistorThreshold)
            {
                readingPhotoresistor = true;
                do
                {
                    if (analogRead(photoresistorPin) > photoresistorThreshold)
                    {
                        timeNow = millis();
                        code.concat(readPhotoresistor());
                    }
                } while (millis() < timeNow + timeCode);

                decodedMessage.concat(morseToChar(code));
            }
        }
        else if (readingButton || digitalRead(buttonPin) == LOW)
        {
            if (digitalRead(buttonPin) == LOW)
            {
                readingButton = true;
                do
                {
                    if (digitalRead(buttonPin) == LOW)
                    {
                        timeNow = millis();
                        code.concat(readButton());
                    }
                } while (millis() < timeNow + timeCode);

                decodedMessage.concat(morseToChar(code));
            }
        }

        if (previousDecodedMessage.length() != decodedMessage.length())
        {
            previousDecodedMessage = decodedMessage;
            lcd.clear();
            lcd.print(decodedMessage);
        }
    } while (millis() < timeNow + resetTime);
}

char morseToChar(String inputCode)
{
    // due to the morse code logic, a long sequence of else if is required
    if (inputCode == ".-")
    {
        return 'a';
    }
    else if (inputCode == "-...")
    {
        return 'b';
    }
    else if (inputCode == "-.-.")
    {
        return 'c';
    }
    else if (inputCode == "-..")
    {
        return 'd';
    }
    else if (inputCode == ".")
    {
        return 'e';
    }
    else if (inputCode == "..-.")
    {
        return 'f';
    }
    else if (inputCode == "--.")
    {
        return 'g';
    }
    else if (inputCode == "....")
    {
        return 'h';
    }
    else if (inputCode == "..")
    {
        return 'i';
    }
    else if (inputCode == ".---")
    {
        return 'j';
    }
    else if (inputCode == "-.-")
    {
        return 'k';
    }
    else if (inputCode == ".-..")
    {
        return 'l';
    }
    else if (inputCode == "--")
    {
        return 'm';
    }
    else if (inputCode == "-.")
    {
        return 'n';
    }
    else if (inputCode == "---")
    {
        return 'o';
    }
    else if (inputCode == ".--.")
    {
        return 'p';
    }
    else if (inputCode == "--.-")
    {
        return 'q';
    }
    else if (inputCode == ".-.")
    {
        return 'r';
    }
    else if (inputCode == "...")
    {
        return 's';
    }
    else if (inputCode == "-")
    {
        return 't';
    }
    else if (inputCode == "..-")
    {
        return 'u';
    }
    else if (inputCode == "...-")
    {
        return 'v';
    }
    else if (inputCode == ".--")
    {
        return 'w';
    }
    else if (inputCode == "-..-")
    {
        return 'x';
    }
    else if (inputCode == "-.--")
    {
        return 'y';
    }
    else if (inputCode == "--..")
    {
        return 'z';
    }
    else if (inputCode == ".----")
    {
        return '1';
    }
    else if (inputCode == "..---")
    {
        return '2';
    }
    else if (inputCode == "...--")
    {
        return '3';
    }
    else if (inputCode == "....-")
    {
        return '4';
    }
    else if (inputCode == ".....")
    {
        return '5';
    }
    else if (inputCode == "-....")
    {
        return '6';
    }
    else if (inputCode == "--...")
    {
        return '7';
    }
    else if (inputCode == "---..")
    {
        return '8';
    }
    else if (inputCode == "----.")
    {
        return '9';
    }
    else if (inputCode == "-----")
    {
        return '0';
    }
    else
    {
        return '~';
    }
}

char readButton()
{
    // it must be a second or more than a second, but we prefer to have some margin of actuation
    int pulses = 0;

    while (digitalRead(buttonPin) == LOW)
    {
        pulses++;
        timeNow = millis();
        delay(100);
    }
    Serial.print("pulses");
    Serial.println(pulses);
    return pulses >= 10 ? '-' : '.';
}

char readPhotoresistor()
{
    // it must be a second or more than a second, but we prefer to have some margin of actuation
    int pulses = 0;

    while (analogRead(photoresistorPin) > photoresistorThreshold)
    {
        pulses++;
        timeNow = millis();
        delay(100);
    }
    Serial.print("pulses");
    Serial.println(pulses);
    return (pulses >= 10 ? '-' : '.');
}
