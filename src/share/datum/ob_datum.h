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

#ifndef OCEANBASE_DATUM_OB_DATUM_H_
#define OCEANBASE_DATUM_OB_DATUM_H_

#include "common/object/ob_object.h"

namespace oceanbase {
namespace common {

//
// ObDatum is new structure for data of one cell, almost the same as the old ObObj without the
// metadata part. All data types is stored in continuous memory, here is the memory layout of
// all types:
//
//   | type                | memory layout                 | min length | max length          |
//   |---------------------|-------------------------------|------------|---------------------|
//   | ObNullType          | char[0]                       | 0          | 0                   |
//   | ObTinyIntType       | int64_t                       | 8          | 8                   |
//   | ObSmallIntType      | int64_t                       | 8          | 8                   |
//   | ObMediumIntType     | int64_t                       | 8          | 8                   |
//   | ObInt32Type         | int64_t                       | 8          | 8                   |
//   | ObIntType           | int64_t                       | 8          | 8                   |
//   | ObUTinyIntType      | uint64_t                      | 8          | 8                   |
//   | ObUSmallIntType     | uint64_t                      | 8          | 8                   |
//   | ObUMediumIntType    | uint64_t                      | 8          | 8                   |
//   | ObUInt32Type        | uint64_t                      | 8          | 8                   |
//   | ObUInt64Type        | uint64_t                      | 8          | 8                   |
//   | ObFloatType         | float                         | 4          | 4                   |
//   | ObDoubleType        | double                        | 8          | 8                   |
//   | ObUFloatType        | float                         | 4          | 4                   |
//   | ObUDoubleType       | double                        | 8          | 8                   |
//   | ObNumberType        | ObNumberDesc + uint32_t[0~9]  | 4          | 40                  |
//   | ObUNumberType       | ObNumberDesc + uint32_t[0~9]  | 4          | 40                  |
//   | ObDateTimeType      | int64_t                       | 8          | 8                   |
//   | ObTimestampType     | int64_t                       | 8          | 8                   |
//   | ObDateType          | int32_t                       | 4          | 4                   |
//   | ObTimeType          | int64_t                       | 8          | 8                   |
//   | ObYearType          | uint8_t                       | 1          | 1                   |
//   | ObVarcharType       | char[]                        | 0          | type maximum length |
//   | ObCharType          | char[]                        | 0          | type maximum length |
//   | ObHexStringType     | char[]                        | 0          | type maximum length |
//   | ObExtendType        | int64_t                       | 8          | 8                   |
//   | ObUnknownType       | int64_t                       | 8          | 8                   |
//   | ObTinyTextType      | char[]                        | 0          | type maximum length |
//   | ObTextType          | char[]                        | 0          | type maximum length |
//   | ObMediumTextType    | char[]                        | 0          | type maximum length |
//   | ObLongTextType      | char[]                        | 0          | type maximum length |
//   | ObBitType           | uint64_t                      | 8          | 8                   |
//   | ObEnumType          | uint64_t                      | 8          | 8                   |
//   | ObSetType           | uint64_t                      | 8          | 8                   |
//   | ObEnumInnerType     | char[]                        | 0          | type maximum length |
//   | ObSetInnerType      | char[]                        | 0          | type maximum length |
//   | ObTimestampTZType   | UnionTZCtx + int64_t          | 12         | 12                  |
//   | ObTimestampLTZType  | uint16_t(time_desc) + int64_t | 10         | 10                  |
//   | ObTimestampNanoType | uint16_t(time_desc) + int64_t | 10         | 10                  |
//   | ObRawType           | char[]                        | 0          | type maximum length |
//   | ObIntervalYMType    | int64_t                       | 8          | 8                   |
//   | ObIntervalDSType    | int32_t(fractional) + int64_t | 12         | 12                  |
//   | ObNumberFloatType   | ObNumberDesc + uint32_t[0~9]  | 4          | 40                  |
//   | ObNVarchar2Type     | char[]                        | 0          | type maximum length |
//   | ObNCharType         | char[]                        | 0          | type maximum length |
//   | ObURowIDType        | char[]                        | 0          | type maximum length |
//

// ObObj to ObDatum memory layout mapping type, see ObDatum::get_obj_datum_map_type()
enum ObObjDatumMapType : uint8_t {
  OBJ_DATUM_NULL,            // null
  OBJ_DATUM_STRING,          // string
  OBJ_DATUM_NUMBER,          // number
  OBJ_DATUM_8BYTE_DATA,      // 8 bytes ObObj::v_
  OBJ_DATUM_4BYTE_DATA,      // 4 bytes ObObj::v_
  OBJ_DATUM_1BYTE_DATA,      // 1 bytes ObObj::v_
  OBJ_DATUM_4BYTE_LEN_DATA,  // 4 bytes ObObj::val_len_ + 8 bytes ObObj::v_
  OBJ_DATUM_2BYTE_LEN_DATA,  // 2 bytes ObObj::val_len_ + 8 bytes ObObj::v_
  OBJ_DATUM_LOB_LOCATOR,     // lob locator
  OBJ_DATUM_MAPPING_MAX
};

enum DatumReserveSize {
  OBJ_DATUM_NULL_RES_SIZE = 0,
  OBJ_DATUM_1BYTE_DATA_RES_SIZE = 1,
  OBJ_DATUM_4BYTE_DATA_RES_SIZE = 4,
  OBJ_DATUM_8BYTE_DATA_RES_SIZE = 8,
  OBJ_DATUM_2BYTE_LEN_DATA_RES_SIZE = 10,
  OBJ_DATUM_4BYTE_LEN_DATA_RES_SIZE = 12,
  OBJ_DATUM_NUMBER_RES_SIZE = 40,
  OBJ_DATUM_STRING_RES_SIZE = 128,
  OBJ_DATUM_LOB_LOCATOR_RES_SIZE = 128
};

// Pointer to data
struct ObDatumPtr {
  union {
    const char* ptr_;
    const int64_t* int_;
    const uint64_t* uint_;
    const float* float_;
    const double* double_;
    const int64_t* ext_;
    const int64_t* unknown_;
    const number::ObCompactNumber* num_;
    const int64_t* interval_nmonth_;
    const ObIntervalDSValue* interval_ds_;
    const int64_t* datetime_;
    const int32_t* date_;
    const int64_t* time_;
    const uint8_t* year_;
    const ObOTimestampData* timestamp_tz_;
    const ObOTimestampTinyData* timestamp_tiny_;
    const char* inner_enumset_;
    const ObLobLocator* lob_locator_;
  };

