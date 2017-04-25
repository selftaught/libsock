#ifndef __protocol_arp
#define __protocol_arp

/**
 *
 * ARP - Address Resolution Protocol
 *
 * 1. Device 'A' checks its ARP cache for a MAC address
 *    that corresponds to an IP address.
 *
 * 2. Device 'A' will send a broadcast packet out asking
 *    all clients on a LAN for their MAC address if their
 *    IP address matches.
 *
 * 3. Matching device 'B' will create a packet with it's IP
 *    address, MAC address that is being looked for, and then
 *    send that to the device 'A' (unicast).
 *
 * 4. Device 'A' will receive the packet sent back by device 'B'
 *    and then update its ARP cache with device 'B's MAC address
 *    which is included in the packet.
 *
 */

#include "protocols.hpp"
#include "../sockit.hpp"
#include "../leafs/macaddr.hpp"

namespace libsockit::protocols::arp {

};

#endif
