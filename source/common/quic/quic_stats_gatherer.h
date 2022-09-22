#pragma once

#include "quiche/quic/core/quic_ack_listener_interface.h"
#include "envoy/access_log/access_log.h"
#include "envoy/http/header_map.h"
#include "source/server/backtrace.h"

using Envoy::Http::RequestHeaderMap;
using Envoy::Http::ResponseHeaderMap;
using Envoy::Http::ResponseTrailerMap;

namespace Envoy {
namespace Quic {

class QuicStatsGatherer : public quic::QuicAckListenerInterface {

public:
  // Some things we might need later.
  // GetAllTheStuffForLogging(RequestHeaderMap request_headers, ResponseHeaderMap response_headers,
  //                   ResponseTrailerMap response_trailers, StreamInfo stream_info,
  //                   std::list<AccessLog::InstanceSharedPtr> access_log_handlers);

  QuicStatsGatherer(const StreamInfo::BytesMeterSharedPtr& bytes_meter)
      : bytes_meter_(bytes_meter) {
    std::cout << "paulsohn QuicStatsGatherer ctor\n";
    // BACKTRACE_LOG();
  }
  void OnPacketAcked(int acked_bytes, quic::QuicTime::Delta delta_largest_observed) override;
  void OnPacketRetransmitted(int retransmitted_bytes) override;

private:
  const StreamInfo::BytesMeterSharedPtr& bytes_meter_;

protected:
  ~QuicStatsGatherer() override;
};

} // namespace Quic
} // namespace Envoy