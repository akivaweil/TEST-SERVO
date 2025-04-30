# ESP32 Servo Control Web Interface Tutorial

This document explains how to control the servo connected to your ESP32-S3 using a web browser interface.

## Prerequisites

1.  **ESP32-S3 Flashed:** Ensure your Freenove ESP32-S3 board is flashed with the latest `src/Test_Servo.cpp` firmware from this project.
2.  **WiFi Connection:** The ESP32-S3 must be connected to the "Everwood" WiFi network. It will have the static IP address `192.168.1.244`.
3.  **Servo Connected:** A servo motor should be connected to the initially defined pin (GPIO 47) and powered appropriately.
4.  **Web Browser:** Any modern web browser (Chrome, Firefox, Edge, Safari) on a device connected to the same WiFi network.

## Accessing the Control Interface

1.  Open your web browser.
2.  Navigate to the ESP32's IP address: `http://192.168.1.244` (or simply typing `192.168.1.244` might work in most browsers).

You should see a web page titled "ESP32 Servo Control".

## Controlling the Servo via the Web Page

The web page provides controls for the servo:

1.  **Set Servo Angle:**
    *   Use the slider to select the desired angle (0-180).
    *   The angle value is displayed next to the slider label.
    *   The angle command is sent automatically when you release the slider.

2.  **Change Servo Pin:**
    *   Enter the new desired GPIO pin number into the text box under "Change Servo Pin".
    *   Click the "Set Pin" button.
    *   **Important:** Ensure the pin is valid for servo output on your ESP32-S3 and not used for other critical functions. Refer to the pinout.
    *   After a successful change, the status message will update, and the placeholder in the pin input box might update to show the new current pin.

## Status Display

A status message at the bottom of the page indicates the WebSocket connection status and shows responses received from the ESP32 (e.g., confirmations of angle/pin changes, errors).

## Behind the Scenes (WebSocket)

While you interact with the web page, the JavaScript code within the page automatically handles the communication with the ESP32's WebSocket server (running on port 81). You don't need to interact with the WebSocket directly anymore.

## Troubleshooting

*   **Web Page Not Loading:** Verify the ESP32-S3 is powered, connected to the "Everwood" WiFi, and has the IP address `192.168.1.244`. Check the Serial Monitor output for connection status or errors. Ensure your browser device is on the same network.
*   **Controls Not Working:** Check the status message on the web page. It might indicate a WebSocket connection issue. Open your browser's developer console (usually F12) and look for JavaScript errors or WebSocket connection messages.
*   **Servo Not Moving:** Double-check servo wiring (Signal, 5V, GND) and power supply. Verify you are trying to control the correct pin (check the status message or placeholder text for the current pin). 