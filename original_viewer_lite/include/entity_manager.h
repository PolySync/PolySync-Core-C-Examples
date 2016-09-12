/*
 * Copyright (c) 2016 PolySync
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file entity_manager.h
 * @brief Object Entity Manager Interface.
 *
 */




#ifndef ENTITY_MANAGER_H
#define	ENTITY_MANAGER_H




#include "drawable_type.h"
#include "gui.h"




/**
 * @brief Default object timeout interval. [microseconds]
 *
 * See \ref object_s.timeout_interval.
 *
 * 175 milliseconds.
 *
 */
#define     DEFAULT_OBJECT_TIMEOUT      (175000ULL)


/**
 * @brief Default object container timeout interval. [microseconds]
 *
 * See \ref object_container_s.timeout_interval.
 *
 * 500 milliseconds.
 *
 */
#define     DEFAULT_CONTAINER_TIMEOUT   (500000ULL)


/**
 * @brief Default object container parent timeout interval. [microseconds]
 *
 * See \ref object_container_parent_s.timeout_interval.
 *
 * 500 milliseconds.
 *
 */
#define     DEFAULT_PARENT_TIMEOUT      (500000ULL)


// no timeouts


/**
 * @brief Timeout interval not used. [microseconds]
 *
 */
#define     ENTITY_NO_TIMEOUT           (0)




object_s *entity_object_new( const unsigned long long id );


object_s *entity_object_new_copy( const object_s * const copy );


void entity_object_init( const unsigned long long id, object_s * const object );


object_container_s *entity_container_new( const unsigned long long id );


object_container_parent_s *entity_parent_new( const unsigned long long id );


void entity_release_all( GList * const parent_list );


GList *entity_update_timeouts( GList * const parent_list, const unsigned long long compare_time );


object_s *entity_container_search_by_id( const object_container_s * const container, const unsigned long long obj_id );


object_container_s *entity_parent_search_by_id( const object_container_parent_s * const parent, const unsigned long long container_id );


object_container_parent_s *entity_parent_list_search_by_id( const GList * const parent_list, const unsigned long long parent_id );


GList *entity_object_update_copy( GList * const parent_list, const unsigned long long parent_id, const unsigned long long container_id, const object_s * const object );


// removed
//GList *entity_object_update_take( GList * const parent_list, const unsigned long long parent_id, const unsigned long long container_id, const object_s * const object );


void entity_draw_object( const gui_context_s * const gui, const object_s * const object, const GLdouble * const color_rgba );


void entity_draw_container( const gui_context_s * const gui, const object_container_s * const container, const GLdouble * const color_rgba );


void entity_draw_parent( const gui_context_s * const gui, const object_container_parent_s * const parent );


void entity_draw_all( const gui_context_s * const gui, const GList * const parent_list );



#endif	/* ENTITY_MANAGER_H */
