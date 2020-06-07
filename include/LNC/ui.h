#ifndef LNC_UI_H
#define LNC_UI_H
#include <termios.h>
#include "dl.h"
#include <unistd.h>
#include <sys/ioctl.h>

namespace lnc{
    namespace ui{
        enum class ARROW{
            LEFT,
            RIGHT,
            UP,
            DOWN
        };
     
        static struct termios stored_settings;
        int win_size_h;
        int win_size_w;
     
        void no_echo(){
            struct termios new_settings;
            tcgetattr(0,&stored_settings);
            new_settings = stored_settings;
            new_settings.c_lflag &= static_cast<unsigned int>(~ICANON & ~ECHO);
            new_settings.c_cc[VTIME] = 0;
            new_settings.c_cc[VMIN] = 1;
            tcsetattr(0,TCSANOW,&new_settings);
            return;
        }
     
        void echo(){
            tcsetattr(0,TCSANOW,&stored_settings);
            return;
        }
     
        void win_size_test() {
            struct winsize size;
            ioctl( 0, TIOCGWINSZ, (char *) &size );
            lnc::ui::win_size_h = size.ws_row;
            lnc::ui::win_size_w = size.ws_col;
        }
     
        class window{
            private:
                int  x, y, w, h, min_w, min_h, max_w, max_h, bg, fg, sh_bg, sh_fg, sh_offset_x, sh_offset_y;
                char sym_sh, sym;
                bool shadow;
            public: 
                window(int v_x, int v_y, int v_w, int v_h, int v_bg, int v_fg)
                    : x { v_x }, y { v_y }, w { v_w }, h { v_h }, min_w { 0 }, min_h { 0 }, max_w { v_x + v_w }, max_h { v_y + v_h },
                    bg { v_bg }, fg { v_fg }, sh_bg { 40 }, sh_fg { 30 }, sh_offset_x { 1 }, sh_offset_y { 1 }, sym_sh { ' ' },
                    sym { ' ' }, shadow { true }
                {}
                void set_window_position(int val_x, int val_y) { 
                    if(this->x > 1){
                        this->x = val_x;
                    }
                    else { this->x = 1; }
                    if(this->y > 1){
                         this->y = val_y;
                    }
                    else { this->y = 1; }
                }
                void set_window_size(int val_w, int val_h) {
                    if(val_w > max_w){ this->w = max_w; }
                    else if(val_w < min_w){
                        this->w = min_w;
                        set_window_position(this->x - (min_w - val_w), this->y);
                    }
                    else { this->w = val_w; }
                    if(val_h > max_h){ this->h = max_h; }
                    else if(val_h < min_h){ 
                        this->h = min_h;
                        set_window_position(this->x, this->y - (min_h - val_h));
                    }
                    else { this->h = val_h; }
                }
                void set_window_colors(int val_bg, int val_fg, int val_shadow_bg, int val_shadow_fg) { 
                    this->bg = val_bg; this->fg = val_fg; this->sh_bg = val_shadow_bg; this->sh_fg = val_shadow_fg;
                }
                void set_sym(char val_symbol, int val_symbol_shadow) { this->sym = val_symbol; this->sym_sh = val_symbol_shadow; }
                void set_shadow(bool val) { this->shadow = val; }
                void set_shadow_offset(int val_shadow_offset_x, int val_shadow_offset_y ) {
                    this->sh_offset_x = val_shadow_offset_x; this->sh_offset_y = val_shadow_offset_y; 
                }
                void set_window_min_size(int val_min_w, int val_min_h) { this->min_w = val_min_w; this->min_h = val_min_h; }
                void set_window_max_size(int val_max_w, int val_max_h) { this->max_w = val_max_w; this->max_h = val_max_h; }
     
