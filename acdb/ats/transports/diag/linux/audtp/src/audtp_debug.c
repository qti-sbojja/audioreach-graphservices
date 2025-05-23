 /**
 *==============================================================================
 * \file audtp.c
 * \brief
 *                      A T P  P R O T O C O L  L A Y E R
 *                            S O U R C E  F I L E
 *
 *     This file contains implementation of ATP protocol layer
 *
 *     INITIALIZATION REQUIREMENTS:
 *     atp_diag_init() must be called to initialise ATP protocol layer.
 *     atp_diag_init() must be called ~AFTER~ initialization of SOUND TASK
 *
 *     This file is common to both protocol component on PC and protocol
 *     module on AMSS. PC only/phone only code is wrapped with in
 *     #ifdef WINPC
 *
 * \copyright
 *     Copyright (c) Qualcomm Innovation Center, Inc. All rights reserved.
 *     SPDX-License-Identifier: BSD-3-Clause
 *==============================================================================
 */
/*
   --------------------
   |include files                |
   --------------------
   */
#include "audtp.h"
#include "ats_common.h"

/*
   --------------------
   |Constants                  |
   --------------------
   */
const uint8_t atp_protocol_version = 1;

/*
   --------------------
   |Function definitions     |
   --------------------
   */

void print_atp_frame(atp_frame_struct* atp_frame)
{
    if (atp_frame == NULL)
    {
        ATS_ERR("ATS FRAME: frame is null");
        return;
    }

    ATS_ERR("ATS FRAME: version: %d", atp_frame->header.version);
    ATS_ERR("ATS FRAME: header length: %d", atp_frame->header.header_length);
    ATS_ERR("ATS FRAME: frame#: %d", atp_frame->header.frame_number);
    ATS_ERR("ATS FRAME: flags: %d", atp_frame->header.flags);
    ATS_ERR("ATS FRAME: frame offset: %d", atp_frame->header.frame_offset);
    ATS_ERR("ATS FRAME: frame length: %d", atp_frame->header.frame_length);
    ATS_ERR("ATS FRAME: buffer length: %d", atp_frame->header.buffer_length);

    if (atp_frame->data_ptr == NULL)
    {
        ATS_ERR("ATS FRAME: frame data is null");
    }
}

/**
 * FUNCTION init_atp_header
 *
 * DESCRIPTION : Initializes ATP frame headr with default values
 *
 * DEPENDENCIES: atp_frame_ptr should be a valid pointer
 *
 * PARAMS:
 *   atp_frame_ptr - atp_frame pointer which requires initialization
 *
 * RETURN VALUE: None
 *
 * SIDE EFFECTS: Sets the atp_frame to default values which is pointed to by atp_frame_ptr .
 */
void init_atp_header (
        atp_frame_struct *atp_frame_ptr
        )
{
    if ( atp_frame_ptr == NULL )
        return;
    /** Set protocol Version */
    atp_frame_ptr->header.version = atp_protocol_version;

    /** Set header length , which is of fixed size*/
    atp_frame_ptr->header.header_length = sizeof(atp_frame_header_struct);

    /** Turn off all flags */
    atp_frame_ptr->header.flags = 0;
    /** Reset frame number to zero */
    atp_frame_ptr->header.frame_number = 0;
    /** Reset frame offset to zero */
    atp_frame_ptr->header.frame_offset = 0;
    /** Reset frame length to zero */
    atp_frame_ptr->header.frame_length = 0;
    /** Reset buffer length to zero */
    atp_frame_ptr->header.buffer_length = 0;
} /* init_atp_header */

/**
 * FUNCTION free_atp_frame
 *
 * DESCRIPTION : Deallocates memory for atp frame data pointer and sets pointers to NULL.
 * Then deallocates memory for this frame and sets pointer to NULL
 *
 * DEPENDENCIES: atp_frame_ptr should be a valid pointer
 *
 * PARAMS:
 *   atp_frame_ptr - atp_frame pointer which needs to be deallocated
 *
 * RETURN VALUE: None
 *
 * SIDE EFFECTS: None
 *
 */
void free_atp_frame (
        atp_frame_struct* atp_frame_ptr
        )
{
    if(NULL==atp_frame_ptr)
    {
        return;
    }
    /** deallocate data part and set to NULL*/
    if ( atp_frame_ptr->data_ptr != NULL )
    {
        ACDB_FREE(atp_frame_ptr->data_ptr);
        atp_frame_ptr->data_ptr = NULL;
    }
    /** ACDB_FREE frame */
    if( atp_frame_ptr != NULL )
    {
        ACDB_FREE(atp_frame_ptr);
        atp_frame_ptr = NULL;
    }
}/* free_atp_frame */

/**
 * FUNCTION    : break_buffer_into_frames
 *
 * DESCRIPTION : Breaks a given buffer into frames and sets their header info.
 *               Creates a doubly linked list of created frames.
 *               Input buffer can be deallocated.
 *
 * DEPENDENCIES: None.
 *
 * PARAMS:
 *   buffer_ptr - pointer to the input buffer
 *   buffer_length - bufer length
 *   start_node_ptr - linked list of frames start pointer
 *
 * RETURN VALUE: Pointer to the first node of the linked list
 *
 * SIDE EFFECTS: None.
 */
