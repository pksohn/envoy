#include "source/common/quic/quic_stats_gatherer.h"

#include <cstdint>

namespace Envoy {
namespace Quic {

void QuicStatsGatherer::OnPacketAcked(int acked_bytes,
                                      quic::QuicTime::Delta /* delta_largest_observed */) {
  bytes_outstanding_ -= acked_bytes;
  if (bytes_outstanding_ == 0 && fin_sent_ && !logging_done_) {
    maybeDoDeferredLog();
  }
}

void QuicStatsGatherer::maybeDoDeferredLog(bool record_ack_timing) {
  logging_done_ = true;
  if (!deferred_logging_headers_and_trailers_.has_value()) {
    return;
  }
  Http::DeferredLoggingHeadersAndTrailers& headers_and_trailers =
      deferred_logging_headers_and_trailers_.value();
  if (headers_and_trailers.stream_info == nullptr) {
    return;
  }
  if (time_source_ != nullptr && record_ack_timing) {
    headers_and_trailers.stream_info->downstreamTiming().onLastDownstreamAckReceived(*time_source_);
  }
  auto request_headers = headers_and_trailers.request_header_map.get();
  auto response_headers = headers_and_trailers.response_header_map.get();
  auto response_trailers = headers_and_trailers.response_trailer_map.get();
  for (const auto& log_handler : access_log_handlers_) {
    log_handler->log(request_headers, response_headers, response_trailers,
                     *headers_and_trailers.stream_info);
  }
}

} // namespace Quic
} // namespace Envoy
