This is the sub-repository for Smartphone-based Fall Detection.

PURPOSE OF PROJECT:
This project is intended to be used on an Android device for the purpose of detecting falls. If it detects a fall, it starts an alarm via the FallDetectionAlert class. Eventually, it is intended to be set up to notify the user's emergency contact of the fall via email or SMS, but right now the code is not set up for that.

VERSION or DATE: 
7/26/2012

HOW TO START THIS PROJECT:
This project was developed in Eclipse using the Eclipse plug-in for android. When downloading the Eclipse plug-in, the latest version of the Android SDK is automatically installed. However, the target for our code is Android API 10, which uses the Android SDK 2.3.3. So after installing the Eclipse plug-in, it is necessary to open the Android SDK manager and manually select the Android 2.3.3 SDK packages to install. 

After installing the correct version of Android, you will need either an android virtual device (AVD) set up in Eclipse or an Android device with API 10 or later set up for USB debugging in order to compile the code. An AVD would not be particularly useful with this code, however, since the code depends on accelerometer readings, which you cannot get from most laptop or desktop computers, thus it is better to use USB debugging on an Android device. To use your Android phone's USB debugging options you must first install the phone's drivers to your computer. Then you must turn on the "USB debugging" option in your phone's settings. In the Nexus One, this option can be found under "Settings" then "Applications" and "Development." (Note that while these setting options are fairly standard, the buttons may have slightly different names from one Android device to another.)

AUTHORS:
Ariel Keller, Kevin Ringstaff

We also used and edited some of the code made available by Gonzales in his master's thesis:
I. J. D. Gonzales. Fall detection using a smartphone. Master’s thesis, Gjovik University College, 2010-2011