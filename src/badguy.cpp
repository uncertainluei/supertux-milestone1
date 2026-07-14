//  $Id$
// 
//  SuperTux
//  Copyright (C) 2000 Bill Kendrick <bill@newbreedsoftware.com>
//  Copyright (C) 2004 Tobias Glaesser <tobi.web@gmx.de>
//  Copyright (C) 2004 Matthias Braun <matze@braunis.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include "collision.h"
#include "globals.h"
#include "defines.h"
#include "badguy.h"
#include "level.h"
#include "scene.h"
#include "screen.h"
#include "sound.h"
#include "world.h"
#include "tile.h"
#include "resources.h"
#include "sprite_manager.h"

Sprite* img_mriceblock_flat_left;
Sprite* img_mriceblock_flat_right;
Sprite* img_mriceblock_left;
Sprite* img_mriceblock_right;
Sprite* img_smartblock_flat_left;
Sprite* img_smartblock_flat_right;
Sprite* img_smartblock_left;
Sprite* img_smartblock_right;

Sprite* img_jumpy_left_up;
Sprite* img_jumpy_left_down;
Sprite* img_jumpy_left_middle;
Sprite* img_jumpy_right_up;
Sprite* img_jumpy_right_down;
Sprite* img_jumpy_right_middle;

Sprite* img_mrbomb_left;
Sprite* img_mrbomb_right;
Sprite* img_mrbomb_ticking_left;
Sprite* img_mrbomb_ticking_right;
Sprite* img_mrbomb_explosion;

Sprite* img_stalactite;
Sprite* img_stalactite_broken;

Sprite* img_flame;

Sprite* img_fish;
Sprite* img_fish_down;

Sprite* img_lavaball;
Sprite* img_lavaball_down;

Sprite* img_bouncingsnowball_left;
Sprite* img_bouncingsnowball_right;
Sprite* img_bouncingsnowball_squished_left;
Sprite* img_bouncingsnowball_squished_right;
Sprite* img_flyingsnowball_left;
Sprite* img_flyingsnowball_right;
Sprite* img_flyingsnowball_squished_left;
Sprite* img_flyingsnowball_squished_right;
Sprite* img_spiky_left;
Sprite* img_spiky_right;
Sprite* img_snowball_left;
Sprite* img_snowball_right;
Sprite* img_snowball_squished_left;
Sprite* img_snowball_squished_right;
Sprite* img_smartball_left;
Sprite* img_smartball_right;
Sprite* img_smartball_squished_left;
Sprite* img_smartball_squished_right;

#define BADGUY_WALK_SPEED .8f

BadGuyKind  badguykind_from_string(const std::string& str)
{
  if (str == "money" || str == "jumpy") // was money in old maps
    return BAD_JUMPY;
  if (str == "laptop" || str == "mriceblock") // was laptop in old maps
    return BAD_MRICEBLOCK;
  if (str == "mrbomb")
    return BAD_MRBOMB;
  if (str == "stalactite")
    return BAD_STALACTITE;
  if (str == "flame")
    return BAD_FLAME;
  if (str == "fish")
    return BAD_FISH;
  if (str == "lavaball")
    return BAD_LAVABALL;
  if (str == "bouncingsnowball")
    return BAD_BOUNCINGSNOWBALL;
  if (str == "flyingsnowball")
    return BAD_FLYINGSNOWBALL;
  if (str == "spiky")
    return BAD_SPIKY;
  if (str == "snowball" || str == "bsod") // was bsod in old maps
    return BAD_SNOWBALL;
  
  printf("Couldn't convert badguy: '%s'\n", str.c_str());
  return BAD_SNOWBALL;
}

std::string badguykind_to_string(BadGuyKind kind)
{
  switch(kind)
    {
    case BAD_JUMPY:
      return "jumpy";
    case BAD_MRICEBLOCK:
      return "mriceblock";
    case BAD_MRBOMB:
      return "mrbomb";
    case BAD_STALACTITE:
      return "stalactite";
    case BAD_FLAME:
      return "flame";
    case BAD_FISH:
      return "fish";
    case BAD_LAVABALL:
      return "lavaball";
    case BAD_BOUNCINGSNOWBALL:
      return "bouncingsnowball";
    case BAD_FLYINGSNOWBALL:
      return "flyingsnowball";
    case BAD_SPIKY:
      return "spiky";
    case BAD_SNOWBALL:
      return "snowball";
    default:
      return "snowball";
    }
}

