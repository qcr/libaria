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
#ifndef ARSERVERCLIENTDATA_H
#define ARSERVERCLIENTDATA_H

#include <Aria/Aria.h>

class ArServerClientData
{
public:
  ArServerClientData(ArServerData *serverData, long mSecInterval, 
		     ArNetPacket *packet)
    {
      myServerData = serverData; 
      myMSecInterval = mSecInterval;
      myPacket.duplicatePacket(packet);
      myReadLength = myPacket.getReadLength();
      myLastSent.setToNow();
    }
  virtual ~ArServerClientData() {}
  ArServerData *getServerData(void) { return myServerData; }
  long getMSec(void) { return myMSecInterval; }
  ArNetPacket *getPacket(void) 
    { myPacket.setReadLength(myReadLength); return &myPacket; }
  ArTime getLastSent (void) { return myLastSent; }
  void setLastSentToNow(void) { myLastSent.setToNow(); }
  void setMSec(long mSec) { myMSecInterval = mSec; }
  void setPacket(ArNetPacket *packet) 
    { 
      myPacket.duplicatePacket(packet); 
      myReadLength = myPacket.getReadLength();
    }
protected:
  ArServerData *myServerData;
  long myMSecInterval;
  ArNetPacket myPacket;
  unsigned int myReadLength;
  ArTime myLastSent;

};

#endif // ARSERVERCLIENTDATA_H
