/*
Copyright 2018 Sekigon

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../lib/mode_icon_reader.c"
#include "../lib/layer_state_reader.c"
#include "../lib/host_led_state_reader.c"
#include "../lib/logo_reader.c"
#include "../lib/keylogger.c"
#include "../lib/timelogger.c"

#include QMK_KEYBOARD_H
#include "app_ble_func.h"

enum custom_keycodes {
    AD_WO_L = SAFE_RANGE, /* Start advertising without whitelist  */
    BLE_DIS,              /* Disable BLE HID sending              */
    BLE_EN,               /* Enable BLE HID sending               */
    USB_DIS,              /* Disable USB HID sending              */
    USB_EN,               /* Enable USB HID sending               */
    DELBNDS,              /* Delete all bonding                   */
    ADV_ID0,              /* Start advertising to PeerID 0        */
    ADV_ID1,              /* Start advertising to PeerID 1        */
    ADV_ID2,              /* Start advertising to PeerID 2        */
    ADV_ID3,              /* Start advertising to PeerID 3        */
    ADV_ID4,              /* Start advertising to PeerID 4        */
    BATT_LV,              /* Display battery level in milli volts */
    DEL_ID0,              /* Delete bonding of PeerID 0           */
    DEL_ID1,              /* Delete bonding of PeerID 1           */
    DEL_ID2,              /* Delete bonding of PeerID 2           */
    DEL_ID3,              /* Delete bonding of PeerID 3           */
    DEL_ID4,              /* Delete bonding of PeerID 4           */
    ENT_DFU,              /* Start bootloader                     */
    ENT_SLP,              /* Deep sleep mode                      */
    QWERTY,
    LOWER,
    RAISE,
    ADJUST,
    BACKLIT,
    EISU,
    KANA,
    RGBRST
};

extern keymap_config_t keymap_config;

enum layer_number {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST
};

#define KC_CTLTB CTL_T(KC_TAB)
#define KC_GUIEI GUI_T(KC_LANG2)
#define KC_ALTKN ALT_T(KC_LANG1)

// Fillers to make layering more clear
#define RSFT_QT MT(MOD_RSFT,KC_QUOT)  // タップで「'」 ホールドで右Shift

#define LOW_ES  LT(_LOWER, KC_LANG2)  // タップで英数 ホールドでLower
#define RIS_KN  LT(_RAISE, KC_LANG1)  // タップでかな ホールドでRaise
#define LOWER  MO(_LOWER)           // ホールドでAdjustレイヤーをon
#define RAISE  MO(_RAISE)           // ホールドでAdjustレイヤーをon
#define ADJUST  MO(_ADJUST)           // ホールドでAdjustレイヤーをon
#define MY_VOLD LALT(LSFT(KC_VOLD))   // 細かいボリュームダウン
#define MY_VOLU LALT(LSFT(KC_VOLU))   // 細かいボリュームアップ