void break_buffer_into_frames (
        char_t* buffer_ptr,
        uint32_t buffer_length,
        atp_frame_dblnk_lst_struct** start_node_ptr
        )
{
    /** variable to hold the starting point of the dbl lnk lst.*/
    atp_frame_dblnk_lst_struct *start_ptr = NULL;
    uint32_t frame_byte_offset = 0;
    /** initialize frame count to zero */
    uint8_t frame_count = 0;
    /** calculate max payload size */
#ifdef WINPC
    uint16_t atp_data_size = DIAG_REQ_PKT_SIZE - sizeof(atp_frame_header_struct);
#else
    uint16_t atp_data_size = DIAG_RES_PKT_SIZE - sizeof(atp_frame_header_struct);
#endif
    uint16_t frame_length = atp_data_size;
    /** Variable to hold the length of the buffer to be processed to break into frames*/
    uint32_t remaining_buffer_length = buffer_length;
    /** pointer to hold the previous node in the linked list */
    atp_frame_dblnk_lst_struct *prev_node_ptr = NULL;
    /** pointer to hold the current node in the linked list */
    atp_frame_dblnk_lst_struct *current_node_ptr = NULL;
    /** Initialize the frame count to zero*/
    frame_count = 0;
    /**loop while we have some buffer to break*/
    while(remaining_buffer_length > 0 )
    {
        atp_frame_struct* atp_frame_ptr = NULL;

        /** Create a atp_frame*/
        atp_frame_ptr = ACDB_MALLOC(atp_frame_struct, 1);
        if(NULL==atp_frame_ptr)
        {
            //Klocwork fix: memory leak, return without ACDB_FREE memory allocation
            if (current_node_ptr != NULL)
            {
                ACDB_FREE(current_node_ptr);
            }
            return;
        }
        /** Initialize the header*/
        init_atp_header(atp_frame_ptr);
        /** allocate memory for frame data */
        atp_frame_ptr->data_ptr = ACDB_MALLOC(char_t, atp_data_size);
		/*klocwork fix*/
        if(atp_frame_ptr->data_ptr==NULL)
        {
            if (current_node_ptr != NULL)
            {
                ACDB_FREE(current_node_ptr);
            }
            ACDB_FREE(atp_frame_ptr);
            return;
        }
        /**Calculate the current frame size
         *if this is last frame then frame length can be smalled than max frame length*/
        frame_length = (uint16_t) ((remaining_buffer_length >= atp_data_size) ?
                atp_data_size : remaining_buffer_length );
        /** Set it data flag for this frame*/
        atp_frame_ptr->header.flags |= FLAG_DATAFRAME_MASK;
        /** If this is the last frame in the buffer, set LAST frame flag also*/
        if( frame_length == remaining_buffer_length )
        {
            //Set LAST frame FLAG
            atp_frame_ptr->header.flags |= FLAG_LASTFRAME_MASK;
        }
        /**Increment frame count*/
        frame_count++;
        /** Set header info for current frame*/
        atp_frame_ptr->header.buffer_length = buffer_length;
        atp_frame_ptr->header.frame_offset = frame_byte_offset;
        atp_frame_ptr->header.frame_number = frame_count;
        atp_frame_ptr->header.frame_length = frame_length;

        /** copy data into frame*/
        ATS_MEM_CPY_SAFE(atp_frame_ptr->data_ptr,atp_data_size,buffer_ptr,frame_length);

        /** increment buffer pointer by the amount of data we copied*/
        buffer_ptr += frame_length;

        /** increment offset */
        frame_byte_offset +=frame_length;
        /**Calculate remaining buffer length*/
        remaining_buffer_length -=frame_length;
        current_node_ptr = ACDB_MALLOC(atp_frame_dblnk_lst_struct, 1);
        if(NULL==current_node_ptr)
        {
            ACDB_FREE(atp_frame_ptr->data_ptr);
            ACDB_FREE(atp_frame_ptr);
            //			ACDB_FREE(atp_frame_ptr->data_ptr);	//duplicate ACDB_FREE atp_frame_ptr
            return;
        }
        /**If this is first node in the linked list, save it as start_ptr*/
        if ( start_ptr == NULL )
        {
            start_ptr = current_node_ptr;
        }
        /**Initialize previous node pointer and next node pointer of current node*/
        current_node_ptr->previous_node_ptr = current_node_ptr->next_node_ptr = NULL;
        /**set the frame pointer of the current node to the atp frame we created just
         *now
         */
        current_node_ptr->frame_ptr = atp_frame_ptr;
        /**Set  the previous node pointer of the current node */
        current_node_ptr->previous_node_ptr = prev_node_ptr;
        /**set the next node pointer of the previous node */
        if ( prev_node_ptr != NULL )
        {
            prev_node_ptr->next_node_ptr = current_node_ptr;
        }
        /**Save the current node as previous node . Required to set for next node*/
        prev_node_ptr = current_node_ptr;
    }

    *start_node_ptr = start_ptr;
} /* break_buffer_into_frames */

/**
 * FUNCTION : initialize_buffer_context

 * DESCRIPTION : Initializes buffer context.
 * his should be called once before using the buffer for transmission
 *
 * DEPENDENCIES: None.
 *
 * PARAMS:
 *   buf_cntxt_ptr - pointer to the buffer context which requires initialization
 *
 * RETURN VALUE: None
 *
 * SIDE EFFECTS: None.
 */
void initialize_buffer_context (
        buffer_context_struct *buf_cntxt_ptr
        )
{
    buf_cntxt_ptr->buffer_length = 0;
    buf_cntxt_ptr->buffer_ptr = NULL;
    buf_cntxt_ptr->is_buffer_allocated = FALSE;
    buf_cntxt_ptr->frame_count = 0 ;
} /* initialize_buffer_context */

/**
 * FUNCTION  : copy_frame_to_buffer
 * DESCRIPTION : copies a frame on to buffer.
 * Takes the offset info from header to find the location to which frame data should copy.
 *
 * DEPENDENCIES: None.
 *
 * PARAMS:
 *   buf_cntxt_ptr - pointer to the address of buffer context
 *   frame_ptr - pointer to the frame which needs to be copied
 *
 * RETURN VALUE: bool_t. TRUE indicates this frame is last frame in the buffer. FALSE indicates
 * this is NOT last frame.
 *
 * SIDE EFFECTS: None.
 */