  ObDatumPtr() : ptr_(NULL)
  {}
} __attribute__((packed));

// Datum description, is null or length of data
struct ObDatumDesc {
  union {
    struct {
      uint32_t len_ : 31;
      uint32_t null_ : 1;
    };
    // Pack null flag and length, you can use it to unset null flag and set length of data.
    uint32_t pack_;
  };

  ObDatumDesc() : pack_(0)
  {}
  void set_null()
  {
    pack_ = 1u << 31;
  }
  bool is_null() const
  {
    return 1 == null_;
  }
  void unset_null()
  {
    null_ = false;
  }
} __attribute__((packed));

// Datum structure, multiple inheritance from ObDatumPtr and ObDatumDesc makes
// %ptr_, %len_ access easier.
struct ObDatum : public ObDatumPtr, public ObDatumDesc {
  ObDatumPtr& ptr()
  {
    return *this;
  }
  const ObDatumPtr& ptr() const
  {
    return *this;
  }
  ObDatumDesc& desc()
  {
    return *this;
  };
  const ObDatumDesc& desc() const
  {
    return *this;
  };

  ObDatum() : ObDatumPtr(), ObDatumDesc()
  {}

  inline void reset()
  {
    new (this) ObDatum();
  }
  static bool binary_equal(const ObDatum& r, const ObDatum& l)
  {
    bool equal = true;
    if (r.null_ != l.null_) {
      equal = false;
    } else if (!r.is_null()) {
      if (r.len_ != l.len_) {
        equal = false;
      } else {
        equal = (0 == MEMCMP(r.ptr_, l.ptr_, r.len_));
      }
    }
    return equal;
  }
  static ObObjDatumMapType get_obj_datum_map_type(const ObObjType type);
  static uint32_t get_reserved_size(const ObObjDatumMapType type);
  // From ObObj, the caller is responsible for ensuring %ptr_ has enough memory
  inline int from_obj(const ObObj& obj, const ObObjDatumMapType map_type);
  // From ObObj, the caller is responsible for ensuring %ptr_ has enough memory
  inline int from_obj(const ObObj& obj);

  // Convert to ObObj
  inline int to_obj(ObObj& obj, const ObObjMeta& meta, const ObObjDatumMapType map_type) const;
  inline int to_obj(ObObj& obj, const ObObjMeta& meta) const;

