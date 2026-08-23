# Spotify Lyrics LCD

Показывает синхронизированные тексты песен из Spotify на LCD-дисплее (через Arduino).

## Как это работает

- `Lyrics_led.py` — следит за текущим треком в Spotify, тянет синхронизированные тексты с [lrclib.net](https://lrclib.net) и отправляет текущую строку в Arduino по serial.
- `arduino/sketch_aug20a.ino` — принимает строку по Serial и выводит её на LCD 16x2 (I2C), разбивая длинный текст на страницы.
- `test.py` — простой скрипт для проверки serial-соединения с Arduino.

## Установка

1. Клонируйте репозиторий и установите зависимости:
   ```bash
   pip install -r requirements.txt
   ```
2. Скопируйте `.env.example` в `.env` и впишите свои данные Spotify-приложения (создать на https://developer.spotify.com/dashboard):
   ```bash
   cp .env.example .env
   ```
3. Прошейте `arduino/sketch_aug20a.ino` в Arduino (нужна библиотека `LiquidCrystal_I2C`).
4. Укажите правильный COM-порт в `Lyrics_led.py` (переменная `arduino = serial.Serial("COM5", 9600)`).
5. Запустите:
   ```bash
   python Lyrics_led.py
   ```

## Важно

Файл `.env` содержит секретные ключи и **не должен попадать в git** — он уже добавлен в `.gitignore`.
