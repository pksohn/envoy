#include "source/common/quic/quic_stats_gatherer.h"
#include "source/server/backtrace.h"

namespace Envoy {
namespace Quic {

QuicStatsGatherer::~QuicStatsGatherer() {}

void QuicStatsGatherer::OnPacketAcked(int acked_bytes, quic::QuicTime::Delta ack_delay_time) {
  // Check if all bytes sent in this request have been acked.
  // These below don't quite work because they include the frame header bytes (so far in integration
  // tests, 2 bytes) where the acks only include the frame payload bytes.
  uint64_t bytesSent = bytes_meter_->wireBytesSent();
  uint64_t headerBytesSent = bytes_meter_->headerBytesSent();

  std::cout << "paulsohn OnPacketAcked with acked_bytes: " << std::to_string(acked_bytes)
            << ", ack_delay_time: " << std::to_string(ack_delay_time.ToSeconds())
            << ", bytes sent: " << std::to_string(bytesSent)
            << ", header bytes sent: " << std::to_string(headerBytesSent) << "\n";

  // TODO: Check if fin bit was sent.
  // Can use the stream's write_side_closed state (?)

  // TODO: record timestamp when ack was received
  // - get "now".
  // - subtract ack_delay_time to get "final ack time".
  // - add "final ack time" to streaminfo.

  // TODO: do the deferred logging.
  // capture all the things from the filter manager log() call:
  // - stream info
  // - request, response, trailer maps
  // - loggers
  // and do the logging after the new timing is added to stream info.

  // BACKTRACE_LOG();
}

void QuicStatsGatherer::OnPacketRetransmitted(int retransmitted_bytes) {
  // do we need to implement?
  // suppress the "retransmitted_bytes is unused" build error?
  std::cout << "paulsohn OnPacketRetransmitted: " << std::to_string(retransmitted_bytes) << "\n";
}

} // namespace Quic
} // namespace Envoy