bool_t copy_frame_to_buffer (
        buffer_context_struct *buf_cntxt_ptr,
        atp_frame_struct *frame_ptr
        )
{
    char_t *dest_loc_ptr = NULL;
    /**If buffer is not created yet, create and initialize */
    if ( buf_cntxt_ptr->is_buffer_allocated == FALSE )
    {
        buf_cntxt_ptr->buffer_ptr = ACDB_MALLOC(char_t, frame_ptr->header.buffer_length);
		//Klocwork fix: check input ptr if it is a NULL ptr before memset
        if (buf_cntxt_ptr->buffer_ptr == NULL)
        {
            return FALSE;
        }
        memset(buf_cntxt_ptr->buffer_ptr, 0, frame_ptr->header.buffer_length);
        buf_cntxt_ptr->is_buffer_allocated = TRUE ;
        /** assign buffer length*/
        buf_cntxt_ptr->buffer_length = frame_ptr->header.buffer_length;
    }

    /** Calculate destination loaction where to copy the frame*/
    dest_loc_ptr = buf_cntxt_ptr->buffer_ptr + frame_ptr->header.frame_offset;
    /** copy frame on to buffer */
	ATS_MEM_CPY_SAFE( dest_loc_ptr,(buf_cntxt_ptr->buffer_length-frame_ptr->header.frame_offset), frame_ptr->data_ptr, frame_ptr->header.frame_length );
    /** Note the frame count */
    if( buf_cntxt_ptr->frame_count < frame_ptr->header.frame_number )
        buf_cntxt_ptr->frame_count = frame_ptr->header.frame_number;

    /** return TRUE if this is last frame else FALSE*/
    if ( frame_ptr->header.flags & FLAG_LASTFRAME_MASK )
        return TRUE;
    else
        return FALSE;
} /* copy_frame_to_buffer */

/**
 * FUNCTION    : free_linked_list

 * DESCRIPTION : traverse the linked list and ACDB_FREE all (frame data,
 frame and linked list node )the memory.
 *
 * DEPENDENCIES: None.
 *
 * PARAMS:
 *   start_ptr - start node of the linked list
 *
 * RETURN VALUE: None
 *
 * SIDE EFFECTS: None.
 */
void free_linked_list (
        atp_frame_dblnk_lst_struct *start_ptr
        )
{
    /**declare and initalise all required pointer for traversing the linked list*/
    atp_frame_dblnk_lst_struct *current_node_ptr=NULL;
    atp_frame_dblnk_lst_struct *prev_node_ptr=NULL;
    atp_frame_struct *frame_ptr = NULL ;
    /** If there are no nodes just return*/
    if ( start_ptr == NULL )
        return;
    /**Initislise the current node to start node*/
    current_node_ptr = start_ptr;
    do
    {
        /** ACDB_FREE the atp frame*/
        frame_ptr = current_node_ptr->frame_ptr;
        free_atp_frame(frame_ptr);
        /**Save the current node as previous node to ACDB_FREE it later*/
        prev_node_ptr = current_node_ptr;
        /**Get the next node*/
        current_node_ptr = current_node_ptr->next_node_ptr;
        /**ACDB_FREE the previous node*/
        if ( prev_node_ptr != NULL )
            ACDB_FREE(prev_node_ptr);
    }
    while(current_node_ptr);
} /* free_linked_list */


#ifdef WINPC
/**
 * FUNCTION    : atp_create_diag_req_pkt
 *
 * DESCRIPTION : creates diag request packet and puts the given atp frame into payload
 *
 * DEPENDENCIES: None.
 *
 * PARAMS:
 *   atp_frame_ptr - pointer to atp frame which needs to be put in as pay load
 *   atp_diag_pkt - pointer to pointer of diag request pakect to be created
 *
 * RETURN VALUE: void
 *
 * SIDE EFFECTS: None.
 */
void atp_create_diag_req_pkt (
        atp_frame_struct* atp_frame_ptr,
        diag_pkt_req_struct** atp_diag_pkt
        )
{
    //atp frame header size
    uint8_t headersize = sizeof(atp_frame_header_struct);
    /**create a diag packet*/
    *atp_diag_pkt = (diag_pkt_req_struct* )malloc(sizeof(diag_pkt_req_struct));
    if(NULL==(*atp_diag_pkt))
    {
        return;
    }
    /**initialize the diag packet memory to zero*/
    memset(*atp_diag_pkt,0,sizeof(diag_pkt_req_struct));
    /**Set diag packet header*/
    /**Set diag command code*/
    (*atp_diag_pkt)->header.cmdCode   = DIAG_EXTEND_CMDCODE;
    /**set susbsystem ID as audio subsystem*/
    (*atp_diag_pkt)->header.subsysId  = AUDIO_SUBSYS_ID;
    /**set the sub system command code */
    (*atp_diag_pkt)->header.subsysCmdCode = SUBSYS_CMDCODE;

    //copy atp frame header to payload
	ATS_MEM_CPY_SAFE((*atp_diag_pkt)->payload_req,sizeof((*atp_diag_pkt)->payload_req),&atp_frame_ptr->header,headersize);

    //copy frame data to payload
	ATS_MEM_CPY_SAFE((*atp_diag_pkt)->payload_req+headersize,(sizeof((*atp_diag_pkt)->payload_req)-headersize),atp_frame_ptr->data_ptr,
            atp_frame_ptr->header.frame_length);
    return;
}

/**
 * FUNCTION    : atp_create_frame_from_diag_resp
 *
 * DESCRIPTION : extracts atp frame from diag resp packet
 *
 * DEPENDENCIES: None.
 *
 * PARAMS:
 *   atp_diag_pkt - pointer to diag response packet from which to extract atp fraem
 *   atp_frame_ptr - pointer to pinter where to extrat the atp frame
 *
 * RETURN VALUE: void
 *
 * SIDE EFFECTS: None.
 */
