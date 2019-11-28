#include "ui_elements.h"
#include "math.h"

/* COLOR CHANGES IN THE UI */
// TODO: Define a nice kind of greyish tint for these
#define COLOR_WHEN_HOVERED 0xb5b6
#define COLOR_INACTIVE 0x6b4d

/* BUTTON */

struct Button {
    Rect_t rect;
    Sprite_t* sprite;
    bool shown, is_active, hovered;
    void (*func)();
};

// Empty string to not draw a sprite
Button_t* new_button(const char* sprite_file_name, void (*func)(), Rect_t rect) {
    
    Button_t* button = (Button_t*) malloc(sizeof(Button_t));
    if (button == NULL) {
        printf("new_button: Failed to allocate memory for the Button\n");
        return NULL;
    }

    button->rect = rect;

    if (strcmp("", sprite_file_name) == 0)
        button->sprite = NULL;
    else {
        button->sprite = new_sprite(0, 0, 1, sprite_file_name);
        if (button->sprite == NULL) {
            printf("new_button: Failed to create Button's Sprite\n");
            free(button);
            return NULL;
        }
    }

    button->is_active = true;
    button->shown = true;
    button->hovered = false;

    if (func == NULL) {
        printf("new_button: Function pointer invalid\n");
        free_sprite(button->sprite);
        free(button);
        return NULL;
    }
    button->func = func;

    return button;
}

Button_t* new_button_auto_size(const char* sprite_file_name, void (*func)(), Vec2d_t pos) {
    
    Button_t* button = (Button_t*) malloc(sizeof(Button_t));
    if (button == NULL) {
        printf("new_button: Failed to allocate memory for the Button\n");
        return NULL;
    }

    button->sprite = new_sprite(0, 0, 1, sprite_file_name);
    if (button->sprite == NULL) {
        printf("new_button: Failed to create Button's Sprite\n");
        free(button);
        return NULL;
    }

    button->rect = rect(pos.x, pos.y, sprite_get_width(button->sprite), sprite_get_height(button->sprite));

    button->is_active = true;
    button->shown = true;
    button->hovered = false;

    if (func == NULL) {
        printf("new_button: Function pointer invalid\n");
        free_sprite(button->sprite);
        free(button);
        return NULL;
    }
    button->func = func;

    return button;
}

// Empty string to not draw a sprite
Button_t* new_button_existing_sprite(Sprite_t *sprite, void (*func)(), Rect_t rect) {
    
    Button_t* button = (Button_t*) malloc(sizeof(Button_t));
    if (button == NULL) {
        printf("new_button_existing_sprite: Failed to allocate memory for the Button\n");
        return NULL;
    }

    button->rect = rect;

    if (sprite == NULL) {
        printf("new_button_existing_sprite: Sprite cannot be a NULL pointer\n");
        free(button);
        return NULL;
    }
    button->sprite = sprite;

    button->is_active = true;
    button->shown = true;
    button->hovered = false;

    if (func == NULL) {
        printf("new_button_existing_sprite: Function pointer invalid\n");
        free(button);
        return NULL;
    }
    button->func = func;

    return button;
}

void free_button(Button_t* button) {

    if (button == NULL) {
        printf("free_button_and_sprite: Cannot free a NULL pointer\n");
        return;
    }

    if (button->sprite != NULL)
        free_sprite(button->sprite);

    free(button);
}

void free_button_without_sprite(Button_t* button) {
    if (button == NULL) {
        printf("free_button: Cannot free a NULL pointer\n");
        return;
    }

    free(button);
}


inline void button_hide(Button_t* button) {
    button->shown = false;
}

inline void button_show(Button_t* button) {
    button->shown = true;
}

inline void button_activate(Button_t* button) {
    button->is_active = true;
}

inline void button_deactivate (Button_t* button) {
    button->is_active = false;
}

void update_button(Button_t* button, MouseCursor_t* cursor) {
    if (button->is_active && button->shown) {
        if (is_cursor_inside_rect(cursor, &button->rect)) {
            button->hovered = true;
            if (cursor_left_button_down(cursor))
                button->func();
        }
        else
            button->hovered = false;
    }
}

void render_button(Button_t* button) {
    if (button->shown && button->sprite != NULL) {
        if (!button->is_active)
            draw_sprite(button->sprite, &button->rect, COLOR_INACTIVE, false);
        else if (button->hovered)
            draw_sprite(button->sprite, &button->rect, COLOR_WHEN_HOVERED, false);
        else
            draw_sprite(button->sprite, &button->rect, COLOR_NO_MULTIPLY, false);
    }
}

/* SLIDER */

struct Slider {
    Vec2d_t bar_pos;
    Rect_t handle_rect;
    Sprite_t *bar_sprite, *handle_sprite;
    bool shown, is_active, hovered, being_moved;
    // state is in [0, max_state[
    uint8_t state, max_state; // Also represents the icon's position in the bar
    Vec2d_t start_pos, end_pos, cursor_offset;
    void (*func)(uint8_t);
};


