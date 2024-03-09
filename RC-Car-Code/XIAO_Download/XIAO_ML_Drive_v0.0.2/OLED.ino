/*


//This is for running the oled
void RunOled()
{
    display.clearDisplay();

    // Set text color
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(0, 0);
    if (Promode_On == 1)
    {
        display.print("Promode: On");
    }
    else
    {
        display.print("Promode: Off");
    }

    if (Promode_On == 1)
    {
        display.setCursor(0, 10);
        display.print("Speed: ");
        display.print(CurrentSpeed);

        display.setCursor(0, 20);
        display.print("Angel: ");
        display.print(CurrentAngel);
    }
    else if (Promode_On == 2)
    {
        display.setCursor(0, 10);
        display.print("Speed: Unknown");

        display.setCursor(0, 20);
        display.print("Angel: Unknown");

        display.setCursor(0, 30);
        display.print("Objects: ");
        display.print(myObjectCode);
    }

    display.display();
}

*/
