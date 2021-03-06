// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TestProto.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "TestProto.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* test_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  test_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_TestProto_2eproto() {
  protobuf_AddDesc_TestProto_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "TestProto.proto");
  GOOGLE_CHECK(file != NULL);
  test_descriptor_ = file->message_type(0);
  static const int test_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(test, code_),
  };
  test_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      test_descriptor_,
      test::default_instance_,
      test_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(test, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(test, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(test));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_TestProto_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    test_descriptor_, &test::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_TestProto_2eproto() {
  delete test::default_instance_;
  delete test_reflection_;
}

void protobuf_AddDesc_TestProto_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\017TestProto.proto\"\024\n\004test\022\014\n\004code\030\001 \002(\t", 39);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "TestProto.proto", &protobuf_RegisterTypes);
  test::default_instance_ = new test();
  test::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_TestProto_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_TestProto_2eproto {
  StaticDescriptorInitializer_TestProto_2eproto() {
    protobuf_AddDesc_TestProto_2eproto();
  }
} static_descriptor_initializer_TestProto_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int test::kCodeFieldNumber;
#endif  // !_MSC_VER

test::test()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:test)
}

void test::InitAsDefaultInstance() {
}

test::test(const test& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:test)
}

void test::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  code_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

test::~test() {
  // @@protoc_insertion_point(destructor:test)
  SharedDtor();
}

void test::SharedDtor() {
  if (code_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete code_;
  }
  if (this != default_instance_) {
  }
}

void test::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* test::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return test_descriptor_;
}

const test& test::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_TestProto_2eproto();
  return *default_instance_;
}

test* test::default_instance_ = NULL;

test* test::New() const {
  return new test;
}

void test::Clear() {
  if (has_code()) {
    if (code_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
      code_->clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool test::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:test)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string code = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_code()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->code().data(), this->code().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "code");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:test)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:test)
  return false;
#undef DO_
}

void test::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:test)
  // required string code = 1;
  if (has_code()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->code().data(), this->code().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "code");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->code(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:test)
}

::google::protobuf::uint8* test::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:test)
  // required string code = 1;
  if (has_code()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->code().data(), this->code().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "code");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->code(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:test)
  return target;
}

int test::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string code = 1;
    if (has_code()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->code());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void test::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const test* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const test*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void test::MergeFrom(const test& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_code()) {
      set_code(from.code());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void test::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void test::CopyFrom(const test& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool test::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void test::Swap(test* other) {
  if (other != this) {
    std::swap(code_, other->code_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata test::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = test_descriptor_;
  metadata.reflection = test_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
