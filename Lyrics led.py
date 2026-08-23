import spotipy
from spotipy.oauth2 import SpotifyOAuth
from dotenv import load_dotenv
import requests
import time
import os
from colorama import init, Style
import serial
init(autoreset=True)

load_dotenv()

arduino = serial.Serial("COM5", 9600)
time.sleep(2)

sp = spotipy.Spotify(auth_manager=SpotifyOAuth(
    client_id=os.getenv("SPOTIPY_CLIENT_ID"),
    client_secret=os.getenv("SPOTIPY_CLIENT_SECRET"),
    redirect_uri=os.getenv("SPOTIPY_REDIRECT_URI"),
    scope="user-read-currently-playing user-read-playback-state"
))

def get_current_track():
    result = sp.current_playback()
    if not result or not result["is_playing"]:
        return None
    track = result["item"]
    return {
        "title": track["name"],
        "artist": track["artists"][0]["name"],
        "position_ms": result["progress_ms"]
    }

def get_lyrics(artist, title):
    url = "https://lrclib.net/api/get"
    params = {"track_name": title, "artist_name": artist}
    try:
        response = requests.get(url, params=params, timeout=5)
    except requests.exceptions.RequestException:
        print("Connection error")
        return None
    if response.status_code != 200:
        print("Lyrics not found")
        return None
    data = response.json()
    if not data.get("syncedLyrics"):
        print("Synced lyrics unavailable")
        return None
    return data["syncedLyrics"]

def parse_lyrics(synced_lyrics):
    lines = []
    for line in synced_lyrics.strip().split("\n"):
        try:
            time_part = line[1:9]
            text_part = line[10:].strip()
            minutes, seconds = time_part.split(":")
            ms = (int(minutes) * 60 + float(seconds)) * 1000
            lines.append({"ms": ms, "text": text_part})
        except:
            continue
    return lines

def send_to_lcd(text):
    text = text.replace("\n", " ").strip()


    arduino.write((text + "\n").encode("utf-8"))

track = get_current_track()
if not track:
    print("Play a song on Spotify!")
else:
    lyrics = get_lyrics(track["artist"], track["title"])
    if not lyrics:
        print("Lyrics not found")
    else:
        parsed = parse_lyrics(lyrics)
        print(Style.BRIGHT + f"\n🎵 {track['artist']} - {track['title']}\n")
        last_line = ""
        current_title = track["title"]

        while True:
            track = get_current_track()
            if not track:
                pause_start = time.time()
                print("\nSpotify paused...")
                while not track:
                    time.sleep(2)
                    track = get_current_track()
                    if time.time() - pause_start > 180:
                        print("Paused too long — exiting")
                        exit()
                os.system('cls')

            if track["title"] != current_title:
                os.system('cls')
                current_title = track["title"]
                last_line = ""
                new_lyrics = get_lyrics(track["artist"], track["title"])
                if not new_lyrics:
                    print("Lyrics not found")
                    time.sleep(2)
                    continue
                parsed = parse_lyrics(new_lyrics)

            position = track["position_ms"]
            current_line = ""
            for line in parsed:
                if line["ms"] <= position:
                    current_line = line["text"]

            if current_line != last_line and current_line:
                os.system('cls')
                print(Style.BRIGHT + f"\n🎵 {track['artist']} - {track['title']}\n")
                print(Style.BRIGHT + current_line)
                print()

                send_to_lcd(current_line)

                last_line = current_line

            time.sleep(0.1)