  // remember to check is_null() before get_bool()
  inline bool get_bool() const
  {
    return 0 != get_int();
  }
  inline bool is_false() const
  {
    return !is_null() && 0 == get_int();
  }
  inline bool is_true() const
  {
    return !is_null() && 0 != get_int();
  }
  inline int8_t get_int8() const
  {
    return *(reinterpret_cast<const int8_t *>(int_));
  }
  inline int8_t get_tinyint() const
  {
    return *(reinterpret_cast<const int8_t *>(int_));
  }
  inline int16_t get_smallint() const
  {
    return *(reinterpret_cast<const int16_t *>(int_));
  }
  inline int32_t get_mediumint() const
  {
    return *(reinterpret_cast<const int32_t *>(int_));
  }
  inline int32_t get_int32() const
  {
    return *(reinterpret_cast<const int32_t *>(int_));
  }
  inline int64_t get_int() const
  {
    return *int_;
  }
  inline uint8_t get_uint8() const
  {
    return *(reinterpret_cast<const uint8_t *>(uint_));
  }
  inline uint8_t get_utinyint() const
  {
    return *(reinterpret_cast<const uint8_t *>(uint_));
  }
  inline uint16_t get_usmallint() const
  {
    return *(reinterpret_cast<const uint16_t *>(uint_));
  }
  inline uint32_t get_umediumint() const
  {
    return *(reinterpret_cast<const uint32_t *>(uint_));
  }
  inline uint32_t get_uint32() const
  {
    return *(reinterpret_cast<const uint32_t *>(uint_));
  }
  inline uint64_t get_uint64() const
  {
    return *uint_;
  }
  inline uint64_t get_uint() const
  {
    return *uint_;
  }
  inline float get_float() const
  {
    return *float_;
  }
  inline double get_double() const
  {
    return *double_;
  }
  inline float get_ufloat() const
  {
    return *float_;
  }
  inline double get_udouble() const
  {
    return *double_;
  }
  inline int64_t get_ext() const
  {
    return *ext_;
  }
  inline int64_t get_unknown() const
  {
    return *unknown_;
  }
  inline uint64_t get_bit() const
  {
    return *uint_;
  }
  inline uint64_t get_enum() const
  {
    return *uint_;
  }
  inline uint64_t get_set() const
  {
    return *uint_;
  }
  inline uint64_t get_enumset() const
  {
    return *uint_;
  }
  inline const number::ObCompactNumber& get_number() const
  {
    return *num_;
  }
  inline const number::ObNumber::Desc& get_number_desc() const
  {
    return num_->desc_;
  }
  inline const uint32_t* get_number_digits() const
  {
    return &num_->digits_[0];
  }
  // for ObIntervalYMType  (which is ObIntervalTC type class)
  inline int64_t get_interval_ym() const
  {
    return *interval_nmonth_;
  }
  // for ObIntervalDSType (which is ObIntervalTC type class)
  inline int64_t get_interval_nmonth() const
  {
    return *interval_nmonth_;
  }
  inline const ObIntervalDSValue& get_interval_ds() const
  {
    return *interval_ds_;
  }
  inline int64_t get_datetime() const
  {
    return *datetime_;
  }
  inline int64_t get_timestamp() const
  {
    return *datetime_;
  }
  inline int32_t get_date() const
  {
    return *date_;
  }
  inline int64_t get_time() const
  {
    return *time_;
  }
  inline uint8_t get_year() const
  {
    return *year_;
  }
  // for ObTimestampTZType (which is ObOTimestampTC type class)
  inline const ObOTimestampData& get_otimestamp_tz() const
  {
    return *timestamp_tz_;
  }
  // for ObTimestampLTZType and ObTimestampNanoType (both ObOTimestampTC type class)
  inline ObOTimestampData get_otimestamp_tiny() const
  {
    ObOTimestampData res;
    res.time_ctx_.time_desc_ = timestamp_tiny_->desc_;
    res.time_ctx_.tz_desc_ = 0;
    res.time_us_ = timestamp_tiny_->time_us_;
    return res;
  }
  inline const ObString get_string() const
  {
    return ObString(len_, ptr_);
  }
  inline int get_enumset_inner(ObEnumSetInnerValue& inner_value) const
  {
    int64_t pos = 0;
    return inner_value.deserialize(ptr_, len_, pos);
  }
  inline const ObLobLocator& get_lob_locator() const
  {
    return *lob_locator_;
  }

  // Setter functions for ObDatum.
  // CAUTION: The caller is responsible for ensuring %ptr_ has enough memory.
  //
  // We reserve enough memory in sql expression, it's safe to use the setter functions in sql
  // expression evaluate.
  // Set integer, same as the following ObObj interfaces:
  //    set_tinyint, set_smallint, set_mediumint, set_int32, set_int
  inline void set_int(const int64_t v)
  {
    *no_cv(int_) = v;
    pack_ = sizeof(int64_t);
  };
  inline void set_int32(const int32_t v)
  {
    set_int(static_cast<int64_t>(v));
  }
  // Set unsigned integer, same as following ObObj interfaces:
  //    set_utinyint, set_usmallint, set_umediumint, set_uint32, set_uint
  inline void set_uint(const uint64_t v)
  {
    *no_cv(uint_) = v;
    pack_ = sizeof(uint64_t);
  }
  inline void set_uint32(const uint32_t v)
  {
    set_uint(static_cast<uint64_t>(v));
  }
  inline void set_bit(const uint64_t v)
  {
    set_uint(v);
  }

  inline void set_bool(const bool v)
  {
    set_int(static_cast<int64_t>(v));
  }
  inline void set_true()
  {
    set_bool(true);
  }
  inline void set_false()
  {
    set_bool(false);
  }

