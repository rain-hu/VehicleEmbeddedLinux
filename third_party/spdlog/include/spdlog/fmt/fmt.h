#pragma once

//
// Include a bundled header-only copy of fmtlib or an external one.
// By default spdlog include its own copy.
//
#include <spdlog/tweakme.h>

// 强制使用bundled fmt，忽略SPDLOG_FMT_EXTERNAL定义
#if defined(SPDLOG_USE_STD_FORMAT)  // SPDLOG_USE_STD_FORMAT is defined - use std::format
#include <format>
#else
// 总是使用bundled fmt，不管SPDLOG_FMT_EXTERNAL如何定义
#if !defined(SPDLOG_COMPILED_LIB) && !defined(FMT_HEADER_ONLY)
#define FMT_HEADER_ONLY
#endif
#ifndef FMT_USE_WINDOWS_H
#define FMT_USE_WINDOWS_H 0
#endif
#include <spdlog/fmt/bundled/format.h>
#endif
