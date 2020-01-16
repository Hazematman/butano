#include "btn_sprite_ptr.h"

#include "btn_size.h"
#include "btn_sprite_builder.h"
#include "btn_sprites_manager.h"
#include "btn_sprite_affine_mat_builder.h"

namespace btn
{

sprite_ptr sprite_ptr::create(fixed x, fixed y, const sprite_item& item, int graphics_index)
{
    return create(fixed_point(x, y), item, graphics_index);
}

sprite_ptr sprite_ptr::create(const fixed_point& position, const sprite_item& item, int graphics_index)
{
    sprite_builder builder(item, graphics_index);
    builder.set_position(position);
    return sprite_ptr(sprites_manager::create(move(builder)));
}

sprite_ptr sprite_ptr::create(const sprite_builder& builder)
{
    return sprite_ptr(sprites_manager::create(sprite_builder(builder)));
}

sprite_ptr sprite_ptr::create(sprite_builder&& builder)
{
    return sprite_ptr(sprites_manager::create(move(builder)));
}

sprite_ptr::sprite_ptr(const sprite_ptr& other) :
    sprite_ptr(other._id)
{
    sprites_manager::increase_usages(_id);
}

sprite_ptr& sprite_ptr::operator=(const sprite_ptr& other)
{
    if(_id != other._id)
    {
        _destroy();
        _id = other._id;
        sprites_manager::increase_usages(_id);
    }

    return *this;
}

sprite_ptr::sprite_ptr(sprite_ptr&& other) :
    sprite_ptr(other._id)
{
    other._id = nullptr;
}

sprite_ptr& sprite_ptr::operator=(sprite_ptr&& other)
{
    swap(_id, other._id);
    return *this;
}

size sprite_ptr::dimensions() const
{
    return sprites_manager::dimensions(_id);
}

const sprite_tiles_ptr& sprite_ptr::tiles() const
{
    return sprites_manager::tiles_ptr(_id);
}

void sprite_ptr::set_tiles(sprite_tiles_ptr tiles_ptr)
{
    sprites_manager::set_tiles_ptr(_id, move(tiles_ptr));
}

void sprite_ptr::set_tiles(const sprite_item& item, int graphics_index, create_mode create_mode)
{
    set_tiles(item.tiles_item(), graphics_index, create_mode);
}

void sprite_ptr::set_tiles(const sprite_tiles_item& tiles_item, int graphics_index, create_mode create_mode)
{
    set_tiles(tiles_item.tiles_ptr(graphics_index, create_mode));
}

const sprite_palette_ptr& sprite_ptr::palette() const
{
    return sprites_manager::palette_ptr(_id);
}

void sprite_ptr::set_palette(sprite_palette_ptr palette_ptr)
{
    sprites_manager::set_palette_ptr(_id, move(palette_ptr));
}

void sprite_ptr::set_palette(const sprite_item& item, create_mode create_mode)
{
    set_palette(item.palette_item(), create_mode);
}

void sprite_ptr::set_palette(const sprite_palette_item& palette_item, create_mode create_mode)
{
    set_palette(palette_item.palette_ptr(create_mode));
}

void sprite_ptr::set_tiles_and_palette(const sprite_item& item, int graphics_index, create_mode create_mode)
{
    set_tiles(item, graphics_index, create_mode);
    set_palette(item, create_mode);
}

fixed sprite_ptr::x() const
{
    return position().x();
}

fixed sprite_ptr::y() const
{
    return position().y();
}

const fixed_point& sprite_ptr::position() const
{
    return sprites_manager::position(_id);
}

void sprite_ptr::set_x(fixed x)
{
    set_position(fixed_point(x, position().y()));
}

void sprite_ptr::set_y(fixed y)
{
    set_position(fixed_point(position().x(), y));
}

void sprite_ptr::set_position(fixed x, fixed y)
{
    set_position(fixed_point(x, y));
}

void sprite_ptr::set_position(const fixed_point& position)
{
    sprites_manager::set_position(_id, position);
}

fixed sprite_ptr::rotation_angle() const
{
    if(const optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat_ptr(_id))
    {
        return affine_mat->rotation_angle();
    }

    return 0;
}

void sprite_ptr::set_rotation_angle(fixed rotation_angle)
{
    if(optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat_ptr(_id))
    {
        affine_mat->set_rotation_angle(rotation_angle);
    }
    else if(rotation_angle != 0)
    {
        sprite_affine_mat_builder affine_mat_builder;
        affine_mat_builder.set_rotation_angle(rotation_angle);
        affine_mat_builder.set_horizontal_flip(horizontal_flip());
        affine_mat_builder.set_vertical_flip(vertical_flip());
        set_remove_affine_mat_when_not_needed(true);
        set_affine_mat(affine_mat_builder.build());
    }
}

fixed sprite_ptr::scale_x() const
{
    if(const optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat_ptr(_id))
    {
        return affine_mat->scale_x();
    }

    return 0;
}

void sprite_ptr::set_scale_x(fixed scale_x)
{
    if(optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat_ptr(_id))
    {
        affine_mat->set_scale_x(scale_x);
    }
    else if(scale_x != 1)
    {
        sprite_affine_mat_builder affine_mat_builder;
        affine_mat_builder.set_scale_x(scale_x);
        affine_mat_builder.set_horizontal_flip(horizontal_flip());
        affine_mat_builder.set_vertical_flip(vertical_flip());
        set_remove_affine_mat_when_not_needed(true);
        set_affine_mat(affine_mat_builder.build());
    }
}

fixed sprite_ptr::scale_y() const
{
    if(const optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat_ptr(_id))
    {
        return affine_mat->scale_y();
    }

    return 0;
}

void sprite_ptr::set_scale_y(fixed scale_y)
{
    if(optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat_ptr(_id))
    {
        affine_mat->set_scale_y(scale_y);
    }
    else if(scale_y != 1)
    {
        sprite_affine_mat_builder affine_mat_builder;
        affine_mat_builder.set_scale_y(scale_y);
        affine_mat_builder.set_horizontal_flip(horizontal_flip());
        affine_mat_builder.set_vertical_flip(vertical_flip());
        set_remove_affine_mat_when_not_needed(true);
        set_affine_mat(affine_mat_builder.build());
    }
}

int sprite_ptr::bg_priority() const
{
    return sprites_manager::bg_priority(_id);
}

void sprite_ptr::set_bg_priority(int bg_priority)
{
    sprites_manager::set_bg_priority(_id, bg_priority);
}

int sprite_ptr::z_order() const
{
    return sprites_manager::z_order(_id);
}

void sprite_ptr::set_z_order(int z_order)
{
    sprites_manager::set_z_order(_id, z_order);
}

bool sprite_ptr::horizontal_flip() const
{
    return sprites_manager::horizontal_flip(_id);
}

void sprite_ptr::set_horizontal_flip(bool horizontal_flip)
{
    sprites_manager::set_horizontal_flip(_id, horizontal_flip);
}

bool sprite_ptr::vertical_flip() const
{
    return sprites_manager::vertical_flip(_id);
}

void sprite_ptr::set_vertical_flip(bool vertical_flip)
{
    sprites_manager::set_vertical_flip(_id, vertical_flip);
}

bool sprite_ptr::mosaic_enabled() const
{
    return sprites_manager::mosaic_enabled(_id);
}

void sprite_ptr::set_mosaic_enabled(bool mosaic_enabled)
{
    sprites_manager::set_mosaic_enabled(_id, mosaic_enabled);
}

bool sprite_ptr::double_size() const
{
    return sprites_manager::double_size(_id);
}

void sprite_ptr::set_double_size(bool double_size)
{
    sprites_manager::set_double_size(_id, double_size);
}

bool sprite_ptr::visible() const
{
    return sprites_manager::visible(_id);
}

void sprite_ptr::set_visible(bool visible)
{
    sprites_manager::set_visible(_id, visible);
}

bool sprite_ptr::ignore_camera() const
{
    return sprites_manager::ignore_camera(_id);
}

void sprite_ptr::set_ignore_camera(bool ignore_camera)
{
    sprites_manager::set_ignore_camera(_id, ignore_camera);
}

const optional<sprite_affine_mat_ptr>& sprite_ptr::affine_mat() const
{
    return sprites_manager::affine_mat_ptr(_id);
}

void sprite_ptr::set_affine_mat(optional<sprite_affine_mat_ptr> affine_mat_ptr)
{
    sprites_manager::set_affine_mat_ptr(_id, move(affine_mat_ptr));
}

bool sprite_ptr::remove_affine_mat_when_not_needed() const
{
    return sprites_manager::remove_affine_mat_when_not_needed(_id);
}

void sprite_ptr::set_remove_affine_mat_when_not_needed(bool remove_when_not_needed)
{
    sprites_manager::set_remove_affine_mat_when_not_needed(_id, remove_when_not_needed);
}

void sprite_ptr::_destroy()
{
    if(_id)
    {
        sprites_manager::decrease_usages(_id);
        _id = nullptr;
    }
}

}
