/**
  *****************************************************************************
  * @author  Robert Fairley
  * @brief   Abstraction over UDP library providing common driver functions.
  *
  * @defgroup Header
  * @ingroup  lwip
  * @{
  *****************************************************************************
  */




#ifndef UDP_DRIVER_H
#define UDP_DRIVER_H




/********************************* Includes **********************************/
#include "UdpRawInterface.h"
#include "OsInterface.h"

using cmsis::OsInterface;
using lwip::UdpRawInterface;


/************************** udp_driver **************************/
namespace udp {

// Constants
// ----------------------------------------------------------------------------
constexpr TickType_t SEMAPHORE_WAIT_NUM_MS = 10;
constexpr TickType_t SEMAPHORE_WAIT_NUM_TICKS = pdMS_TO_TICKS(SEMAPHORE_WAIT_NUM_MS);

// Classes and structs
// ----------------------------------------------------------------------------
class UdpDriver {
public:
    UdpDriver();
    UdpDriver(const ip_addr_t ipaddrIn,
              const ip_addr_t ipaddrPcIn,
              const u16_t portIn,
              const u16_t portPcIn,
              const UdpRawInterface *udpInterfaceIn,
              const OsInterface *osInterfaceIn);
    ~UdpDriver();

    /* User-facing - typically call directly. */
    bool initialize();
    bool setupReceive(udp_recv_fn recvCallback);
    bool receive(uint8_t *rxArrayOut, const size_t numBytes);
    bool transmit(const uint8_t *txArrayIn, const size_t numBytes);

    /* Utility - public but typically no need to call directly. */
    bool bytesToPacket(const uint8_t *byteArrayIn, const size_t numBytes, struct pbuf *pPbuf) const;
    u16_t packetToBytes(uint8_t *byteArrayOut, const size_t numBytes, struct pbuf *pPbuf) const;
    void signalReceiveCplt();
    void waitReceiveCplt();

    /* Accessors - public but typically no need to call directly. */
    void setRecvPbuf(struct pbuf *pPbuf);

    const ip_addr_t                     getIpaddr() const;
    const ip_addr_t                     getIpaddrPc() const;
    const u16_t                         getPort() const;
    const u16_t                         getPortPc() const;
    const UdpRawInterface*  			getUdpInterface() const;
    const OsInterface*             		getOsInterface() const;
    struct udp_pcb*                     getPcb() const;
    struct pbuf*                        getRecvPbuf() const;

    void forgetPcb();
    void forgetRecvPbuf();

private:
    /* UdpDriver configuration. */
    const ip_addr_t ipaddr      = {0x0};
    const ip_addr_t ipaddrPc    = {0x0};
    const u16_t port            = 0;
    const u16_t portPc          = 0;

    /* External interfaces. */
    const UdpRawInterface *udpInterface     = nullptr;
    const OsInterface *osInterface                  = nullptr;

    /* Data modified internally by the Raw API. */
    /* TODO: decide whether NULL or nullptr, since lwIP API will use NULL when setting these. */
    struct udp_pcb *pcb     = nullptr;
    struct pbuf *recvPbuf   = nullptr;

    /* Synchronization. */
    mutable osSemaphoreId recvSemaphore; /* TODO: replace with binary semaphore-style task notification. */
    mutable osStaticSemaphoreDef_t recvSemaphoreControlBlock;
    mutable osMutexId recvPbufMutex;
    mutable osStaticMutexDef_t recvPbufMutexControlBlock;
};

} // end namespace udp_driver

/**
 * @}
 */
/* end - Header */

#endif /* UDP_DRIVER_H */