  inline void set_float(const float v)
  {
    *no_cv(float_) = v;
    pack_ = sizeof(float);
  }
  inline void set_double(const double v)
  {
    *no_cv(double_) = v;
    pack_ = sizeof(double);
  }
  inline void set_enum(const uint64_t v)
  {
    *no_cv(uint_) = v;
    pack_ = sizeof(uint64_t);
  }
  inline void set_set(const uint64_t v)
  {
    *no_cv(uint_) = v;
    pack_ = sizeof(uint64_t);
  }

  inline void set_interval_nmonth(const int64_t interval_nmonth)
  {
    *no_cv(interval_nmonth_) = interval_nmonth;
    pack_ = sizeof(int64_t);
  };
  inline void set_interval_ym(const int64_t interval_nmonth)
  {
    *no_cv(interval_nmonth_) = interval_nmonth;
    pack_ = sizeof(int64_t);
  };
  inline void set_interval_ds(const ObIntervalDSValue& v)
  {
    *no_cv(interval_ds_) = v;
    pack_ = v.get_store_size();
  }
  inline void set_datetime(const int64_t v)
  {
    *no_cv(datetime_) = v;
    pack_ = sizeof(int64_t);
  }
  inline void set_timestamp(const int64_t v)
  {
    *no_cv(datetime_) = v;
    pack_ = sizeof(int64_t);
  }
  // set otimestamp value of ObTimestampTZType
  inline void set_otimestamp_tz(const ObOTimestampData& v)
  {
    no_cv(timestamp_tz_)->time_ctx_ = v.time_ctx_;
    no_cv(timestamp_tz_)->time_us_ = v.time_us_;
    pack_ = sizeof(ObOTimestampData);
  }
  // set otimestamp value of ObTimestampLTZType and ObTimestampNanoType
  inline void set_otimestamp_tiny(const ObOTimestampData& v)
  {
    no_cv(timestamp_tiny_)->desc_ = v.time_ctx_.time_desc_;
    no_cv(timestamp_tiny_)->time_us_ = v.time_us_;
    pack_ = sizeof(ObOTimestampTinyData);
  }
  // Same as ObObj::set_time
  inline void set_time(const int64_t v)
  {
    set_int(v);
  }
  inline void set_otimestamp(const ObOTimestampData& v)
  {
    memcpy(no_cv(ptr_), &v, sizeof(v));
    pack_ = sizeof(v);
  }
  inline void set_otimestamp(const int64_t time_us, const uint16_t time_desc)
  {
    memcpy(no_cv(ptr_), &time_us, sizeof(time_us));
    memcpy(no_cv(ptr_) + sizeof(time_us), &time_desc, sizeof(time_desc));
  }

  // Same as ObObj::set_date
  inline void set_date(const int32_t v)
  {
    memcpy(no_cv(ptr_), &v, sizeof(v));
    pack_ = sizeof(v);
  }
  // Same as ObObj::set_year
  inline void set_year(const int8_t v)
  {
    memcpy(no_cv(ptr_), &v, sizeof(v));
    pack_ = sizeof(v);
  }

  // Set number, deep copy all number digits here.
  OB_INLINE void set_number(const number::ObNumber& num);
  // Set compact number, deep copy all number digits too.
  OB_INLINE void set_number(const number::ObCompactNumber& cnum);

  inline void set_string(const ObString& v)
  {
    ptr_ = v.ptr();
    pack_ = v.length();
  }
  inline void set_string(const char* ptr, const int64_t len)
  {
    ptr_ = ptr;
    pack_ = len;
  }
  inline void set_enumset_inner(const ObString& v)
  {
    set_string(v);
  }
  inline void set_enumset_inner(const char* ptr, const int64_t len)
  {
    set_string(ptr, len);
  }
  inline void set_urowid(const ObURowIDData& urowid_data)
  {
    ptr_ = reinterpret_cast<const char*>(urowid_data.rowid_content_);
    pack_ = urowid_data.rowid_len_;
  }
  inline void set_lob_locator(const ObLobLocator& value)
  {
    lob_locator_ = &value;
    pack_ = value.get_total_size();
  }
  inline void set_datum(const ObDatum& other)
  {
    *this = other;
  }
  inline int deep_copy(const ObDatum& src, char* buf, int64_t max_size, int64_t& pos)
  {
    int ret = OB_SUCCESS;
    *this = src;
    if (!null_) {
      if (pos + len_ > max_size) {
        ret = OB_BUF_NOT_ENOUGH;
      } else {
        MEMCPY(buf + pos, src.ptr_, len_);
        ptr_ = buf + pos;
        pos += len_;
      }
    }
    return ret;
  }

  inline int deep_copy(const ObDatum& src, ObIAllocator& alloc)
  {
    int ret = OB_SUCCESS;
    *this = src;
    if (!null_) {
      const int64_t alloc_len = src.len_ > 0 ? src.len_ : 1;
      char* buf = static_cast<char*>(alloc.alloc(alloc_len));
      if (OB_ISNULL(buf)) {
        ret = OB_ALLOCATE_MEMORY_FAILED;
        COMMON_LOG(WARN, "allocate memory failed", K(alloc_len), K(ret));
      } else {
        MEMCPY(buf, src.ptr_, src.len_);
        // need set ptr_ after memory copy, if this == &src
        ptr_ = buf;
      }
    }
    return ret;
  }