void atp_create_frame_from_diag_resp (
        diag_pkt_resp_struct* atp_diag_pkt,
        atp_frame_struct** atp_frame_ptr
        )
{
    uint8_t headersize = sizeof(atp_frame_header_struct);
    /**create atp frame*/
    *atp_frame_ptr = (atp_frame_struct* )malloc(sizeof(atp_frame_struct));
    if(NULL==*atp_frame_ptr)
    {
        return;
    }
    /**copy header to atp_frame*/
	ATS_MEM_CPY_SAFE(&(*atp_frame_ptr)->header,headersize,atp_diag_pkt->payload_resp,headersize);
    /**allocate required memory to hold data*/
    (*atp_frame_ptr)->data_ptr = (char_t *) malloc((*atp_frame_ptr)->header.frame_length);

    //Klocwork fix: check ptr if it is NULL
    if ((*atp_frame_ptr)->data_ptr == NULL)
    {
        ACDB_FREE((*atp_frame_ptr));
        return;
    }
    /**copy frame data*/
	ATS_MEM_CPY_SAFE((*atp_frame_ptr)->data_ptr,(*atp_frame_ptr)->header.frame_length, atp_diag_pkt->payload_resp+headersize,
            (*atp_frame_ptr)->header.frame_length);
    return;
}
#endif

/**
 * FUNCTION    : atp_create_frame_from_diag_req
 *
 * DESCRIPTION : creates atp frame from diag request packet
 *
 * DEPENDENCIES: None.
 *
 * PARAMS:
 *   atp_diag_pkt - pointer to diag request packet from which to extract atp fraem
 *   atp_frame_ptr - pointer to pinter where to extrat the atp frame
 *
 * RETURN VALUE: void
 *
 * SIDE EFFECTS: None.
 */
void atp_create_frame_from_diag_req (
        diag_pkt_req_struct* atp_diag_pkt,
        atp_frame_struct** atp_frame_ptr
        )
{
    uint8_t headersize = sizeof(atp_frame_header_struct);
    /**create atp frame*/
    *atp_frame_ptr = ACDB_MALLOC(atp_frame_struct, 1);
    if(NULL==(*atp_frame_ptr))
    {
        return;
    }
    /**copy header part to atp_frame*/
    ATS_MEM_CPY_SAFE(&(*atp_frame_ptr)->header,headersize,atp_diag_pkt->payload_req,headersize);
    (*atp_frame_ptr)->data_ptr = NULL ;
    if ( (*atp_frame_ptr)->header.frame_length > 0 )
    {
        /**allocate required memory to hold data*/
        (*atp_frame_ptr)->data_ptr = ACDB_MALLOC(char_t, (*atp_frame_ptr)->header.frame_length);
        if(NULL==((*atp_frame_ptr)->data_ptr))
        {
            return;
        }
        /**copy frame data*/
		ATS_MEM_CPY_SAFE((*atp_frame_ptr)->data_ptr,(*atp_frame_ptr)->header.frame_length, atp_diag_pkt->payload_req+headersize,
                (*atp_frame_ptr)->header.frame_length);
    }
    return;
}

/**
 * FUNCTION    : create_atp_frame
 *
 * DESCRIPTION : creates atp frame and initializes
 *
 * DEPENDENCIES: None.
 *
 * PARAMS:
 *   atp_frame_ptr - pointer of pointer of atp_frame_struct
 *
 * RETURN VALUE: void
 *
 * SIDE EFFECTS: None.
 */
void create_atp_frame (
        atp_frame_struct** atp_frame_ptr
        )
{
    /**create atp frame*/
    *atp_frame_ptr = ACDB_MALLOC(atp_frame_struct, 1);
    if(NULL==(*atp_frame_ptr))
    {
        return;
    }
    /**initialize its header*/
    init_atp_header(*atp_frame_ptr);
    /**Set data pointer to null*/
    (*atp_frame_ptr)->data_ptr = NULL;
    return;
}/*create_atp_frame*/

#ifdef WINPC

/**
 * FUNCTION    : atp_perform_handshake
 *
 * DESCRIPTION : Performs hand shake with RX.
 * Should be called each time before sending a buffer from PC.
 *
 * DEPENDENCIES: None.
 *
 * PARAMS:
 *   context_ptr - pointer to the atp session
 *
 * RETURN VALUE: void
 *
 * SIDE EFFECTS: None.
 */
bool_t atp_perform_handshake (
        atp_pc_context_struct *context_ptr
        )
{
    //Cariable to hold the diag request packet, which is wraper of hand shake frame
    diag_pkt_req_struct* atp_diag_req_pkt = NULL;
    //Received diag response in response to hand shake
    diag_pkt_resp_struct atp_diag_resp_pkt;
    //pointer to hand shake frame
    atp_frame_struct *atp_hsk_frame_ptr = NULL;
    //pointer to response frame for hand shake frame
    atp_frame_struct *atp_resp_frame_ptr=NULL;
    //flag to indicate if packet is sent successfully
    bool_t is_pkt_sent = TRUE;
    //flag to indicate is hand shake is success
    bool_t is_hsk_success = TRUE;
    /**Create a ATP frame*/
    create_atp_frame(&atp_hsk_frame_ptr);
    /**Set hand shake frame*/
    atp_hsk_frame_ptr->header.flags |= FLAG_HANDSHAKE_MASK;
    /**Create a diag request paket*/
    atp_create_diag_req_pkt(atp_hsk_frame_ptr, &atp_diag_req_pkt);
    /**ACDB_FREE the hand shake frame, as we have copied it to diag request packet*/
    free_atp_frame(atp_hsk_frame_ptr);
    if( atp_diag_req_pkt != NULL )
    {
        /**Send the diag request packet*/
        is_pkt_sent = context_ptr->send_atp_frame_ptr(atp_diag_req_pkt,&atp_diag_resp_pkt);
        ACDB_FREE(atp_diag_req_pkt);
    }

    if ( FALSE == is_pkt_sent )
    {
        /**if failed to send the packet, return error */
        return FALSE;
    }
    else
    {
        /**Extract atp frame from diag response packet*/
        atp_create_frame_from_diag_resp(&atp_diag_resp_pkt,&atp_resp_frame_ptr);
        /**if RX acknowleded the hand shake it send a frame GetNextFrame flag set*/
        if( (atp_resp_frame_ptr->header.flags & FLAG_GETNEXTFRAME_MASK) == 0 )
            is_hsk_success = FALSE;
        else
            is_hsk_success = TRUE;
        free_atp_frame(atp_resp_frame_ptr);
        return is_hsk_success;
    }
}

