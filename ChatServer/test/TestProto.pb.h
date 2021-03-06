// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TestProto.proto

#ifndef PROTOBUF_TestProto_2eproto__INCLUDED
#define PROTOBUF_TestProto_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_TestProto_2eproto();
void protobuf_AssignDesc_TestProto_2eproto();
void protobuf_ShutdownFile_TestProto_2eproto();

class test;

// ===================================================================

class test : public ::google::protobuf::Message {
 public:
  test();
  virtual ~test();

  test(const test& from);

  inline test& operator=(const test& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const test& default_instance();

  void Swap(test* other);

  // implements Message ----------------------------------------------

  test* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const test& from);
  void MergeFrom(const test& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string code = 1;
  inline bool has_code() const;
  inline void clear_code();
  static const int kCodeFieldNumber = 1;
  inline const ::std::string& code() const;
  inline void set_code(const ::std::string& value);
  inline void set_code(const char* value);
  inline void set_code(const char* value, size_t size);
  inline ::std::string* mutable_code();
  inline ::std::string* release_code();
  inline void set_allocated_code(::std::string* code);

  // @@protoc_insertion_point(class_scope:test)
 private:
  inline void set_has_code();
  inline void clear_has_code();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* code_;
  friend void  protobuf_AddDesc_TestProto_2eproto();
  friend void protobuf_AssignDesc_TestProto_2eproto();
  friend void protobuf_ShutdownFile_TestProto_2eproto();

  void InitAsDefaultInstance();
  static test* default_instance_;
};
// ===================================================================


// ===================================================================

// test

// required string code = 1;
inline bool test::has_code() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void test::set_has_code() {
  _has_bits_[0] |= 0x00000001u;
}
inline void test::clear_has_code() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void test::clear_code() {
  if (code_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    code_->clear();
  }
  clear_has_code();
}
inline const ::std::string& test::code() const {
  // @@protoc_insertion_point(field_get:test.code)
  return *code_;
}
inline void test::set_code(const ::std::string& value) {
  set_has_code();
  if (code_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    code_ = new ::std::string;
  }
  code_->assign(value);
  // @@protoc_insertion_point(field_set:test.code)
}
inline void test::set_code(const char* value) {
  set_has_code();
  if (code_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    code_ = new ::std::string;
  }
  code_->assign(value);
  // @@protoc_insertion_point(field_set_char:test.code)
}
inline void test::set_code(const char* value, size_t size) {
  set_has_code();
  if (code_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    code_ = new ::std::string;
  }
  code_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:test.code)
}
inline ::std::string* test::mutable_code() {
  set_has_code();
  if (code_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    code_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:test.code)
  return code_;
}
inline ::std::string* test::release_code() {
  clear_has_code();
  if (code_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = code_;
    code_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void test::set_allocated_code(::std::string* code) {
  if (code_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete code_;
  }
  if (code) {
    set_has_code();
    code_ = code;
  } else {
    clear_has_code();
    code_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:test.code)
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_TestProto_2eproto__INCLUDED
