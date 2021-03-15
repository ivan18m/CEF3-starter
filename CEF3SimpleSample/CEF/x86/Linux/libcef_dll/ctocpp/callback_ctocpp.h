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
// $hash=d5bd0e618c060e9410fdd4760d1e941e443a8010$
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_CALLBACK_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_CALLBACK_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_callback_capi.h"
#include "include/cef_callback.h"
#include "libcef_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefCallbackCToCpp : public CefCToCppRefCounted<CefCallbackCToCpp,
                                                     CefCallback,
                                                     cef_callback_t> {
 public:
  CefCallbackCToCpp();
  virtual ~CefCallbackCToCpp();

  // CefCallback methods.
  void Continue() OVERRIDE;
  void Cancel() OVERRIDE;
};

#endif  // CEF_LIBCEF_DLL_CTOCPP_CALLBACK_CTOCPP_H_
