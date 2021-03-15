// Copyright (c) 2021 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=b6fc50fa3a6e0156743a5c3fce97b1a683e1720a$
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_V8EXCEPTION_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_V8EXCEPTION_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_v8_capi.h"
#include "include/cef_v8.h"
#include "libcef_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefV8ExceptionCToCpp : public CefCToCppRefCounted<CefV8ExceptionCToCpp,
                                                        CefV8Exception,
                                                        cef_v8exception_t> {
 public:
  CefV8ExceptionCToCpp();
  virtual ~CefV8ExceptionCToCpp();

  // CefV8Exception methods.
  CefString GetMessage() OVERRIDE;
  CefString GetSourceLine() OVERRIDE;
  CefString GetScriptResourceName() OVERRIDE;
  int GetLineNumber() OVERRIDE;
  int GetStartPosition() OVERRIDE;
  int GetEndPosition() OVERRIDE;
  int GetStartColumn() OVERRIDE;
  int GetEndColumn() OVERRIDE;
};

#endif  // CEF_LIBCEF_DLL_CTOCPP_V8EXCEPTION_CTOCPP_H_