BadGuy::BadGuy(float x, float y, BadGuyKind kind_, bool stay_on_platform_)
  : removable(false), squishcount(0)
{
  base.x   = x;
  base.y   = y;    
  base.width  = 0;
  base.height = 0;
  base.xm  = 0;
  base.ym  = 0;

  stay_on_platform = stay_on_platform_;
  mode     = NORMAL;
  dying    = DYING_NOT;
  kind     = kind_;
  old_base = base;
  dir      = LEFT;
  seen     = false;
  disable_collision = false;

  animation_offset = 0;
  sprite_left = sprite_right = 0;
  physic.reset();
  timer.init(true);

  // default properties
  burnable = true;
  bumpable = true;
  invulnerable = false;
  turn_when_bumped = true;
  check_tiles = true;

  switch (kind) {
    case BAD_SNOWBALL:
      physic.set_velocity(-BADGUY_WALK_SPEED, 0);
      if (stay_on_platform)
        set_sprite(img_smartball_left, img_smartball_right);
      else
        set_sprite(img_snowball_left, img_snowball_right);
      break;
    case BAD_MRICEBLOCK:
      physic.set_velocity(-BADGUY_WALK_SPEED, 0);
      set_sprite_mriceblock(false);
      break;
    case BAD_MRBOMB:
      physic.set_velocity(-BADGUY_WALK_SPEED, 0);
      set_sprite(img_mrbomb_left, img_mrbomb_right);
      break;
    case BAD_BOMB:
      disable_collision = true;
      set_sprite(img_mrbomb_ticking_left, img_mrbomb_ticking_right);
      break;
    case BAD_JUMPY:
      turn_when_bumped = false;
      set_sprite(img_jumpy_left_up, img_jumpy_right_up);
      break;
    case BAD_BOUNCINGSNOWBALL:
      physic.set_velocity(-1.3, 0);
      set_sprite(img_bouncingsnowball_left, img_bouncingsnowball_right);
      break;
    case BAD_STALACTITE:
      bumpable = false;
      invulnerable = true;
      turn_when_bumped = false;
      check_tiles = false;
      physic.enable_gravity(false);
      set_sprite(img_stalactite, img_stalactite);
      break;
    case BAD_FLYINGSNOWBALL:
      bumpable = false;
      physic.enable_gravity(false);
      set_sprite(img_flyingsnowball_left, img_flyingsnowball_right);
      break;
    case BAD_SPIKY:
      physic.set_velocity(-BADGUY_WALK_SPEED, 0);
      set_sprite(img_spiky_left, img_spiky_right);
      break;
    case BAD_FLAME:
      bumpable = false;
      burnable = false;
      invulnerable = true;
      turn_when_bumped = false;
      check_tiles = false;
      base.ym = 0; // we misuse base.ym as angle for the flame
      physic.enable_gravity(false);
      set_sprite(img_flame, img_flame);
      return; // Do not correct position if it is inside a wall
    case BAD_FISH:
      bumpable = false;
      turn_when_bumped = false;
      check_tiles = false; // Let it jump into lava ig
      physic.enable_gravity(true);
      set_sprite(img_fish, img_fish);
      return; // Do not correct position if it is inside a wall
    case BAD_LAVABALL:
      burnable = false;
      bumpable = false;
      turn_when_bumped = false;
      check_tiles = false;
      base.ym = y; // Is base.ym even used by the game?
      physic.enable_gravity(true);
      set_sprite(img_lavaball_down, img_lavaball_down);
      return; // Do not correct position if it is inside a wall
  }
  // if we're in a solid tile at start correct that now
  if(collision_object_map(base)) 
    {
      std::cout << "Warning: badguy started in wall: kind: " << badguykind_to_string(kind) 
                << " pos: (" << base.x << ", " << base.y << ")" << std::endl;
      while(collision_object_map(base))
        --base.y;
    }
}

void
BadGuy::action_mriceblock(double frame_ratio)
{
  Player& tux = *World::current()->get_tux();

  if(mode != HELD)
    fall();
  
  /* Move left/right: */
  if (mode != HELD)
    {
      // move
      physic.apply(frame_ratio, base.x, base.y);
      if (dying != DYING_FALLING)
        collision_swept_object_map(&old_base,&base);
    }
  else if (mode == HELD)
    { /* FIXME: The pbad object shouldn't know about pplayer objects. */
      /* If we're holding the iceblock */
      dir = tux.dir;
      if(dir==RIGHT)
        {
          base.x = tux.base.x + 16;
          base.y = tux.base.y + tux.base.height/1.5 - base.height;
        }
      else /* facing left */
        {
          base.x = tux.base.x - 16;
          base.y = tux.base.y + tux.base.height/1.5 - base.height;
        }
      if(collision_object_map(base))
        {
          base.x = tux.base.x;
          base.y = tux.base.y + tux.base.height/1.5 - base.height;
        }

      if(tux.input.fire != DOWN) /* SHOOT! */
        {
          if(dir == LEFT)
            base.x -= 24;
          else
            base.x += 24;
          old_base = base;

          mode=KICK;
          tux.kick_timer.start(KICKING_TIME);
          set_sprite_mriceblock(true);
          physic.set_velocity_x((dir == LEFT) ? -3.5 : 3.5);
          play_sound(SND_KICK);
        }
    }

  if (!dying)
    {
      int changed = dir;
      check_horizontal_bump();
      if(mode == KICK && changed != dir)
        play_sound(SND_RICOCHET, base.x);
    }

  /* Handle mode timer: */
  if (mode == FLAT)
    {
      if(!timer.check())
        {
          mode = NORMAL;
          set_sprite_mriceblock(false);
          physic.set_velocity( (dir == LEFT) ? -BADGUY_WALK_SPEED : BADGUY_WALK_SPEED, 0);
        }
    }
}

