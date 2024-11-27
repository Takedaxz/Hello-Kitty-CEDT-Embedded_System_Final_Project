#include <Arduino.h>
#include "esp_camera.h"

// Define GPIO pin mappings for the ESP32-CAM (AI-Thinker model)
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM      5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

void setup() {
  Serial.begin(115200); // Initialize UART communication
  Serial.setTimeout(50);

  // Camera configuration
  camera_config_t config;
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
  config.xclk_freq_hz = 20000000;  // XCLK frequency
  config.pixel_format = PIXFORMAT_JPEG; // Output format

  // Camera settings
  config.frame_size = FRAMESIZE_QVGA; // Frame size (QVGA = 320x240)
  config.jpeg_quality = 12;          // JPEG quality (lower = better quality)
  config.fb_count = 1;               // Number of frame buffers

  // Initialize the camera
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera Init Failed!");
    return;
  }

  Serial.println("Camera Ready");
}

void loop() {
  // Capture a frame
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Send image size and data over UART
  uint32_t image_size = fb->len;
  Serial.write((uint8_t*)&image_size, sizeof(image_size)); // Send size first
  Serial.write(fb->buf, fb->len);                         // Send image data

  esp_camera_fb_return(fb); // Return the frame buffer
  delay(1000);              // Adjust the capture rate (1 frame per second)
}
