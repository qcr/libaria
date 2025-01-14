/*
Adept MobileRobots Robotics Interface for Applications (ARIA)
Copyright (C) 2004-2005 ActivMedia Robotics LLC
Copyright (C) 2006-2010 MobileRobots Inc.
Copyright (C) 2011-2015 Adept Technology, Inc.
Copyright (C) 2016 Omron Adept Technologies, Inc.

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

If you wish to redistribute ARIA under different terms, contact 
Adept MobileRobots for information about a commercial version of ARIA at 
robots@mobilerobots.com or 
Adept MobileRobots, 10 Columbia Drive, Amherst, NH 03031; +1-603-881-7960
*/
#ifndef ARSERVERMODERATIODRIVE_H
#define ARSERVERMODERATIODRIVE_H

#include <Aria/ariaTypedefs.h>
#include <Aria/Aria.h>
#include "ArServerMode.h"

class ArServerHandlerCommands;

class ArServerModeRatioDrive : public ArServerMode
{
public:
  /// Constructor
  AREXPORT ArServerModeRatioDrive(ArServerBase *server, ArRobot *robot,
				  bool takeControlOnJoystick = true,
				  bool useComputerJoystick = true, 
				  bool useRobotJoystick = true, 
				  bool useServerCommands = true, 
				  const char *name = "ratioDrive",
				  bool robotJoystickOverridesLocks = true);
  /// Destructor
  AREXPORT virtual ~ArServerModeRatioDrive();
  AREXPORT virtual void activate(void);
  AREXPORT virtual void deactivate(void);
  /// This adds commands that'll let you disable and enable safe driving
  AREXPORT void addControlCommands(ArServerHandlerCommands *handlerCommands);
  /// Sets the ratio drive action
  AREXPORT void ratioDrive(double transRatio, 
                           double rotRatio, 
			   double throttleRatio,
                           bool isActivating = false,
			   double latRatio = 0);
  /// Adds to a config in a section
  AREXPORT void addToConfig(ArConfig *config, const char *section);
  AREXPORT virtual void userTask(void);
  AREXPORT virtual ArActionGroup *getActionGroup(void) { return &myRatioDriveGroup;}
  ArActionRatioInput *getActionRatioInput(void) { return myRatioAction; }
  /// Sets whether we're safe driving or not
  AREXPORT void setSafeDriving(bool safe, bool internal = false);
  /// Gets whether we're driving safely or not
  AREXPORT bool getSafeDriving(void);
  /// Sets whether we're using the range devices that depend on location
  AREXPORT void setUseLocationDependentDevices(
	  bool useLocationDependentDevices, bool internal = false);
  /// Gets whether we're using the range devices that depend on location
  AREXPORT bool getUseLocationDependentDevices(void);
  /// Adds a callback mode for when safe driving is activated
  void addSafeDrivingCallback(ArFunctor *functor, int position = 50)
    { mySafeDrivingCallbacks.addCallback(functor, position); }
  /// Removes a callback mode for when safe driving is activated
  void remSafeDrivingCallback(ArFunctor *functor)
    { mySafeDrivingCallbacks.remCallback(functor); }
  /// Adds a callback mode for when unsafe driving is activated
  void addUnsafeDrivingCallback(ArFunctor *functor, int position = 50)
    { myUnsafeDrivingCallbacks.addCallback(functor, position); }
  /// Removes a callback mode for when unsafe driving is activated
  void remUnsafeDrivingCallback(ArFunctor *functor)
    { myUnsafeDrivingCallbacks.remCallback(functor); }
  /// Adds a callback mode for when trying to back up
  void addDrivingBackwardsCallback(ArFunctor *functor, int position = 50)
    { myDrivingBackwardsCallbacks.addCallback(functor, position); }
  /// Removes a callback mode for when trying to back up
  void remDrivingBackwardsCallback(ArFunctor *functor)
    { myDrivingBackwardsCallbacks.remCallback(functor); }
      
protected:
  AREXPORT void serverRatioDrive(ArServerClient *client,
				    ArNetPacket *packet);
  AREXPORT void serverSetSafeDrive(ArServerClient *client,
				      ArNetPacket *packet);
  AREXPORT void serverGetSafeDrive(ArServerClient *client,
				      ArNetPacket *packet);
  AREXPORT void serverSafeDrivingEnable(void);
  AREXPORT void serverSafeDrivingDisable(void);
  AREXPORT void ratioFireCallback(void);
  AREXPORT void joyUserTask(void);
  ArJoyHandler *myJoyHandler;
  ArRobotJoyHandler *myRobotJoyHandler;

  bool myTakeControlOnJoystick;
  bool myUseComputerJoystick;
  bool myUseRobotJoystick;
  bool myUseServerCommands;
  bool myPrinting;
  bool myRobotJoystickOverridesLock;
  double myTransRatio;
  double myRotRatio;
  double myThrottleRatio;
  double myLatRatio;

  double myTimeout;

  bool myGotServerCommand;
  bool myLastTimedOut;

  ArActionDeceleratingLimiter *myLimiterForward;
  ArActionDeceleratingLimiter *myLimiterBackward;
  ArActionDeceleratingLimiter *myLimiterLateralLeft;
  ArActionDeceleratingLimiter *myLimiterLateralRight;
  ArRatioInputJoydrive *myJoydrive;
  ArRatioInputRobotJoydrive *myRobotJoydrive;
  ArActionRatioInput *myRatioAction;
  ArActionMovementParameters *myMovementParameters;
  ArActionGroup myRatioDriveGroup;
  bool myDriveSafely;
  bool myNewDriveSafely;
  bool myUseLocationDependentDevices;
  bool myLastRobotSafeDrive;

  ArTime myLastCommand;

  // SEEKUR
  bool mySentRecenter;

  ArCallbackList mySafeDrivingCallbacks;
  ArCallbackList myUnsafeDrivingCallbacks;
  ArCallbackList myDrivingBackwardsCallbacks;

  ArFunctorC<ArServerModeRatioDrive> myJoyUserTaskCB;
  ArFunctor2C<ArServerModeRatioDrive, ArServerClient *, ArNetPacket *> myServerSetSafeDriveCB;
  ArFunctor2C<ArServerModeRatioDrive, ArServerClient *, ArNetPacket *> myServerGetSafeDriveCB;
  ArFunctor2C<ArServerModeRatioDrive, ArServerClient *, ArNetPacket *> myServerRatioDriveCB;
  ArFunctorC<ArServerModeRatioDrive> myRatioFireCB;
  
  // for the simple commands
  ArServerHandlerCommands *myHandlerCommands;
  ArFunctorC<ArServerModeRatioDrive> myServerSafeDrivingEnableCB;
  ArFunctorC<ArServerModeRatioDrive> myServerSafeDrivingDisableCB;
};


#endif // ARNETMODERATIODRIVE_H
