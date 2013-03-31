# libas

Library for the AS5145 and similar devices for Arduino

## Usage

The library can be initialised using the default constructor
``` C
libas tracker(ClockPin, DataPin, ChipSelectPin);
```

Data can be read from the device using
``` C
int position = tracker->GetPosition();
```
which will fetch and return the current position