Slider_t* new_slider(const char* bar_sprite_file_name, const char* handle_sprite_file_name, void (*func)(uint8_t), Vec2d_t bar_pos, uint8_t max_state, Vec2d_t start_pos, Vec2d_t end_pos) {
    
    Slider_t* slider = (Slider_t*) malloc(sizeof(Slider_t));
    if (slider == NULL) {
        printf("new_slider: Failed to allocate memory for the Slider\n");
        return NULL;
    }

    slider->bar_pos = bar_pos;

    slider->bar_sprite = new_sprite(0, 0, 1, bar_sprite_file_name);
    if (slider->bar_sprite == NULL) {
        printf("new_slider: Failed to create Slider's bar Sprite\n");
        free(slider);
        return NULL;
    }

    slider->handle_sprite = new_sprite(0, 0, 1, handle_sprite_file_name);
    if (slider->handle_sprite == NULL) {
        printf("new_slider: Failed to create Slider's handle Sprite\n");
        free_sprite(slider->bar_sprite);
        free(slider);
        return NULL;
    }

    slider->is_active = true;
    slider->shown = true;
    slider->hovered = true;
    slider->being_moved = false;

    slider->state = 0;
    slider->max_state = max_state;

    slider->start_pos = start_pos;
    slider->end_pos = end_pos;
    slider->cursor_offset = vec2d(0.0f, 0.0f);

    slider->handle_rect = rect(
        start_pos.x,
        start_pos.y,
        sprite_get_width(slider->handle_sprite),
        sprite_get_height(slider->handle_sprite)
    );

    if (func == NULL) {
        printf("new_slider: Function pointer invalid\n");
        free_sprite(slider->bar_sprite);
        free_sprite(slider->handle_sprite);
        free(slider);
        return NULL;
    }
    slider->func = func;

    return slider;
}


void free_slider(Slider_t* slider) {

    if (slider == NULL) {
        printf("free_slider_and_sprite: Cannot free a NULL pointer\n");
        return;
    }

    free_sprite(slider->bar_sprite);
    free_sprite(slider->handle_sprite);

    free(slider);
}

inline void slider_hide(Slider_t* slider) {
    slider->shown = false;
}

inline void slider_show(Slider_t* slider) {
    slider->shown = true;
}

inline void slider_activate(Slider_t* slider) {
    slider->is_active = true;
}

inline void slider_deactivate (Slider_t* slider) {
    slider->is_active = false;
}

void update_slider(Slider_t* slider, MouseCursor_t* cursor) {
    if (slider->is_active && slider->shown) {
        if (slider->being_moved) {

            // Update position
            slider->handle_rect.x = fclamp(
                cursor_get_x(cursor) + slider->cursor_offset.x, slider->start_pos.x, slider->end_pos.x);
            
            slider->handle_rect.y = fclamp(
                cursor_get_y(cursor) + slider->cursor_offset.y,
                slider->start_pos.y, slider->end_pos.y);

            if (!cursor_left_button(cursor)) {

                // Get the most appropriate state
                Vec2d_t p = rect_get_origin(&slider->handle_rect);
                float endpoint_distace = point_distance(slider->start_pos, slider->end_pos);
                float distance_to_start = point_distance(slider->start_pos, p);

                // Calculate closest possible state
                slider->state = (uint8_t) ffloor(distance_to_start / endpoint_distace * slider->max_state);

                slider->being_moved = false;
                slider->func(slider->state);
            }
        }
        else if (is_cursor_inside_rect(cursor, &slider->handle_rect)) {
            slider->hovered = true;
            if (cursor_left_button_down(cursor)) {
                slider->cursor_offset = subtract_vec2d(
                    vec2d(  cursor_get_x(cursor),
                            cursor_get_y(cursor)),
                    rect_get_origin(&slider->handle_rect));

                slider->being_moved = true;
            }
        }
        else
            slider->hovered = false;
    }
}

void render_slider(Slider_t* slider) {


    if (slider->shown) {
        if (!slider->is_active) {
            // Draw default bar
            draw_sprite_floats(slider->bar_sprite, slider->bar_pos.x, slider->bar_pos.y, COLOR_INACTIVE, false);
            // Draw handle
            draw_sprite(slider->handle_sprite, &slider->handle_rect, COLOR_INACTIVE, false);
        }
        else if (slider->hovered) {
            // Draw default bar
            draw_sprite_floats(slider->bar_sprite, slider->bar_pos.x, slider->bar_pos.y, COLOR_NO_MULTIPLY, false);
            // Draw handle
            draw_sprite(slider->handle_sprite, &slider->handle_rect, COLOR_WHEN_HOVERED, false);
        }
        else {
            // Draw default bar
            draw_sprite_floats(slider->bar_sprite, slider->bar_pos.x, slider->bar_pos.y, COLOR_NO_MULTIPLY, false);
            // Draw handle
            draw_sprite(slider->handle_sprite, &slider->handle_rect, COLOR_NO_MULTIPLY, false);
        }
    }
}







/* KNOB */

typedef struct Knob {
    Rect_t rect;
    Sprite_t *backdrop, *pointer;
    bool shown, is_active;
    void (*func)(uint8_t);
} Knob_t;

Knob_t* new_knob() {
    return NULL;
}

void free_knob(Knob_t* knob) {

}

/* SWITCHBOARD */

struct SwitchBoard {
    Button_t *laser_buttons[3];
    Slider_t *speed_slider, *jump_slider;
    Knob_t *gravity_knob;
};
