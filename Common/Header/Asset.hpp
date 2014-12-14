/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2
 * See CREDITS.TXT file for authors and copyrights
 *
 * File:   Asset.hpp
 * Author: Bruno de Lacheisserie
 *
 * Created on 8 december 2014, 00:01
 */

#ifndef ASSET_HPP
#define	ASSET_HPP

constexpr
bool IsAltair() {
    return false;
}

static inline
bool IsDithered() {
#ifdef DITHER
    return true;
#elif defined(ANDROID) && defined(__arm__)
    return is_dithered;
#else
    return false;
#endif
}

/**
 * Returns whether the application is running on a Kobo e-book reader.
 */
constexpr
static inline bool
IsKobo()
{
#ifdef KOBO
  return true;
#else
  return false;
#endif
}

/**
 * Returns whether the application is running on an old version of
 * Windows CE (pre 5.0).  Starting with version 5.0, several bug
 * workarounds are disabled at compile time.
 */
constexpr
static inline bool
IsOldWindowsCE()
{
#if defined(_WIN32_WCE) && _WIN32_WCE < 0x0500
  return true;
#else
  return false;
#endif
}

/**
 * Returns whether the application is running on Android
 */
constexpr
static inline bool
IsAndroid()
{
#if defined(ANDROID)
  return true;
#else
  return false;
#endif
}

/**
 * Returns whether the application is running on Pocket PC / Windows
 * CE / Windows Mobile.
 */
constexpr
static inline bool
IsWindowsCE()
{
#ifdef _WIN32_WCE
  return true;
#else
  return false;
#endif
}


/**
 * Does this device have a touch screen?  This is useful to know for
 * sizing controls, as a touch screen may require bigger areas.
 */
constexpr
static inline bool
HasTouchScreen()
{
  return IsAndroid() || (IsWindowsCE() && !IsAltair()) || IsKobo();
}


#endif	/* ASSET_HPP */

