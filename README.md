# SunBoard Project (Orbital 23')

The Moonboard is a popular training tool used by climbers across the world. It is a standardised, interactive training wall with LED lights embedded into it. The LEDs are controlled by a control unit (Moonboard control box), and light up to indicate the current problem. The control unit connects to the Moonboard companion app via Bluetooth, and can be controlled by users through the app.

## Aim

Our aim is to reverse engineer the moonboard protocol and build. In addition, we want to build a handheld moonboard controller, so that climbers have an easier way of designing and “remixing” routes.

## Project Structure

- `app`: React Native project for driver mobile application.
- `mock-moonboard`: MoonBoard emulator source forked from [link]().
- `pcb`: schematics for the board remote controller.
- `sunboard-controller`: board remote controller source.
