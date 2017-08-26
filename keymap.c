#include "satan.h"
#include "mousekey.h"
#include <string.h>

/*
 * Here we want to map the ISO layout to a mapping that corresponds to the PCB's matrix.
 * We ignore certain keys:
 * k1d: Usually the backslash/pipe button on ANSI boards, this key has been relegated to the right of left shift.
 * k3c: This is a key to the *right* of right shift, used for split-shift layouts. It appears on its left in the keyboard matrix.
 * k43 through k49, excluding k45 - These are dead keys to populate the matrix width.
*/
#define KEYMAP_ISO( \
  k00,   k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0d, \
  k10,    k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c,      \
  k20,     k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, k2c, k2d, \
  k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b,     k3d,       \
  k40,  k41,  k42,                 k45,           k4a,   k4b,   k4c,   k4d   \
) { \
	{k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0d}, \
	{k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, XXX}, \
	{k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, k2c, k2d}, \
	{k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, XXX, k3d}, \
	{k40, k41, k42, XXX, XXX, k45, XXX, XXX, XXX, XXX, k4a, k4b, k4c, k4d}  \
}

// Here, we use #define to name our layers instead of having to remember indices directly.

#define BASE_LAYER 0 // Regular alphanumerics and modifiers, except caps lock used for the F_KEYS_LAYER and Apps used for the NAV_LAYER
#define F_KEYS_LAYER 1 // Maps F1 through F12 onto the keys KC_1 through KC_EQUALS (k01 through k0c)
#define EXT_F_KEYS_LAYER 2 // Like F_KEYS_LAYER, but with F13 through F24 - These keys are seldom used and make good candidates for software macros.

#define NAV_LAYER 3 // Maps WASD to the up, left, down, and right arrow keys respectively. Also includes toggles for 'deeper' layers.
#define EXT_NAV_LAYER 4 // Toggled by pressing caps lock while in NAV_LAYER - Maps WASD to the page up, home, page down, and end keys respectively.

#define INTERNAL_LAYER 5 // Toggled by pressing right super key while in F_KEYS_LAYER - contains mouse keys, media keys, and bootmagic toggles (à la nkro)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  #define _BS KC_NONUS_BSLASH
  #define CAPS_F_TOGG LT(F_KEYS_LAYER, KC_CAPS)
  [BASE_LAYER] = KEYMAP_ISO( \
    KC_ESCAPE, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPACE, \
    KC_TAB,     KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, \
    CAPS_F_TOGG, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT, \
    KC_LSFT, _BS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,    KC_RSHIFT, \
    KC_LCTRL,  KC_LGUI,  KC_LALT,         KC_SPACE,          KC_RALT,  KC_RGUI,  MO(NAV_LAYER),  KC_RCTL \
  ),

  [F_KEYS_LAYER] = KEYMAP_ISO( \
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______,                        _______,                       _______, _______, _______, _______
  ),

  [EXT_F_KEYS_LAYER] = KEYMAP_ISO(
    _______, KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______,                        _______,                       _______, _______, _______, _______
  ),

  #define CAPS_ENAV TG(EXT_NAV_LAYER)
  #define INTERN TG(INTERNAL_LAYER)
  [NAV_LAYER] = KEYMAP_ISO(
    KC_GRAVE,  _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DELETE, \
    KC_PSCR,   _______, KC_UP,   _______,  _______, _______, _______, _______, _______, _______, _______, _______,  RESET , \
    CAPS_ENAV, KC_LEFT, KC_DOWN, KC_RIGHT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,   _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,   _______, _______,                        _______,                       _______,  INTERN,  _______, _______
  ),

  [EXT_NAV_LAYER] = KEYMAP_ISO(
    _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, KC_PGUP,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, KC_HOME, KC_PGDOWN, KC_END, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______,                         _______,                       _______, _______, _______, _______
  ),

  #define NKRO_TOGG MAGIC_TOGGLE_NKRO
  [INTERNAL_LAYER] = KEYMAP_ISO(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, NKRO_TOGG, _______, _______, _______, _______, _______, \
    _______, _______, _______,                        _______,                       _______, _______, _______, _______
  )
};

bool was_nav_on;
void matrix_scan_user(void) {
  if (IS_LAYER_OFF(NAV_LAYER)) {
    if (was_nav_on) {
      layer_off(EXT_NAV_LAYER);
      layer_off(INTERNAL_LAYER);
      was_nav_on = false;
    }
  } else {
    was_nav_on = true;
  }
}

/* Transparent layer:
_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
_______, _______, _______,                        _______,                       _______, _______, _______, _______
*/