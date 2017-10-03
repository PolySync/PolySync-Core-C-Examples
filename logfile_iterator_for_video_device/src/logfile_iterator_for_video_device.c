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
 * \example logfile_iterator_for_velodyne_hdl.c
 *
 * Shows how to use the Logfile API routines to iterate over a Velodyne HDL
 * PolySync logfile and access the raw data, outside the normal replay time domain.
 *
 */



#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// API headers
#include "polysync_core.h"
#include "polysync_sdf.h"
#include "polysync_node.h"
#include "polysync_message.h"
#include "polysync_logfile.h"
#include "polysync_video.h"

#include "pixmap_ppm.h"
#include "pixmap_ppm_gz.h"
#include "pixmap_png.h"
#include "pixmap_jpg.h"

// *****************************************************
// static global types/macros
// *****************************************************

/**
 * @brief PolySync node name.
 *
 */
static const char NODE_NAME[] = "polysync-logfile-iterator-for-video-device-c";


/**
 * @brief Logfile path we'll use instead of the automatic API-generated name.
 *
 * This is the path to the sample logfile session that's shipped with the release
 *
 */
static const char LOGFILE_PATH[] = "/home/snewton/Downloads/70802/video-device.1688895556945937.plog";


/**
 * @brief PolySync 'ps_byte_array_msg' type name.
 *
 */
static const char IMAGE_DATA_MSG_NAME[] = "ps_image_data_msg";


/**
 * @brief Structure to store the byte array message type
 *
 */
typedef struct
{
    ps_node_ref node_ref;
    ps_msg_type image_data_msg_type;
    char in_file[PSYNC_DEFAULT_STRING_LEN];
    char out_file[PSYNC_DEFAULT_STRING_LEN];
    unsigned long long img_count;
} context_s;


// *****************************************************
// static declarations
// *****************************************************

/**
 * @brief Logfile iterator callback.
 *
 * @warning When logfile is empty, expect:
 * \li \ref ps_logfile_attributes.data_count == 0
 * \li msg_type == \ref PSYNC_MSG_TYPE_INVALID
 * \li log_record == NULL
 *
 * @param [in] file_attributes Logfile attributes loaded by the logfile API.
 * @param [in] msg_type Message type identifier for the message in \ref ps_rnr_log_record.data, as seen by this data model.
 * @param [in] log_record Logfile record loaded by the logfile API.
 * @param [in] user_data A pointer to user data, provided by \ref psync_logfile_foreach_iterator.
 *
 */
static void logfile_iterator_callback(
        const ps_logfile_attributes * const file_attributes,
        const ps_msg_type msg_type,
        const ps_rnr_log_record * const log_record,
        void * const user_data);




// *****************************************************
// static definitions
// *****************************************************


unsigned long GLOBAL_COUNT = 0;
//
static void logfile_iterator_callback(
        const ps_logfile_attributes * const file_attributes,
        const ps_msg_type msg_type,
        const ps_rnr_log_record * const log_record,
        void * const user_data)
{

    // int ret = DTC_NONE;
    context_s * const context = (context_s*) user_data;

    // if logfile is empty, only attributes are provided
    if(log_record != NULL)
    {
        printf("log record - index: %lu - size: %lu bytes - prev_size: %lu bytes - RnR timestamp (header.timestamp): %llu\n",
            (unsigned long) log_record->index,
            (unsigned long) log_record->size,
            (unsigned long) log_record->prev_size,
            (unsigned long long) log_record->timestamp);

        // we only want to read byte array messages, where the sensors payload
        // is typically stored
        if(msg_type == context->image_data_msg_type)
        {

            if(GLOBAL_COUNT > 4)
            {
                return;
            }

            GLOBAL_COUNT++;

            const ps_msg_ref msg = (ps_msg_ref) log_record->data;
            const ps_image_data_msg * const image_data_msg = (ps_image_data_msg*) msg;

            struct pixmap* img = pixmap_alloc(image_data_msg->width, image_data_msg->height);

            img->bytes = image_data_msg->data_buffer._buffer;

            char png_name[16] = {0};
            char jpg_name[16] = {0};
            snprintf(png_name, 16, "img_%llu.png", context->img_count);
            snprintf(jpg_name, 16, "img_%llu.jpg", context->img_count);

            ++context->img_count;

            pixmap_write_png(img, png_name);
            pixmap_write_jpg(img, jpg_name);

        }
    }
}




// *****************************************************
// main
// *****************************************************
int main(int argc, char **argv)
{
    // polysync return status
    int ret = DTC_NONE;

    // context data
    context_s context;

    memset(&context, 0, sizeof(context));


    context.img_count = 0;

    // init core API
    if((ret = psync_init(
            NODE_NAME,
            PSYNC_NODE_TYPE_API_USER,
            PSYNC_DEFAULT_DOMAIN,
            PSYNC_SDF_ID_INVALID,
            PSYNC_INIT_FLAG_STDOUT_LOGGING,
            &context.node_ref)) != DTC_NONE)
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "main -- psync_init - ret: %d",
                ret);
        return EXIT_FAILURE;
    }

     // get the message type for 'ps_image_data_msg'
    ret = psync_message_get_type_by_name(
            context.node_ref,
            IMAGE_DATA_MSG_NAME,
            &context.image_data_msg_type);

    if(ret != DTC_NONE)
    {
        psync_log_error("psync_message_get_type_by_name - ret: %d", ret);
        (void) psync_release(&context.node_ref);
        return EXIT_FAILURE;
    }

    // initialize logfile API resources
    if((ret = psync_logfile_init(context.node_ref)) != DTC_NONE)
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "main -- psync_logfile_init - ret: %d",
                ret);
        goto GRACEFUL_EXIT_STMNT;
    }

    // iterate over the logfile data
    if((ret = psync_logfile_foreach_iterator(
            context.node_ref,
            LOGFILE_PATH,
            logfile_iterator_callback,
            &context)) != DTC_NONE)
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "main -- psync_logfile_foreach_iterator - ret: %d",
                ret);
        goto GRACEFUL_EXIT_STMNT;
    }

    // using 'goto' to allow for an easy example exit
    GRACEFUL_EXIT_STMNT:

    // release logfile API resources
    if((ret = psync_logfile_release(context.node_ref)) != DTC_NONE)
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "main -- psync_logfile_release - ret: %d",
                ret);
    }

	// release core API
    if((ret = psync_release(&context.node_ref)) != DTC_NONE)
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "main -- psync_release - ret: %d",
                ret);
    }

    return EXIT_SUCCESS;
}
