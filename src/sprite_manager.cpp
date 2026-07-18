//  $Id$
//
//  SuperTux
//  Copyright (C) 2004 Ingo Ruhnke <grumbel@gmx.de>
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <SDL2/SDL_render.h>
#include <iostream>
#include "sprite_manager.h"
#include "sprite.h"

SpriteManager::SpriteManager(const std::string& filename)
{
  load_resfile(filename);
}

SpriteManager::~SpriteManager()
{
  for(std::map<std::string, Sprite*>::iterator i = sprites.begin();
      i != sprites.end(); ++i) {
    delete i->second;
  }
}

void
SpriteManager::load_resfile(const std::string& filename)
{
  lisp_object_t* root_obj = lisp_read_from_file(filename);
  if (!root_obj)
    {
      std::cout << "SpriteManager: Couldn't load: " << filename << std::endl;
      return;
    }

  lisp_object_t* cur = root_obj;

  if (strcmp(lisp_symbol(lisp_car(cur)), "supertux-resources") != 0)
    return;
  cur = lisp_cdr(cur);

  while(cur)
    {
      lisp_object_t* el = lisp_car(cur);

      if (strcmp(lisp_symbol(lisp_car(el)), "sprite") == 0)
        {
          LispReader reader(lisp_cdr(el));
          Sprite* sprite = new Sprite(reader);
          add(sprite);
          if (sprite)
            try_make_copies(sprite, reader);
        }
      else
          std::cout << "SpriteManager: Unknown tag" << std::endl;

      cur = lisp_cdr(cur);
    }

  lisp_free(root_obj);
}

void
SpriteManager::add(Sprite* sprite)
{
  Sprites::iterator i = sprites.find(sprite->get_name());
  if (i == sprites.end())
  {
    sprites[sprite->get_name()] = sprite;
    return;
  }
  delete i->second;
  i->second = sprite;
  std::cout << "Warning: duplicate entry: '" << sprite->get_name() << "'" << std::endl;
}

void
SpriteManager::try_make_copies(Sprite *sprite, LispReader &reader)
{
  std::vector<std::string> toCopy;
  Sprite* newSprite;

  if (reader.read_string_vector("copy-as", &toCopy))
  {
    for(std::vector<std::string>::size_type i = 0; i < toCopy.size(); ++i)
    {
      newSprite = new Sprite();
      newSprite->copy(toCopy[i], sprite, SDL_FLIP_NONE);
      add(newSprite);
    }
  }
  toCopy.clear();
  if (reader.read_string_vector("mirror-as", &toCopy))
  {
    for(std::vector<std::string>::size_type i = 0; i < toCopy.size(); ++i)
    {
      newSprite = new Sprite();
      newSprite->copy(toCopy[i], sprite, SDL_FLIP_HORIZONTAL);
      add(newSprite);
    }
  }
}

Sprite*
SpriteManager::load(const std::string& name)
{
  Sprites::iterator i = sprites.find(name);
  if (i != sprites.end())
    {
      return i->second;
    }
  else
    {
      std::cout << "SpriteManager: Sprite '" << name << "' not found" << std::endl;
      return 0;
    }
}

/* EOF */