                int  get_x() { return this->x; }
                int  get_y() { return this->y; }
                int  get_w() { return this->w; }
                int  get_h() { return this->h; }
                int  get_bg() { return this->bg; } 
                int  get_fg() { return this->fg; }
                char get_sym() { return this->sym; }
                bool get_shadow() { return this->shadow; }
                int  get_sh_bg() { return this->sh_bg; }
                int  get_sh_fg() { return this->sh_fg; }
                int  get_sh_offset_x() { return this->sh_offset_x; }
                int  get_sh_offset_y() { return this->sh_offset_y; }
                char get_sym_sh()  { return this->sym_sh; }
                int  get_max_width() { return max_w; }
                int  get_max_height() { return max_h; }
                int  get_min_width() { return min_w; }
                int  get_min_height() { return min_h; }
     
                void draw_window(){
                    if(shadow){
                        dl::draw_rectangle(x + sh_offset_x, y + sh_offset_y, w + x + sh_offset_x, h + y + sh_offset_y, sh_fg, sh_bg, sym_sh);
                    }
                    dl::draw_rectangle(x, y, w + x, h + y, fg, bg, sym);
                }
     
                void clear_window(){
                    if(shadow){
                        dl::draw_rectangle(x, y, w + h + sh_offset_x, h + y + sh_offset_y, 39, 49, ' ');
                    }
                    else{ 
                        dl::draw_rectangle(x, y, w + x, h + y, 39, 49, ' '); 
                    }
                }
        };
        class form{
            private:
                int x, y, w, bg, fg;
                std::string text;
                char sym;
            public: 
                form(int v_x, int v_y, int v_w, int v_bg, int v_fg)
                    : x { v_x }, y { v_y }, w { v_w }, bg { v_bg }, fg { v_fg }, text { "" }, sym { ' ' }
                {}
                void set_form_position(int val_x, int val_y) { this->x = val_x; this->y = val_y; }
                void set_width (int val) { this->w = val; }
                void set_color (int val_bg, int val_fg) { this->bg = val_bg; this->fg = val_fg; }
                void set_text (std::string val) { this->text = val; }
                void set_sym (char val) { this->sym  = val; }
                void pop_back_string () { if(text.length() != 0) this->text.pop_back(); }
     
                int get_x () { return this->x; }
                int get_w () { return this->w; }
                int get_y () { return this->y; }
                int get_bg () { return this->bg; }
                int get_fg () { return this->fg; }
                std::string get_text () { return this->text; }
                char get_sym  () { return this->sym; }
     
                void draw_form(){
                    std::string tmp_text { text };
                    dl::draw_rectangle(x, y, x + w, y, fg, bg, sym);
                    if(static_cast<int>(this->text.length()) > this->w){
                        tmp_text.resize(static_cast<unsigned long>(this->w - 2));
                        tmp_text += "...";
                    }
                    dl::printxy(x, y, tmp_text, bg, fg);
                }
        };
        class buttons_array {
            private:
                int array_size_w, array_size_h, but_arr_pos_x, but_arr_pos_y, active_button_x, active_button_y, bg, fg, active_bg, active_fg, space_w, space_h;
            public: 
                buttons_array(int v_but_arr_pos_x, int v_but_arr_pos_y, int v_bg, int v_fg, int v_active_bg, int v_active_fg, int v_array_size_w, int v_array_size_h)
                    : array_size_w { v_array_size_w }, array_size_h { v_array_size_h }, but_arr_pos_x { v_but_arr_pos_x }, but_arr_pos_y { v_but_arr_pos_y },
                    active_button_x { 0 }, active_button_y { 0 }, bg { v_bg }, fg { v_fg }, active_bg { v_active_bg }, active_fg { v_active_fg },
                    space_w { 1 }, space_h { 2 }
                {}
                void set_buttons_pos (int v_x, int v_y) { this->but_arr_pos_x = v_x; this->but_arr_pos_y = v_y; }
                void set_active_button (int x, int y) { this->active_button_x = x; this->active_button_y = y; }
                void set_colors (int v_bg, int v_fg) { this->bg = v_bg; this->fg = v_fg; }
                void set_active_colors (int v_bg, int v_fg) { this->active_bg = v_bg; this->active_fg = v_fg; }
                void set_space (int v_width, int v_height) { this->space_w = v_width; this->space_h = v_height - 1; }
                void set_array_size (int v_width, int v_height) { this->array_size_w = v_width; this->array_size_h = v_height; }
     
