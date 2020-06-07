#include <iostream>
#include <string>
#include <LNC/ui.h>
#include <settings.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <csignal>


/* Parameters */
namespace calc{
    const int buttons_array_x { 5 };
    const int buttons_array_y { 5 };
    bool no_repeat { true };
    bool buttons_action { false };
    bool other_actions { false };
    bool end { false };
    bool redraw { false };
}

/* Set interface */
lnc::ui::window calc_window(1, 1, 20, 13, calc_color_bg, calc_color_fg);
lnc::ui::buttons_array calc_buttons(calc_window.get_x() + 1, calc_window.get_y() + 4, button_color_bg, button_color_fg,
        button_color_active_bg, button_color_active_fg, calc::buttons_array_x, calc::buttons_array_y);
lnc::ui::form calc_form_1 (calc_window.get_x() + 1, calc_window.get_y() + 1, calc_window.get_w() - 2, num_screen_bg, num_screen_fg);
lnc::ui::form calc_form_2 (calc_window.get_x() + 1, calc_window.get_y() + 2, calc_window.get_w() - 1, num_screen_bg, num_screen_fg);
lnc::ui::picture calc_picture (calc_window.get_x() + 21, calc_form_2.get_y() + 2, 29, 9, "logoscreen.esc");

/* Set buttons array */
std::string buttons_array [ calc::buttons_array_x ] [ calc::buttons_array_y ] {
    { " Q ", " C ", " B ", " s ", " c " },
	{ " 1 ", " 2 ", " 3 ", " / ", " √ " },
	{ " 4 ", " 5 ", " 6 ", " * ", " ^ " },
	{ " 7 ", " 8 ", " 9 ", " - ", " ( " },
	{ " 0 ", " . ", " = ", " + ", " ) " }
};

/* Set array for printing on text forms */
std::string symbols_pos_array [ calc::buttons_array_x ] [ calc::buttons_array_y ] {
    { "",  "",  "",  "s(", "c(" },
	{ "1", "2", "3", "/",  "sqrt(" },
	{ "4", "5", "6", "*",  "^" },
	{ "7", "8", "9", "-",  "(" },
	{ "0", ".", "",  "+",  ")" }
};

/* Set array for keycodes of buttons */
int keycodes_pos_array [ calc::buttons_array_x ] [ calc::buttons_array_y ] {
    { 113, 99, 127, 100, 118},
    { 49,  50, 51,  47,  115},
    { 52,  53, 54,  42,  94},
    { 55,  56, 57,  45,  40},
    { 48,  46, 61,  43,  41}
};

/* Calculator functions */

void set_active_button_func(int pos_x, int pos_y){
    calc_form_2.set_text(calc_form_2.get_text() + symbols_pos_array[pos_y][pos_x]);
    calc_buttons.set_active_button(pos_x, pos_y);
    calc::buttons_action = true;
    calc::no_repeat = true;
}

void send_to_second_form() {
    calc_form_1.set_text(calc_form_1.get_text() + calc_form_2.get_text());
    calc_form_2.set_text("");
}

void calculating_func(int x_pos, int y_pos) {
    if(calc::no_repeat){
        send_to_second_form();
        set_active_button_func(x_pos, y_pos);
        /* KOSTYL */
        std::ifstream result_file("result");
        system(( "echo \"scale = 2; " + calc_form_1.get_text() + "\" | bc -l > result").c_str());
        std::string tmp_res;
        getline(result_file, tmp_res);
        result_file.close();
        calc_form_2.set_text(tmp_res);
        tmp_res.clear();
        calc::buttons_action = true;
        calc::no_repeat = false;
    }
}

void no_repeat_protect(int x_pos, int y_pos){
    if(calc::no_repeat){
        send_to_second_form();
        set_active_button_func(x_pos, y_pos); 
        calc::no_repeat = false;
    }
}

/* For system signals */
void sign_interrupt(int sig_num){
    sig_num = 0;
    calc::end = true;
}

void sign_tty_stop(int sig_num){
    sig_num = 0;
    lnc::ui::close_prog();
    raise(SIGSTOP);
}

void sign_continue(int sig_num){
    sig_num = 0;
    lnc::ui::begin_prog();
    calc::redraw = true;
}

