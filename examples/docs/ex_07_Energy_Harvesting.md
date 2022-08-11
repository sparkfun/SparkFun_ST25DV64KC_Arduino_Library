# Example 7 - Energy Harvesting

An example showing how to enable the ST25DV's energy harvesting feature using the SparkFun ST25DV64KC Arduino Library.

## Key Features

- Enabling energy harvesting

## Energy Harvesting

The ST25DV can be used to harvest small amounts of power when an RF field is present.

Only _small amounts_ of power can be generated. Perhaps enough to charge a small super capacitor to power a _very_ low-power microcontroller.

By default, energy harvesting is "on demand only" (when the EH_EN bit is set). If the power is removed, harvesting is automatically disabled.

It is possible to configure the tag so energy harvesting is always enabled after boot (if possible) by setting the EH_MODE bit.

## Enable EH

To enable energy harvesting, Example 7 opens a security session and then sets the EH_EN bit. A voltage is then produced on the EH
breakout pin whenever an RF field is present.

```C++
  tag.setEH_CTRL_DYNBit(BIT_EH_CTRL_DYN_EH_EN, true);
```

!!! note
    The voltage on the EH pin _could_ be higher than 3.3V. Take care if you are using a microcontroller analog pin to measure the voltage.

