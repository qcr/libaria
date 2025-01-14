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
#include <Aria/Aria.h>
#include "ArNetworking.h"

void handleUpdate(ArNetPacket *packet)
{
  if (packet->getPacketSource() == ArNetPacket::TCP)
    printf("Tcp ... \n");
  else
    printf("Udp ... \n");
}

int main(int argc, char **argv)
{
  ArClientBase client;
  ArGlobalFunctor1<ArNetPacket *> handleUpdateCB(&handleUpdate);

  Aria::init();
  //ArLog::init(ArLog::StdOut, ArLog::Verbose);
  ArTime startTime;
  startTime.setToNow();

  /* Aria components use this to get options off the command line: */
  ArArgumentParser parser(&argc, argv);

  /* This will be used to connect our client to the server, including
   * various bits of handshaking (e.g. sending a password, retrieving a list
   * of data requests and commands...)
   * It will get the hostname from the -host command line argument: */
  ArClientSimpleConnector clientConnector(&parser);

  parser.loadDefaultArguments();

  /* Check for -help, and unhandled arguments: */
  if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    exit(0);
  }

  
  /* Connect our client object to the remote server: */
  if (!clientConnector.connectClient(&client))
  {
    if (client.wasRejected())
      printf("Server '%s' rejected connection, exiting\n", client.getHost());
    else
      printf("Could not connect to server '%s', exiting\n", client.getHost());
    exit(1);
  } 

  printf("Took %ld msec to connect\n", startTime.mSecSince());
  
  client.runAsync();
  
  client.lock();
  client.addHandler("update", &handleUpdateCB);
  
  client.requestOnce("update");
  client.unlock();
  ArUtil::sleep(1000);

  client.lock();
  client.requestOnceUdp("update");
  client.unlock();
  ArUtil::sleep(1000);
  client.lock();
  client.disconnect();
  client.unlock();
  ArUtil::sleep(50);
  exit(0);
}