  // Print HEX value of ptr_, more detailed is printed by type guessing.
  // Try not to use this, but use DATUM2STR which interprets it via meta.
  DECLARE_TO_STRING;

  // Remove const value qualification for pointer.
  template <typename T>
  inline T* no_cv(const T* ptr)
  {
    return const_cast<T*>(ptr);
  }

  template <ObObjDatumMapType type>
  inline void obj2datum(const ObObj& obj);
  template <ObObjDatumMapType type>
  inline void datum2obj(ObObj& obj) const;

} __attribute__((packed));

// Get payload of datum for reading, some type class will specialization this.
template <ObObjTypeClass TC>
struct ObDatumPayload {
  // return ptr_ for default implement
  static inline const char* get(const ObDatum& d)
  {
    return d.ptr_;
  }
};

template <>
struct ObDatumPayload<ObNullTC> {
  static inline void get(const ObDatum& d)
  {
    UNUSED(d);
    return;
  }
};

template <>
struct ObDatumPayload<ObIntTC> {
  static inline int64_t get(const ObDatum& d)
  {
    return *d.int_;
  }
};

template <>
struct ObDatumPayload<ObUIntTC> {
  static inline uint64_t get(const ObDatum& d)
  {
    return *d.uint_;
  }
};

template <>
struct ObDatumPayload<ObFloatTC> {
  static inline float get(const ObDatum& d)
  {
    return *d.float_;
  }
};

template <>
struct ObDatumPayload<ObDoubleTC> {
  static inline double get(const ObDatum& d)
  {
    return *d.double_;
  }
};

template <>
struct ObDatumPayload<ObNumberTC> {
  static inline const number::ObCompactNumber& get(const ObDatum& d)
  {
    return *d.num_;
  }
};

template <>
struct ObDatumPayload<ObDateTimeTC> {
  static inline int64_t get(const ObDatum& d)
  {
    return *d.int_;
  }
};

template <>
struct ObDatumPayload<ObDateTC> {
  static inline int32_t get(const ObDatum& d)
  {
    return *reinterpret_cast<const int32_t*>(d.ptr_);
  }
};

template <>
struct ObDatumPayload<ObTimeTC> {
  static inline int64_t get(const ObDatum& d)
  {
    return *d.int_;
  }
};

template <>
struct ObDatumPayload<ObYearTC> {
  static inline uint8_t get(const ObDatum& d)
  {
    return *reinterpret_cast<const uint8_t*>(d.year_);
  }
};

// ObStringTC: default implement, return ptr_

template <>
struct ObDatumPayload<ObExtendTC> {
  static inline int64_t get(const ObDatum& d)
  {
    return *d.int_;
  }
};

template <>
struct ObDatumPayload<ObUnknownTC> {
  static inline int64_t get(const ObDatum& d)
  {
    return *d.int_;
  }
};

// ObTextTC: default implement, return ptr_

template <>
struct ObDatumPayload<ObBitTC> {
  static inline uint64_t get(const ObDatum& d)
  {
    return *d.uint_;
  }
};

template <>
struct ObDatumPayload<ObEnumSetTC> {
  static inline uint64_t get(const ObDatum& d)
  {
    return *d.uint_;
  }
};

// ObEnumSetInnerTC: default implement, return ptr_
// ObOTimestampTC: no corresponding structure defined, need interpret ptr_
// ObRawTC: default implement, return ptr_
// ObIntervalTC : no corresponding structure defined, need interpret ptr_

OB_INLINE void ObDatum::set_number(const number::ObNumber& num)
{
  no_cv(num_)->desc_ = num.d_;
  const int64_t len = num.d_.len_ * sizeof(*num.get_digits());
  memcpy(&no_cv(num_)->digits_[0], num.get_digits(), len);
  pack_ = len + sizeof(*num_);
}

OB_INLINE void ObDatum::set_number(const number::ObCompactNumber& cnum)
{
  pack_ = sizeof(cnum) + cnum.desc_.len_ * sizeof(cnum.digits_[0]);
  memcpy(no_cv(num_), &cnum, len_);
}

template <>
inline void ObDatum::obj2datum<OBJ_DATUM_NULL>(const ObObj&)
{
  set_null();
}

template <>
inline void ObDatum::datum2obj<OBJ_DATUM_NULL>(ObObj& obj) const
{
  obj.set_null();
}

template <>
inline void ObDatum::obj2datum<OBJ_DATUM_STRING>(const ObObj& obj)
{
  ptr_ = const_cast<char*>(obj.v_.string_);
  pack_ = obj.val_len_;
}

template <>
inline void ObDatum::datum2obj<OBJ_DATUM_STRING>(ObObj& obj) const
{
  obj.val_len_ = len_;
  obj.v_.string_ = ptr_;
}

template <>
inline void ObDatum::obj2datum<OBJ_DATUM_NUMBER>(const ObObj& obj)
{
  memcpy(no_cv(ptr_), &obj.nmb_desc_, sizeof(obj.nmb_desc_));
  if (OB_LIKELY(1 == obj.nmb_desc_.len_)) {
    memcpy(no_cv(ptr_) + sizeof(obj.nmb_desc_), obj.v_.nmb_digits_, sizeof(*obj.v_.nmb_digits_));
    pack_ = sizeof(obj.nmb_desc_) + sizeof(*obj.v_.nmb_digits_);
  } else {
    memcpy(no_cv(ptr_) + sizeof(obj.nmb_desc_), obj.v_.nmb_digits_, sizeof(*obj.v_.nmb_digits_) * obj.nmb_desc_.len_);
    pack_ = sizeof(obj.nmb_desc_) + sizeof(*obj.v_.nmb_digits_) * obj.nmb_desc_.len_;
  }
}

template <>
inline void ObDatum::datum2obj<OBJ_DATUM_NUMBER>(ObObj& obj) const
{
  memcpy(&obj.nmb_desc_, ptr_, sizeof(obj.nmb_desc_));
  obj.v_.string_ = ptr_ + sizeof(obj.nmb_desc_);
}

template <>
inline void ObDatum::obj2datum<OBJ_DATUM_8BYTE_DATA>(const ObObj& obj)
{
  memcpy(no_cv(ptr_), &obj.v_.uint64_, sizeof(uint64_t));
  pack_ = sizeof(uint64_t);
}

template <>
inline void ObDatum::datum2obj<OBJ_DATUM_8BYTE_DATA>(ObObj& obj) const
{
  // keep obj.val_len_ is unchanged
  memcpy(&obj.v_.uint64_, ptr_, sizeof(uint64_t));
}

template <>
inline void ObDatum::obj2datum<OBJ_DATUM_4BYTE_DATA>(const ObObj& obj)
{
  memcpy(no_cv(ptr_), &obj.v_.uint64_, sizeof(uint32_t));
  pack_ = sizeof(uint32_t);
}

template <>
inline void ObDatum::datum2obj<OBJ_DATUM_4BYTE_DATA>(ObObj& obj) const
{
  // reset obj.v_ first, see ObObj::set_float_value
  obj.v_.uint64_ = 0;
  memcpy(&obj.v_.uint64_, ptr_, sizeof(uint32_t));
}

template <>
inline void ObDatum::obj2datum<OBJ_DATUM_1BYTE_DATA>(const ObObj& obj)
{
  memcpy(no_cv(ptr_), &obj.v_.uint64_, sizeof(uint8_t));
  pack_ = sizeof(uint8_t);
}

template <>
inline void ObDatum::datum2obj<OBJ_DATUM_1BYTE_DATA>(ObObj& obj) const
{
  obj.v_.uint64_ = 0;
  memcpy(&obj.v_.uint64_, ptr_, sizeof(uint8_t));
}

template <>
inline void ObDatum::obj2datum<OBJ_DATUM_4BYTE_LEN_DATA>(const ObObj& obj)
{
  memcpy(no_cv(ptr_), &obj.val_len_, sizeof(uint32_t));
  memcpy(no_cv(ptr_) + sizeof(uint32_t), &obj.v_.uint64_, sizeof(uint64_t));
  pack_ = sizeof(uint32_t) + sizeof(uint64_t);
}

template <>
inline void ObDatum::datum2obj<OBJ_DATUM_4BYTE_LEN_DATA>(ObObj& obj) const
{
  memcpy(&obj.val_len_, ptr_, sizeof(uint32_t));
  memcpy(&obj.v_.uint64_, ptr_ + sizeof(uint32_t), sizeof(uint64_t));
}

template <>
inline void ObDatum::obj2datum<OBJ_DATUM_2BYTE_LEN_DATA>(const ObObj& obj)
{
  memcpy(no_cv(ptr_), &obj.val_len_, sizeof(uint16_t));
  memcpy(no_cv(ptr_) + sizeof(uint16_t), &obj.v_.uint64_, sizeof(uint64_t));
  pack_ = sizeof(uint16_t) + sizeof(uint64_t);
}

template <>
inline void ObDatum::datum2obj<OBJ_DATUM_2BYTE_LEN_DATA>(ObObj& obj) const
{
  // rest obj.val_len_ first, see ObObj::set_otimestamp_value
  obj.val_len_ = 0;
  memcpy(&obj.val_len_, ptr_, sizeof(uint16_t));
  memcpy(&obj.v_.uint64_, ptr_ + sizeof(uint16_t), sizeof(uint64_t));
}

template <>
inline void ObDatum::obj2datum<OBJ_DATUM_LOB_LOCATOR>(const ObObj& obj)
{
  lob_locator_ = const_cast<ObLobLocator*>(obj.v_.lob_locator_);
  pack_ = obj.val_len_;
}

template <>
inline void ObDatum::datum2obj<OBJ_DATUM_LOB_LOCATOR>(ObObj& obj) const
{
  obj.val_len_ = len_;
  obj.v_.lob_locator_ = lob_locator_;
}

inline int ObDatum::from_obj(const ObObj& obj, const ObObjDatumMapType map_type)
{
  int ret = common::OB_SUCCESS;
  if (obj.is_null()) {
    set_null();
  } else {
    switch (map_type) {
      case OBJ_DATUM_NULL: {
        obj2datum<OBJ_DATUM_NULL>(obj);
        break;
      }
      case OBJ_DATUM_STRING: {
        obj2datum<OBJ_DATUM_STRING>(obj);
        break;
      }
      case OBJ_DATUM_NUMBER: {
        obj2datum<OBJ_DATUM_NUMBER>(obj);
        break;
      }
      case OBJ_DATUM_8BYTE_DATA: {
        obj2datum<OBJ_DATUM_8BYTE_DATA>(obj);
        break;
      }
      case OBJ_DATUM_4BYTE_DATA: {
        obj2datum<OBJ_DATUM_4BYTE_DATA>(obj);
        break;
      }
      case OBJ_DATUM_1BYTE_DATA: {
        obj2datum<OBJ_DATUM_1BYTE_DATA>(obj);
        break;
      }
      case OBJ_DATUM_4BYTE_LEN_DATA: {
        obj2datum<OBJ_DATUM_4BYTE_LEN_DATA>(obj);
        break;
      }
      case OBJ_DATUM_2BYTE_LEN_DATA: {
        obj2datum<OBJ_DATUM_2BYTE_LEN_DATA>(obj);
        break;
      }
      case OBJ_DATUM_LOB_LOCATOR: {
        obj2datum<OBJ_DATUM_LOB_LOCATOR>(obj);
        break;
      }
      case OBJ_DATUM_MAPPING_MAX: {
        ret = common::OB_ERR_UNEXPECTED;
        COMMON_LOG(WARN, "invalid obj datum mapping", K(ret), K(obj), K(map_type));
      }
    }
  }
  return ret;
}

inline int ObDatum::from_obj(const ObObj& obj)
{
  int ret = common::OB_SUCCESS;
  if (obj.is_null()) {
    set_null();
  } else {
    switch (obj.get_type()) {
      case ObNullType: {
        obj2datum<OBJ_DATUM_NULL>(obj);
        break;
      }
      case ObVarcharType:
      case ObCharType:
      case ObHexStringType:
      case ObTinyTextType:
      case ObTextType:
      case ObMediumTextType:
      case ObLongTextType:
      case ObEnumInnerType:
      case ObSetInnerType:
      case ObRawType:
      case ObNVarchar2Type:
      case ObNCharType:
      case ObURowIDType:
      case ObJsonType: {
        obj2datum<OBJ_DATUM_STRING>(obj);
        break;
      }
      case ObNumberType:
      case ObUNumberType:
      case ObNumberFloatType: {
        obj2datum<OBJ_DATUM_NUMBER>(obj);
        break;
      }
      case ObTinyIntType:
      case ObSmallIntType:
      case ObMediumIntType:
      case ObInt32Type:
      case ObIntType:
      case ObUTinyIntType:
      case ObUSmallIntType:
      case ObUMediumIntType:
      case ObUInt32Type:
      case ObUInt64Type:
      case ObDoubleType:
      case ObUDoubleType:
      case ObDateTimeType:
      case ObTimestampType:
      case ObTimeType:
      case ObExtendType:
      case ObUnknownType:
      case ObBitType:
      case ObEnumType:
      case ObSetType:
      case ObIntervalYMType: {
        obj2datum<OBJ_DATUM_8BYTE_DATA>(obj);
        break;
      }
      case ObFloatType:
      case ObUFloatType:
      case ObDateType: {
        obj2datum<OBJ_DATUM_4BYTE_DATA>(obj);
        break;
      }
      case ObYearType: {
        obj2datum<OBJ_DATUM_1BYTE_DATA>(obj);
        break;
      }
      case ObTimestampTZType:
      case ObIntervalDSType: {
        obj2datum<OBJ_DATUM_4BYTE_LEN_DATA>(obj);
        break;
      }
      case ObTimestampLTZType:
      case ObTimestampNanoType: {
        obj2datum<OBJ_DATUM_2BYTE_LEN_DATA>(obj);
        break;
      }
      case ObLobType: {
        obj2datum<OBJ_DATUM_LOB_LOCATOR>(obj);
        break;
      }
      case ObMaxType: {
        ret = common::OB_ERR_UNEXPECTED;
        COMMON_LOG(WARN, "invalid obj type", K(ret), K(obj), K(obj.get_type()));
      }
    }
  }
  return ret;
}

inline int ObDatum::to_obj(ObObj& obj, const ObObjMeta& meta, const ObObjDatumMapType map_type) const
{
  int ret = common::OB_SUCCESS;
  if (null_) {
    obj.set_null();
  } else {
    obj.meta_ = meta;
    switch (map_type) {
      case OBJ_DATUM_NULL: {
        datum2obj<OBJ_DATUM_NULL>(obj);
        break;
      }
      case OBJ_DATUM_STRING: {
        datum2obj<OBJ_DATUM_STRING>(obj);
        break;
      }
      case OBJ_DATUM_NUMBER: {
        datum2obj<OBJ_DATUM_NUMBER>(obj);
        break;
      }
      case OBJ_DATUM_8BYTE_DATA: {
        datum2obj<OBJ_DATUM_8BYTE_DATA>(obj);
        break;
      }
      case OBJ_DATUM_4BYTE_DATA: {
        datum2obj<OBJ_DATUM_4BYTE_DATA>(obj);
        break;
      }
      case OBJ_DATUM_1BYTE_DATA: {
        datum2obj<OBJ_DATUM_1BYTE_DATA>(obj);
        break;
      }
      case OBJ_DATUM_4BYTE_LEN_DATA: {
        datum2obj<OBJ_DATUM_4BYTE_LEN_DATA>(obj);
        break;
      }
      case OBJ_DATUM_2BYTE_LEN_DATA: {
        datum2obj<OBJ_DATUM_2BYTE_LEN_DATA>(obj);
        break;
      }
      case OBJ_DATUM_LOB_LOCATOR: {
        datum2obj<OBJ_DATUM_LOB_LOCATOR>(obj);
        break;
      }
      case OBJ_DATUM_MAPPING_MAX: {
        ret = common::OB_ERR_UNEXPECTED;
        COMMON_LOG(WARN, "invalid obj datum mapping", K(ret), K(map_type));
      }
    }
  }
  return ret;
}

inline int ObDatum::to_obj(ObObj& obj, const ObObjMeta& meta) const
{
  int ret = common::OB_SUCCESS;
  if (null_) {
    obj.set_null();
  } else {
    obj.meta_ = meta;
    switch (meta.get_type()) {
      case ObNullType: {
        datum2obj<OBJ_DATUM_NULL>(obj);
        break;
      }
      case ObVarcharType:
      case ObCharType:
      case ObHexStringType:
      case ObTinyTextType:
      case ObTextType:
      case ObMediumTextType:
      case ObLongTextType:
      case ObEnumInnerType:
      case ObSetInnerType:
      case ObRawType:
      case ObNVarchar2Type:
      case ObNCharType:
      case ObURowIDType:
      case ObJsonType: {
        datum2obj<OBJ_DATUM_STRING>(obj);
        break;
      }
      case ObNumberType:
      case ObUNumberType:
      case ObNumberFloatType: {
        datum2obj<OBJ_DATUM_NUMBER>(obj);
        break;
      }
      case ObTinyIntType:
      case ObSmallIntType:
      case ObMediumIntType:
      case ObInt32Type:
      case ObIntType:
      case ObUTinyIntType:
      case ObUSmallIntType:
      case ObUMediumIntType:
      case ObUInt32Type:
      case ObUInt64Type:
      case ObDoubleType:
      case ObUDoubleType:
      case ObDateTimeType:
      case ObTimestampType:
      case ObTimeType:
      case ObExtendType:
      case ObUnknownType:
      case ObBitType:
      case ObEnumType:
      case ObSetType:
      case ObIntervalYMType: {
        datum2obj<OBJ_DATUM_8BYTE_DATA>(obj);
        break;
      }
      case ObFloatType:
      case ObUFloatType:
      case ObDateType: {
        datum2obj<OBJ_DATUM_4BYTE_DATA>(obj);
        break;
      }
      case ObYearType: {
        datum2obj<OBJ_DATUM_1BYTE_DATA>(obj);
        break;
      }
      case ObTimestampTZType:
      case ObIntervalDSType: {
        datum2obj<OBJ_DATUM_4BYTE_LEN_DATA>(obj);
        break;
      }
      case ObTimestampLTZType:
      case ObTimestampNanoType: {
        datum2obj<OBJ_DATUM_2BYTE_LEN_DATA>(obj);
        break;
      }
      case ObLobType: {
        datum2obj<OBJ_DATUM_LOB_LOCATOR>(obj);
        break;
      }
      case ObMaxType: {
        ret = common::OB_ERR_UNEXPECTED;
        COMMON_LOG(WARN, "invalid obj type", K(ret), K(meta.get_type()));
      }
    }
  }
  return ret;
}

}  // end namespace common
}  // end namespace oceanbase

#endif  // OCEANBASE_DATUM_OB_DATUM_H_