void
BadGuy::check_horizontal_bump(bool checkcliff)
{
    float halfheight = base.height / 2;
    if (dir == LEFT && issolid( base.x, (int) base.y + halfheight))
    {
        if (kind == BAD_MRICEBLOCK && mode == KICK)
            World::current()->trybreakbrick(base.x, base.y + halfheight, false, dir);

        dir = RIGHT;
        physic.set_velocity(-physic.get_velocity_x(), physic.get_velocity_y());
        return;
    }
    if (dir == RIGHT && issolid( base.x + base.width, (int)base.y + halfheight))
    {
        if (kind == BAD_MRICEBLOCK && mode == KICK)
            World::current()->trybreakbrick(base.x + base.width, (int) base.y + halfheight, false, dir);
         
        dir = LEFT;
        physic.set_velocity(-physic.get_velocity_x(), physic.get_velocity_y());
        return;
    }

    // don't check for cliffs when we're falling
    if(!checkcliff)
        return;
    if(!issolid(base.x + base.width/2, base.y + base.height))
        return;
    
    if(dir == LEFT && !issolid(base.x, (int) base.y + base.height + halfheight))
    {
        dir = RIGHT;
        physic.set_velocity(-physic.get_velocity_x(), physic.get_velocity_y());
        return;
    }
    if(dir == RIGHT && !issolid(base.x + base.width,
                (int) base.y + base.height + halfheight))
    {
        dir = LEFT;
        physic.set_velocity(-physic.get_velocity_x(), physic.get_velocity_y());
        return;
    }
}

void
BadGuy::fall()
{
  /* Fall if we get off the ground: */
  if (dying == DYING_FALLING)
  {
    physic.enable_gravity(true);
    return;
  }
  if (!issolid(base.x+base.width/2, base.y + base.height))
  {
    // not solid below us? enable gravity
    physic.enable_gravity(true);

    if (issolid(base.x+base.width/2, base.y-1) && physic.get_velocity_y() > 0)
    {
      base.y = int((base.y - 1)/32) * 32 + base.height;
      physic.set_velocity_y(0);
    }
    return;
  }
  /* Land: */
  if (physic.get_velocity_y() < 0)
    {
      base.y = int((base.y + base.height)/32) * 32 - base.height;
      physic.set_velocity_y(0);
    }
  // no gravity anymore please
  physic.enable_gravity(false);

  if (stay_on_platform && mode == NORMAL)
  {
    if (!issolid(base.x + ((dir == LEFT) ? 0 : base.width),
                  base.y + base.height))
    {
      if (dir == LEFT)
      {
        dir = RIGHT;
        physic.set_velocity_x(fabsf(physic.get_velocity_x()));
      } 
      else
      {
        dir = LEFT;
        physic.set_velocity_x(-fabsf(physic.get_velocity_x()));
      }
    }
  }
}

void
BadGuy::remove_me()
{
  removable = true;
}

void
BadGuy::action_jumpy(double frame_ratio)
{
  const float vy = physic.get_velocity_y();

  // XXX: These tests *should* use location from ground, not velocity
  if (fabsf(vy) > 5.6f)
    set_sprite(img_jumpy_left_down, img_jumpy_right_down);
  else if (fabsf(vy) > 5.3f)
    set_sprite(img_jumpy_left_middle, img_jumpy_right_middle);
  else
    set_sprite(img_jumpy_left_up, img_jumpy_right_up);

  Player& tux = *World::current()->get_tux();

  static const float JUMPV = 6;
    
  fall();
  mode = NORMAL;
  if(issolid(base.x, base.y+32)) // jump when on ground
  {
    physic.set_velocity_y(JUMPV);
    physic.enable_gravity(true);
    mode = JUMPY_JUMP;
  }

  // set direction based on tux
  if(tux.base.x > base.x)
    dir = RIGHT;
  else
    dir = LEFT;

  // move
  physic.apply(frame_ratio, base.x, base.y);
  collision_swept_object_map(&old_base, &base);
}

