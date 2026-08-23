# Spotify Lyrics LCD

Displays synced lyrics of the currently playing Spotify track on an LCD screen, via Arduino.

## How it works

- `Lyrics_led.py` — polls the currently playing Spotify track, fetches synced lyrics from [lrclib.net](https://lrclib.net), and sends the current line to the Arduino over serial.
- `arduino/sketch_aug20a.ino` — receives a line over serial and displays it on a 16x2 I2C LCD, splitting longer text into pages.
- `test.py` — a simple script to test the serial connection with the Arduino.

## Hardware required

- Arduino Uno
- LCD 1602 display with I2C module

## Authorship

- The Python code (`Lyrics_led.py`, `test.py`) was written by **Ramazan Sultan**.
- The Arduino C++ code (`arduino/sketch_aug20a.ino`) was fully written by AI.

## Setup

1. Clone the repository and install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
2. Copy `.env.example` to `.env` and fill in your Spotify app credentials (create one at https://developer.spotify.com/dashboard):
   ```bash
   cp .env.example .env
   ```
3. Flash `arduino/sketch_aug20a.ino` to the Arduino Uno (requires the `LiquidCrystal_I2C` library).
4. Set the correct COM port in `Lyrics_led.py` (the `arduino = serial.Serial("COM5", 9600)` line).
5. Run:
   ```bash
   python Lyrics_led.py
   ```

## Important

The `.env` file contains secret API keys and **must never be committed to git** — it's already covered by `.gitignore`.

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.

Copyright (c) 2026 Ramazan Sultan
