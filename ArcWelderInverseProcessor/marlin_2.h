////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arc Welder: Marlin 2 arc interpolation simulator.  Please see the copyright notices in the function definitions
// starting with plan_arc_ for the original license.
//
// Converts G2/G3(arc) commands back to G0/G1 commands.  Intended to test firmware changes to improve arc support.
// This reduces file size and the number of gcodes per second.
//
// Built using the 'Arc Welder: Anti Stutter' library
//
// Copyright(C) 2021 - Brad Hochgesang
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
//
// You can contact the author at the following email address: 
// FormerLurker@pm.me
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "firmware.h"


#define MARLIN_2_XYZE 4

class marlin_2 :
  public firmware
{
public:
  enum class marlin_2_firmware_versions { V2_0_9_1 = 0, V2_0_9_2 = 1};
  /// <summary>
  /// Types and enums taken from https://github.com/MarlinFirmware/Marlin/blob/cce585f6ca2235d0a534e8f3043d6d502b3bd93b/Marlin/src/core/types.h
  /// </summary>
  // This enum was simplified
  enum AxisEnum : uint8_t {
    X_AXIS=0, 
    Y_AXIS=1, 
    Z_AXIS=2, 
    E_AXIS=3,
  };
  
  marlin_2(firmware_arguments args);
  virtual ~marlin_2();
  virtual std::string interpolate_arc(firmware_position& target, double i, double j, double r, bool is_clockwise) override;
  virtual firmware_arguments get_default_arguments_for_current_version() const override;
  virtual void apply_arguments() override;
private:
  marlin_2_firmware_versions marlin_2_version_;
  std::string gcodes_;
  float* current_position;
  float feedrate_mm_s;
  /// <summary>
  /// A struct representing the prusa configuration store.  Note:  I didn't add the trailing underscore so this variable name will match the original source algorithm name.
  /// </summary>
  typedef void(marlin_2::* plan_arc_func)(
    const float(&cart)[MARLIN_2_XYZE],   // Destination position
    const float(&offset)[2], // Center of rotation relative to current_position
    const bool clockwise,     // Clockwise?
    const uint8_t circles     // Take the scenic route
  );

  void plan_arc_2_0_9_1(
    const float (&cart)[MARLIN_2_XYZE],   // Destination position
    const float (&offset)[2], // Center of rotation relative to current_position
    const bool clockwise,     // Clockwise?
    const uint8_t circles     // Take the scenic route
  );
  void plan_arc_2_0_9_2(
    const float(&cart)[MARLIN_2_XYZE],   // Destination position
    const float(&offset)[2], // Center of rotation relative to current_position
    const bool clockwise,     // Clockwise?
    const uint8_t circles     // Take the scenic route
  );
  
  bool buffer_line(const float(&cart)[MARLIN_2_XYZE], double fr_mm_s, int active_extruder);
  void apply_motion_limits(float (&pos)[MARLIN_2_XYZE]);
  plan_arc_func plan_arc_;

  // Marlin Function Defs
  float HYPOT(float x, float y);
  float ATAN2(float x, float y);
  float RADIANS(float x);
  float COS(float x);
  float SIN(float s);
  float ABS(float x);
  float FLOOR(float x);
  float NOLESS(uint16_t x, uint16_t y);
  float sq(float x);
  float MMS_SCALED(float x);
  bool NEAR_ZERO(float x);
  bool NEAR(float x, float y);
  bool WITHIN(float N, float L, float H);
  float CEIL(float x);
  float constrain(float value, float arg_min, float arg_max);
  float _MAX(float x, float y);
  float _MIN(float x, float y);
  float RECIPROCAL(float x);
  void COPY(float target[MARLIN_2_XYZE], const float(&source)[MARLIN_2_XYZE]);
};