#define TGL_LOW TG(_LOWER)            // Lowerトグル
#define TGL_RIS TG(_RAISE)            // Raiseトグル
#define _______ KC_TRNS
#define XXXXXXX KC_NO

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT( \
  //,-----------------------------------.                ,--------------------------------.
        Q,     W,     E,     R,     T,                      Y,     U,     I,     O,     P,\
  //|------+------+------+------+------|                |------+------+------+------+-----|
        A,     S,     D,     F,     G,                      H,     J,     K,     L,  SCLN,\
  //|------+------+------+------+------|                |------+------+------+------+-----|
        Z,     X,     C,     V,     B,                      N,     M,  COMM,   DOT,  SLSH,\
  //|------+------+------+------+------+------|  |------+------+------+------+------+-----|
                          GUIEI, LOWER,   SPC,      ENT, RAISE, ALTKN \
                       //`--------------------'  `--------------------'
  ),
  [_LOWER] = LAYOUT_kc( \
  //,-----------------------------------.               ,----------------------------------.
       1,     2,     3,     4,     5,                      6,     7,     8,     9,     0, \
  //|------+------+------+------+------|                |------+------+------+------+-----|
       F1,    F2,    F3,    F4,    F5,                     F6,    F7,    F8,    F9,   F10,\
  //|------+------+------+------+------|                |------+------+------+------+-----|
      F11,   F12,   F13,   F14,   F15,                    F16,   F17,   F18,   F19,   F20,\
  //|------+------+------+------+------+------|  |------+------+------+------+------+-----|
                          GUIEI, LOWER,  SPC,      ENT,  RAISE, ALTKN \
                       //`--------------------'  `--------------------'
  ),

  [_RAISE] = LAYOUT_kc( \
  //,----------------------------------.                ,----------------------------------.
      EXLM,    AT,  HASH,   DLR,  PERC,                   CIRC,  AMPR,  ASTR,  LPRN,  RPRN,\
  //|------+------+------+------+------|                |------+------+------+------+------|
     XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,                   MINS,   EQL,  LCBR,  RCBR,  PIPE,\
  //|------+------+------+------+------|                |------+------+------+------+------|
     XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,                   UNDS,  PLUS,  LBRC,  RBRC,  BSLS,\
  //|------+------+------+------+------+------|  |------+------+------+------+------+------.
                          GUIEI, LOWER,  SPC,      ENT, RAISE, ALTKN \
                      //`--------------------'  `--------------------'
  ),

  [_ADJUST] = LAYOUT_kc( \
  //,----------------------------------.                ,----------------------------------.
      LRST, XXXXX, XXXXX, XXXXX, XXXXX,                  XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,\
  //|------+------+------+------+------|                |------+------+------+------+------|
      LHUI,  LSAI,  LVAI, XXXXX, XXXXX,                  XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,\
  //|------+------+------+------+------|                |------+------+------+------+------|
      LHUD,  LSAD,  LVAD, XXXXX, XXXXX,                  XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|  |------+------+------+------+------+------|
                          GUIEI, LOWER,   SPC,      ENT, RAISE, ALTKN \
                       //`--------------------'  `--------------------'
  )
};


};

size_t sizeof_keymaps() {
  return sizeof(keymaps);
}

// // define variables for reactive RGB
// bool TOG_STATUS = false;
// int RGB_current_mode;
// #ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
  layer_state_set(default_layer);
}

// // Setting ADJUST layer RGB back to default
// void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
//   if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
//     #ifdef RGBLIGHT_ENABLE
//       //rgblight_mode(RGB_current_mode);
//     #endif
//     layer_on(layer3);
//   } else {
//     layer_off(layer3);
//   }
// }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    set_keylog(keycode, record);
    set_timelog();
  }

  char str[16];
  if (record->event.pressed) {
    switch (keycode) {
    case DELBNDS:
      delete_bonds();
      return false;
    case AD_WO_L:
      restart_advertising_wo_whitelist();
      return false;
    case USB_EN:
      set_usb_enabled(true);
      return false;
      break;
    case USB_DIS:
      set_usb_enabled(false);
      return false;
      break;
    case BLE_EN:
      set_ble_enabled(true);
      return false;
      break;
    case BLE_DIS:
      set_ble_enabled(false);
      return false;
      break;
    case ADV_ID0:
      restart_advertising_id(0);
      return false;
    case ADV_ID1:
      restart_advertising_id(1);
      return false;
    case ADV_ID2:
      restart_advertising_id(2);
      return false;
    case ADV_ID3:
      restart_advertising_id(3);
      return false;
    case ADV_ID4:
      restart_advertising_id(4);
      return false;
    case DEL_ID0:
      delete_bond_id(0);
      return false;
    case DEL_ID1:
      delete_bond_id(1);
      return false;
    case DEL_ID2:
      delete_bond_id(2);
      return false;
    case DEL_ID3:
      delete_bond_id(3);
      return false;
    case BATT_LV:
      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
      return false;
    case ENT_DFU:
      bootloader_jump();
      return false;
    }
  }
  else if (!record->event.pressed) {
    switch (keycode) {
    case ENT_SLP:
      sleep_mode_enter();
      return false;
    }
  }
  return true;
}
;
