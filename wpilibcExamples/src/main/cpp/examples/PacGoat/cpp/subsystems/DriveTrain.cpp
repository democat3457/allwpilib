// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystems/DriveTrain.h"

#include <frc/Joystick.h>
#include <units/length.h>
#include <wpi/math>

#include "commands/DriveWithJoystick.h"

DriveTrain::DriveTrain() : frc::Subsystem("DriveTrain") {
  // AddChild("Front Left CIM", m_frontLeftCIM);
  // AddChild("Front Right CIM", m_frontRightCIM);
  // AddChild("Back Left CIM", m_backLeftCIM);
  // AddChild("Back Right CIM", m_backRightCIM);

  // Configure the DifferentialDrive to reflect the fact that all our
  // motors are wired backwards and our drivers sensitivity preferences.
  m_robotDrive.SetSafetyEnabled(false);
  m_robotDrive.SetExpiration(0.1);
  m_robotDrive.SetMaxOutput(1.0);
  m_leftCIMs.SetInverted(true);
  m_rightCIMs.SetInverted(true);

#ifndef SIMULATION
  // Converts to feet
  m_rightEncoder.SetDistancePerPulse(0.0785398);
  m_leftEncoder.SetDistancePerPulse(0.0785398);
#else
  // Circumference = diameter * pi. 360 tick simulated encoders.
  m_rightEncoder.SetDistancePerPulse(units::foot_t{4_in}.to<double>() *
                                     wpi::math::pi / 360.0);
  m_leftEncoder.SetDistancePerPulse(units::foot_t{4_in}.to<double>() *
                                    wpi::math::pi / 360.0);
#endif

  AddChild("Right Encoder", m_rightEncoder);
  AddChild("Left Encoder", m_leftEncoder);

// Configure gyro
#ifndef SIMULATION
  m_gyro.SetSensitivity(0.007);  // TODO: Handle more gracefully?
#endif
  AddChild("Gyro", m_gyro);
}

void DriveTrain::InitDefaultCommand() {
  SetDefaultCommand(new DriveWithJoystick());
}

void DriveTrain::TankDrive(double leftAxis, double rightAxis) {
  m_robotDrive.TankDrive(leftAxis, rightAxis);
}

void DriveTrain::Stop() {
  m_robotDrive.TankDrive(0.0, 0.0);
}

frc::Encoder& DriveTrain::GetLeftEncoder() {
  return m_leftEncoder;
}

frc::Encoder& DriveTrain::GetRightEncoder() {
  return m_rightEncoder;
}

double DriveTrain::GetAngle() {
  return m_gyro.GetAngle();
}