                int get_but_array_pos_x () { return this->but_arr_pos_x; }
                int get_but_array_pos_y () { return this->but_arr_pos_y; }
                int get_active_button_x () { return this->active_button_x; }
                int get_active_button_y () { return this->active_button_y; }
                int get_bg () { return this->bg; }
                int get_fg () { return this->fg; }
                int get_active_bg () { return this->active_bg; }
                int get_active_fg () { return this->active_fg; }
                int get_space_w () { return this->space_w; }
                int get_space_h () { return this->space_h - 1; }
                int get_array_size_w () { return this->array_size_w; }
                int get_array_size_h () { return this->array_size_h; }
               
                void select_by_arrows(lnc::ui::ARROW arrow) {
                    switch(arrow){
                        case lnc::ui::ARROW::RIGHT:
                            ++active_button_x;
                            break;
                        case lnc::ui::ARROW::LEFT:
                            --active_button_x;
                            break;
                        case lnc::ui::ARROW::UP:
                            --active_button_y;
                            break;
                        case lnc::ui::ARROW::DOWN:
                            ++active_button_y;
                            break;
                    }
                    if(active_button_x >= array_size_w){
                        --active_button_x;
                    }
                    if(active_button_y >= array_size_h){
                        --active_button_y;
                    }
                    if(active_button_x < 0){
                        ++active_button_x;
                    }
                    if(active_button_y < 0){
                        ++active_button_y;
                    }
                }
     
                template<std::size_t buttons_w, std::size_t buttons_h>
                void draw_buttons(std::string (&array)[ buttons_h ][ buttons_w ]){
                    int tmp_x { 0 };
                    int tmp_y { 0 };
                    for(int count_1 { 0 }; count_1 < static_cast<int>(buttons_h); ++count_1){
                        for(int count_2 { 0 }; count_2 < static_cast<int>(buttons_w); ++count_2){
                            dl::printxy(but_arr_pos_x + tmp_x, but_arr_pos_y + tmp_y, array[ count_1 ][ count_2 ], bg, fg);
                            if(count_1 == active_button_y && count_2 == active_button_x){
                                dl::printxy(but_arr_pos_x + tmp_x, but_arr_pos_y + tmp_y, array[ count_1 ][ count_2 ], active_bg, active_fg);
                            }
                            else{
                                dl::printxy(but_arr_pos_x + tmp_x, but_arr_pos_y + tmp_y, array[ count_1 ][ count_2 ], bg, fg);
                            }
     
                            tmp_x += static_cast<int>(array[ count_1 ][ count_2 ].length()) + space_w;
                        }
                        tmp_x = 0;
                        tmp_y += space_h;
                    }
                }
        };
        class picture{
            private:
                int pos_x, pos_y, width, height;
                std::string pic;
            public:
                picture(int v_pos_x, int v_pos_y, int v_width, int v_height, std::string picname)
                    : pos_x { v_pos_x }, pos_y { v_pos_y }, width { v_width }, height { v_height }
                {
                    dl::load_picture(picname, pic);
                }
                void set_pos_x(int v_pos_x){ this->pos_x = v_pos_x; }
                void set_pos_y(int v_pos_y){ this->pos_y = v_pos_y; }
                void set_width(int v_width){ this->width = v_width; }
                void set_height(int v_height){ this->height = v_height; }
                void set_picture(std::string picname){ dl::load_picture(picname, pic); }
     
                int get_pos_x() { return pos_x; }
                int get_pos_y() { return pos_y; }
                int get_width() { return width; }
                int get_height() { return height; }
                std::string get_picture() { return pic; }
                void draw_picture(){
                    dl::draw_picture(this->pos_x, this->pos_y, this->pic);
                }
        };
        void begin_prog(){
            lnc::ui::no_echo();
            dl::cursor_mode(dl::CURSOR::HIDE);
            dl::clear();
        }
        void close_prog(){
            dl::cursor_mode(dl::CURSOR::SHOW);
            lnc::ui::echo();
            dl::clear();
        }
    }     
}

#endif /* LNC_UI_H */
