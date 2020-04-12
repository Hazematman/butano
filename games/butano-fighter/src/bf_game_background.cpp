#include "bf_game_background.h"

#include "btn_blending.h"
#include "btn_rect_window.h"
#include "btn_regular_bg_builder.h"
#include "btn_clouds_bg_item.h"
#include "btn_stage_1_bg_item.h"

namespace bf::game
{

namespace
{
    constexpr const btn::fixed blending_transparency = 0.4;
    constexpr const int hblank_effect_multiplier = 32;

    btn::regular_bg_ptr _create_ground_bg()
    {
        btn::regular_bg_builder builder(btn::bg_items::stage_1);
        builder.set_mosaic_enabled(true);
        return builder.release_build();
    }

    btn::regular_bg_ptr _create_clouds_bg()
    {
        btn::regular_bg_builder builder(btn::bg_items::clouds);
        builder.set_priority(2);
        builder.set_blending_enabled(true);
        builder.set_mosaic_enabled(true);
        return builder.release_build();
    }
}

background::background() :
    _ground_move_action(_create_ground_bg(), 0, 0.5),
    _clouds_move_action(_create_clouds_bg(), -1.0 / 16, 0.5),
    _hblank_effect(btn::regular_bg_position_hblank_effect_ptr::create_horizontal(
                       _ground_move_action.bg(), _hblank_effect_deltas))
{
    btn::blending::set_transparency_alpha(blending_transparency);
    btn::window::internal().set_show_bg(_clouds_move_action.bg(), false);
    _wave_generator.set_amplitude(3);
    _hblank_effect.set_visible(false);
}

void background::show_bomb_open(int frames)
{
    btn::bg_palette_ptr ground_palette = _ground_move_action.bg().palette();
    ground_palette.set_grayscale_intensity(0);
    _ground_palette_grayscale_action.emplace(btn::move(ground_palette), frames / 2, 1);

    btn::bgs_mosaic::set_stretch(0.1);
    _mosaic_action = btn::bgs_mosaic_stretch_loop_action(4, 0.2);
}

void background::show_bomb_close(int frames)
{
    BTN_ASSERT(frames > 0, "Invalid frames: ", frames);

    btn::bgs_mosaic::set_stretch(0);
    _mosaic_action.reset();

    btn::bg_palette_ptr ground_palette = _ground_move_action.bg().palette();
    ground_palette.set_grayscale_intensity(1);
    _ground_palette_grayscale_action.emplace(btn::move(ground_palette), frames, 0);

    _hblank_effect.set_visible(true);
    _bomb_fade_frames = frames;
}

void background::show_clouds()
{
    btn::blending::set_transparency_alpha(0);
    _blending_action.emplace(20, blending_transparency);
}

void background::show_hero_dying()
{
    _ground_palette_inverted_action.emplace(_ground_move_action.bg().palette(), 5);
    _clouds_palette_inverted_action.emplace(_clouds_move_action.bg().palette(), 5);
    btn::green_swap::set_enabled(true);
    _green_swap_action.emplace(5);
}

void background::show_hero_dead()
{
    _ground_palette_inverted_action.reset();
    _clouds_palette_inverted_action.reset();
    _green_swap_action.reset();
}

void background::update()
{
    _ground_move_action.update();
    _clouds_move_action.update();

    if(_mosaic_action)
    {
        _mosaic_action->update();
    }

    if(_blending_action)
    {
        _blending_action->update();

        if(_blending_action->done())
        {
            _blending_action.reset();
        }
    }

    if(_ground_palette_grayscale_action)
    {
        _ground_palette_grayscale_action->update();

        if(_ground_palette_grayscale_action->done())
        {
            _ground_palette_grayscale_action.reset();
        }
    }

    if(_ground_palette_inverted_action)
    {
        _ground_palette_inverted_action->update();
        _clouds_palette_inverted_action->update();
        _green_swap_action->update();
    }

    if(_bomb_fade_frames)
    {
        --_bomb_fade_frames;

        if(_bomb_fade_frames)
        {
            _wave_generator.set_speed(_bomb_fade_frames * hblank_effect_multiplier);
            _wave_generator.generate(_hblank_effect_deltas);
            _hblank_effect.reload_deltas_ref();
        }
        else
        {
            _hblank_effect.set_visible(false);
        }
    }
}

}