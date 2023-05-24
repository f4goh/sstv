/* 
 * File:   Camera.cpp
 * Author: philippe SIMIER Lycée Touchard Washington Le Mans
 * 
 * Classe pour ESP32-CAM  (AI Thinker)
 * 
 * Created on 23 juin 2022, 14:08
 */

#include "Camera.h"

Camera::Camera() :
fb(NULL),
s(NULL),
count(0) {
 
    esp_log_level_set("gpio", ESP_LOG_NONE);
}



Camera::~Camera() {
}

/**
 * @brief initialise la camera avec un format et une taille d'image
 *        Cette fonction détecte et configure la caméra sur l'interface I2C, 
 *        alloue le framebuffer et les tampons DMA, 
 *        initialise l'entrée I2S parallèle et configure les descripteurs DMA. 
 *        Actuellement, cette fonction ne peut être appelée qu'une seule fois et 
 *        il n'y a aucun moyen de désinitialiser ce module sans passer par un reset camera. 
 * @param pixFormat   YUV422, GRAYSCALE, RGB565, JPEG
 * @param size            FRAMESIZE_QVGA (320 x 240)
 *                        FRAMESIZE_CIF  (352 x 288)
 *                        FRAMESIZE_VGA  (640 x 480)
 *                        FRAMESIZE_SVGA (800 x 600)
 *                        FRAMESIZE_XGA  (1024 x 768)
 *                        FRAMESIZE_SXGA (1280 x 1024)
 *                        FRAMESIZE_UXGA (1600 x 1200)
 * 
 * N'utilisez pas de tailles supérieures à VGA lorsqu'elles ne sont pas au format JPEG
 * @return            true si tout c'est bien passé
 */
bool Camera::init(pixformat_t pixFormat, framesize_t size) {

    camera_config_t config;
    bool retour;

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = pixFormat; // PIXFORMAT_YUV422; //PIXFORMAT_RGB565;
    config.frame_size = size; // QQVGA-UXGA N'utilisez pas de tailles supérieures à VGA lorsqu'elles ne sont pas JPEG
    config.jpeg_quality = 10; // 0-63 un nombre inférieur signifie une meilleure qualité
    config.fb_count = 1; // s'il y en a plusieurs, i2s s'exécute en mode continu. Utiliser uniquement avec JPEG

    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        retour = false;
    } else {
        s = esp_camera_sensor_get();
        retour = true;
    }

    return retour;
}

/**
 * @brief vertical flip
 */
void Camera::vflip() {

    s->set_vflip(s, 1);
}

/**
 * @brief horizontal mirror
 */
void Camera::hmirror() {
    s->set_hmirror(s, 1);
}

/**
 * 
 * @param level -2 to 2
 */
/*
void Camera::setContrast(int level) {
    if (level <= 2 && level >= -2)
        s->set_contrast(s, level);
}
*/

void Camera::setBrightness(int level) {
    if (level <= 2 && level >= -2)
        s->set_brightness(s, level);
    
}

void Camera::setWb(int balance) {
     if (balance <= 0 && balance >= 1)
        s->set_whitebal(s, balance);
    
}
void Camera::setWbMode(int balance) {
     if (balance <= 0 && balance >= 4)
        s->set_wb_mode(s, balance);
    
}

void Camera::setContrast(int contrast) {
     if (contrast <= -2 && contrast >= 2)
        s->set_contrast(s, contrast);
    
}
void Camera::setCtrlGain(int mode){
    if (mode <=0 && mode >=1)
        s->set_gain_ctrl(s, mode);
   
}

void Camera::setAgcGain(int mode){
    if (mode <=0 && mode >=30)
        s->set_agc_gain(s, mode);
  
}

void Camera::setExposureCtrl(int valeur){
   if (valeur <=0 && valeur >=1)
        s->set_exposure_ctrl(s, valeur);

}

void Camera::setAecValue(int expo){
   if (expo <=0 && expo >=1200)
       s->set_aec_value(s, expo);

}

void Camera::setAwbGain(int gain){
      if (gain <=0 && gain >=1)
       s->set_awb_gain(s, gain);
}