/* Drawing interface */
void interface_thread(){
    int tmp_size_w { 0 };
    int tmp_size_h { 0 };
    calc_window.set_window_colors(calc_color_bg, calc_color_fg, calc_color_shbg, calc_color_shfg);
    calc_window.set_window_min_size(20, 13);
    calc_window.set_window_max_size(50, 13);
    calc_window.draw_window();
    calc_form_1.draw_form();
    calc_form_2.draw_form(); 
    calc_buttons.draw_buttons(buttons_array);
    for(;;){
        lnc::ui::win_size_test();
        if(tmp_size_w != lnc::ui::win_size_w || tmp_size_h != lnc::ui::win_size_h || calc::redraw){
            tmp_size_w = lnc::ui::win_size_w;
            tmp_size_h = lnc::ui::win_size_h;
            if(tmp_size_w < (calc_window.get_min_width() + 1) ||
                    tmp_size_h < (calc_window.get_min_height() + 1)){
                lnc::dl::clear();
                lnc::dl::printxy(1, 1, "Your screen\ntoo small", 49, 37);
            }
            else {
                calc_window.set_window_size(
                        lnc::ui::win_size_w - calc_window.get_x() - calc_window.get_sh_offset_x(),
                        lnc::ui::win_size_h - calc_window.get_y() - calc_window.get_sh_offset_y()
                        );
                calc_form_1.set_form_position(calc_window.get_x() + 1, calc_window.get_y() + 1);
                calc_form_2.set_form_position(calc_window.get_x() + 1, calc_window.get_y() + 2);
                calc_form_1.set_width(calc_window.get_w() - 2);
                calc_form_2.set_width(calc_window.get_w() - 2);
                calc_buttons.set_buttons_pos(calc_window.get_x() + 1, calc_window.get_y() + 4);
                lnc::dl::clear(); 
                calc_window.draw_window();
                calc_form_1.draw_form();
                calc_form_2.draw_form();           
                calc_buttons.draw_buttons(buttons_array);
                if(calc_window.get_w() >= calc_window.get_max_width()){
                    calc_picture.set_pos_x(calc_window.get_x() + 21);
                    calc_picture.set_pos_y(calc_form_2.get_y() + 2);
                    calc_picture.draw_picture();
                }
                calc::redraw = false;
            }
        }
        if(calc::buttons_action){
           calc_buttons.draw_buttons(buttons_array);
           calc_form_1.draw_form();
           calc_form_2.draw_form();
           calc::buttons_action = false;
        }
        if(calc::other_actions){
            if(calc_window.get_w() >= calc_window.get_max_width()){
                calc_picture.draw_picture();
            }
            calc::other_actions = false;
        }
        if(calc::end){
            lnc::ui::close_prog();
            exit(0);
        }
    }
}

int main(){
    lnc::ui::begin_prog();
    signal(SIGINT, sign_interrupt);
    signal(SIGTSTP, sign_tty_stop);
    signal(SIGCONT, sign_continue);
    std::thread interface(interface_thread);
    interface.detach();
    int keycode { 0 };
    bool boolean_pos_to_key { false };
    for(;;){
        if(boolean_pos_to_key) {
            keycode = keycodes_pos_array [ calc_buttons.get_active_button_y() ][ calc_buttons.get_active_button_x() ]; 
            boolean_pos_to_key = false;
        }
        else keycode = getchar();
        switch( keycode ) {
            case 67:    /* Right */
                calc_buttons.select_by_arrows(lnc::ui::ARROW::RIGHT);
                calc::buttons_action = true;
				break;
            case 68:    /* Left */
                calc_buttons.select_by_arrows(lnc::ui::ARROW::LEFT);
                calc::buttons_action = true;
				break;
			case 65:    /* Up */
                calc_buttons.select_by_arrows(lnc::ui::ARROW::UP);
                calc::buttons_action = true;
				break;
			case 66:    /* Down */
                calc_buttons.select_by_arrows(lnc::ui::ARROW::DOWN);
                calc::buttons_action = true;
				break;
			case 10:    /* Return */
                boolean_pos_to_key = true;
				break;
			case 49: set_active_button_func(0, 1); break;  /* 1 */
			case 50: set_active_button_func(1, 1); break;  /* 2 */
			case 51: set_active_button_func(2, 1); break;  /* 3 */
			case 52: set_active_button_func(0, 2); break;  /* 4 */
			case 53: set_active_button_func(1, 2); break;  /* 5 */
			case 54: set_active_button_func(2, 2); break;  /* 6 */
			case 55: set_active_button_func(0, 3); break;  /* 7 */
			case 56: set_active_button_func(1, 3); break;  /* 8 */
			case 57: set_active_button_func(2, 3); break;  /* 9 */
			case 48: set_active_button_func(0, 4); break;  /* 0 */
			case 47: no_repeat_protect(3, 1); break;  /* / */
			case 42: no_repeat_protect(3, 2); break;  /* * */
			case 45: no_repeat_protect(3, 3); break;  /* - */
			case 43: no_repeat_protect(3, 4); break;  /* + */
            case 61: calculating_func(2, 4); break;  /* = */
			case 99: 
                     calc_form_1.set_text("");
                     calc_form_2.set_text("");
                     set_active_button_func(1, 0);
                     calc::no_repeat = true;
                     break;  /* c */
			case 115: set_active_button_func(4, 1); break; /* s */
			case 94: no_repeat_protect(4, 2); break;  /* ^ */
			case 40: set_active_button_func(4, 3); break;  /* ( */
			case 41: set_active_button_func(4, 4); break;  /* ) */
            case 113: set_active_button_func(0, 0); calc::end = true; break; /* q */
			case 100: set_active_button_func(3, 0); break; /* d */
			case 118: set_active_button_func(4, 0); break; /* v */
            case 127: calc_form_2.pop_back_string(); set_active_button_func(2, 0); break; /* Backspace */
			case 46: set_active_button_func(1, 4); break;  /* . */
            case 104: calc_picture.set_picture("helpscreen.esc"); calc::other_actions = true; break; /* h */
			case 98: calc_picture.set_picture("logoscreen.esc"); calc::other_actions = true; break;   /* b */
        }
    }
}
