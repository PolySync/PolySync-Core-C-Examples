/**
 * @file ps_interface.c
 * @brief PolySync Interface Source.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <glib-2.0/glib.h>
#include "polysync_core.h"
#include "polysync_node.h"
#include "polysync_sdf.h"
#include "polysync_message.h"

#include "common.h"
#include "gui.h"
#include "ps_interface.h"



// *****************************************************
// static global structures
// *****************************************************




// *****************************************************
// public global data
// *****************************************************


// docs in \ref ps_interface.h
const char       PS_NODE_NAME[]     = "polysync-parrot-visualizer";

// docs in \ref ps_interface.h
const char       PS_PLATFORM_MOTION_MSG_NAME[] = "ps_platform_motion_msg";

// docs in \ref ps_interface.h
const char       PS_PLATFORM_STEERING_CMD_MSG_NAME[] = "ps_platform_steering_command_msg";

// docs in \ref ps_interface.h
const char       PS_PLATFORM_THROTTLE_CMD_MSG_NAME[] = "ps_platform_throttle_command_msg";



// *****************************************************
// static global data
// *****************************************************




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************


//
static void psync_steering_cmd_handler( 
        const ps_msg_type msg_type, 
        const ps_msg_ref const message, 
        void * const user_data )
{
    // local vars
    node_data_s     *node_data  = NULL;
    ps_guid         src_guid    = PSYNC_GUID_INVALID;

    // cast
    node_data = (node_data_s*) user_data;

    // ignore if not valid
    if( (node_data == NULL) || (message == NULL) )
    {
        return;
    }
    
    ps_platform_steering_command_msg * steering_msg = 
            (ps_platform_steering_command_msg*) message;
    
    node_data->vehicle_cmds.currentSteeringCommand = 
            steering_msg->steering_wheel_angle;
}

//
static void psync_throttle_cmd_handler( 
        const ps_msg_type msg_type, 
        const ps_msg_ref const message, 
        void * const user_data )
{
    // local vars
    node_data_s     *node_data  = NULL;
    ps_guid         src_guid    = PSYNC_GUID_INVALID;

    // cast
    node_data = (node_data_s*) user_data;

    // ignore if not valid
    if( (node_data == NULL) || (message == NULL) )
    {
        return;
    }
    
    ps_platform_throttle_command_msg * throttle_msg = 
            ( ps_platform_throttle_command_msg* ) message;
    
    node_data->vehicle_cmds.currentThrottleCommand = 
            throttle_msg->throttle_command;
}



// *****************************************************
// public definitions
// *****************************************************

//
node_data_s *init_polysync( void )
{
    // local vars
    node_data_s *node_data = NULL;

    // create
    if( (node_data = malloc( sizeof(*node_data) )) == NULL )
    {
        return NULL;
    }

    // zero
    memset( node_data, 0, sizeof(*node_data) );
    node_data->node = PSYNC_NODE_REF_INVALID;
    node_data->msg_type_platform_motion = PSYNC_MSG_TYPE_INVALID;

    // init polysync
    if( psync_init(
            PS_NODE_NAME,
            PSYNC_NODE_TYPE_API_USER,
            PSYNC_DEFAULT_DOMAIN,
            PSYNC_SDF_ID_INVALID,
            PSYNC_INIT_FLAG_STDOUT_LOGGING,
            &node_data->node ) != DTC_NONE )
    {
        free( node_data );
        return NULL;
    }

    // disable handlers
    if( psync_node_set_flag( node_data->node, NODE_FLAG_HANDLERS_ENABLED, 0 ) != DTC_NONE )
    {
        (void) psync_release( &node_data->node );
        free( node_data );
        return NULL;
    }

    // get type
    if( psync_message_get_type_by_name( node_data->node, PS_PLATFORM_STEERING_CMD_MSG_NAME, &node_data->msg_type_steering_cmd ) != DTC_NONE )
    {
        (void) psync_release( &node_data->node );
        free( node_data );
        return NULL;
    }

    // register listener
    if( psync_message_register_listener( node_data->node, node_data->msg_type_steering_cmd, psync_steering_cmd_handler, node_data ) != DTC_NONE )
    {
        (void) psync_release( &node_data->node );
        free( node_data );
        return NULL;
    }
    
    // get type
    if( psync_message_get_type_by_name( node_data->node, PS_PLATFORM_THROTTLE_CMD_MSG_NAME, &node_data->msg_type_throttle_cmd ) != DTC_NONE )
    {
        (void) psync_release( &node_data->node );
        free( node_data );
        return NULL;
    }

    // register listener
    if( psync_message_register_listener( node_data->node, node_data->msg_type_throttle_cmd, psync_throttle_cmd_handler, node_data ) != DTC_NONE )
    {
        (void) psync_release( &node_data->node );
        free( node_data );
        return NULL;
    }
    
    // get type for publisher
    if( psync_message_get_type_by_name( node_data->node, PS_PLATFORM_MOTION_MSG_NAME, &node_data->msg_type_platform_motion ) != DTC_NONE )
    {
        (void) psync_release( &node_data->node );
        free( node_data );
        return NULL;
    }

    // enable handlers
    if( psync_node_set_flag( node_data->node, NODE_FLAG_HANDLERS_ENABLED, 1 ) != DTC_NONE )
    {
        (void) psync_release( &node_data->node );
        free( node_data );
        return NULL;
    }


    // return new node data
    return node_data;
}


//
void release_polysync( node_data_s * const node_data )
{
    if( node_data == NULL )
    {
        return;
    }
    
    // disable handlers
    (void) psync_node_set_flag( node_data->node, NODE_FLAG_HANDLERS_ENABLED, 0 );

    // wait a little
    usleep( 100000 );

    // release polysync
    (void) psync_release( &node_data->node );
}