void
BadGuy::action_mrbomb(double frame_ratio)
{
  check_horizontal_bump(true);
  fall();
  physic.apply(frame_ratio, base.x, base.y);
  collision_swept_object_map(&old_base,&base); 
}

void
BadGuy::action_bomb(double frame_ratio)
{
  static const int TICKINGTIME = 1000;
  
  if (mode == NORMAL)
  {
    mode = BOMB_TICKING;
    timer.start(TICKINGTIME);
  }
  if (mode == BOMB_EXPLODE) // If exploding, then do not fall!
  {
    if (!timer.check())
      remove_me();
    return;
  }
  if (mode == BOMB_TICKING && !timer.check())
    explode_bomb();

  fall();


  // move
  physic.apply(frame_ratio, base.x, base.y);                 
  collision_swept_object_map(&old_base,&base);
}

void
BadGuy::explode_bomb()
{
  static const int EXPLODETIME = 1000;
  invulnerable = true;
  disable_collision = false; // now the bomb hurts
  mode = BOMB_EXPLODE;
  set_sprite(img_mrbomb_explosion, img_mrbomb_explosion);
  dying = DYING_NOT; 
  timer.start(EXPLODETIME);
  play_sound(SND_EXPLODE, base.x); // play explosion sound
}

void
BadGuy::action_stalactite(double frame_ratio)
{
  Player& tux = *World::current()->get_tux();

  static const int RANGE = 40;
    
  if (mode == NORMAL) 
  {
    // start shaking when tux is below the stalactite and at least 40 pixels
    // near
    if(tux.base.x + 32 > base.x - RANGE && tux.base.x < base.x + 32 + RANGE
            && tux.base.y + tux.base.height > base.y)
      crack_stalactite();
    return;
  }
  if (mode == STALACTITE_SHAKING)
  {
    base.x = old_base.x + (rand() % 6) - 3; // TODO this could be done nicer...
    if(!timer.check())
      mode = STALACTITE_FALL;
    return;
  }
  fall();
  if (mode == STALACTITE_FALL && issolid(base.x+base.width/2, base.y+base.height)) // Destroy if we collides with land
  {
    timer.start(2000);
    dying = DYING_SQUISHED;
    mode = FLAT;
    set_sprite(img_stalactite_broken, img_stalactite_broken);
    play_sound(SND_STALACTITE_CRASH, base.x);
  }

  // move
  physic.apply(frame_ratio, base.x, base.y);
}

void
BadGuy::crack_stalactite()
{
  static const int SHAKETIME = 800;
  play_sound(SND_STALACTITE_CRACK, base.x);
  timer.start(SHAKETIME);
  mode = STALACTITE_SHAKING;
}

void
BadGuy::action_flame(double frame_ratio)
{
    static const float radius = 100;
    static const float speed = 0.02;
    base.x = old_base.x + cos(base.ym) * radius;
    base.y = old_base.y + sin(base.ym) * radius;

    base.ym = fmodf(base.ym + frame_ratio * speed, 2*M_PI);
}

void
BadGuy::action_fish(double frame_ratio)
{
  static const float JUMPV = 6;
  static const int WAITTIME = 1000;
  
  // go in wait mode when back in water
  if(gettile(base.x, base.y+ base.height)->water
        && physic.get_velocity_y() <= 0 && mode == NORMAL)
  {
    mode = FISH_WAIT;
    set_sprite(0, 0);
    physic.set_velocity(0, 0);
    physic.enable_gravity(false);
    timer.start(WAITTIME);
    disable_collision = true;
  }
  else if(mode == FISH_WAIT && !timer.check())
  {
    // jump again
    set_sprite(img_fish, img_fish);
    mode = NORMAL;
    physic.set_velocity(0, JUMPV);
    physic.enable_gravity(true);
    disable_collision = false;
  }

  physic.apply(frame_ratio, base.x, base.y);
  collision_swept_object_map(&old_base, &base);

  if(physic.get_velocity_y() < 0)
    set_sprite(img_fish_down, img_fish_down);
}

