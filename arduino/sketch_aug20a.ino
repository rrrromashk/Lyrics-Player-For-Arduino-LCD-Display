#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int MAX_PAGES = 20;

String pages[MAX_PAGES];

int pageCount = 0;
int currentPage = 0;

unsigned long lastPageChange = 0;
const unsigned long PAGE_DELAY = 2000;


// ==================================================
// SETUP
// ==================================================

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}


// ==================================================
// LOOP
// ==================================================

void loop() {

  // Получаем новую строку от Python
  if (Serial.available() > 0) {

    String text = Serial.readStringUntil('\n');
    text.trim();

    if (text.length() > 0) {

      splitText(text);

      currentPage = 0;

      showPage(currentPage);

      lastPageChange = millis();
    }
  }


  // Переходим на следующую страницу
  if (pageCount > 1) {

    if (
      currentPage < pageCount - 1 &&
      millis() - lastPageChange >= PAGE_DELAY
    ) {

      currentPage++;

      showPage(currentPage);

      lastPageChange = millis();
    }
  }
}


// ==================================================
// РАЗБИВАЕМ ТЕКСТ НА СТРАНИЦЫ
// ==================================================

void splitText(String text) {

  pageCount = 0;

  String line1 = "";
  String line2 = "";


  while (text.length() > 0 && pageCount < MAX_PAGES) {

    // Убираем лишние пробелы в начале
    while (text.startsWith(" ")) {
      text.remove(0, 1);
    }

    if (text.length() == 0) {
      break;
    }


    // Ищем следующее слово
    int spaceIndex = text.indexOf(' ');

    String word;

    if (spaceIndex == -1) {
      word = text;
      text = "";
    }
    else {
      word = text.substring(0, spaceIndex);
      text.remove(0, spaceIndex + 1);
    }


    // ==========================================
    // Слово длиннее 16 символов
    // ==========================================

    if (word.length() > 16) {

      // Сначала сохраняем текущую страницу
      if (line1.length() > 0 || line2.length() > 0) {

        savePage(line1, line2);

        line1 = "";
        line2 = "";
      }


      // Разбиваем длинное слово
      while (word.length() > 16 && pageCount < MAX_PAGES) {

        pages[pageCount] = word.substring(0, 16);
        pageCount++;

        word = word.substring(16);
      }


      // Остаток длинного слова
      if (word.length() > 0) {
        line1 = word;
      }

      continue;
    }


    // ==========================================
    // Пытаемся добавить слово в первую строку
    // ==========================================

    if (line1.length() == 0) {

      line1 = word;

    }
    else if (line1.length() + 1 + word.length() <= 16) {

      line1 += " ";
      line1 += word;

    }

    // ==========================================
    // Иначе пробуем вторую строку
    // ==========================================

    else if (line2.length() == 0) {

      line2 = word;

    }
    else if (line2.length() + 1 + word.length() <= 16) {

      line2 += " ";
      line2 += word;

    }

    // ==========================================
    // Обе строки заполнены
    // ==========================================

    else {

      savePage(line1, line2);

      line1 = word;
      line2 = "";
    }
  }


  // Сохраняем остаток
  if (line1.length() > 0 || line2.length() > 0) {

    savePage(line1, line2);
  }


  // Защита от пустой строки
  if (pageCount == 0) {

    pages[0] = "|";
    pageCount = 1;
  }
}


// ==================================================
// СОХРАНЯЕМ СТРАНИЦУ
// ==================================================

void savePage(String line1, String line2) {

  if (pageCount >= MAX_PAGES) {
    return;
  }

  // Храним две строки через разделитель
  pages[pageCount] = line1 + "|" + line2;

  pageCount++;
}


// ==================================================
// ПОКАЗЫВАЕМ СТРАНИЦУ
// ==================================================

void showPage(int index) {

  if (index < 0 || index >= pageCount) {
    return;
  }

  String page = pages[index];

  int separator = page.indexOf('|');

  String line1 = "";
  String line2 = "";


  if (separator == -1) {

    line1 = page;

  }
  else {

    line1 = page.substring(0, separator);
    line2 = page.substring(separator + 1);
  }


  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(line1);

  lcd.setCursor(0, 1);
  lcd.print(line2);
}