void Camera::setSpecialEffect(int effet){
    if (effet <=0 && effet >=6)
       s->set_special_effect(s, effet);
}



/**
 * 
 * @param framesize
 */
void Camera::setFrameSize(framesize_t framesize) {
    s->set_framesize(s, framesize);
}


/**
 * @brief Resetting camera by power down line
 * @param tick nb of ms for delay
 */
void Camera::reset(byte tick) {
    esp_camera_deinit();
    Serial.println("Resetting camera by power down line");
    digitalWrite(PWDN_GPIO_NUM, LOW);
    delay(1);
    digitalWrite(PWDN_GPIO_NUM, HIGH);
    delay(tick);
}

/**
 * @brief initialise le lecteur de carte SD (Bus SPI) et vérifie si une carte est présente
 * @return true si une carte SD est présente
 */
bool Camera::SDinit() {

    bool retour;

    // Initialisation du bus SPI
    SPI.begin(14, 2, 15, 13);
    delay(10);

    // Initialisation de la carte SD
    if (SDFileSystem.begin(13)) {

        Serial.println("Wiring is correct and a card is present.");
        Serial.println();

        Serial.print("Card type: ");
        switch (SDFileSystem.cardType()) {
            case CARD_NONE:
                Serial.println("NONE");
                break;
            case CARD_MMC:
                Serial.println("MMC");
                break;
            case CARD_SD:
                Serial.println("SD");
                break;
            case CARD_SDHC:
                Serial.println("SDHC");
                break;
            default:
                Serial.println("Unknown");
        }


        uint64_t cardSize = SDFileSystem.cardSize() / (1024 * 1024);
        Serial.printf("SD Card Size: %llu Mb\r\n", cardSize);

        uint64_t totalBytes = SDFileSystem.totalBytes() / (1024 * 1024);
        Serial.printf("Total bytes : %llu Mb\r\n", totalBytes);

        uint64_t usedBytes = SDFileSystem.usedBytes() / 1024;
        Serial.printf("Used bytes  : %llu Kb\r\n\n", usedBytes);

        retour = true;


    } else {
        Serial.println("Initialization card failed.");
        retour = false;
    }

    return retour;
}

/**
 * @brief       Enregistre la photo sur la carte SD
 * @param name  le nom du fichier
 * @return      true si le fichier est enregistré
 */
bool Camera::SaveSD(String name,  String ext) {

    // Path where new picture will be saved in SD Card

    String path = "/" + name + String(count) + "." + ext;
    Serial.printf("Picture file name: %s\n\r", path.c_str());

    File file = SDFileSystem.open(path.c_str(), FILE_WRITE);

    if (!file) {
        Serial.println("Failed to open file in writing mode");
        return false;
    } else {
        file.write(fb->buf, fb->len); // payload (image), payload length
        Serial.printf("Saved file to path: %s\n\r", path.c_str());
        count++;
    }
    file.close();
    esp_camera_fb_return(fb);
    return true;
}

/**
 * @brief  capture a frame  
 * @return true if OK
 */
void Camera::frameback() {
    esp_camera_fb_return(fb);
}


/**
 * @brief  capture a frame  
 * @return true if OK
 */
bool Camera::capturePhoto() {

    fb = NULL;

    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Frame buffer could not be acquired");
        return false;
    }
    return true;
}

/**
 * @brief  return Pointer to the pixel data
 * @return the pointer to the pixel data
 */
uint8_t * Camera::getBuf() {
    return fb->buf;
}

/**
 * 
 * @return Length of the buffer in bytes
 */
size_t Camera::getLen() {
    return fb->len;
}

/**
 * 
 * @return Width of the buffer in pixels
 */
size_t Camera::getwidth() {
    return fb->width;
}

/**
 * 
 * @return Height of the buffer in pixels
 */
size_t Camera::getheight() {
    return fb->height;
}

/**
 * @brief turn on the flash for N seconds
 * @param N nombre de secondes
 */
void Camera::flash(int N) {
    digitalWrite(FLASH_GPIO_NUM, HIGH);
    delay(N * 1000);
    digitalWrite(FLASH_GPIO_NUM, LOW);
}