void
BadGuy::action_lavaball(double frame_ratio)
{
  static const float JUMPV = 5;
  static const int WAITTIME = 1000;
    
  // go in wait mode when back in water
  if(gettile(base.x, base.y+ base.height)->water
        && physic.get_velocity_y() <= 0 && mode == NORMAL)
  {
    mode = FISH_WAIT;
    set_sprite(0, 0);
    physic.set_velocity(0, 0);
    physic.enable_gravity(false);
    timer.start(WAITTIME);
    disable_collision = true;
  }
  else if(mode == FISH_WAIT && !timer.check())
  {
    // jump again
    set_sprite(img_lavaball, img_lavaball);
    mode = NORMAL;
    float height = base.y-base.ym;
    if (height <= 16)
      height = JUMPV;
    else
      height = sqrt(height/World::current()->get_level()->gravity)*1.5f;

    physic.set_velocity(0, height);
    physic.enable_gravity(true);
    disable_collision = false;
  }

  physic.apply(frame_ratio, base.x, base.y);
  if(physic.get_velocity_y() < 0)
    set_sprite(img_lavaball_down, img_lavaball_down);
}

void
BadGuy::action_bouncingsnowball(double frame_ratio)
{
  static const float JUMPV = 4.5;
    
  fall();

  // jump when on ground
  mode = NORMAL;
  if(issolid(base.x, base.y+32))
    {
      physic.set_velocity_y(JUMPV);
      physic.enable_gravity(true);
    }

  // check for right/left collisions
  check_horizontal_bump();

  physic.apply(frame_ratio, base.x, base.y);
  collision_swept_object_map(&old_base, &base);
}

void
BadGuy::action_flyingsnowball(double frame_ratio)
{
  static const float FLYINGSPEED = 1;
  static const int DIRCHANGETIME = 1000;

  // set direction based on Tux
  Player& tux = *World::current()->get_tux();  
  if(tux.base.x > base.x)
    dir = RIGHT;
  else
    dir = LEFT;

  if (mode == NORMAL)
  {
    mode = FLY_UP;
    physic.set_velocity_y(FLYINGSPEED);
    timer.start(DIRCHANGETIME/2);
  }
  if (!timer.check())
  {
    if (mode == FLY_UP)
    {
      mode = FLY_DOWN;
      physic.set_velocity_y(-FLYINGSPEED);
    }
    else
    {
      mode = FLY_UP;
      physic.set_velocity_y(FLYINGSPEED);
    }
    timer.start(DIRCHANGETIME);
  }

  physic.apply(frame_ratio, base.x, base.y);
  collision_swept_object_map(&old_base, &base);                                                  
}

void
BadGuy::action_spiky(double frame_ratio)
{
  check_horizontal_bump();
  fall();
#if 0
  // jump when we're about to fall
  if (physic.get_velocity_y() == 0 && 
          !issolid(base.x+base.width/2, base.y + base.height)) {
    physic.enable_gravity(true);
    physic.set_velocity_y(2);
  }
#endif

  physic.apply(frame_ratio, base.x, base.y);
  collision_swept_object_map(&old_base,&base);   
}

void
BadGuy::action_snowball(double frame_ratio)
{
  check_horizontal_bump();
  fall();
  physic.apply(frame_ratio, base.x, base.y);
  collision_swept_object_map(&old_base,&base);
}

void
BadGuy::action_dead_squished(double frame_ratio)
{
  physic.enable_gravity(true);
  physic.apply(frame_ratio, base.x, base.y);
  collision_swept_object_map(&old_base, &base);

  // Remove it if time's up
  if (!timer.check())
      remove_me();
}

void
BadGuy::action_dead_falling(double frame_ratio)
{
  physic.enable_gravity(true);
  physic.apply(frame_ratio, base.x, base.y);
}

