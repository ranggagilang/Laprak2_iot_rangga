#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define LDRPIN 32  // Pin Analog untuk LDR
#define TIMEDHT 2000  // Update setiap 2 detik

// Inisialisasi sensor dan LCD
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Jika tidak tampil, coba ganti 0x27 ke 0x3F

// Variabel untuk menyimpan data sensor
float humidity, celsius;
int lightIntensity;
uint32_t timerDHT = 0;

// Fungsi untuk membaca suhu dan kelembapan
void getTemperature() {
    humidity = dht.readHumidity();
    celsius = dht.readTemperature();

    if (isnan(humidity) || isnan(celsius)) {
        Serial.println("Failed to read from DHT sensor!");
        humidity = 0.0;
        celsius = 0.0;
    }
}

// Fungsi untuk membaca intensitas cahaya
void getLightIntensity() {
    lightIntensity = analogRead(LDRPIN);
}

// Cek apakah LCD terdeteksi di I2C
void checkI2C() {
    Wire.begin();
    Serial.println("Scanning for I2C devices...");
    for (byte address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            Serial.print("Found I2C device at address 0x");
            Serial.println(address, HEX);
        }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Monitoring Started...");

    checkI2C(); // Cek alamat I2C LCD

    dht.begin();
    lcd.init();  // Pastikan menggunakan lcd.init() bukan lcd.begin();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("System Starting...");
    delay(2000);
    lcd.clear();
}

void loop() {
    getTemperature();
    getLightIntensity();

    Serial.print("Suhu: "); Serial.print(celsius); Serial.print(" °C | ");
    Serial.print("Kelembapan: "); Serial.print(humidity); Serial.print(" % | ");
    Serial.print("Cahaya: "); Serial.println(lightIntensity);

    // Tampilkan di LCD tanpa flicker
    lcd.setCursor(0, 0);
    lcd.print("S: "); lcd.print(celsius, 1); lcd.print("C ");
    lcd.print("H: "); lcd.print(humidity, 1); lcd.print("% ");

    lcd.setCursor(0, 1);
    lcd.print("Light: "); lcd.print(lightIntensity);
    lcd.print("    "); // Tambahkan spasi untuk menghapus digit lama

    delay(2000);
}