/**
 * FUNCTION : atp_send_buffer

 * DESCRIPTION :
 * Send a request buffer to target.
 * If buffer sent successfully, returns the response through the call back
 *function: receive_resp_buffer_ptr of context_ptr.
 *
 * DEPENDENCIES : send_atp_frame_ptr memeber in context_ptr must be pointing to
 *                           appropriate function.
 *
 * PARAMS:
 *   buffer_ptr - pointer to the input buffer
 *   buffer_length - bufer length
 *   atp_pc_context_ptr - pointer to ATP session(on PC side)
 *
 * RETURN VALUE : If sent successfully returns TRUE or FALSE

 * SIDE EFFECTS : Frees buffer_ptr and resets the pointer to NULL.
 */
bool_t atp_send_buffer (
        char_t* buffer_ptr,
        uint32_t buffer_length,
        atp_pc_context_struct *atp_pc_context_ptr
        )
{
    diag_pkt_req_struct* atp_current_diag_req_pkt_ptr = NULL;
    diag_pkt_resp_struct atp_diag_resp_pkt;
    bool_t is_pkt_sent = TRUE;
    bool_t is_last_frame_received = FALSE;
    atp_frame_struct *atp_resp_frame_ptr=NULL;
    atp_frame_struct *atp_gnf_frame_ptr=NULL;
    /**   Initialize response buffer context */
    initialize_buffer_context(&atp_pc_context_ptr->resp_buf_cntxt);
    /**
     *Break the request buffer into atp frames and create a linked list.
     *Get the start node of the created linked list.
     */
    break_buffer_into_frames(buffer_ptr, buffer_length,
            &atp_pc_context_ptr->start_node_ptr);
    /** Free the request buffer */
    if(buffer_ptr)
    {
        ACDB_FREE(buffer_ptr);
        buffer_ptr = NULL;
    }
    /** We are going to start sending the frames from linked list.
     *So assign the start node as current node
     */
    atp_pc_context_ptr->current_node_ptr = atp_pc_context_ptr->start_node_ptr;
    /** If there are no nodes in linked list just return FALSE (error)*/
    if ( atp_pc_context_ptr->current_node_ptr == NULL )
        return FALSE;
    /** Perform hand shake with RX to inform that we are going to start
      transmission
      of fresh request buffer*/
    if ( FALSE == atp_perform_handshake(atp_pc_context_ptr) )
        return FALSE;
    /** Wrap the first atp_frame into  a diag packet */
    atp_create_diag_req_pkt(atp_pc_context_ptr->current_node_ptr->frame_ptr,
            &atp_current_diag_req_pkt_ptr);
    /** Start traversing the linked list to send all atp_frames one by one*/
    while( 1 )
    {
        /** Send atp frame through the call back function of application layer*/
        /** If the packet is send successfully, we receive response atp frame
          wrapped in diag packet*/
        is_pkt_sent = atp_pc_context_ptr->send_atp_frame_ptr(atp_current_diag_req_pkt_ptr,
                &atp_diag_resp_pkt);
        /** ACDB_FREE the diag packet*/
        ACDB_FREE(atp_current_diag_req_pkt_ptr);
        /** Check the return code of send_atp_frame_ptr().
         *If failed to send the packet, return FALSE (error)
         */
        if ( FALSE == is_pkt_sent )
            return FALSE;
        /** No error? unwrap the diag packet to get response atp frame*/
        atp_create_frame_from_diag_resp(&atp_diag_resp_pkt,&atp_resp_frame_ptr);
        /** If the current frame is not the last frame... */
        if( atp_pc_context_ptr->current_node_ptr->next_node_ptr != NULL )
        {
            /** Expect a atp frame with GNF(Gent Next Frame) flag set*/
            if( !(atp_resp_frame_ptr->header.flags & FLAG_GETNEXTFRAME_MASK) )
            {
                /**.If we havn't received a GNF frame, its an error return FALSE(erro)*/
                free_atp_frame(atp_resp_frame_ptr);
                return FALSE;
            }
            /** If we received a frame with GNF flag set, send next frame and
             *increment the current node pointer to next node in the lined list
             */
            atp_pc_context_ptr->current_node_ptr =
                atp_pc_context_ptr->current_node_ptr->next_node_ptr;
            /** wrap the next atp frame into diag packet*/
            atp_create_diag_req_pkt(atp_pc_context_ptr->current_node_ptr->frame_ptr,
                    &atp_current_diag_req_pkt_ptr);
        }
        else
        {
            /** If we have sent all frames from request buffer, expect a frame from
              response buffer. i.e a frame with Data flag set.*/
            if( !(atp_resp_frame_ptr->header.flags & FLAG_DATAFRAME_MASK) )
            {
                /** If we hav not received a Data frame, return FALSE (error) */
                free_atp_frame(atp_resp_frame_ptr);
                return FALSE;
            }
            /** If yes, copy frame to response buffer */
            is_last_frame_received = copy_frame_to_buffer(
                    &atp_pc_context_ptr->resp_buf_cntxt,
                    atp_resp_frame_ptr);
            free_atp_frame(atp_resp_frame_ptr);
            /** If we received a data frame.with LAST flag set, give the response
              buffer to application layer*/
            if( is_last_frame_received == TRUE )
            {
                atp_pc_context_ptr->receive_resp_buffer_ptr(
                        atp_pc_context_ptr->resp_buf_cntxt.buffer_ptr,
                        atp_pc_context_ptr->resp_buf_cntxt.buffer_length);
                /** clear the request buffer and reinitialize the response buffer context*/
                ACDB_FREE(atp_pc_context_ptr->resp_buf_cntxt.buffer_ptr);
                initialize_buffer_context(&atp_pc_context_ptr->resp_buf_cntxt);
                return TRUE;
            }
            else
            {
                /** Else  request next frame from RX */
                /** create GNF frame */
                create_atp_frame(&atp_gnf_frame_ptr);
                /** Set GNF flag*/
                atp_gnf_frame_ptr->header.flags |= FLAG_GETNEXTFRAME_MASK;
                /** Prepare the diag packet with the next frame we need to send*/
                atp_create_diag_req_pkt(atp_gnf_frame_ptr,
                        &atp_current_diag_req_pkt_ptr);
                /** ACDB_FREE the atp frame*/
                free_atp_frame( atp_gnf_frame_ptr );
            }
        }
    }/*while*/
    return FALSE;
}/*atp_send_buffer*/