void
BadGuy::action(double frame_ratio)
{
  // Remove if it's far off the screen:
  if (base.x < scroll_x - OFFSCREEN_DISTANCE)
    {
      remove_me();                                                
      return;
    }

  // BadGuy fall below the ground
#ifndef RES320X240
  if (base.y > screen->h) {
#else
  if (base.y > 640) {
#endif
    remove_me();
    return;
  }

  // Once it's on screen, it's activated!
#ifndef RES320X240
  if (base.x <= scroll_x + screen->w + OFFSCREEN_DISTANCE)
#else
  if (base.x <= scroll_x + 640 + OFFSCREEN_DISTANCE)
#endif
    seen = true;

  if(!seen)
    return;

  // If they're dead, override their action
  switch (dying) {
    case DYING_NOT:
    default:
      break;
    case DYING_FALLING:
      return action_dead_falling(frame_ratio);
    case DYING_SQUISHED:
      return action_dead_squished(frame_ratio);
  }

  if (check_tiles)
    check_current_tiles();

  switch (kind) {
    case BAD_MRICEBLOCK:
      return action_mriceblock(frame_ratio);
    case BAD_JUMPY:
      return action_jumpy(frame_ratio);
    case BAD_MRBOMB:
      return action_mrbomb(frame_ratio);
    case BAD_BOMB:
      return action_bomb(frame_ratio);
    case BAD_STALACTITE:
      return action_stalactite(frame_ratio);
    case BAD_FLAME:
      return action_flame(frame_ratio);
    case BAD_FISH:
      return action_fish(frame_ratio);
    case BAD_LAVABALL:
      return action_lavaball(frame_ratio);
    case BAD_BOUNCINGSNOWBALL:
      return action_bouncingsnowball(frame_ratio);
    case BAD_FLYINGSNOWBALL:
      return action_flyingsnowball(frame_ratio);
    case BAD_SPIKY:
      return action_spiky(frame_ratio);
    case BAD_SNOWBALL:
      return action_snowball(frame_ratio);
    default:
      return;
  }
}

void
BadGuy::check_current_tiles()
{
  World::current()->try_tile_interact(base.x, base.y, this, CO_BADGUY);
  World::current()->try_tile_interact(base.x+31, base.y, this, CO_BADGUY);
  World::current()->try_tile_interact(base.x, base.y + base.height, this, CO_BADGUY);
  World::current()->try_tile_interact(base.x+31, base.y + base.height, this, CO_BADGUY);
}

void
BadGuy::draw()
{
  // Don't try to draw stuff that is outside of the screen
#ifndef RES320X240
  if(base.x <= scroll_x - base.width || base.x >= scroll_x + screen->w)
#else
  if(base.x <= scroll_x - base.width || base.x >= scroll_x + 640)
#endif
    return;
  
  if(sprite_left == 0 || sprite_right == 0)
    {
      return;
    }

  Sprite* sprite = (dir == LEFT) ? sprite_left : sprite_right;
  sprite->draw(base.x - scroll_x, base.y);

  if (debug_mode)
    fillrect(base.x - scroll_x, base.y, base.width, base.height, 75,0,75, 150);
}

void
BadGuy::set_sprite(Sprite* left, Sprite* right) 
{
  if (1)
    {
      base.width = 32;
      base.height = 32;
    }
  else
    {
      // FIXME: Using the image size for the physics and collision is
      // a bad idea, since images should always overlap there physical
      // representation
      if(left != 0) {
        if(base.width == 0 && base.height == 0) {
          base.width  = left->get_width();
          base.height = left->get_height();
        } else if(base.width != left->get_width() || base.height != left->get_height()) {
          base.x -= (left->get_width() - base.width) / 2;
          base.y -= left->get_height() - base.height;
          base.width = left->get_width();
          base.height = left->get_height();
          old_base = base;
        }
      } else {
        base.width = base.height = 0;
      }
    }

  animation_offset = 0;
  sprite_left  = left;
  sprite_right = right;
}

void
BadGuy::set_sprite_mriceblock(bool flat)
{
  if (flat)
  {
    if (stay_on_platform)
      set_sprite(img_smartblock_flat_left, img_smartblock_flat_right);
    else
      set_sprite(img_mriceblock_flat_left, img_mriceblock_flat_right);
    return;
  }
  if (stay_on_platform)
    set_sprite(img_smartblock_left, img_smartblock_right);
  else
    set_sprite(img_mriceblock_left, img_mriceblock_right);
}

void
BadGuy::bump()
{
  // these can't be bumped
  if(invulnerable || !bumpable)
    return;

  physic.set_velocity_y(3);
  kill_me(25);
}

void
BadGuy::squish_me(Player* player)
{
  player->jump_of_badguy(this);
    
  World::current()->add_score(base.x - scroll_x,
                              base.y, 50 * player_status.score_multiplier);
  play_sound(SND_SQUISH);
  player_status.score_multiplier++;

  dying = DYING_SQUISHED;
  timer.start(2000);
  physic.set_velocity(0, 0);
}

void
BadGuy::squish(Player* player)
{
  static const int MAX_ICEBLOCK_SQUICHES = 10;
    
  if(kind == BAD_MRBOMB) {
    // mrbomb transforms into a bomb now
    World::current()->add_bad_guy(base.x, base.y, BAD_BOMB);
   
    player->jump_of_badguy(this);
    World::current()->add_score(base.x - scroll_x, base.y, 50 * player_status.score_multiplier);
    play_sound(SND_SQUISH);
    player_status.score_multiplier++;
    remove_me();
    return;

  } else if (kind == BAD_MRICEBLOCK) {
    if (mode == NORMAL || mode == KICK)
      {
        /* Flatten! */
        play_sound(SND_STOMP);
        mode = FLAT;
        set_sprite_mriceblock(true);
        physic.set_velocity_x(0);

        timer.start(4000);
      } else if (mode == FLAT) {
        /* Kick! */
        play_sound(SND_KICK);

        if (player->base.x < base.x + (base.width/2)) {
          physic.set_velocity_x(5);
          dir = RIGHT;
        } else {
          physic.set_velocity_x(-5);
          dir = LEFT;
        }

        mode = KICK;
        player->kick_timer.start(KICKING_TIME);
        set_sprite_mriceblock(true);
      }

    player->jump_of_badguy(this);

    player_status.score_multiplier++;

    // check for maximum number of squiches
    squishcount++;
    if(squishcount >= MAX_ICEBLOCK_SQUICHES) {
      kill_me(50);
      return;
    }
    
    return;
  } else if(kind == BAD_FISH) {
    // fish can only be killed when falling down
    if(physic.get_velocity_y() >= 0)
      return;
    
    player->jump_of_badguy(this);
	      
    World::current()->add_score(base.x - scroll_x, base.y, 25 * player_status.score_multiplier);
    player_status.score_multiplier++;
     
    // simply remove the fish...
    remove_me();
    return;
  } else if(kind == BAD_BOUNCINGSNOWBALL) {
    squish_me(player);
    set_sprite(img_bouncingsnowball_squished_left,img_bouncingsnowball_squished_right);
    return;
  } else if(kind == BAD_FLYINGSNOWBALL) {
    squish_me(player);
    set_sprite(img_flyingsnowball_squished_left,img_flyingsnowball_squished_right);
    return;
  } else if(kind == BAD_SNOWBALL) {
    squish_me(player);
    if (stay_on_platform)
      set_sprite(img_smartball_squished_left, img_smartball_squished_right);
    else
      set_sprite(img_snowball_squished_left, img_snowball_squished_right);
    return;
  }
}

void
BadGuy::kill_me(int score)
{
  if(removable || invulnerable)
    return;

  dying = DYING_FALLING;
  if (kind == BAD_MRBOMB || kind == BAD_BOMB)
  {
    // mrbomb explodes, giving no points
    explode(this, true);
    return;
  }
  if(kind == BAD_MRICEBLOCK) {
    set_sprite_mriceblock(true);
    if(mode == HELD) {
      mode = NORMAL;
      Player& tux = *World::current()->get_tux();  
      tux.holding_something = false;
    }
  }
  
  physic.enable_gravity(true);

  /* Gain some points: */
  if (score > 0)
    World::current()->add_score(base.x - scroll_x, base.y,
                    score * player_status.score_multiplier);

  /* Play death sound: */
  play_sound(SND_FALL);
}

void BadGuy::explode(BadGuy *badguy, bool instant)
{
  if (badguy->kind == BAD_BOMB)
  {
    if (instant)
      badguy->explode_bomb();
    return;
  }
  BadGuy* bomb = World::current()->add_bad_guy(badguy->base.x, badguy->base.y, BAD_BOMB);
  badguy->remove_me();
  if (instant)
    bomb->explode_bomb();
}

void
BadGuy::collision(void *p_c_object, int c_object, CollisionType type)
{
  BadGuy* pbad_c    = NULL;

  if(type == COLLISION_BUMP) {
    bump();
    return;
  }

  if(type == COLLISION_SQUISH) {
    Player* player = static_cast<Player*>(p_c_object);
    squish(player);
    return;
  }

  /* COLLISION_NORMAL */
  switch (c_object)
    {
    case CO_BULLET:
        if (kind == BAD_STALACTITE && mode == NORMAL)
        {
          crack_stalactite();
          return;
        }
        if (!invulnerable && burnable)
          kill_me(10);
      break;

    case CO_BADGUY:
      pbad_c = (BadGuy*) p_c_object;

      /* If we're a kicked/held mriceblock, kill any badguys we hit */
      if (kind == BAD_MRICEBLOCK && mode > FLAT)
        {
          pbad_c->kill_me(25);

          // a held mriceblock gets kills the enemy too but falls to ground then
          if (mode == HELD) 
            kill_me(0);
          break;
        }

      /* Kill badguys that run into exploding bomb */
      else if (kind == BAD_BOMB && !disable_collision)
      {
        pbad_c->kill_me(50);
      }

      /* Kill any badguys that get hit by stalactite */
      else if (kind == BAD_STALACTITE && !disable_collision)
      {
        pbad_c->kill_me(50);
      }

      /* When enemies run into eachother, make them change directions */
      else
      {
        // Jumpy, fish, flame, stalactites are exceptions
        if (!pbad_c->turn_when_bumped)
          break;

        // Bounce off of other badguy if we land on top of him
        if (base.y + base.height < pbad_c->base.y + pbad_c->base.height)
        {
          if (pbad_c->dir == LEFT)
          {
            dir = RIGHT;
            physic.set_velocity(fabsf(physic.get_velocity_x()), 2);
          }
          else if (pbad_c->dir == RIGHT)
          {
            dir = LEFT;
            physic.set_velocity(-fabsf(physic.get_velocity_x()), 2);
          }



          break;
        }
        else if (base.y + base.height > pbad_c->base.y + pbad_c->base.height)
          break;

        if (pbad_c->kind != BAD_FLAME)
          {
            if (dir == LEFT)
            {
              dir = RIGHT;
              physic.set_velocity_x(fabsf(physic.get_velocity_x()));
            }
            else if (dir == RIGHT)
            {
              dir = LEFT;
              physic.set_velocity_x(-fabsf(physic.get_velocity_x()));
            }

          }
      }
      
      break;

    case CO_PLAYER:
      Player* player = static_cast<Player*>(p_c_object);
      /* Get kicked if were flat */
      if (mode == FLAT && !disable_collision && !dying)
      {
        play_sound(SND_KICK);

        // Hit from left side
        if (player->base.x < base.x) {
          physic.set_velocity_x(5);
          dir = RIGHT;
        }
        // Hit from right side
        else {
          physic.set_velocity_x(-5);
          dir = LEFT;
        }

        mode = KICK;
        player->kick_timer.start(KICKING_TIME);
        set_sprite_mriceblock(true);
      }
      break;

    }
}


//---------------------------------------------------------------------------

void load_badguy_gfx()
{
  img_mriceblock_flat_left = sprite_manager->load("mriceblock-flat-left");
  img_mriceblock_flat_right = sprite_manager->load("mriceblock-flat-right");
  img_mriceblock_left = sprite_manager->load("mriceblock-left");
  img_mriceblock_right = sprite_manager->load("mriceblock-right");
  img_smartblock_flat_left = sprite_manager->load("smartblock-flat-left");
  img_smartblock_flat_right = sprite_manager->load("smartblock-flat-right");
  img_smartblock_left = sprite_manager->load("smartblock-left");
  img_smartblock_right = sprite_manager->load("smartblock-right");

  img_jumpy_left_up = sprite_manager->load("jumpy-left-up");
  img_jumpy_left_down = sprite_manager->load("jumpy-left-down");
  img_jumpy_left_middle = sprite_manager->load("jumpy-left-middle");
  img_jumpy_right_up = sprite_manager->load("jumpy-right-up");
  img_jumpy_right_down = sprite_manager->load("jumpy-right-down");
  img_jumpy_right_middle = sprite_manager->load("jumpy-right-middle");
  
  img_mrbomb_left = sprite_manager->load("mrbomb-left");
  img_mrbomb_right = sprite_manager->load("mrbomb-right");
  img_mrbomb_ticking_left = sprite_manager->load("mrbomb-ticking-left");
  img_mrbomb_ticking_right = sprite_manager->load("mrbomb-ticking-right");
  img_mrbomb_explosion = sprite_manager->load("mrbomb-explosion");
  img_stalactite = sprite_manager->load("stalactite");
  img_stalactite_broken = sprite_manager->load("stalactite-broken");

  img_flame = sprite_manager->load("flame");

  img_fish = sprite_manager->load("fish");
  img_fish_down = sprite_manager->load("fish-down");
  img_lavaball = sprite_manager->load("lavaball");
  img_lavaball_down = sprite_manager->load("lavaball-down");

  img_bouncingsnowball_left = sprite_manager->load("bouncingsnowball-left");
  img_bouncingsnowball_right = sprite_manager->load("bouncingsnowball-right");
  img_bouncingsnowball_squished_left = sprite_manager->load("bouncingsnowball-squished-left");
  img_bouncingsnowball_squished_right = sprite_manager->load("bouncingsnowball-squished-right");
  img_flyingsnowball_left = sprite_manager->load("flyingsnowball-left");
  img_flyingsnowball_right = sprite_manager->load("flyingsnowball-right");
  img_flyingsnowball_squished_left = sprite_manager->load("flyingsnowball-squished-left");
  img_flyingsnowball_squished_right = sprite_manager->load("flyingsnowball-squished-right");
  img_spiky_left = sprite_manager->load("spiky-left");
  img_spiky_right = sprite_manager->load("spiky-right");
  img_snowball_left = sprite_manager->load("snowball-left");
  img_snowball_right = sprite_manager->load("snowball-right");
  img_snowball_squished_left = sprite_manager->load("snowball-squished-left");
  img_snowball_squished_right = sprite_manager->load("snowball-squished-right");
  img_smartball_left = sprite_manager->load("smartball-left");
  img_smartball_right = sprite_manager->load("smartball-right");
  img_smartball_squished_left = sprite_manager->load("smartball-squished-left");
  img_smartball_squished_right = sprite_manager->load("smartball-squished-right");
}

void free_badguy_gfx()
{
}

// EOF //
