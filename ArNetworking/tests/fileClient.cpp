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

ArClientFileLister *fileLister;
ArClientFileToClient *fileToClient;
ArClientFileFromClient *fileFromClient;
ArClientDeleteFileOnServer *deleteFileOnServer;

bool done = false;

void updated(int ret)
{
  done = true;
  if (ret == 0)
    fileLister->log(false);
  else
    printf("Bad update %d\n", ret);
}

void fileReceived(int ret)
{
  done = true;
  if (ret == 0)
    printf("Got file '%s'\n", fileToClient->getFileName());
  else
    printf("Didn't get file '%s' because %d\n", fileToClient->getFileName(), 
	   ret);
}

void fileSent(int ret)
{
  done = true;
  if (ret == 0)
    printf("Sent file '%s'\n", fileFromClient->getFileName());
  else
    printf("Didn't send file '%s' because %d\n", 
	   fileFromClient->getFileName(), 
	   ret);
}

void fileDeleted(int ret)
{
  done = true;
  if (ret == 0)
    printf("Deleted file '%s'\n", deleteFileOnServer->getFileName());
  else
    printf("Didn't delete file '%s' because %d\n", 
	   deleteFileOnServer->getFileName(), 
	   ret);
}

void waitForDone(void)
{
  while (!done && Aria::getRunning())
    ArUtil::sleep(1);
  done = false;
}

int main(int argc, char **argv)
{
  std::string hostname;
  Aria::init();
  //ArLog::init(ArLog::StdOut, ArLog::Verbose);
  ArClientBase client;


  ArArgumentParser parser(&argc, argv);

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

  fileLister = new ArClientFileLister(&client);
  fileLister->addUpdatedCallback(new ArGlobalFunctor1<int>(&updated));

  fileToClient = new ArClientFileToClient(&client);
  fileToClient->addFileReceivedCallback(
	  new ArGlobalFunctor1<int>(&fileReceived));

  fileFromClient = new ArClientFileFromClient(&client);
  fileFromClient->addFileSentCallback(
	  new ArGlobalFunctor1<int>(&fileSent));

  deleteFileOnServer = new ArClientDeleteFileOnServer(&client);
  deleteFileOnServer->addFileDeletedCallback(
	  new ArGlobalFunctor1<int>(&fileDeleted));

  client.runAsync();

  /*
  if (deleteFileOnServer->deleteFileFromDirectory(NULL, "all.bob"))
    waitForDone();
  else
    printf("Error deleting file\n");

  if (deleteFileOnServer->deleteFileFromDirectory("1", "all.bob"))
    waitForDone();
  else
    printf("Error deleting file\n");

  if (deleteFileOnServer->deleteFileFromDirectory("1/3", "all.bob"))
    waitForDone();
  else
    printf("Error deleting file\n");


  if (deleteFileOnServer->deleteFileFromDirectory("fun", "all.bob"))
    waitForDone();
  else
    printf("Error deleting file\n");

  if (deleteFileOnServer->deleteFileFromDirectory("argh", "all.bob"))
    waitForDone();
  else
    printf("Error deleting file\n");
  */

  /*
  done = false;
  if (fileFromClient->putFileToDirectory("argh", "png", "app_logo.png", ArClientFileFromClient::SPEED_SLOW))
    waitForDone();
  else
    printf("Error before sending file\n");
  */
  done = false;
  if (fileFromClient->putFileToDirectory(NULL, "dox", "doxygen.conf", ArClientFileFromClient::SPEED_SLOW))
    waitForDone();
  else
    printf("Error before sending file\n");


/*
  done = false;
  if (fileFromClient->putFileToDirectory("argh", "png", "app_logo.png")
    waitForDone();
  else
    printf("Error before sending file\n");

  done = false;
  if (fileFromClient->putFileToDirectory(NULL, "dox", "doxygen.conf")
    waitForDone();
  else
    printf("Error before sending file\n");
*/
/*
  fileLister->changeToTopDir();
  waitForDone();
  fileLister->changeToDir("argh");
  waitForDone();
  fileLister->log(true);

  fileToClient->getFileFromDirectory("argh\\um", "doxygen", "slashes");
  waitForDone();
  fileToClient->getFileFromDirectory("argh", "DOXYGEN.Conf", "blah.um.2");
  waitForDone();
  fileToClient->getFileFromDirectory("argh", "wehlkjadsf", "blah.um.2");
  waitForDone();
  fileToClient->getFileFromDirectory("argh", "1", "blah.um");
  waitForDone();
  fileToClient->getFileFromDirectory(NULL, "1", "blah.um.2");
  waitForDone();
  */
  /*
  fileToClient->getFileFromDirectory(NULL, "all.map", "all.map.2");
  waitForDone();

  fileToClient->getFileFromDirectory(NULL, "um.map", "all.map.2");
  waitForDone();

  fileToClient->getFileFromDirectory(NULL, "all.map", "all.map.3");
  waitForDone();

  fileToClient->getFileFromDirectory(NULL, "configClient.txt", "something");
  waitForDone();
  
  */

  /*
  fileLister->changeToTopDir();
  waitForDone();
  fileLister->changeToDir("0level");
  waitForDone();
  fileLister->changeToDir("1level");
  waitForDone();
  fileLister->upOneDir();
  waitForDone();
  fileLister->changeToDir("1level2");
  waitForDone();
  */

  

  Aria::shutdown();
  return 0;

}