#else
/**
 * FUNCTION : atp_receive_atp_frame

 * DESCRIPTION :
 *This is the main part of RX algorithm
 * Receives an atp frame and copies on to buffer.
 *If all frame are received calls the callback function of the application layer
 *to give the request buffer and to recive the response buffer. Then sends the
 *response buffer to TX.
 *
 * DEPENDENCIES : receive_req_buffer_ptr memeber in context_ptr must be pointing to
 * appropriate function, which can process the request buffer and can provide response buffer
 *
 * PARAMS:
 *   context_ptr - pointer to the atp session
 *   atp_frame_ptr - pointer to the atp frame. which needs to be processed in the give atp context
 *
 * RETURN VALUE : If sent successfully returns TRUE or FALSE

 * SIDE EFFECTS : None
 */
bool_t atp_receive_atp_frame (
        atp_phone_context_struct *context_ptr,
        atp_frame_struct *atp_frame_ptr
        )
{
    /*flag to indicate whether the received frame is last frame in the request
      buffer*/
    bool_t is_last_frame = FALSE;
    /*pointer to the response buffer*/
    char_t* resp_buf_ptr = NULL ;
    /*variable to hold response buffer length*/
    uint32_t resp_buf_length=0;
    /*pointer to the response frame created inresponse to the atp_frame-ptr*/
    atp_frame_struct *atp_resp_frame_ptr=NULL;
    uint8_t atp_frame_header_length = sizeof(atp_frame_header_struct);
    /**We received a handshake frame, so drop all the frames from previous session
     *and get ready for new session.
     */
    if( (atp_frame_ptr->header.flags & FLAG_HANDSHAKE_MASK) != 0 )
    {
        ATS_ERR("ATS FRAME: handshake");
        /** we are expecting a frame with GNF flag */
        if ( context_ptr->is_next_frame_gnf == TRUE )
        {
            ATS_ERR("ATS FRAME: handshake GNF = TRUE");
            context_ptr->is_next_frame_gnf = FALSE;
            /**we have some response frames left in previous session ACDB_FREE them*/
            free_linked_list(context_ptr->resp_start_node_ptr);
        }
        else
        {
            ATS_ERR("ATS FRAME: recieving frames from previous sessions req buffer");
            /**We were receiving frames from previous session request buffer, so ACDB_FREE
             *the request buffer*/
            if ( context_ptr->req_buf_cntxt.buffer_ptr != NULL )
                ACDB_FREE(context_ptr->req_buf_cntxt.buffer_ptr);
            /**initialise the response buffer context*/
            initialize_buffer_context(&context_ptr->req_buf_cntxt);
        }/*hand shake if*/
        /** inform TX that we are ready to receive new request buffer */
        /**create GNF frame and it is our response frame for hand shake frame*/
        create_atp_frame(&context_ptr->atp_current_resp_frame_ptr);
        //Klocwork fix: check input if it is a NULL ptr
        if (context_ptr->atp_current_resp_frame_ptr == NULL)
        {
            ATS_ERR("ATS FRAME: current resp frame ptr is null");
           return FALSE;
        }

        ATS_ERR("ATS FRAME: setting GNF mask");
        /**Set GNF flag*/
        context_ptr->atp_current_resp_frame_ptr->header.flags |=FLAG_GETNEXTFRAME_MASK ;
        return TRUE;
    }
    /**If we have not received a hand shake frame.
     * we have left some frames in response buffer so we are expecting a GNF frame
     */
    if ( context_ptr->is_next_frame_gnf == TRUE )
    {
        ATS_ERR("ATS FRAME: in GNF condition");
        /**If  we have not received a GNF frame so its an error  because still we have
         *some frame is response buffer*/
        if( (atp_frame_ptr->header.flags & FLAG_GETNEXTFRAME_MASK) == 0 )
        {
            ATS_ERR("ATS FRAME: error condition if we are expecting a GNF but have frames in response buffer");
            return FALSE;
        }
        /** move to next node pointer to send
         *If we don't have frames means some unexpected error*/
        if( context_ptr->resp_current_node_ptr == NULL )
        {
            ATS_ERR("ATS FRAME: resp curr node ptr is null BEFORE setting to next node ptr");
            return FALSE;
        }
        context_ptr->resp_current_node_ptr = context_ptr->resp_current_node_ptr->next_node_ptr;

        if (context_ptr->resp_current_node_ptr == NULL)
        {
            ATS_ERR("ATS FRAME: resp curr node ptr is null AFTER setting to next node ptr");
            return FALSE;
        }
        /** if this is the last frame in response, note that we are not expecting a GNF
         *frame
         */
        if ( context_ptr->resp_current_node_ptr->next_node_ptr == NULL )
        {
            ATS_ERR("ATS FRAME: last frame! not expecting a GNF");
            context_ptr->is_next_frame_gnf = FALSE;
        }
        else
        {
            /**current node is not the last node in response buffer linked list so
             *next frame should be GNF frame
             */
            context_ptr->is_next_frame_gnf = TRUE;
            ATS_ERR("ATS FRAME: still more frames! expecting a GNF");
        }

        ATS_ERR("ATS FRAME: creating fresponse frame");
        /**create response frame*/
        create_atp_frame(&atp_resp_frame_ptr);
        //Klocwork fix: check input if it is NULL ptr before perform memcpy
        if (atp_resp_frame_ptr == NULL || context_ptr->resp_current_node_ptr == NULL)
        {
            if(atp_resp_frame_ptr == NULL)
                ATS_ERR("ATS FRAME: atp_resp_frame_ptr is null. returning false");
            if (context_ptr->resp_current_node_ptr == NULL)
                ATS_ERR("ATS FRAME: context_ptr->resp_current_node_ptr is null. returning false");
            return FALSE;
        }

        ATS_ERR("ATS FRAME: copy header info into response frame");
        /**copy header info to response frame*/
		ATS_MEM_CPY_SAFE(atp_resp_frame_ptr,atp_frame_header_length,
                &context_ptr->resp_current_node_ptr->frame_ptr->header,
                atp_frame_header_length);

        ATS_ERR("ATS FRAME: create mem for data");

        /**create memory for data */
        atp_resp_frame_ptr->data_ptr =
            ACDB_MALLOC(char_t, atp_resp_frame_ptr->header.frame_length);


        /**copy data to response frame*/
        if (atp_resp_frame_ptr->data_ptr == NULL)
        {
            ATS_ERR("ATS FRAME: unable to allocate memory for ats response frame data ptr");

            ACDB_FREE(atp_resp_frame_ptr);
            return FALSE;
        }

        ATS_ERR("ATS FRAME: copy data into response frame");
		ATS_MEM_CPY_SAFE(atp_resp_frame_ptr->data_ptr,atp_resp_frame_ptr->header.frame_length,
                context_ptr->resp_current_node_ptr->frame_ptr->data_ptr,
                atp_resp_frame_ptr->header.frame_length
              );

        ATS_ERR("ATS FRAME: set resp frame ptr to current node");

        /** set response frame pointer as current node frame pointer*/
        context_ptr->atp_current_resp_frame_ptr= atp_resp_frame_ptr;

        if ( context_ptr->resp_current_node_ptr->next_node_ptr == NULL )
        {
            //Since this is the last frame we can ACDB_FREE the response buffer linked list
            free_linked_list(context_ptr->resp_start_node_ptr);
        }
        return TRUE;
    }
    else
    {
        ATS_ERR("ATS FRAME: data frame");

        /**we are expecting a data frame
         *we have not received a data frame so its an error*/
        if ((atp_frame_ptr->header.flags & FLAG_DATAFRAME_MASK) == 0)
        {
            ATS_ERR("ATS FRAME: not a data frame! return false");

            return FALSE;
        }
        ATS_ERR("ATS FRAME: copy frame to our buffer");

        /**copy frame to our buffer*/
        is_last_frame = copy_frame_to_buffer(&context_ptr->req_buf_cntxt,atp_frame_ptr);

        if ( is_last_frame )
        {
            ATS_ERR("ATS FRAME: this is the last frame");

            /**we received last frame*/
            /**send request buffer to ACM*/
            if ( context_ptr->receive_req_buffer_ptr == NULL )
            //CR: coding format fix
            {
                ATS_ERR("ATS FRAME: context_ptr->receive_req_buffer_ptr is null");

                return FALSE;
            }
            context_ptr->receive_req_buffer_ptr((uint8_t*)context_ptr->req_buf_cntxt.buffer_ptr,
                    context_ptr->req_buf_cntxt.buffer_length,
                    (uint8_t**)&resp_buf_ptr,&resp_buf_length);

            /**ACDB_FREE the request buffer*/
            ACDB_FREE(context_ptr->req_buf_cntxt.buffer_ptr);
			if(resp_buf_ptr == NULL)
			{
                ATS_ERR("ATS FRAME: resp_buf_ptr is null");

				return FALSE;
			}

            ATS_ERR("ATS FRAME: create response buffer START");

            initialize_buffer_context(&context_ptr->req_buf_cntxt);
            /**break the response buffer into frames*/
            break_buffer_into_frames(resp_buf_ptr,resp_buf_length,
                    &context_ptr->resp_start_node_ptr);
            /**ACDB_FREE the response buffer*/
            //ACDB_FREE(resp_buf_ptr);
            //Klocwork fix: check if src ptr is a NULL ptr
            if (context_ptr->resp_start_node_ptr == NULL)
            {
               ATS_ERR("context_ptr->resp_start_node_ptr is null");
               return FALSE;
            }
            /**set current node pointer to start pointer*/
            context_ptr->resp_current_node_ptr = context_ptr->resp_start_node_ptr;
            if ( context_ptr->resp_current_node_ptr->next_node_ptr == NULL )
            {
                context_ptr->is_next_frame_gnf = FALSE;
            }
            else
            {
                context_ptr->is_next_frame_gnf = TRUE;
            }
            create_atp_frame(&atp_resp_frame_ptr);
            //Klocwork fix: check if input it is a NULL ptr
            if (atp_resp_frame_ptr == NULL)
            {
                ATS_ERR("unable to create atp response frame");
                return FALSE;
            }
            /**copy header info to response frame*/
			ATS_MEM_CPY_SAFE(atp_resp_frame_ptr,atp_frame_header_length,
                    &context_ptr->resp_current_node_ptr->frame_ptr->header,
                    atp_frame_header_length);
            /**create memory for data */
            atp_resp_frame_ptr->data_ptr =
                ACDB_MALLOC(char_t, atp_resp_frame_ptr->header.frame_length);
            /**copy data to response frame*/
            //Klocwork fix: check input if it is a NULL ptr before perform memcpy
            if (atp_resp_frame_ptr->data_ptr == NULL)
            {
                ATS_ERR("unable to allocate mem for atp_resp_frame_ptr->data_ptr");
                ACDB_FREE(atp_resp_frame_ptr);
                return FALSE;
            }
			ATS_MEM_CPY_SAFE(atp_resp_frame_ptr->data_ptr,atp_resp_frame_ptr->header.frame_length,
                    context_ptr->resp_current_node_ptr->frame_ptr->data_ptr,
                    atp_resp_frame_ptr->header.frame_length
                  );
            /** set response frame pointer as current node frame pointer*/
            context_ptr->atp_current_resp_frame_ptr= atp_resp_frame_ptr;
            if ( context_ptr->resp_current_node_ptr->next_node_ptr == NULL )
            {
                //Since this is the last frame we can ACDB_FREE the response buffer linked list
                free_linked_list(context_ptr->resp_start_node_ptr);
            }

            ATS_ERR("ATS FRAME: create response buffer END");
            return TRUE;
        }
        else
        {
            ATS_ERR("ATS FRAME: not the last data frame! requeset next frame ");
            /**this is not last frame so request next frame*/
            /**create GNF frame and it is our response frame*/
            create_atp_frame(&context_ptr->atp_current_resp_frame_ptr);
            //Klocwork fix: if input is a NULL ptr
            if (context_ptr->atp_current_resp_frame_ptr == NULL)
            {
                return FALSE;
            }
            context_ptr->atp_current_resp_frame_ptr->header.flags |=FLAG_GETNEXTFRAME_MASK ;
            return TRUE;
        }
    }/*else if( context_ptr->is_next_frame_gnf == TRUE )*/
    /*return FALSE; This is not reachable*/
}/*atp_receive_atp_frame*/
#endif

