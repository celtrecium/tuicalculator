/* This header file contains Tuicalc settings */

#ifndef TUICALC_SETTINGS_H
#define TUICALC_SETTINGS_H
#include <LNC/colors.h>

int calc_color_bg{static_cast<int>(lnc::COLOR::BG::CYAN)};
int calc_color_fg{static_cast<int>(lnc::COLOR::FG::WHITE)};
int calc_color_shbg{static_cast<int>(lnc::COLOR::BG::BLACK)};
int calc_color_shfg{static_cast<int>(lnc::COLOR::FG::BRIGHT_BLACK)};
int button_color_bg{static_cast<int>(lnc::COLOR::BG::BRIGHT_WHITE)};
int button_color_fg{static_cast<int>(lnc::COLOR::FG::BLACK)};
int button_color_active_bg{static_cast<int>(lnc::COLOR::BG::BRIGHT_CYAN)};
int button_color_active_fg{static_cast<int>(lnc::COLOR::FG::BLACK)};
int num_screen_bg{static_cast<int>(lnc::COLOR::BG::BLACK)};
int num_screen_fg{static_cast<int>(lnc::COLOR::FG::WHITE)};
int logo_bg{static_cast<int>(lnc::COLOR::BG::BRIGHT_CYAN)};
int logo_fg{static_cast<int>(lnc::COLOR::FG::BLACK)};

#endif /* TUICALC_SETTINGS_H */
