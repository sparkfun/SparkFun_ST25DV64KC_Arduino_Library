# Example 6 - RF Field Detection (using Interrupts)

An example showing how to detect an RF field (using interrupts) using the SparkFun ST25DV64KC Arduino Library.

## Key Features

- Opening a security session
- Configuring the tag's GPO pin for RF detection
- Using Arduino interrupts to detect the field

## GPO Pin

The ST25DV has a very useful GPO (General Purpose Output) pin. It can be configured in a number of different ways.

This example configures GPO to indicate the presence of an RF field. We can then use the signal on GPO to trigger an interrupt on our Arduino board.

## Arduino Interrupts

There are many good tutorials out there showing how to use Arduino interrupts. Here is a brief summary of how it is done in this example:

```C++
// Use a jumper cable to link the ST25DV64KC GPO1 pin to a digital pin
const uint8_t GPO_PIN = 2; // Change this to match the digital pin you have linked GPO1 to

static volatile bool interruptChanged = false;

void myISR() // Interrupt Service Routine
{
  interruptChanged = true;
}
```

This code creates ```interruptChanged``` as a static volatile boolean variable, and sets it to false.

```static``` means it is always present in memory. It is never deleted or destroyed. This is actually redundant because ```interruptChanged``` is _global_ but we include it for clarity.
(If we had created ```interruptChanged``` in the ```loop()``` then the static would be very important.)

```volatile``` means that ```interruptChanged``` could change at any moment - when an interrupt occurs - and so the compiler should always read ```interruptChanged``` directly
and not use any kind of local copy for speed.

```myISR()``` is our interrupt service routine. It is the small piece of code which is called when the interrupt takes place. There are some 'golden rules' when writing ISRs:

- Always keep them short
- Always use ```volatile``` variables if they need to be accessed by code in the ```loop()```
- Don't try to do anything time consuming inside them - always keep them short
- Don't ever use Serial prints inside ISRs

In ```setup()```, GPO_PIN (Digital Pin 2) is configured to create the interrupt:

```C++
  pinMode(GPO_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(GPO_PIN), myISR, CHANGE);
```

Now ```myISR``` will be called whenever pin 2 changes state.

## GPO Pin

We need to link the tag's GPO breakout pad to Pin 2 on our Arduino board. You may want to solder header pins onto the tag breakout and then use a jumper cable to connect them.

Pin 2 is an interrupt pin on our original Uno-like RedBoards (ATMega328P). You can change the pin number to any other interrupt-capable pin if needed.

The example goes on to configure the GPO pin using ```setGPO1Bit```. Remember that we can only change the tag's settings if a security session has been opened.

```C++
  Serial.println(F("Configuring GPO1 to toggle on field change only."));
  tag.setGPO1Bit(BIT_GPO1_FIELD_CHANGE_EN, true);
  Serial.println(F("Disabling other bits."));
  tag.setGPO1Bit(BIT_GPO1_RF_USER_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_ACTIVITY_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_INTERRUPT_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_PUT_MSG_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_GET_MSG_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_WRITE_EN, false);

  Serial.println(F("Enabling GPO_EN bit."));
  tag.setGPO1Bit(BIT_GPO1_GPO_EN, true);
```

You can see from the code that GPO can be configured to do many different things. In this example we only want it to change when the RF field changes,
so we only set the ```BIT_GPO1_FIELD_CHANGE_EN``` bit to **true**. All the other bits are set to **false**.

We also need to actually enable the GPO pin with ```BIT_GPO1_GPO_EN```.

You can guess from the bit names that it is possible to use GPO to indicate other events, such as an RF write taking place.

Run the example and bring your phone near to the tag. You will see the count increase rapidly each time an NFC transfer takes place.
