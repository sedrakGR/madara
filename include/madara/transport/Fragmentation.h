#ifndef  _MADARA_FRAGMENTATION_HEADER_H_
#define  _MADARA_FRAGMENTATION_HEADER_H_

/**
 * @file Fragmentation.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the message header used by transports to piece
 * together fragments of a large knowledge update and helpers classes
 * for transports that want to handle fragmented updates
 **/

#include <map>
#include <string>
#include <string.h>
#include "madara/utility/stdint.h"
#include "madara/MADARA_export.h"
#include "madara/transport/MessageHeader.h"

namespace madara
{
  namespace transport
  {
    #define FRAGMENTATION_MADARA_ID    "KFRG1.3"
    
    /**
    * @class FragmentMessageHeader
    * @brief Defines a fragmentation header which allows for multi-part
    *        messages that are only applied once all fragments are received.
    *        
    *        Format:
    *
    *        [0] [64 bit unsigned size]<br />
    *        [8] [8 byte transport id] (prefixed with 'KFRG')<br />
    *        [16] [32 byte domain name]<br />
    *        [48] [64 byte originator (generally host:port)]<br />
    *        [112] [32 bit unsigned type]<br />
    *           2 = MULTIASSIGN (most common type)<br />
    *        [116] [32 bit unsigned num updates]<br />
    *        [120] [32 bit unsigned quality (type of priority)]<br />
    *        [124] [64 bit unsigned Lamport clock]<br />
    *        [132] [64 bit unsigned wall clock timestamp]<br />
    *        [140] [8 bit unsigned ttl--for rebroadcasts]<br />
    *        [141] [32 bit update number out of num updates]<br />
    *        [145] [knowledge updates start here in the buffer]
    */
    
    class MADARA_Export FragmentMessageHeader : public MessageHeader
    {
    public:

      /**
       * Constructor
       **/
      FragmentMessageHeader ();

      /**
       * Destructor
       **/
      virtual ~FragmentMessageHeader ();

      /**
      * Returns the size of the encoded MessageHeader class, which may be
      * different from sizeof (MessageHeader) because of compiler
      * optimizations for word boundaries
      **/
      virtual uint32_t encoded_size (void) const;
      
      /**
      * Returns the size of the encoded MessageHeader class, which may be
      * different from sizeof (MessageHeader) because of compiler
      * optimizations for word boundaries
      **/
      static uint32_t static_encoded_size (void);
      
      /**
      * Returns the number of updates indicated in the header
      * @return  updates of the encoded buffer
      **/
      static uint32_t get_updates (const char * buffer);

      /**
      * Reads a MessageHeader instance from a buffer and updates
      * the amount of buffer room remaining.
      * @param     buffer     the readable buffer where data is stored
      * @param     buffer_remaining  the count of bytes remaining in the
      *                              buffer to read
      * @return    current buffer position for next read
      **/
      virtual const char * read (const char * buffer,
        int64_t & buffer_remaining);

      /**
       * Assignment operator for regular message header
       **/
      void operator= (MessageHeader & header);

      /**
      * Writes a MessageHeader instance to a buffer and updates
      * the amount of buffer room remaining.
      * @param     buffer     the readable buffer where data is stored
      * @param     buffer_remaining  the count of bytes remaining in the
      *                              buffer to read
      * @return    current buffer position for next write
      **/
      virtual char * write (char * buffer, int64_t & buffer_remaining);

      /**
      * Compares the fields of this instance to another instance
      * @param     other      the other instance to compare against
      * @return    true if equal, false otherwise
      **/
      virtual bool equals (const MessageHeader & other);

      /**
       * Tests the buffer for a reduced message identifier
       * @return   true if identifier indicates reduced message header
       **/
      static inline bool fragment_message_header_test (const char * buffer)
      {
        return strncmp (&(buffer[8]), FRAGMENTATION_MADARA_ID, 7) == 0; 
      }

      uint32_t   update_number;
    };
    
    /**
     * Map of fragment identifiers to fragments
     **/
    typedef  std::map <uint32_t, const char *> FragmentMap;

    /**
     * Map of clocks to fragments
     **/
    typedef  std::map <uint64_t, FragmentMap> ClockFragmentMap;

    /**
     * Map of originator to a map of clocks to fragments
     **/
    typedef  std::map <
      std::string, ClockFragmentMap> OriginatorFragmentMap;

    /**
     * Adds a fragment to an originator fragment map and returns
     * the aggregate message if the message is complete.
     * @param originator   the originator of the message
     * @param clock        the clock of the message
     * @param update_number fragment identifier within clock message
     * @param fragment     the fragment to add
     * @param queue_length number of clock entries allowed per originator
     * @param map          a map of existing message fragments
     * @param clear        if true, clears a fragment entry if message
     *                     is complete
     * @return   unmanaged buffer that contains completed message.
     *           If not zero, you must clean this up with delete []
     **/
    MADARA_Export char * add_fragment (const char * originator, uint64_t clock,
      uint32_t update_number, const char * fragment,
      uint32_t queue_length,
      OriginatorFragmentMap & map, bool clear = true);

    /**
     * Pieces together a fragment map into a single buffer
     * @param  map     map containing fragments
     * @return   unmanaged buffer that contains completed message.
     *           If not zero, you must clean this up with delete []
     **/
    MADARA_Export char * defrag (FragmentMap & map);
    
    /**
     * Deletes fragments within a fragment map and clears the map
     * @param  map     map containing fragments
     **/
    MADARA_Export void delete_fragments (FragmentMap & map);

    /**
     * Breaks a large packet into smaller packets
     * @param  source   large packet that needs to be fragmented
     * @param  fragment_size  maximum fragment size
     * @param  map      map containing resulting fragments
     **/
    MADARA_Export void frag (
      const char * source, uint32_t fragment_size, FragmentMap & map);
    
    /**
     * Breaks a large packet into smaller packets
     * @param   originator   the originator of the message
     * @param   clock        the clock of the message
     * @param   map          a map of existing message fragments
     * @return  true if the clock entry has all fragments received
     **/
    MADARA_Export bool is_complete (const char * originator, uint64_t clock,
      OriginatorFragmentMap & map);

    /**
     * Checks if a fragment already exists within a fragment map
     * @param originator   the originator of the message
     * @param clock        the clock of the message
     * @param update_number fragment identifier within clock message
     * @param map          a map of existing message fragments
     * @return   true if 
     **/
    MADARA_Export bool exists (const char * originator, uint64_t clock,
      uint32_t update_number, OriginatorFragmentMap & map);
  }
}

#endif   // _MADARA_FRAGMENTATION_HEADER_H_