/**
 * FUNCTION : atp_create_diag_resp_pkt
 *
 * DESCRIPTION : creates a diag resp packet from atp frame
 *
 * DEPENDENCIES: None.
 *
 * PARAMS:
 *   atp_frame_ptr - frame to be wrapped in response diag packet
 *   atp_diag_pkt - created diag response packet
 *
 * RETURN VALUE: None
 *
 * SIDE EFFECTS: None.
 */
void atp_create_diag_resp_pkt (
        atp_frame_struct* atp_frame_ptr,
        diag_pkt_resp_struct** atp_diag_pkt
        )
{
    ATS_ERR("DIAG PKT: Creating diag response packet");
    print_atp_frame(atp_frame_ptr);
    uint8_t headersize = sizeof(atp_frame_header_struct);
    /** Create diag response packet*/
    (*atp_diag_pkt) = ACDB_MALLOC(diag_pkt_resp_struct, 1);
    if(NULL==(*atp_diag_pkt))
    {
        return;
    }
    /**Initialise the memeory*/
    memset(*atp_diag_pkt,0,sizeof(diag_pkt_resp_struct));
    /**set diag response packet header info*/
    (*atp_diag_pkt)->header.cmdCode   = DIAG_EXTEND_CMDCODE;
    (*atp_diag_pkt)->header.subsysId  = AUDIO_SUBSYS_ID;
    (*atp_diag_pkt)->header.subsysCmdCode = SUBSYS_CMDCODE;

    /** copy atp header info*/
	ATS_MEM_CPY_SAFE(&(*atp_diag_pkt)->payload_resp[0], headersize, &atp_frame_ptr->header, headersize);

    /**copy frame data*/
    if (atp_frame_ptr->data_ptr != NULL)
    {
	    ATS_MEM_CPY_SAFE(
            &(*atp_diag_pkt)->payload_resp[headersize], atp_frame_ptr->header.frame_length,
            atp_frame_ptr->data_ptr, atp_frame_ptr->header.frame_length);
    }
    return;
}

