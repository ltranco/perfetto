/*
 * Copyright (C) 2022 The Android Open Source Project
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

#ifndef SRC_TRACE_PROCESSOR_VIEWS_SLICE_VIEWS_H_
#define SRC_TRACE_PROCESSOR_VIEWS_SLICE_VIEWS_H_

#include "src/trace_processor/db/view.h"
#include "src/trace_processor/tables/metadata_tables.h"
#include "src/trace_processor/tables/slice_tables.h"
#include "src/trace_processor/tables/track_tables_py.h"
#include "src/trace_processor/views/macros.h"

namespace perfetto {
namespace trace_processor {
namespace views {

// TODO(lalitm): add support in document generator for views.
#define PERFETTO_TP_THREAD_SLICE_VIEW_DEF(NAME, FROM, JOIN, COL, FCOL)      \
  NAME(ThreadSliceView, "exp_thread_slice")                                 \
  PERFETTO_TP_VIEW_EXPORT_FROM_COLS(PERFETTO_TP_SLICE_TABLE_DEF, FCOL)      \
  COL(utid, track, utid)                                                    \
  COL(thread_name, thread, name)                                            \
  COL(upid, thread, upid)                                                   \
  FROM(tables::SliceTable, slice)                                           \
  JOIN(tables::ThreadTrackTable, track, id, slice, track_id, View::kNoFlag) \
  JOIN(tables::ThreadTable, thread, id, track, utid,                        \
       View::kIdAlwaysPresent | View::kTypeCheckSerialized)

PERFETTO_TP_DECLARE_VIEW(PERFETTO_TP_THREAD_SLICE_VIEW_DEF);

}  // namespace views
}  // namespace trace_processor
}  // namespace perfetto

#endif  // SRC_TRACE_PROCESSOR_VIEWS_SLICE_VIEWS_H_
