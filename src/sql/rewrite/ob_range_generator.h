/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */
#ifndef OCEANBASE_SQL_REWRITE_OB_RANGE_GENERATOR_H_
#define OCEANBASE_SQL_REWRITE_OB_RANGE_GENERATOR_H_

#include "sql/rewrite/ob_query_range_define.h"
#include "sql/rewrite/ob_expr_range_converter.h"

namespace oceanbase
{
namespace sql
{

struct ObTmpRange : public common::ObDLinkBase<ObTmpRange>
{
  ObTmpRange(common::ObIAllocator &allocator)
    : start_(NULL),
      end_(NULL),
      include_start_(false),
      include_end_(false),
      always_true_(false),
      always_false_(false),
      min_offset_(0),
      max_offset_(0),
      column_cnt_(0),
      is_phy_rowid_(false),
      allocator_(allocator)
  {
  }
  int copy(ObTmpRange &other);
  int init_tmp_range(int64_t column_count);
  void set_always_true();
  void set_always_false();
  int intersect(ObTmpRange &other, bool &need_delay);
  int formalize();
  int refine_final_range();
  DECLARE_TO_STRING;
  // int tailor_final_range(int64_t column_count);
  common::ObObj *start_;
  common::ObObj *end_;
  bool include_start_;
  bool include_end_;
  bool always_true_;
  bool always_false_;
  int min_offset_;
  int max_offset_;
  int column_cnt_;
  bool is_phy_rowid_;
  common::ObIAllocator &allocator_;
};

typedef ObDList<ObTmpRange> TmpRangeList;

struct ObTmpInParam
{
  ObTmpInParam(common::ObIAllocator &allocator)
    : always_false_(false),
      in_param_(allocator) {}

  TO_STRING_KV(K_(always_false), K(in_param_));
  bool always_false_;
  ObFixedArray<ObObj*, ObIAllocator> in_param_;
};

class ObRangeGenerator
{
public:
  ObRangeGenerator(ObIAllocator &allocator,
                   ObExecContext &exec_ctx,
                   const ObPreRangeGraph *pre_range_graph,
                   ObIArray<common::ObNewRange *> &ranges,
                   bool &all_single_value_ranges,
                   const common::ObDataTypeCastParams &dtc_params)
    : allocator_(allocator),
      exec_ctx_(exec_ctx),
      pre_range_graph_(pre_range_graph),
      ranges_(ranges),
      all_single_value_ranges_(all_single_value_ranges),
      dtc_params_(dtc_params),
      range_map_(pre_range_graph->get_range_map()),
      always_true_range_(nullptr),
      always_false_range_(nullptr),
      all_tmp_ranges_(allocator),
      tmp_range_lists_(nullptr),
      all_tmp_node_caches_(allocator),
      always_false_tmp_range_(nullptr),
      is_generate_ss_range_(false),
      cur_datetime_(0)
  {
    if (OB_NOT_NULL(exec_ctx_.get_physical_plan_ctx())) {
      cur_datetime_ = exec_ctx_.get_physical_plan_ctx()->get_cur_time().get_datetime();
    }
  }

  int generate_ranges();
  int generate_ss_ranges();
private:
  int generate_tmp_range(ObTmpRange *&tmp_range, const int64_t column_cnt);
  int generate_one_range(ObTmpRange &tmp_range);
  int generate_precise_get_range(const ObRangeNode &node);
  int generate_standard_ranges(const ObRangeNode *node);
  int formalize_standard_range(const ObRangeNode *node, ObTmpRange &range);
  int generate_complex_ranges(const ObRangeNode *node);
  int formalize_complex_range(const ObRangeNode *node);
  int generate_one_complex_range();
  int final_range_node(const ObRangeNode *node, ObTmpRange *&range, bool need_cache);
  int final_in_range_node(const ObRangeNode *node, const int64_t in_idx, ObTmpRange *&range);
  int get_result_value(const int64_t param_idx, ObObj &val, bool &is_valid, ObExecContext &exec_ctx) const;

  int cast_value_type(ObTmpRange &range);
  int try_cast_value(const ObRangeColumnMeta &meta,
                     ObObj &value,
                     int64_t &cmp);
  int generate_contain_exec_param_range();
  int merge_and_remove_ranges();
  int try_intersect_delayed_range(ObTmpRange &range);
  int create_new_range(ObNewRange *&range, int64_t column_cnt);
  inline bool is_const_expr_or_null(int64_t idx) { return idx < OB_RANGE_EXTEND_VALUE || OB_RANGE_NULL_VALUE == idx; }
  int final_not_in_range_node(const ObRangeNode &node,
                              const int64_t not_in_idx,
                              ObTmpInParam *in_param,
                              ObTmpRange *&range);
  int generate_tmp_not_in_param(const ObRangeNode &node,
                                ObTmpInParam *&in_param);
private:
  ObRangeGenerator();
  static const int64_t RANGE_BUCKET_SIZE = 1000;
private:
  ObIAllocator &allocator_;
  ObExecContext &exec_ctx_;
  const ObPreRangeGraph *pre_range_graph_;
  ObIArray<common::ObNewRange *> &ranges_;
  bool &all_single_value_ranges_;
  const common::ObDataTypeCastParams &dtc_params_;
  const ObRangeMap &range_map_;
  common::ObNewRange *always_true_range_;
  common::ObNewRange *always_false_range_;
  ObFixedArray<ObTmpRange*, ObIAllocator> all_tmp_ranges_;
  TmpRangeList* tmp_range_lists_;
  ObFixedArray<void*, ObIAllocator> all_tmp_node_caches_;
  ObTmpRange *always_false_tmp_range_;
  bool is_generate_ss_range_;
  int64_t cur_datetime_;
};


} // namespace sql
} // namespace oceanbase
#endif // OCEANBASE_SQL_REWRITE_OB_RANGE_GENERATOR_H_