#ifndef WINPC



/**
 * FUNCTION : atp_receive_diag_pkt
 *
 * DESCRIPTION :
 * Receive a request diag packet from acm diag dispatcher (avsACMdiag_cmd())
 *.and return a response diag packet.
 *
 * DEPENDENCIES : receive_req_buffer_ptr memeber in atp_phone_context_ptr must be
 *pointing to.appropriate function, which can process the request buffer and
 *provide a response buffer.
 *
 * PARAMS:
 *   atp_diag_req_pkt_ptr - pointer to the diag packet which contains the atp frame from request buffer
 *   atp_diag_rsp_pkt_ptr - pointer to the diag packet which contains the atp frame from response buffer
 *   atp_phone_context_ptr - pointer to the phone context to whome request to be sent and resposne received
 *
 * RETURN VALUE : If sent successfully returns TRUE or FALSE
 *
 * SIDE EFFECTS : Will NOT Free response buffer.
 */
void atp_receive_diag_pkt (
        diag_pkt_req_struct *atp_diag_req_pkt_ptr,
        diag_pkt_resp_struct **atp_diag_rsp_pkt_ptr,
        atp_phone_context_struct *atp_phone_context_ptr
        )
{
	//Klocwork fix: //A temporary NULL ptr to hold atp frame extracted from request diag packet
	atp_frame_struct* atp_frame_ptr = NULL;
    /** Create a atp frame from diag request packet */
    atp_create_frame_from_diag_req(atp_diag_req_pkt_ptr, &atp_frame_ptr);

    print_atp_frame(atp_frame_ptr);

    /** Send extracted atp frame to RX algorithm, which accumulates all the
      request frames to construct a request buffer.*/
    //Klocwork fix: Null pointer check before calling atp_receive_atp_frame command
    if (atp_frame_ptr != NULL)
    {
        (void)atp_receive_atp_frame(atp_phone_context_ptr, atp_frame_ptr);
    }
    /** ACDB_FREE the extracted atp frame*/
    //Klocwork fix: Null pointer check before calling atp_receive_atp_frame command
     if (atp_frame_ptr != NULL)
    {
        free_atp_frame(atp_frame_ptr);
    }
    /** if response atp frame is ready...*/
    if ( atp_phone_context_ptr->atp_current_resp_frame_ptr != NULL )
    {
        /** create a diag packet which wraps the response frame*/
        atp_create_diag_resp_pkt(
                atp_phone_context_ptr->atp_current_resp_frame_ptr,
                atp_diag_rsp_pkt_ptr
                );
        /**ACDB_FREE the response frame*/
        free_atp_frame(atp_phone_context_ptr->atp_current_resp_frame_ptr);
    }
    else
    {
        /**else return NULL as response diag packet*/
        *atp_diag_rsp_pkt_ptr = NULL;
    }
}
#endif

//#endif /*FEATURE_AVS_AUDIO_TRANSPORT_PROTOCOL || WINPC*/
