// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define CURL_STATICLIB
#include <curl/curl.h>

// add headers that you want to pre-compile here
#pragma once
#include "framework.h"
#include "MinHook.h"
#include "gd.h"
//#include "nfd.h"
//#include "matplist.hpp"
/*
#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>
*/
#include <cocos2d.h>
#include <ExtensionMacros.h>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <spine/extension.h>
#include <curl/curl.h>
#include <GUI/CCControlExtension/CCControlUtils.h>
//#include "HTTPrequest.hpp"

using namespace cocos2d;

#endif //PCH_H
