/*
 * Copyright (C) 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRC_TRACE_PROCESSOR_IMPORTERS_PROTO_NETWORK_TRACE_MODULE_H_
#define SRC_TRACE_PROCESSOR_IMPORTERS_PROTO_NETWORK_TRACE_MODULE_H_

#include <cstdint>

#include "perfetto/protozero/scattered_heap_buffer.h"
#include "protos/perfetto/trace/trace_packet.pbzero.h"
#include "src/trace_processor/importers/common/parser_types.h"
#include "src/trace_processor/importers/proto/proto_importer_module.h"
#include "src/trace_processor/storage/trace_storage.h"
#include "src/trace_processor/types/trace_processor_context.h"

namespace perfetto {
namespace trace_processor {

class NetworkTraceModule : public ProtoImporterModule {
 public:
  explicit NetworkTraceModule(TraceProcessorContext* context);
  ~NetworkTraceModule() override = default;

  // Tokenize and de-intern NetworkPacketBundles so that bundles of multiple
  // packets are sorted appropriately. This splits bundles with per-packet
  // details (packet_timestamps and packet_lengths) into one NetworkTraceEvent
  // per packet. Bundles with aggregates (i.e. total_packets) are forwarded
  // after de-interning the packet context.
  ModuleResult TokenizePacket(
      const protos::pbzero::TracePacket::Decoder& decoder,
      TraceBlobView* packet,
      int64_t ts,
      PacketSequenceState* state,
      uint32_t field_id) override;

  void ParseTracePacketData(const protos::pbzero::TracePacket::Decoder& decoder,
                            int64_t ts,
                            const TracePacketData&,
                            uint32_t field_id) override;

 private:
  void ParseNetworkPacketEvent(int64_t ts, protozero::ConstBytes blob);

  // Helper to simplify pushing a TracePacket to the sorter. The caller fills in
  // the packet buffer and uses this to push for sorting and reset the buffer.
  void PushPacketBufferForSort(int64_t timestamp, PacketSequenceState* state);

  TraceProcessorContext* context_;
  protozero::HeapBuffered<protos::pbzero::TracePacket> packet_buffer_;

  const StringId net_arg_length_;
  const StringId net_arg_ip_proto_;
  const StringId net_arg_tcp_flags_;
  const StringId net_arg_tag_;
  const StringId net_arg_local_port_;
  const StringId net_arg_remote_port_;
  const StringId net_ipproto_tcp_;
  const StringId net_ipproto_udp_;
};

}  // namespace trace_processor
}  // namespace perfetto

#endif  // SRC_TRACE_PROCESSOR_IMPORTERS_PROTO_NETWORK_TRACE_MODULE_H_
